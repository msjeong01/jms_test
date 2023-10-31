#!/usr/bin/env python

from __future__ import print_function
from service_and_client.srv import AddTwoInts, AddTwoIntsResponse
import rospy

def handle_add_two_ints(req):   # AddTwoIntsRequest instances
    print("Returning [%s + %s = %s]" %(req.a, req.b, (req.a + req.b)))
    return AddTwoIntsResponse(req.a + req.b)    # return AddTwoIntsResponse

def add_two_ints_server():
    rospy.init_node('add_two_ints_server')
    s = rospy.Service('add_two_ints', AddTwoInts, handle_add_two_ints)  # service named 'add_two_ints', service type 'AddTwoInts'
                                                                        # all requests are passed to 'handle_add_two_ints' function
    print("Ready to add two ints")
    rospy.spin()    # keeps code from exiting until the service is shutdown

if __name__ == "__main__" :
    add_two_ints_server()