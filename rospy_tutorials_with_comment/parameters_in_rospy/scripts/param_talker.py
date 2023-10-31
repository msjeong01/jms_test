#!/usr/bin/env python

# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.

import rospy
from std_msgs.msg import String

def param_talker():
    rospy.init_node('param_talker')

    # fetch a /global parameter
    global_example = rospy.get_param("/global_example")     # get a global parameter
    rospy.loginfo("%s is %s", rospy.resolve_name('/global_example'), global_example)

    # fetch the utterance parameter from our parent namespace
    utterance = rospy.get_param('utterance')    # get a parameter from our parent namespace
    rospy.loginfo("%s is %s", rospy.resolve_name('utterance'), utterance)

    # fetch topic_name from the ~private namespace
    topic_name = rospy.get_param('~topic_name')     # get a parameter from our private namespace
    rospy.loginfo("%s is %s", rospy.resolve_name('~topic_name'), topic_name)

    # fetch a parameter, using 'default_valuer' if it doesn't exist
    default_param = rospy.get_param('default_param', 'default_value')
    rospy.loginfo('%s is %s', rospy.resolve_name('default_param'), default_param)

    # fetch a group(dictionary) of parameters
    gains = rospy.get_param('gains')
    p, i, d = gains['P'], gains['I'], gains['D']
    rospy.loginfo("gains are %s %s %s", p, i, d)

    # set some parameters
    rospy.loginfo('setting parameters...')
    rospy.set_param('list_of_floats', [1., 2., 3., 4.,])
    rospy.set_param('bool_True', True)
    rospy.set_param('~private_bar', 1+2)
    rospy.set_param('to_delete', 'baz')
    rospy.loginfo('...parameters have been set')

    # delete a parameter
    if rospy.has_param('to_delete'):
        rospy.delete_param('to_delete')
        rospy.loginfo("deleted %s parameter" %rospy.resolve_name('to_delete'))
    else:
        rospy.loginfo('parameter %s was already deleted' %rospy.resolve_name('to_delete'))

    # search for a parameter
    param_name = rospy.search_param('global_example')
    rospy.loginfo('found global_example parameter under key: %s'%param_name)

    # publish the value of utterance repeatedly
    pub = rospy.Publisher(topic_name, String, queue_size=10)
    while not rospy.is_shutdown():
        pub.publish(utterance)
        rospy.loginfo(utterance)
        rospy.sleep(1)

if __name__ == '__main__':
    try:
        param_talker()
    except rospy.ROSInterruptException: pass