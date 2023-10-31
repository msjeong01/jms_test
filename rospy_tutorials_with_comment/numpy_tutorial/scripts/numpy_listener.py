#!/usr/bin/env python

PKG = 'numpy_tutorial'
import roslib; roslib.load_manifest(PKG)

import rospy
from numpy_tutorial.msg import Floats
from rospy.numpy_msg import numpy_msg

def callback(data):
    print(rospy.get_name(), "I heard %s"%str(data.data))

def listener():
    rospy.init_node('listener')     # node name: listener로 초기화
    # rospy.Subscriber("floats", Floats, callback)
        # /listener I heard (1.100000023841858, 2.200000047683716, 3.299999952316284, 4.400000095367432, 5.5)

    rospy.Subscriber("floats", numpy_msg(Floats), callback)     # topic name: floats, msg type: Floats
                                                                # numpy_msg(Floats)로 수신 msg -> numpy 배열로 변환
                                                                # 수신 msg callback에 전달
        # /listener I heard [1.1 2.2 3.3 4.4 5.5]
    rospy.spin()

if __name__ == '__main__':  # 실행시
    listener()  # listener() 함수 호출
