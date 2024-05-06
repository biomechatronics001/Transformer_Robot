#!/usr/bin/env python
import sys, select, tty, termios
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float64

# Global Variables
jointTopic = 'joint11_position_controller/command' # Initialize on startup

# -1 represents limits from -3.14 to 0, 1 represents 0 to 3.14
jointLimits = {'11': -1, '16': 1, '17': -1, '18': 1, '21': -1, '22': 1, '26': 1,'27': -1, '28': 1,'31': 1, '32': 1,'33': 1,'36': 1, '38':1, '41': -1,
'43': 1, '46': 1, '48':1,'b1': 1,'b2': -1,'t1': 1,'t2': 1}
validJoints = {'11':'joint11_position_controller/command', '16': 'joint16_position_controller/command', '17': 'joint17_position_controller/command',
'18': 'joint18_position_controller/command', '21': 'joint21_position_controller/command', '22':'joint22_position_controller/command',
'26':'joint26_position_controller/command','27': 'joint27_position_controller/command', '28':'joint28_position_controller/command',
'31': 'joint31_position_controller/command', '32':'joint32_position_controller/command','33': 'joint33_position_controller/command',
'36':'joint36_position_controller/command', '38':'joint38_position_controller/command', '41': 'joint41_position_controller/command',
'43': 'joint43_position_controller/command', '46': 'joint46_position_controller/command', '48':'joint48_position_controller/command',
'b1': 'bottom11_position_controller/command','b2': 'bottom11prime_position_controller/command','t1': 'top21_position_controller/command',
't2': 'top21prime_position_controller/command'}

key_presses = []
key_pressed = ''
pairedAngle = 0;
stopInterpreter = 0
animate = False

key_pub = None
key_sub = None

def key_interpreter(msg, key_pub):
    global jointTopic, key_pressed, key_presses, pairedAngle, stopInterpreter

    incrementAngle = 0.0314
    # Determining Key Pressed from msg topic
    key_pressed = ''
    if len(msg.data) < 1:
        key_pressed = -1 # empty key
        print("No keyboard input!")
    else:
        key_pressed = msg.data
    print("Pressed:",key_pressed)

    # Handling Key Presses
    if key_pressed in validJoints:
        pairing(key_pressed)
    elif key_pressed == 'rr':
        key_presses = []
        pairedAngle = 0
        print('Resetting paired topics!')
    elif key_pressed == 'RR':
        resetJoints()
    elif key_pressed == 'ww' or key_pressed == 'sw':
        print("Incrementing paired angles!")
        updatePairedAngles(key_pressed)
    elif key_pressed == 'ss' or key_pressed == 'ws':
        print("Decrementing paired angles!")
        updatePairedAngles(key_pressed)
    elif key_pressed == 'hh':
        print (validJoints.keys())
    elif key_pressed == '  ':
        stopInterpreter = 1
    else:
        print("Invalid input. Press hh to see valid active joints")

# =========== Functions handling when interpreter enters PAIRED state ===========
# Function that adds a joint to a list of paired joints
def pairing(key_pressed):
    global key_presses
    toBeAdded = key_pressed

    repeat = False
    for index in range(len(key_presses)):
        curr = key_presses[index]
        if toBeAdded == curr:
            repeat = True
            break
    if not repeat:
        key_presses.append(toBeAdded)
        print("Adding joint "+str(key_pressed)+" to paired topics")
    else:
        key_presses.remove(toBeAdded)
        print("Removed joint "+str(key_pressed)+" from paired topics")

# Function that determines if paired angle reached it's limit
def reachedPairedLimit():
    global pairedAngle
    limit = 0
    if pairedAngle >= 0 and pairedAngle <= 3.14:
        print("Pairing Angle is:",pairedAngle)
        return False
    else:
        if pairedAngle < 0:
            pairedAngle = 0
        else:
            pairedAngle = 3.14
        print("Reached paired angle limit!")
        print("Pairing Angle is:",pairedAngle)
        return True

# Function that updates the paired joint angles
def updatePairedAngles(key_pressed):
    global key_presses, pairedAngle, key_pub, jointLimits
    print("Updating paired angles")

    oppositeAngle = 0
    if key_pressed == 'ww' or key_pressed == 'sw': # Pressed w
        pairedAngle = pairedAngle + 0.0314
        oppositeAngle = (-1)*pairedAngle
    elif key_pressed == 'ss' or key_pressed == 'ws': # Pressed s
        pairedAngle = pairedAngle - 0.0314
        oppositeAngle = (-1)*pairedAngle

    if not reachedPairedLimit():
        for index in range(len(key_presses)):
            publishingToTopic = validJoints.get(key_presses[index])
            print(publishingToTopic)
            key_pub = rospy.Publisher(publishingToTopic, Float64, queue_size=1)

            if jointLimits.get(key_presses[index]) == 1:
                key_pub.publish(pairedAngle)
            else:
                key_pub.publish(oppositeAngle)

# Function that resets robot configuration
def resetJoints():
    global key_pub

    print("Resetting joint angles!")
    for key in validJoints.keys():
        publishingToTopic = validJoints.get(key)
        print(publishingToTopic)
        key_pub = rospy.Publisher(publishingToTopic, Float64, queue_size=1)
        key_pub.publish(0)
    print("Done resetting!")

# =========== Functions that run when this python file is ran ===========
def startInterpreter():
    global key_pub, key_sub, stopInterpreter

    print("STARTED THE INTERPRETER!")
    rospy.init_node("keyboard_interpreter")
    key_pub = rospy.Publisher(jointTopic, Float64, queue_size=1)
    key_sub = rospy.Subscriber('keys', String, key_interpreter, key_pub) #Key interpreter is the callback function
    rate = rospy.Rate(100)

    while not rospy.is_shutdown() and stopInterpreter == 0:
        rate.sleep()
    print("STOPPING THE INTERPRETER!")

if __name__=='__main__':
    try:
        startInterpreter()
    except rospy.ROSInterruptException:
        pass
