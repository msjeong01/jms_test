#!/usr/bin/env python
# coding: utf-8

import rospy
import rospkg
import os
import threading
import traceback
import heapq
import yaml
import tarfile
import re
import shutil, psutil

from datetime import datetime, timedelta
# from sp_task.recovery_behavior import print_log
from recovery_behavior import print_log
import rospy.logger_level_service_caller
from std_msgs.msg import Int8
from std_srvs.srv import Trigger
from bag_packing.srv import SetData, SetDataAction
from bag_packing.msg import Status

pkgs = rospkg.RosPack()

ROBOT_MODEL = os.getenv("ROBOT_MODEL","sr1")

PATH_HOME = os.getenv("HOME")
PATH_SP_TASK = pkgs.get_path("sp_task")
PATH_RPG = pkgs.get_path("ros_parameters_group")

PATH_ROS_DB = os.getenv("ROS_DB_PATH","/home/%s/ROS_DB"%os.getenv("HOME"))
PATH_BAGS = "%s/sp_task/bags"%PATH_ROS_DB
PATH_ABORT = "%s/abort"%PATH_BAGS

PATH_RECORD = "%s/task"%PATH_BAGS
PATH_CONFIG_RPG = "%s/param/%s/system/record.yaml"%(PATH_RPG,ROBOT_MODEL)
if os.path.isfile(PATH_CONFIG_RPG):
    PATH_CONFIG_FILE = PATH_CONFIG_RPG
else:
    PATH_CONFIG_FILE = "%s/config/record.yaml"%PATH_SP_TASK
PATH_BAG_PACKING_SAVE = "%s/.ros/bags/savebag"%PATH_HOME
PATH_BAG_PACKING_DIR = "%s/.ros/bags"%PATH_HOME

REFERENCE_DELETE_RECORD_FILE = rospy.get_param("~recording_cnt_task",1) # count
REFERENCE_DELETE_ABORT_FILE = rospy.get_param("~recording_day_abort",1) # days
REFERENCE_MOVE_RECORD_FILE = rospy.get_param("~recording_cnt_move", 1)  # count
MAX_ABORT_DIR_SIZE = 10737418240 # bytes(10GB)

MODE_NON_RECORDING = 0
MODE_RECORDING = 1

STATUS_NOT_RECORDING = 0
STATUS_RECORDING_NORMAL = 1
STATUS_RECORDING_ACTION = 2

PATTERN_TIME = "%Y-%m-%d-%H-%M-%S"  ## ex) 2022-12-16-12-52-48

class TaskRecorder:
    """
    external tirigger method : 
        Task : [start_record, stop_record, save_record, remove_record_files]
        Action : [start_record_partial, stop_record_partial, save_record_partial]        
    """
    def __init__(self):
        self.subs=[];self.pubs={}; self.services=[]; self.caller={}
        printLog("Record Path : %s"%PATH_CONFIG_FILE,rospy.WARN)
        self.is_enable = False
        self.record_mode = MODE_NON_RECORDING
        self.status_bagpacking = Status()
        self.last_save_file_patial = None
        self.start_record_date = None
        self.bag_date_list = []
        self.abort_event = threading.Event()

        self.set_save_time()
        self.pubs['mode']=rospy.Publisher('TARU/record_mode',Int8,queue_size=1)
 
        self.subs.append(rospy.Subscriber('bag_packing_node/record_status',Status,self.record_status_cb,queue_size=1))

        thread_publish = threading.Thread(target=self.publish_thread,args=(),name="RecordMode")
        thread_publish.daemon=True
        thread_publish.start()
    
    def __del__(self):
        for sub in self.subs: sub.unregister()
        for pub in self.pubs.values(): pub.unregister()
        for ser in self.services: ser.shutdown()

    def set_activation(self,mode):
        self.is_enable = mode
        printLog("record mode is %s"%self.is_enable)
        rospy.set_param("~enable_record",self.is_enable)
    
    def set_save_time(self):
        self.save_time_string = datetime.strftime(datetime.now(),PATTERN_TIME)
    
    def get_save_time(self):
        return self.save_time_string

    def publish_thread(self):
        try:
            while not rospy.is_shutdown():
                rospy.sleep(1)
                self.pubs['mode'].publish(self.record_mode)
        except Exception as e:
            printLog("thread_publish err\n"%traceback.format_exc(),rospy.WARN)

    def check_path(self,path,ref,is_day=True):
        """
        path should contain file name.
        if not exist path, make path
        if file has elapsed ref, delete the file
        is_day == True : check count of day
        is_day == False : check count of files from lastday
        """
        def delete_files():
            def get_validation_time(name):
                try:
                    time_part = name.split("_")[0]
                    file_time = datetime.strptime(time_part,PATTERN_TIME)
                    return True, file_time
                except Exception as e:
                    return False, None
            today = datetime.today()
            files = os.listdir(folder)
            if is_day:
                for file in files:
                    res = get_validation_time(file)
                    if not res[0]:
                        os.system("rm -rf %s/%s"%(folder,file))
                    else:
                        file_time = res[1]
                        if (today - file_time).days >= ref:
                            os.remove("%s/%s"%(folder,file))
            else:
                heap=[]
                for file in files:
                    res = get_validation_time(file)
                    if not res[0]:
                        os.system("rm -rf %s/%s"%(folder,file))                        
                    else:
                        heap.append(file)
                remained_files = heapq.nlargest(ref,heap)
                for file in heap:
                    if not file in remained_files:
                        os.remove("%s/%s"%(folder,file))

        folder = path.replace(path.split("/")[-1],"")
        if not os.path.isdir(folder):
            os.makedirs(folder)
        delete_files()  

    def check_abort_disk_usage(self):
        # free = psutil.disk_usage(PATH_ABORT).free
        st = os.statvfs(PATH_ABORT)
        free = st.f_bavail * st.f_frsize
        global MAX_ABORT_DIR_SIZE; MAX_ABORT_DIR_SIZE = free*(0.7)
        printLog("MAX_ABORT_DIR_SIZE: %s GB"%(MAX_ABORT_DIR_SIZE/(1024.0**3)))

    def get_dir_size(self, path):
        for root, dirs, files in os.walk(path):
            return sum([os.path.getsize(os.path.join(root, f)) for f in files])  ## bytes

    def get_file_size(self, path, files):
        # return sum(os.path.getsize(os.path.join(path, f)) for f in files)/(1024.0**3)  ## GB
        return os.path.getsize(os.path.join(path, files))  ## bytes

    def cmp_abort_bags(self, destination):
        """
        When the action is aborted, compress the current and the previous action bag files
        as many as REFERENCE_MOVE_RECORD_FILE
        """
        def get_bags_to_move():
            bag_list = []
            file_extension = (".bag", ".bag.active")
            curr_date = self.bag_date_list[0]
            bag_list = [f for f in os.listdir(PATH_BAG_PACKING_DIR) if f.endswith(file_extension) and f.startswith(curr_date)]

            previous_dates = self.bag_date_list[1:REFERENCE_MOVE_RECORD_FILE+1]
            previous_list = [f for f in os.listdir(PATH_BAG_PACKING_DIR) if f.endswith(file_extension) and f.startswith(tuple(previous_dates))]
            bag_list.extend(sorted(previous_list, key=lambda x: int(re.search(r'_(\d+)\.bag', x).group(1)), reverse=True)[0:10])
            printLog("bag_list: %s"%bag_list, rospy.WARN)
            return bag_list

        def check_abort_size(files):
            curr_capacity = self.get_dir_size(PATH_ABORT)
            # new_abort_size = get_file_size(PATH_BACK_PACKING_DIR, files)
            new_abort_size = sum(self.get_file_size(PATH_BAG_PACKING_DIR, f) for f in files)

            if new_abort_size+curr_capacity > MAX_ABORT_DIR_SIZE:
                over_size = new_abort_size+curr_capacity-MAX_ABORT_DIR_SIZE
                modification_time = sorted([(f, os.path.getctime(os.path.join(PATH_ABORT, f))) for f in os.listdir(PATH_ABORT)], key=lambda x: x[1])

                target_size = 0
                target_files = []
                for f_path, mtime in modification_time:
                    target_files.append(os.path.join(PATH_ABORT, f_path))
                    target_size += self.get_file_size(PATH_ABORT, f_path)
                    if target_size >= over_size: break

                for f_path in target_files:
                    printLog("Delete: %s"%f_path); os.remove(f_path)

        bag_list = get_bags_to_move()
        check_abort_size(bag_list)
        with tarfile.open(destination, "w") as tar:
            for bag in bag_list:
                try:
                    if self.abort_event.is_set(): #return
                        self.last_save_file_patial = destination
                        printLog("ABORT again, Will delete :%s"%destination)
                        os.remove(destination)
                        return
                    src = "%s/%s"%(PATH_BAG_PACKING_DIR, bag)
                    tar.add(src, arcname=os.path.basename(src))
                except Exception as e:
                    printLog("add %s in tar fail\n%s"%(src, e), rospy.ERROR)
        printLog("ABORT list: %s "%os.listdir(PATH_ABORT), rospy.WARN)

    def start_record(self):
        if not self.is_enable: return
        self.record_mode = MODE_RECORDING
        self.pubs['mode'].publish(self.record_mode) ## publish once

    def stop_record(self):
        if not self.is_enable: return
        self.set_save_time()
        self.record_mode = MODE_NON_RECORDING
        self.pubs['mode'].publish(self.record_mode) ## publish once

    def save_record(self,name):
        """
        save record files when task complete
        will execute save_record -> remove_record_files
        """
        if not self.is_enable: return False
        def run():
            success = True
            # self.set_save_time()
            try:
                # printLog("[%s] save record"%name,rospy.WARN)
                resp = self.call_save_entire(name)
                if resp == None or not resp.success : success = False
                else:
                    save_file = resp.message.split(":")[-1].replace(" ","").split(".tar")[0]+".tar.gz"
                    ### move from bag_packing to PATH_RECORD
                    source_file = "%s/%s"%(PATH_BAG_PACKING_SAVE,save_file)
                    destination = "%s/%s"%(PATH_RECORD,save_file)
                    # printLog("[save_record] [%s] -> [%s]"%(source_file,destination))
                    self.check_path(destination,REFERENCE_DELETE_RECORD_FILE,is_day=False) ## remained last n count of file
                    os.rename(source_file,destination) ## move /home/syscon/.ros/bags/savebag -> /home/syscon/ROS_DB/sp_task/bags/task
                    success = True
                    self.start_record_date=None; self.bag_date_list=[]; self.abort_event.clear()
                    self.remove_record_files()
            except Exception as e:
                printLog("[Task Record] save_record fail\n%s"%traceback.format_exc(),rospy.WARN)
            return success
        name = name.replace(" ","")
        thread_temp = threading.Thread(target=run,args=())
        thread_temp.daemon=True
        thread_temp.start()

    def start_record_partial(self,action_name):
        if not self.is_enable: return False
        def check_action_name():
            """
            check action name in CONFIG FILE
            """
            with open(PATH_CONFIG_FILE,'r') as f:
                yam = yaml.safe_load(f)            
            success = True if action_name in yam.keys() else False
            return success
        success = check_action_name()
        if not success : 
            printLog("[%s] can not find matched name in config file"%action_name,rospy.WARN)
            return success
        resp = self.call_action_record(True,PATH_CONFIG_FILE,action_name)
        if resp == None or not resp.success : success = False
        else:
            self.start_record_date = resp.message.split(":")[-1].replace(" ", "")
            self.bag_date_list.insert(0, self.start_record_date)
            printLog("start_record_date: %s"%self.start_record_date, rospy.WARN)
            printLog("record_date_list: %s"%self.bag_date_list, rospy.WARN)
        return success

    def stop_record_partial(self):
        if not self.is_enable: return False
        success = True        
        resp = self.call_action_record(False,'','')
        if resp == None: success = False
        return success

    def delete_last_save_file(self):
        self.abort_event.set(); printLog("Set event TRUE, sleeo 0.1"); rospy.sleep(0.1)
        printLog("delete_last_save_file is call. [%s]"%self.last_save_file_patial)
        if self.last_save_file_patial != None:
            os.system("rm -rf %s"%self.last_save_file_patial)

    def save_record_partial(self,task_id,mission_id,action_id,action_name):
        """
        save partial record files when robot abort
        """
        if not self.is_enable: return False
        def run():
            success = True
            try:
                printLog("Execute thread")
                # self.abort_event.set(); printLog("Set event TRUE"); rospy.sleep(0.1)
                self.abort_event.clear(); printLog("Set event FALSE")
                curr_time = datetime.now().strftime("%Y-%m-%d-%H-%M-%S")
                destination_file_name = "%s_%s_%s_%s_%s.tar.gz"%(curr_time,task_id,mission_id,action_id,action_name) ### format : {DATE}_{TASK_ID}_{MISSION_ID}_{ACTION_ID}_{ACTION_NAME}.tar
                destination = "%s/%s"%(PATH_ABORT, destination_file_name)
                self.check_path(destination, REFERENCE_DELETE_ABORT_FILE, is_day=True)
                ### move from bag_packing to PATH_ABORT
                self.check_abort_disk_usage()
                self.cmp_abort_bags(destination)
                self.last_save_file_patial = destination
                success = True
            except Exception as e:
                printLog("save_record_partial fail\n%s"%traceback.format_exc(),rospy.WARN)
            # finally: self.abort_event.clear()
            return success
        task_id = task_id.replace(" ","")
        mission_id = mission_id.replace(" ","")
        action_id = action_id.replace(" ","")
        action_name = action_name.replace(" ","")
        thread_temp = threading.Thread(target=run,args=())
        thread_temp.daemon=True
        thread_temp.start()

    def remove_record_files(self,is_refresh_time=False):
        if not self.is_enable: return False
        if is_refresh_time:
            self.set_save_time()
        success = True
        rep = self.call_remove_files()
        if rep == None: success = False
        return success

    ### Topic Callback
    def record_status_cb(self,msg):
        self.status_bagpacking = msg

    ### Service Caller
    def call_action_record(self,mode,path,action_name):
        service_name = "bag_packing_node/action_record"
        srv_type = SetDataAction
        req = srv_type._request_class()
        req.mode = mode
        req.path = path
        req.action_name = action_name
        rep = self.service_caller(service_name,srv_type,req)
        return rep

    def call_remove_files(self):
        service_name = "bag_packing_node/remove_files"
        srv_type = SetData
        req = srv_type._request_class()
        req.data = str(self.get_save_time())
        rep = self.service_caller(service_name,srv_type,req)
        return rep

    def call_save_entire(self,name):
        service_name = "bag_packing_node/save_bag"
        srv_type = SetData
        req = srv_type._request_class()
        req.data = name
        rep = self.service_caller(service_name,srv_type,req)
        return rep

    def call_save_action(self):
        service_name = "bag_packing_node/save_action_bag"
        srv_type = Trigger
        req = srv_type._request_class()
        rep = self.service_caller(service_name,srv_type,req)
        return rep

    ### Utill method
    def service_caller(self, srv_name,srv_type,req):
        """
        for service check and call
        return response, if fail to call then return None
        """
        resp = None
        try:
            # printLog("[%s] call.\n%s"%(srv_name,req))
            rospy.wait_for_service(srv_name, timeout=0.5)
            srvs = rospy.ServiceProxy(srv_name, srv_type)
            resp = srvs.call(req)
            # printLog("[%s] recv: %s"%(srv_name,resp),rospy.WARN)
        except rospy.ServiceException as se:
            printLog("[%s] service call failed: %s"%(srv_name,se),rospy.WARN)
        except rospy.ROSException as re:
            printLog("[%s] service Communication Err, re-Check the Connection.%s"%(srv_name,re),rospy.WARN)
        except Exception as e:
            printLog("[%s] service call err: \n%s"%(srv_name,traceback.format_exc()),rospy.WARN)
        finally:
            return resp


### Utill method without class
def printLog(msg,level=rospy.WARN):
    def getNodeName():
        return rospy.get_name().split('.')[-1].replace('/','').replace(os.getenv("ROS_HOSTNAME"),"")
    node_name = "%s][Record"%getNodeName()
    if level == rospy.INFO:
        rospy.loginfo('[%s] %s'%(node_name,msg))
    elif level == rospy.WARN:
        rospy.logwarn('[%s] %s'%(node_name,msg))
    elif level == rospy.ERROR:
        rospy.logerr('[%s] %s'%(node_name,msg))
    elif level == rospy.FATAL:
        rospy.logfatal('[%s] %s'%(node_name,msg))

if __name__ == "__main__":
    rospy.init_node("task_recorder_test")
    tr = TaskRecorder()
    # tr.check_path("%s/2022-12-16-12-52-48_329.bag"%PATH_RECORD,VALUE_DELETE_RECORD_FILE,is_day=False)    
