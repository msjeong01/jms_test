import rospy
import os
import threading
import traceback
import rospkg
import subprocess
import tarfile
import shutil
import copy
from datetime import datetime, timedelta

from std_msgs.msg import Int8
from std_srvs.srv import Trigger, TriggerResponse

pkgs = rospkg.RosPack()

PATH_HOME = os.getenv("HOME")
# PATH_ROS_DB = os.getenv("ROS_DB_PATH","/home/%s/ROS_DB"%os.getenv("HOME"))
PATH_ROS_DB = "%s/ROS_DB"%PATH_HOME
PATH_BAGS = "%s/sp_task/bags"%PATH_ROS_DB
PATH_ABORT = "%s/abort"%PATH_BAGS

PATH_BAG_PACKING_SAVE = "%s/.ros/bags/savebag"%PATH_HOME

PATH_BAG_PACKING_DIR = "%s/.ros/bags"%PATH_HOME
PATH_BAG_PACKING_TEMP = "%s/.ros/bags/temp"%PATH_HOME
PATH_BAG_PACKING_PREVIOUS = "%s/previous_bags"%PATH_BAG_PACKING_TEMP

REFERENCE_MOVE_RECORD_FILE = rospy.get_param("~recording_cnt_move", 2)  # count

class TaskRecorder:
    def __init__(self):
        self.is_enable = True
        self.last_abort_action = None
        self.num = 0

        rospy.Subscriber('test', Int8, self.test_cb)
        rospy.Service('bag_packing_node/save_action_bag', Trigger, self.save_action_bag_cb)

        rospy.loginfo("initialized")

    def save_action_bag_cb(self, req):
        if self.num == 1:
            msg = "[bag_packing_node] Success to save : 2024-05-22-10-10-10_waypoint.tar.gz"
        elif self.num == 2:
            msg = "[bag_packing_node] Success to save : 2024-05-22-12-12-12_waypoint.tar.gz"
        elif self.num == 3:
            msg = "[bag_packing_node] Success to save : 2024-05-22-12-30-30_stanby.tar.gz"
        elif self.num == 4:
            msg = "[bag_packing_node] Success to save : 2024-05-22-15-10-10_docking.tar.gz"
        elif self.num == 5:
            msg = "[bag_packing_node] Success to save : 2024-05-22-20-20-20_stanby.tar.gz"
        elif self.num == 6:
            msg = "[bag_packing_node] Success to save : 2024-05-23-10-10-10_basic.tar.gz"

        return TriggerResponse(success=True, message="%s"%msg)


    def test_cb(self, req):
        self.save_record_partial(req.data, req.data, req.data, "test")
        self.num = req.data


    def save_record_partial(self, task_id, mission_id, action_id, action_name):
        """
        Save partial record files when robot aborts
        """
        if not self.is_enable: return False
        def run():
            try:
                success = True
                resp = self.call_save_action()
                if resp == None or not resp.success : success = False
                else:
                    save_file = resp.message.split(":")[-1].replace(" ","").split(".tar")[0]+".tar.gz"
                    ### move from bag_packing to PATH_ABORT
                    source_file = "%s/%s"%(PATH_BAG_PACKING_SAVE,save_file)
                    # destination_file_name = "%s_%s_%s_%s_%s.tar.gz"%(save_file.split("_")[0],task_id,mission_id,action_id,action_name) ### format : {DATE}_{TASK_ID}_{MISSION_ID}_{ACTION_ID}_{ACTION_NAME}.tar
                    # destination = "%s/%s"%(PATH_ABORT,destination_file_name)
                    # printLog("[save_record_partial] [%s] -> [%s]"%(source_file,destination))

                    self.add_previous_bags(save_file, source_file)
                    # self.check_path(destination,REFERENCE_DELETE_ABORT_FILE,is_day=True)
                    # os.rename(source_file,destination) ## move /home/syscon/.ros/bags/savebag -> /home/syscon/ROS_DB/sp_task/bags/abort
                    success = True

            return success

        thread_temp = threading.Thread(target=run, args=())
        thread_temp.daemon = True
        thread_temp.start()


    def add_previous_bags(self, save_file, source_file):
        def get_bags_to_move():
            abort_file_date = save_file.split("_")[0]
            check_extension = (".bag", ".bag.active")
            files = [f for f in os.listdir(PATH_BAG_PACKING_DIR) if f.endswith(check_extension) and not f.startswith(abort_file_date)]

            sort_dates = sorted(set(f.split("_")[0] for f in files), reverse=True)
            select_dates = sort_dates[:REFERENCE_MOVE_RECORD_FILE]

            bag_list = []
            for f in files:
                file_date = f.split("_")[0]
                if file_date in select_dates:
                    bag_list.append(f)

            return bag_list

        def check_path_exists():
            if not os.path.exists(PATH_BAG_PACKING_TEMP):
                os.makedirs(PATH_BAG_PACKING_TEMP)

            if not os.path.exists(PATH_BAG_PACKING_PREVIOUS):
                os.makedirs(PATH_BAG_PACKING_PREVIOUS)

        try:
            check_path_exists()
            if self.last_abort_action is not None:
                new_abort_action = save_file.split(".tar.gz")[0]

                if self.last_abort_action != new_abort_action:
                    if len(os.listdir(PATH_BAG_PACKING_PREVIOUS)) != 0:
                        os.system("cd %s && rm *"%PATH_BAG_PACKING_PREVIOUS)
                    # for file in os.listdir(PATH_BAG_PACKING_PREVIOUS): os.unlink(file)


                    bag_list = get_bags_to_move()
                    for bag in bag_list:
                        src = "%s/%s"%(PATH_BAG_PACKING_DIR, bag)
                        dst = "%s/%s"%(PATH_BAG_PACKING_PREVIOUS, bag)
                        shutil.copy(src, dst)
                    self.last_abort_action = new_abort_action

                else:
                    if new_abort_action == self.first_abort_action: return

            else: #self.last_abort_action is None:
                self.last_abort_action = save_file.split(".tar.gz")[0]
                self.first_abort_action = copy.deepcopy(self.last_abort_action)
                return


            bag_dir_cmp = "%s/%s"%(PATH_BAG_PACKING_TEMP, save_file)
            os.rename(source_file, bag_dir_cmp)

            with tarfile.open(bag_dir_cmp, 'r:gz') as tr:
                tr.extractall(PATH_BAG_PACKING_TEMP)

            bag_dir_unpacked = bag_dir_cmp.replace(".tar.gz", "")
            os.remove(bag_dir_cmp)

            # copy_tree(PATH_BAG_PACKING_PREVIOUS, bag_dir_unpacked)
            # shutil.copytree(PATH_BAG_PACKING_PREVIOUS, bag_dir_unpacked+"/previous_bags")
            os.system("cp -r %s %s"%(PATH_BAG_PACKING_PREVIOUS, bag_dir_unpacked))

            with tarfile.open(bag_dir_cmp, 'w:gz') as tr:
                tr.add(bag_dir_unpacked, arcname=os.path.basename(bag_dir_unpacked))

            os.rename(bag_dir_cmp, source_file)
            shutil.rmtree(bag_dir_unpacked)

        except Exception as e:
            rospy.logerr("[Task_recorder] Error %s", str(e))

    def call_save_action(self):
        service_name = "bag_packing_node/save_action_bag"
        srv_type = Trigger
        req = srv_type._request_class()
        rep = self.service_caller(service_name, srv_type, req)
        return rep

    def service_caller(self, srv_name, srv_type, req):
        """
        For service check and call
        Return response, if fail to call then return None
        """
        resp = None
        try:
            rospy.wait_for_service(srv_name, timeout=0.5)
            srvs = rospy.ServiceProxy(srv_name, srv_type)
            resp = srvs.call(req)
        except rospy.ServiceException as se:
            printLog("[%s] service call failed: %s" % (srv_name, se), rospy.WARN)
        except rospy.ROSException as re:
            printLog("[%s] service Communication Err, re-Check the Connection.%s" % (srv_name, re), rospy.WARN)
        except Exception as e:
            printLog("[%s] service call err: \n%s" % (srv_name, traceback.format_exc()), rospy.WARN)
        finally:
            return resp



### Utility method without class
def printLog(msg, level=rospy.WARN):
    def getNodeName():
        return rospy.get_name().split('.')[-1].replace('/', '').replace(os.getenv("ROS_HOSTNAME"), "")
    node_name = "%s][Record" % getNodeName()
    if level == rospy.INFO:
        rospy.loginfo('[%s] %s' % (node_name, msg))
    elif level == rospy.WARN:
        rospy.logwarn('[%s] %s' % (node_name, msg))
    elif level == rospy.ERROR:
        rospy.logerr('[%s] %s' % (node_name, msg))
    elif level == rospy.FATAL:
        rospy.logfatal('[%s] %s' % (node_name, msg))


if __name__ == "__main__":
    rospy.init_node("task_recorder_test")
    tr = TaskRecorder()
    rospy.spin()
