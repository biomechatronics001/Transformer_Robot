#!/usr/bin/env python3

import rospy
import math
from sensor_msgs.msg import JointState
from std_msgs.msg import Header

def talker():
    pub = rospy.Publisher('jointStates', JointState, queue_size=10)
    rospy.init_node('teleop')
    rate = rospy.Rate(100) # 10hz
    hello_str = JointState()
    hello_str.header = Header()
    hello_str.header.stamp = rospy.Time.now()

    jointNames = ['joint1','joint2','joint3','joint4','joint5','joint6','joint7']
    setJoints = [-1.57, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    steps = 580

    rospy.sleep(2);

    # Set joints to initial position
    hello_str.name = jointNames;
    hello_str.position = setJoints;
    hello_str.velocity = [1,1,1,1,1,1,1]
    hello_str.effort = [1,1,1,1,1,1,1]
    pub.publish(hello_str)

    # Start animation sequence
    for sequence in range(1,11):
        # Print in the terminal regarding which sequence is starting (for debugging)
        print("starting!")
        print(sequence)

        # insertSequence is a function that takes an integer as a parameter. Depending
        # on what this sequence parameter is, this function returns a list/array that corresponds
        # to that sequence (can find this function below)
        newJoints = insertSequence(sequence)

        # subtract is another function that takes the initial joint and final joint angle lists and
        # subtracts them. Divides the difference between joint angles by "step count".
        # This is basically our joint step count array that we can keep adding to the intitial joint angle
        difference  = subtract(setJoints,newJoints,steps)

        # for loop that adds to the initial joint angles and publishes them. Equivalent to FPS
        for count in range(steps):
            # Adds initial joints with joint step count array (increments angles by a little bit)
            setJoints = add(setJoints,difference)
            # Publishes new joint array
            hello_str.name = jointNames;
            hello_str.position = setJoints;
            hello_str.velocity = [1,1,1,1,1,1,1]
            hello_str.effort = [1,1,1,1,1,1,1]
            pub.publish(hello_str)
            rospy.sleep(0.001)
            #print(setJoints)

        # for debugging purposes, prints joint angles at end of animation sequence in terminal
        print(setJoints)
        print("done")

    print("talker done")

# A custom function that subtracts two arrays and divides the difference by some amt called "steps"
def subtract(x,y,steps):
    difference = [];
    for count in range(len(x)):
        termToInsert = (y[count] - x[count])/steps
        difference.append(termToInsert)

    print(difference)
    return difference

# A custom function that adds two arrays
def add(x,y):
    sum = []
    for count in range(len(x)):
        termToInsert = x[count] + y[count]
        sum.append(termToInsert)
    return sum

# A custom function that takes an integer as an argument and returns an array that corresponds to that
def insertSequence(seq):
    newJoints = [];
    if seq == 1:
        newJoints = [-1.57, 0, 3.14, 0, 0, 0, -3.14]
    elif seq == 2:
        newJoints = [-1.57, -3.14, 3.14, -3.14, 0, -3.14, -3.14]
    elif seq == 3:
        newJoints = [-1.57, -3.14, 0, -3.14, 0, -3.14, 0]
    elif seq == 4:
        newJoints = [1.57, -3.14, 0, -3.14, -3.14, -3.14, 0]
    elif seq == 5:
        newJoints = [1.57, 0, 0, 0, -3.14, 0, 0]
    elif seq == 6:
        newJoints = [-1.57, 0, 0, 0, 0, 0, 0]
    elif seq == 7:
        newJoints = [0, -1.57, 1.57, 0, -1.57, -1.57, -1.57]
    elif seq == 8:
        newJoints = [0, -1.57, 1.57, -1.57, -1.57, -1.57, -1.57]
    elif seq == 9:
        newJoints = [0, -1.57, 1.57, 0, -1.57, -1.57, -1.57]
    elif seq == 10:
        newJoints = [-1.57, 0, 0, 0, 0, 0, 0]
    else:
        print("Entered sequence is out of bounds!")
    return  newJoints;

# What runs when this python script is enabled using rosrun. Runs "talker function" above
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
