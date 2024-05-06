#!/usr/bin/env python
from tkinter import ttk
from tkinter import *
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float64
import math
from functools import partial
import time

root = Tk() #initializes the interpreter and creates a root window (will do this implicity without it) (better explanation here:https://stackoverflow.com/questions/24729119/what-does-calling-tk-actually-do)
root.title("UI Control Panel")
columns = ("Joint", "Angle","Configs")
Table = ttk.Treeview(root, height=29, show="headings", columns=columns)
'''
height: specifies # of rows visible when widget appears,
show: specifies a list containing zero or more of the following values, specifying which elements of the tree to display
columns: A list of column identifiers, specifying the number of columns and their names
'''

Table.column("Joint", width=200, anchor='center') #name of column, width of column in pixels, anchor specifies how the text in this column should be aligned with respect to the cell
Table.column("Angle", width=200, anchor='center')
Table.column("Configs", width=350, anchor='center')

Table.heading("Joint", text="Joint") #column name, text: text displayed in column heading
Table.heading("Angle", text="Angle")
Table.heading("Configs", text="Configs")

Table.pack(side=LEFT, fill=BOTH)

joint_names = ['11', '16', '17', '18','21','22','26','27','28','31','32','33','36','38','41','43','46','48','b1','b2','t1','t2'] #list of our active joints
angles = ['0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'] #robot starts with all angles at 0 degrees

for i in range(min(len(joint_names), len(angles))):
    #print("Inserting",joint_names[i],"and",angles[i],"at",i)
    Table.insert('', i, values=(joint_names[i], angles[i]))

rospy.init_node("keyboard_interpreter")
key_pub = None
jointLimits = {'11': -1, '16': 1, '17': -1, '18': 1, '21': -1, '22': 1, '26': 1,'27': -1, '28': 1,'31': 1, '32': 1,'33': 1,'36': 1, '38':1, '41': -1,
'43': 1, '46': 1, '48':-1,'b1': 1,'b2': -1,'t1': 1,'t2': 1}
jointTopics = {'11':'joint11_position_controller/command', '16': 'joint16_position_controller/command', '17': 'joint17_position_controller/command',
'18': 'joint18_position_controller/command', '21': 'joint21_position_controller/command', '22':'joint22_position_controller/command',
'26':'joint26_position_controller/command','27': 'joint27_position_controller/command', '28':'joint28_position_controller/command',
'31': 'joint31_position_controller/command', '32':'joint32_position_controller/command','33': 'joint33_position_controller/command',
'36':'joint36_position_controller/command', '38':'joint38_position_controller/command', '41': 'joint41_position_controller/command',
'43': 'joint43_position_controller/command', '46': 'joint46_position_controller/command', '48':'joint48_position_controller/command',
'b1': 'bottom11_position_controller/command','b2': 'bottom11prime_position_controller/command','t1': 'top21_position_controller/command',
't2': 'top21prime_position_controller/command'}
#joint number, # ID
ids = {'11': 'I001', '16': 'I002', '17': 'I003', '18': 'I004', '21': 'I005', '22': 'I006', '26': 'I007','27': 'I008',
'28': 'I009','31': 'I00A', '32': 'I00B','33': 'I00C','36': 'I00D', '38':'I00E', '41': 'I00F','43': 'I010',
'46': 'I011', '48':'I012','b1': 'I013','b2': 'I014','t1': 'I015','t2': 'I016'}

def treeview_sort_column(tv, col, reverse): #function that sorts the columns in reverse order
    l = [(tv.set(k, col), k) for k in tv.get_children('')]
    l.sort(reverse=reverse)
    for index, (val, k) in enumerate(l):
        tv.move(k, '', index)
        tv.heading(col, command=lambda: treeview_sort_column(tv, col, not reverse))


def set_cell_value(event):
    for item in Table.selection():
        item_text = Table.item(item, "values")
        column = Table.identify_column(event.x)
        row = Table.identify_row(event.y)

    cn = int(str(column).replace('#', ''),16)
    rn = int(str(row).replace('I', ''),16)

    #print("rn,cn",rn,cn)

    if cn != 1 and cn != 3:
        entryedit = Text(root, width=10 + (cn - 1) * 16, height=1)
        entryedit.place(x=16 + (cn - 1) * 130, y=6 + rn * 20)

        def saveedit():
            inputValue = entryedit.get(0.0, "end").rstrip()
            if inputValue == '':
                inputValue = "0"
            Table.set(item, column=column, value=inputValue)
            entryedit.destroy()
            okb.destroy()
        okb = Button(root, text='OK', width=4, command=saveedit)
        okb.place(x=90 + (cn - 1) * 242, y=2 + rn * 20)

    elif cn == 1:
        print("Cannot change these values!")

def publish():
    global key_pub

    identifiers = Table.get_children()
    for i in identifiers:
        currJoint = Table.set(i).get("Joint")
        currJointVal = Table.set(i).get("Angle").rstrip()
        topicToPublish = jointTopics.get(currJoint) # value = dictionary.get(key)

        if currJointVal.isnumeric():
            currJointVal = int(currJointVal) * (math.pi/180)
            if jointLimits.get(currJoint) == -1:
                currJointVal = -1*currJointVal

            key_pub = rospy.Publisher(topicToPublish, Float64, queue_size=1)
            key_pub.publish(currJointVal)
        else:
            print("Invalid joint input for",currJoint)


Table.bind('<Double-1>', set_cell_value)
publishButton = Button(root, text='Publish', bg = 'green', fg = 'white', width=20, command=publish) #creates green Publish Angles button, We want this to have command = some function that publishes the joint angles in each specified row of the table to the respective joint_state_position controller
publishButton.place(x=250, y=(len(joint_names) - 1) * 20 + 95) #coordinates to position button

def resetJoints():
    identifiers = Table.get_children()
    for item in identifiers:
        #print("Item:",item)
        Table.set(item, column = 'Angle', value = '0')

    for topicToPublish in jointTopics.values():
        key_pub = rospy.Publisher(topicToPublish, Float64, queue_size=1)
        key_pub.publish(0)


resetButton = Button(root, text='Reset', bg = 'red', fg = 'white', width=20, command=resetJoints)
resetButton.place(x=250, y=(len(joint_names) - 1) * 20 + 130) #coordinates to position button

execution = False
def publishConfig(config):
    global execution
    if execution == True:
        print("Still moving, please wait!")
        return
    else:
        execution = True

    valsToPublish = {}
    delay = False

    if config == 1: # Roof: flip robot upside-down, 11 + 21 + 31 + 41 (half)
        valsToPublish = {'11':'90','21':'90','31':'90','41':'90'}
    elif config == 2: # Folded in Wheel: 17 + 27 (full), t1 + t2 (half), 33 + 43 (half)
        valsToPublish = {'17':'180','27':'180','t1':'90','t2':'90','33':'90','43':'90'}
        delay = True
    elif config == 3: # Zig-zag: 11 + 41 (half), 21 + 31 (half), b1 + b2 (half)
        valsToPublish = {'11':'90','41':'90','21':'90','31':'90','b1':'90','b2':'90'}
    elif config == 4: # Folded in zig-zag
        valsToPublish = {'11':'180','41':'180','21':'180','31':'180','b1':'180','b2':'180'}
    elif config == 5: # Single Cubes Upwards
        valsToPublish = {'18':'90','22':'90','28':'90','36':'90','46':'90'}
    elif config == 6: # Wheel
        valsToPublish = {'16':'90','26':'90','32':'90','38':'90','48':'90'}
    elif config == 7: # Expanded wheel
        valsToPublish = {'16':'90','26':'90','32':'90','38':'90','48':'90'}
    elif config == 8: # Snail: 17 + 27 (full), T1 + T2 (half), 22 + 16 + 18 (half), 33 + 43 (zero)
        valsToPublish = {'17':'180','27':'180','t1':'90','t2':'90','22':'90','16':'90','18':'90', '33':'0','43':'0'}
    elif config == 9: # Flat T: 17 + 27 (full), 33 + 43 (zero), T1 + T2 (half), 22 + 16 + 18 (full)
        valsToPublish = {'17':'180','27':'180','t1':'90','t2':'90','22':'180','16':'180','18':'180','33':'0','43':'0'}
    elif config == 10: # Plane, wing tips: 17 + 27 (full), 33 + 43 (half), T1 + T2 (half), 22 + 16 + 18 (half)
        valsToPublish = {'17':'180','27':'180','33':'90','43':'90','t1':'90','t2':'90','22':'90','16':'90','18':'90'}
    elif config == 11: # Plane without wing tips: 17 + 27 (full), 33 + 43 (half), T1 + T2 (half), 22 + 16 + 18 (full)
        valsToPublish = {'17':'180','27':'180','33':'90','43':'90','t1':'90','t2':'90','22':'180','16':'180','18':'180'}
    elif config == 12: # Wright Bros: 17 + 27 (full), 33 + 43 (half), T1 + T2 (half), 22 + 16 + 18 (full), T1 + T2 (a little)
        valsToPublish = {'17':'180','27':'180','33':'90','43':'90','t1':'90','t2':'90','22':'180','16':'180','18':'180', 't1':'10','t2':'10'}
    else:
        print("Error in publish config!",config)
        return

    counter = 0;
    for joints,vals in ids.items():
        zero_val = Table.set(vals, column = 'Angle', value = '0')

    start = time.time()
    for joints,vals in valsToPublish.items():
        currJointVal = int(vals) * (math.pi/180)
        set_joint_val = Table.set(ids.get(joints), column = 'Angle', value = vals)

        if jointLimits.get(joints) == -1:
            currJointVal = -1*currJointVal

        topicToPublish = jointTopics.get(joints)
        #print(currJointVal,topicToPublish)
        key_pub = rospy.Publisher(topicToPublish, Float64, queue_size=1)
        key_pub.publish(currJointVal)
        counter+=1

        if counter%2 == 0 and delay == True:
            start = time.process_time()
            moving = True
            while moving:
                end = time.process_time()
                #print("Moving, please wait!")
                if (end-start > 3):
                    moving = False
    execution = False

def locomotion(dir):
    print("Executing locomotion!")
    step1 = {'17':90, '27':90}
    step2 = {'17':0, '27':0, 't1':180, 't2':180}
    step3 = {'17':180, '27':180, 't1':0, 't2':0, '33':180, '43':180}
    step4 = {'17':0, '27':0, 't1':180, 't2':180, '33':0, '43':0}
    step5 = {'t1':0, 't2':0}
    if dir == 'forward':
        steps = [step1, step2, step3, step4, step5]
    else:
        steps = [step2, step3, step4, step5]

    stepNum = 0;
    for indivStep in steps:
        stepNum = stepNum+1
        start = time.process_time()
        for joints,vals in indivStep.items():
            currJointVal = vals * (math.pi/180)
            if jointLimits.get(joints) == -1:
                currJointVal = -1*currJointVal

            topicToPublish = jointTopics.get(joints)
            #print(currJointVal,topicToPublish)
            key_pub = rospy.Publisher(topicToPublish, Float64, queue_size=1)
            key_pub.publish(currJointVal)
        moving = True

        while moving:
            end = time.process_time()
            #print("Moving, please wait!")
            if stepNum == 1 and dir == 'forward':
                waitTime = 3.7
            else:
                waitTime = 6.8
            if end-start > waitTime:
                moving = False

numOfConfigs = 14;
configButtons = []
yPlacement = 20;
labels = {1:"Roof (Flip Robot 1st)", 2:"Folded-in Wheel", 3: "Zig-zag", 4: "Folded-in zig-zag", 5:"Single cubes upwards", 6:"Wheel", 7:"Expanded (Flip Wheel 1st)",
          8: "Snail (Fold-in wheel 1st)", 9: "Flat-T", 10: "Plane w/ wing tips", 11: "Plane w/out wing tips",
          12: "Wright Bros Plane", 13: "Forward Locomotion", 14: "Backward Locomotion"}
for i in range(numOfConfigs):
    label = labels.get(i+1)
    if i != 0:
        yPlacement += 35
    if i == 12:
        button = Button(root, text = label, bg = 'blue',fg='white',width = 35, command= partial(locomotion,'forward'))
        button.place(x = 415, y = yPlacement)
    elif i == 13:
        button = Button(root, text = label, bg = 'blue', fg = 'white', width = 35, command= partial(locomotion, 'backward'))
        button.place(x = 415, y = yPlacement)
    else:
        button = Button(root, text = label, bg = 'blue',fg='white',width = 35, command= partial(publishConfig, i+1))
        button.place(x = 415, y = yPlacement)

resetJoints()

root.mainloop()
