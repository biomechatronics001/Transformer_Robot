function controlIndiv(joint,jointTopic)
    msgType = 'std_msgs/Float64';
    
    [pub,msg] = rospublisher(jointTopic,msgType);
    pause(2) % Wait to ensure publisher is registered
    msg.Data = joint;
    send(pub,msg)
end