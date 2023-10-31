#!/usr/bin/env python

from __future__ import print_function

import sys
import rospy
from service_and_client.srv import *

def add_two_ints_client(x, y):
    rospy.wait_for_service('add_two_ints')  # block until the service named 'add_two_ints' is available
    try:
        add_two_ints = rospy.ServiceProxy('add_two_ints', AddTwoInts)   # handle for calling the service
        resp1 = add_two_ints(x, y) # use this handle just like a normal function
        return resp1.sum    # and call it
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)

def usage():
    return "%s [x y]"%sys.argv[0]   # sys.argv[0]: the name of the script(program) being executed
                                    # ex) add_two_ints_client.py
if __name__ == "__main__":
    if len(sys.argv) == 3:
        x = int(sys.argv[1])
        y = int(sys.argv[2])
    else:
        print(usage())
        sys.exit(1) # exits the python script with a return code 1
                    # indicate an error or an abnormal termination of the script
    print("Requesting %s+%s" %(x, y))
    print("%s + %s = %s" %(x, y, add_two_ints_client(x, y)))