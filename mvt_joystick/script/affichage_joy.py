#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Joy

def callback(msg):
    x=msg.axes[0]
    y=msg.axes[1]
    print('le robot doit:')
    if y>0.1:
        print('avancer')
    elif y<-0.1:
        print('reculer')
    else:
        print('etre arreter')
    
    print('et doit aller:')
    if x>0.1:
        print('a gauche')
    elif x<-0.1:
        print('a droite')
    else:
        print('tout droit')
    
    #print('ok')
    
rospy.init_node('affichage_joy')

rospy.Subscriber("joy", Joy, callback)
      
rospy.spin()


