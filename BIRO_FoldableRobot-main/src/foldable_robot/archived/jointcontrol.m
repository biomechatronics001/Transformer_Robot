function jointcontrol(joint1,joint2,joint3,joint4,joint5,joint6,joint7,joint8)
rosinit
[pub1,msg1] = rospublisher('/joint1_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg1.Data = joint1;
    send(pub1,msg1)
    
[pub2,msg2] = rospublisher('/joint2_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg2.Data = joint2;
    send(pub2,msg2)
    
[pub3,msg3] = rospublisher('/joint3_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg3.Data = joint3;
    send(pub3,msg3)
    
[pub4,msg4] = rospublisher('/joint4_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg4.Data = joint4;
    send(pub4,msg4)
    
[pub5,msg5] = rospublisher('/joint5_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg5.Data = joint5;
    send(pub5,msg5)
    
[pub6,msg6] = rospublisher('/joint6_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg6.Data = joint6;
    send(pub6,msg6)
    
[pub7,msg7] = rospublisher('/joint7_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg7.Data = joint7;
    send(pub7,msg7)
    
[pub8,msg8] = rospublisher('/joint8_position_controller/command', 'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg8.Data = joint8;
    send(pub8,msg8)
rosshutdown
end