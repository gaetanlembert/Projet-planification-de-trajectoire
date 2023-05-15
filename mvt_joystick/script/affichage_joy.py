#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Joy

def callback(msg):
    print('ok')
    
rospy.init_node('affichage_joy')

rospy.Subscriber("joy", Joy, callback)
      
rospy.spin()
