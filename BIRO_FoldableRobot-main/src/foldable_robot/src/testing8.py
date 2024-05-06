#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import JointState
from std_msgs.msg import Header

def talker():
    pub = rospy.Publisher('jointStates', JointState, queue_size=10)
    rospy.init_node('teleop')
    rate = rospy.Rate(100) # 10hz
    hello_str = JointState()
    hello_str.header = Header()
    hello_str.header.stamp = rospy.Time.now()

    hello_str.name = ['joint3', 'joint7']
    hello_str.position = [3.14, -3.14]
    hello_str.velocity = [0.001,0.001]
    hello_str.effort = [1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint2', 'joint4', 'joint6']
    hello_str.position = [-3.14, -3.14, -3.14]
    hello_str.velocity = [0.001,0.001,0.001]
    hello_str.effort = [1,1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint3', 'joint7']
    hello_str.position = [0, 0]
    hello_str.velocity = [0.001,0.001]
    hello_str.effort = [1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint1', 'joint5']
    hello_str.position = [1.57, -3.14]
    hello_str.velocity = [0.001,0.001]
    hello_str.effort = [1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint2', 'joint4', 'joint6']
    hello_str.position = [0, 0, 0]
    hello_str.velocity = [0.001,0.001,0.001]
    hello_str.effort = [1,1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint1', 'joint5']
    hello_str.position = [-1.57, 0]
    hello_str.velocity = [0.001,0.001]
    hello_str.effort = [1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint1', 'joint2', 'joint3','joint5', 'joint6', 'joint7']
    hello_str.position = [0, -1.57, 1.57, -1.57, -1.57, -1.57]
    hello_str.velocity = [0.001,0.001,0.001, 0.001,0.001,0.001]
    hello_str.effort = [1,1,1,1,1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint1', 'joint2', 'joint3', 'joint4', 'joint5', 'joint6', 'joint7']
    hello_str.position = [0, -1.57, 1.57, -1.57, -1.57, -1.57, -1.57]
    hello_str.velocity = [0.001,0.001,0.001,0.001,0.001,0.001,0.001]
    hello_str.effort = [1,1,1,1,1,1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    hello_str.name = ['joint1', 'joint2', 'joint3', 'joint4', 'joint5', 'joint6', 'joint7']
    hello_str.position = [-1.57, 0, 0, 0, 0, 0, 0]
    hello_str.velocity = [0.001,0.001,0.001,0.001,0.001,0.001,0.001]
    hello_str.effort = [1,1,1,1,1,1,1]
    pub.publish(hello_str)

    rospy.sleep(1)

    while not rospy.is_shutdown():
      hello_str.header.stamp = rospy.Time.now()
      pub.publish(hello_str)
      rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
