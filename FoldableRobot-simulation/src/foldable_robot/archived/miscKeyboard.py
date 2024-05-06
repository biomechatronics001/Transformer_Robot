#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist

key_mapping = {  'w': [ 0, 1], 'x': [0, -1],
                 'a': [-1, 0], 'd': [1, 0],
                 's': [ 0, 0] }

def keys_cb(msg, twist_pub):
     if len(msg.data) == 0 or not key_mapping.has_key(msg.data[0]):
         return # unknown key
     vels = key_mapping[msg.data[0]]

     t = Twist()
     t.angular.z = vels[0]
     t.linear.x = vels[1]

     twist_pub.publish(t)

 if __name__ == '__main__':
     rospy.init_node('keys_to_twist')
     twist_pub = rospy.Publisher('cmd_vel', Twist, queue_size=1)
     rospy.Subscriber('keys', String, keys_cb, twist_pub)
     rospy.spin()

# ================BACKUP KEYBOARD TELEOP CODE==================
#!/usr/bin/env python
import sys, select, tty, termios
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float64

# Global Variables
jointAngle = 0
jointTopic = 'joint1_position_controller/command'
jointNum = '1'
key_pressed = ''
pleaseStop = False

def key_interpreter(msg, key_pub):
    global jointAngle
    global jointTopic
    global jointNum
    global key_pressed

    # We have a joint angle of type Float64 that we want to publish, need an increment angle
    incrementAngle = 0.0314

    # Determining Key Pressed from msg topic
    key_pressed = ''
    print("I am data!: ",msg.data)
    if len(msg.data) == 0:
        key_pressed = -1 # empty key
        print("No keyboard input!")
    else:
        key_pressed = str(msg)[7]
    #print(key_pressed)

    # Changing Joint
    if key_pressed.isdigit():
        if int(key_pressed) < 9:
            jointNum = key_pressed
        getJoint(key_pressed)
        print("Joint topic switched to: ",jointTopic)

    # Handling Key Presses
    if key_pressed == 'w':
        print("Current position of joint "+ jointNum +": "+ str(jointAngle)+" radians")
        jointAngle = jointAngle + incrementAngle
        key_pub.publish(jointAngle)
    elif key_pressed == 's':
        print("Current position of joint "+ jointNum +": "+ str(jointAngle)+" radians")
        jointAngle = jointAngle - incrementAngle
        key_pub.publish(jointAngle)

def getJoint(key_pressed):
    global jointTopic
    if key_pressed == '1':
        jointTopic = 'joint1_position_controller/command'
    elif key_pressed == '2':
        jointTopic = 'joint2_position_controller/command'
    elif key_pressed == '3':
        jointTopic = 'joint3_position_controller/command'
    elif key_pressed == '4':
        jointTopic = 'joint4_position_controller/command'
    elif key_pressed == '5':
        jointTopic = 'joint5_position_controller/command'
    elif key_pressed == '6':
        jointTopic = 'joint6_position_controller/command'
    elif key_pressed == '7':
        jointTopic = 'joint7_position_controller/command'
    elif key_pressed == '8':
        jointTopic = 'joint8_position_controller/command'
    else:
        print("Invalid joint input!")

rospy.init_node("keyboard_interpreter")
while pleaseStop == False:
    if key_pressed == 'z':
        pleaseStop = True
        break
    print("I am publishing to the topic: ",jointTopic)
    key_pub = rospy.Publisher(jointTopic, Float64, queue_size=1)
    rospy.Subscriber('keys', String, key_interpreter, key_pub)
    #rospy.sleep(0.5)
    #rospy.spin()
