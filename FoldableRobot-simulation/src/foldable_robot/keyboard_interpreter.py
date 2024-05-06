#!/usr/bin/env python
import sys, select, tty, termios
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float64

# Global Variables
jointAngle = [0, 0, 0, 0, 0, 0, 0, 0]
jointTopic = 'joint1_position_controller/command'
jointNum = 1
# -1 represents limits from -3.14 to 0, 1 represents 0 to 3.14
jointLimits = [1, -1, 1, -1, -1, -1, -1, -1]

pairingTopics = []
pairingMode = False
pairingAngle = 0

key_pressed = ''
changeTopic = False
processing = False

key_pub = None
key_sub = None

def key_interpreter(msg, key_pub):
    global jointAngle, jointTopic, jointNum
    global key_pressed, changeTopic, processing
    global pairingTopics, pairingMode, pairingAngle

    # Standard Mode
    if not processing and not pairingMode:
        # We have a joint angle of type Float64 that we want to publish, need an increment angle
        incrementAngle = 0.0314

        # Determining Key Pressed from msg topic
        key_pressed = ''
        if len(msg.data) == 0:
            key_pressed = -1 # empty key
            print("No keyboard input!")
        else:
            key_pressed = msg.data[0]
        print("Pressed:",key_pressed)

        # Changing Joint
        if key_pressed.isdigit():
            key_pressed = int(key_pressed)
            setJointTopic(key_pressed)
            if key_pressed<9 and key_pressed>0:
                jointNum = key_pressed
                print("Joint topic switched to:",jointTopic)
                changeTopic = True

        # Handling Key Presses
        if key_pressed == 'r':
            resetJoints()
        elif key_pressed == 'w' and not reachedLimit(jointNum, jointAngle[jointNum-1]):
            print("Current position of joint "+ str(jointNum) +": "+ str(jointAngle[jointNum-1])+" radians")
            jointAngle[jointNum-1] = jointAngle[jointNum-1] + incrementAngle
            key_pub.publish(jointAngle[jointNum-1])
        elif key_pressed == 's' and not reachedLimit(jointNum, jointAngle[jointNum-1]):
            print("Current position of joint "+ str(jointNum) +": "+ str(jointAngle[jointNum-1])+" radians")
            jointAngle[jointNum-1] = jointAngle[jointNum-1] - incrementAngle
            key_pub.publish(jointAngle[jointNum-1])
        elif key_pressed == 'p':
            pairingMode = True
            print("Entered pairing mode!")
    # Pairing Mode
    elif pairingMode:
        # Determining Key Pressed from msg topic
        key_pressed = ''
        if len(msg.data) == 0:
            key_pressed = -1 # empty key
            print("No keyboard input!")
        else:
            key_pressed = msg.data[0]
        print("Pressed:",key_pressed)

        # Handling Key Presses
        if key_pressed == 'p':
            pairingMode = False
            print("Exiting pairing mode!")
        elif key_pressed.isdigit():
            key_pressed = int(key_pressed)
            if key_pressed<9 and key_pressed>0:
                pairing(key_pressed)
            else:
                print("Invalid joint numbers!")
        elif key_pressed == 'r':
            pairingTopics = []
            pairingAngle = 0
            print('Resetting paired topics!')
        elif key_pressed == 'w':
            print("Incrementing paired angles!")
            updatePairedAngles(key_pressed)
        elif key_pressed == 's':
            print("Decrementing paired angles!")
            updatePairedAngles(key_pressed)

# =========== Functions handling when interpreter is in INDIVIDUAL state ===========

# Sets Joint Topic based on Key Press
def setJointTopic(key_pressed):
    global jointTopic
    print('setJointTopic successful!')

    if key_pressed == 1:
        jointTopic = 'joint1_position_controller/command'
    elif key_pressed == 2:
        jointTopic = 'joint2_position_controller/command'
    elif key_pressed == 3:
        jointTopic = 'joint3_position_controller/command'
    elif key_pressed == 4:
        jointTopic = 'joint4_position_controller/command'
    elif key_pressed == 5:
        jointTopic = 'joint5_position_controller/command'
    elif key_pressed == 6:
        jointTopic = 'joint6_position_controller/command'
    elif key_pressed == 7:
        jointTopic = 'joint7_position_controller/command'
    elif key_pressed == 8:
        jointTopic = 'joint8_position_controller/command'
    else:
        print("Invalid joint topic input!")
        return

# Function that determines if a joint reached its limit. Takes joint number and angle as parameters
def reachedLimit(jointNum, angle):
    global jointAngle, jointLimits
    limit = 0

    # Determining joint limit
    if jointLimits[jointNum-1] == -1:
        limit = -3.14
    else:
        limit = 3.14

    if limit < 0: # From -3.14 to 0
        if angle >= limit and angle <= 0:
            return False # Limit not reached
        else:
            print("Limit reached!")
            if angle < limit:
                jointAngle[jointNum-1] = limit
            else:
                jointAngle[jointNum-1] = 0
            return True # Limit reached!
    else: # From 0 to 3.14
        if angle >= 0 and angle <= limit:
            return False # Limit not reached
        else:
            print("Limit reached!")
            if angle > limit:
                jointAngle[jointNum-1] = limit
            else:
                jointAngle[jointNum-1] = 0
            return True # Limit reached!

# Function that resets robot configuration
def resetJoints():
    global jointAngle, jointTopic, key_pub

    print("Resetting joint angles!")
    originalJointTopic = jointTopic
    jointAngle = [0, 0, 0, 0, 0, 0, 0, 0]
    processing = True

    for jointIndex in range(len(jointAngle)):
        setJointTopic(jointIndex+1) # Set joint topic
        key_pub = rospy.Publisher(jointTopic, Float64, queue_size=1)
        key_pub.publish(jointAngle[jointIndex])

    jointTopic = originalJointTopic
    key_pub = rospy.Publisher(jointTopic, Float64, queue_size=1)
    processing = False
    print("Done resetting!")

# =========== Functions handling when interpreter enters PAIRED state ===========
# Function that adds a joint to a list of paired joints
def pairing(jointNum):
    global pairingTopics
    toBeAdded = 'joint'+str(jointNum)+'_position_controller/command'

    repeat = False
    for index in range(len(pairingTopics)):
        curr = pairingTopics[index]
        if toBeAdded == curr:
            repeat = True
            break
    if not repeat:
        pairingTopics.append(toBeAdded)
        print("Adding joint "+str(jointNum)+" to paired topics")
    else:
        pairingTopics.remove(toBeAdded)
        print("Removed joint "+str(jointNum)+" from paired topics")

# Function that determines if paired angle reached it's limit
def reachedPairedLimit():
    global pairingAngle
    limit = 0
    if pairingAngle >= 0 and pairingAngle <= 3.14:
        print("Pairing Angle is:",pairingAngle)
        return False
    else:
        if pairingAngle < 0:
            pairingAngle = 0
        else:
            pairingAngle = 3.14
        print("Reached paired angle limit!")
        print("Pairing Angle is:",pairingAngle)
        return True

# Function that updates the paired joint angles
def updatePairedAngles(key_pressed):
    global pairingTopics, pairingAngle, key_pub, jointLimits
    print("Updating paired angles")

    oppositeAngle = 0
    if key_pressed == 'w': # Pressed w
        pairingAngle = pairingAngle + 0.0314
        oppositeAngle = (-1)*pairingAngle
    elif key_pressed == 's': # Pressed s
        pairingAngle = pairingAngle - 0.0314
        oppositeAngle = (-1)*pairingAngle

    if not reachedPairedLimit():
        for index in range(len(pairingTopics)):
            publishingToTopic = pairingTopics[index]
            key_pub = rospy.Publisher(publishingToTopic, Float64, queue_size=1)

            jointLimIndex = int(publishingToTopic[5])-1
            if jointLimits[jointLimIndex] == 1:
                key_pub.publish(pairingAngle)
            else:
                key_pub.publish(oppositeAngle)

# =========== Functions that run when this python file is ran ===========
def startInterpreter():
    global key_pub, key_sub, changeTopic, processing

    print("STARTED THE INTERPRETER!")
    rospy.init_node("keyboard_interpreter")
    key_pub = rospy.Publisher(jointTopic, Float64, queue_size=1)
    key_sub = rospy.Subscriber('keys', String, key_interpreter, key_pub) #Key interpreter is the callback function
    rate = rospy.Rate(100)

    while not rospy.is_shutdown():
        if changeTopic:
            processing = True
            changeTopic = False
            print("Unsubcribing subcriber, remapping topic!")

            key_pub = rospy.Publisher(jointTopic, Float64, queue_size=1)
            key_sub.unregister()
            key_sub = rospy.Subscriber('keys', String, key_interpreter, key_pub)

            print("Finished remapping topic!")
            processing = False
        rate.sleep()

if __name__=='__main__':
    try:
        startInterpreter()
    except rospy.ROSInterruptException:
        pass
