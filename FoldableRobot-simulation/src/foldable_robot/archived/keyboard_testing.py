#!/usr/bin/env python
import sys, select, tty, termios
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float64

def talker():
    key_pub = rospy.Publisher('joint1_position_controller/command', Float64, queue_size=1)
    rospy.init_node("keyboard_driver")
    rate = rospy.Rate(100)

    # We have a joint angle of type Float64 that we want to publish, need an increment angle
    jointAngle = 0
    incrementAngle = 0.0314
    keepIncrementing = False
    keepDecrementing = False
    print("Publishing keystrokes. Press Ctrl-C to exit...")

    while not rospy.is_shutdown():
        keyPressing = _Getch()
        while keyPressing == 'w' or keepIncrementing:
            keepIncrementing = True
            keyPressing = _Getch()
            if keyPressing == 'z':
                keepIncrementing = False

            print("Incrementing!")
            jointAngle = jointAngle + incrementAngle
            key_pub.publish(jointAngle)
            rospy.sleep(0.01)

        if keyPressing == 's' or keepDecrementing:
            keyPressing = sys.stdin.read(1) # Youre not pressing anything
            keepDecrementing = True
            jointAngle = jointAngle - incrementAngle
            key_pub.publish(jointAngle)
            #rospy.sleep(0.01)

        if keyPressing == 'z':
            keepIncrementing = False
            keepDecrementing = False

        rospy.sleep(0.01)

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
