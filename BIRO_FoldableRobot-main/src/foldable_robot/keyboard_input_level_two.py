#!/usr/bin/env python
import sys, select, tty, termios
import rospy
from std_msgs.msg import String

if __name__ == '__main__':
    key_pub = rospy.Publisher('keys', String, queue_size=1)
    rospy.init_node("keyboard_driver")
    rate = rospy.Rate(100)

    old_attr = termios.tcgetattr(sys.stdin)
    tty.setcbreak(sys.stdin.fileno())
    print("Publishing keystrokes. Press space twice to exit...")

    while not rospy.is_shutdown():
        if select.select([sys.stdin], [], [], 0)[0] == [sys.stdin]:
            message = sys.stdin.read(2)
            print("Pressed: ",message)
            key_pub.publish(message)
            if message == '  ':
                print("Stopping keyboard input script!")
                break
        rate.sleep()

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_attr)
