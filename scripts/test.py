import os
import re
import shutil
import psutil
import tarfile
import threading
import traceback
from datetime import datetime
import rospy
from std_msgs.msg import *


PATH_TEMP = '/home/syscon/Music/temp'
PATH_TASK = '/home/syscon/ROS_DB/sp_task/bags/task'
PATH_ABORT = '/home/syscon/Music/5_1'
REFERENCE_MOVE_RECORD_FILE = 10
MAX_ABORT_DIR_SIZE = 10737418240 # bytes(10GB)
# MAX_ABORT_DIR_SIZE = 10 # GB

# file_extension = (".bag", ".bag.active")
select_dates = ["2024-06-13-11-46-48", "2024-06-13-10-37-20"]
# select_dates = ["2024-06-13-12-22-20", "2024-06-13-12-21-53", "2024-06-13-12-21-26", "2024-06-13-12-20-08", "2024-06-13-12-19-58", "2024-06-13-11-46-48", "2024-06-13-10-37-20"]

def get_bags_to_move():
    bag_list = []
    file_extension = (".bag", ".bag.active")
    curr_date = select_dates[0]
    # bag_list = [f for f in os.listdir(PATH_TASK)]
    bag_list = [f for f in os.listdir(PATH_TASK) if f.endswith(file_extension) and f.startswith(curr_date)]

    # previous_dates = select_dates[1:REFERENCE_MOVE_RECORD_FILE+1]
    previous_dates = select_dates[1:]
    previous_list = [f for f in os.listdir(PATH_TASK) if f.endswith(file_extension) and f.startswith(tuple(previous_dates))]
    bag_list.extend(sorted(previous_list, key=lambda x: (x.split('_')[0], int(re.search(r'_(\d+)\.bag', x).group(1))), reverse=True)[:REFERENCE_MOVE_RECORD_FILE])
    # bag_list.extend(sorted(previous_list, reverse=True)[:REFERENCE_MOVE_RECORD_FILE])
    return bag_list

def get_dir_size(path):
    for root, dirs, files in os.walk(path):
        # return sum([os.path.getsize(os.path.join(root, f)) for f in files])/(1024.0**3)  ## GB
        return sum([os.path.getsize(os.path.join(root, f)) for f in files])  ## bytes

def get_file_size(path, files):
    # return sum(os.path.getsize(os.path.join(path, f)) for f in files)/(1024.0**3)  ## GB
    # return os.path.getsize(os.path.join(path, files))/(1024.0**3)  ## GB
    return os.path.getsize(os.path.join(path, files))  ## bytes

def check_abort_size(files):
    curr_capacity = get_dir_size(PATH_ABORT)
    print(curr_capacity/(1024**3))
    # new_abort_size = get_file_size(PATH_TASK, files)
    new_abort_size = sum(get_file_size(PATH_TASK, f) for f in files)
    print(new_abort_size/(1024**3))

    if new_abort_size+curr_capacity > MAX_ABORT_DIR_SIZE:
        over_size = new_abort_size+curr_capacity-MAX_ABORT_DIR_SIZE
        modification_time = sorted([(f, os.path.getctime(os.path.join(PATH_ABORT, f))) for f in os.listdir(PATH_ABORT)], key=lambda x: x[1])
        print(modification_time)

        target_size = 0
        target_files = []
        for f_path, mtime in modification_time:
            target_files.append(os.path.join(PATH_ABORT, f_path))
            target_size += get_file_size(PATH_ABORT, f_path)
            if target_size >= over_size: print(target_size/(1024**3)); break

        for f_path in target_files: print(f_path); os.remove(f_path)

bag_list = get_bags_to_move()
print(bag_list)
# check_abort_size(bag_list)

def check_disk_storage():
    # total, used, free = shutil.disk_usage(PATH_ABORT)
    # print(total/(1024.0**3))
    # print(free/(1024.0**3))
    print("--os---")
    st = os.statvfs(PATH_ABORT)
    free = st.f_bavail * st.f_frsize
    total = st.f_blocks * st.f_frsize
    used = (st.f_blocks - st.f_bfree) * st.f_frsize
    print(free/(1024.0**3))

    # print("--psutil--")
    # free = psutil.disk_usage(PATH_ABORT).free
    # print(free/(1024.0**3))
    global MAX_ABORT_DIR_SIZE; MAX_ABORT_DIR_SIZE = free*(0.7)
    print("MAX_ABORT_DIR_SIZE: %s GB"%(MAX_ABORT_DIR_SIZE/(1024.0**3)))
# check_disk_storage()


# abort_event = threading.Event()
# def save_record_partial(msg):
#     def run():
#         try:
#             print("Execute thread")
#             abort_event.set(); print("set True")
#             rospy.sleep(0.5)
#             abort_event.clear(); print("set False")

#             curr_time = datetime.now().strftime("%Y-%m-%d-%H-%M-%S")
#             destination_file_name = "%s_%s.tar.gz"%(curr_time, "5_1_2_WayPoint.tar.gz") ### format : {DATE}_{TASK_ID}_{MISSION_ID}_{ACTION_ID}_{ACTION_NAME}.tar
#             destination = "%s/%s"%(PATH_TEMP, destination_file_name)

#             bag_list = get_bags_to_move()
#             check_abort_size(bag_list)
#             with tarfile.open(destination, "w") as tar:
#                 for bag in bag_list:
#                     try:
#                         if abort_event.is_set():
#                             print("ABORT_again, Delete :%s"%destination)
#                             os.remove(destination)
#                             return
#                         src = "%s/%s"%(PATH_TASK, bag)
#                         tar.add(src, arcname=os.path.basename(src))
#                     except Exception as e:
#                         print("add %s in tar fail\n%s"%(src, e), rospy.ERROR)
#             print("Done MOVE!!!!!!!!!!")
#         except Exception as e:
#             print("save_record_partial fail\n%s"%traceback.format_exc(),rospy.WARN)

#     thread_temp = threading.Thread(target=run,args=())
#     thread_temp.daemon=True
#     thread_temp.start()

# if __name__ == "__main__":
#     rospy.init_node("abort_test")
#     rospy.Subscriber('abort_test', String, save_record_partial)
#     rospy.spin()