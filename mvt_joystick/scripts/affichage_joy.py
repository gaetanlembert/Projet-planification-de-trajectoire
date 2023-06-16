#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
import threading

twist=Twist()

pub = rospy.Publisher('/mobile_robot/cmd_vel', Twist, queue_size=30)

def callback(msg):
    rospy.loginfo(msg)
    twist.linear.x=4*msg.axes[1]
    twist.angular.z=-2*msg.axes[0]
        
def publisher_thread():
    rate = rospy.Rate(5)
    while not rospy.is_shutdown():
        global twist
        pub.publish(twist)
        rate.sleep()
    
rospy.init_node('joystick')
    
sub = rospy.Subscriber("joy", Joy, callback)
worker = threading.Thread(target=publisher_thread)
worker.start()

rospy.spin()

