#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Joy


def callback(msg):
    print('detect')
    
rospy.init_node('joy_test')

rospy.Subscriber("joy", Joy, callback)
      
rospy.spin()
