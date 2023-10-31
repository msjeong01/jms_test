#!/usr/bin/env python
# every python ROS node have this declaration at the top. 
# it makes sure this script is executed as a Python script

import rospy    # writing a ROS node
from std_msgs.msg import String     # reuse the std_msgs/String message type for publishing

def talker():
    pub = rospy.Publisher('chatter', String, queue_size=10)     # declares the node is publishing to the 'chatter' topic using the message type String.
                                                                # the queue_size limits the amount of queued messages if any subscriber is not receiving the fast enough
    rospy.init_node('talker', anonymous=True)   # rospy.init_node(name, ...) the name of your node. in this case, your node take on the name 'talker'
                                                # it can't contain any slashes '/'
                                                # anonymouse=True ensures tour node has a unique name by adding random numbers to the end of NAME
    rate = rospy.Rate(10)   # 10hz(10 times per second)  # rospy.Rate(): for looping at the desired rate
    while not rospy.is_shutdown():
        hello_str = "hello world %s" %rospy.get_time()
        rospy.loginfo(hello_str)    # messages printed to screen
                                    # written to the Node's log
                                    # written to rosout. rosout is a handy tool for debugging
        pub.publish(hello_str)      # publish a string to our 'chatter' topic
        rate.sleep()    # sleepts to maintain the desired rate through the loop

if __name__ == '__main__':      # the standard Python __main__ check
    try:
        talker()
    except rospy.ROSInterruptException:     # node is shutdown. you don't continue executing code after the sleep()
        pass