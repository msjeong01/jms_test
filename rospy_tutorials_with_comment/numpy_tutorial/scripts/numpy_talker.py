#!/usr/bin/env python
PKG = 'numpy_tutorial'  # numpy_tutorial pkg 이름 저장
import roslib; roslib.load_manifest(PKG)    # roslib.load_manifest(PKG): 패키지 정보 로드, 모든 의존성 리스트를 PYTHONPATH에 추가

import rospy
from rospy.numpy_msg import numpy_msg   # numpy_msg type
from numpy_tutorial.msg import Floats
import numpy    # 다차원 배열 및 수학 함수 라이브러리

def talker():
    pub = rospy.Publisher('floats', numpy_msg(Floats), queue_size=10)   # topic name: floats
                                                                        # msg type: Floats
                                                                        # numpy_msg(): wrapper. allows nodes to deserialize messages directly into numpy arrays
                                                                            # msg <-> numpy 배열 변환
    rospy.init_node('talker', anonymous = True) # node name: talker, anonymous=True=>중복 노드 X
