% Packages needed: ROS Toolbox, Parallel Computing Toolbox
function enhancedJointControl(joint1,joint2,joint3,joint4,joint5,joint6,joint7)
    % Array of joint messages
    jointMessages = [joint1, joint2, joint3, joint4, joint5, joint6, joint7];
    parfor i = 1:7
        % Concatenate string to format: /joint#_position_controller/command
        % /joint1_position_controller/command
        topicName = strcat(strcat('/joint',num2str(i)),'_position_controller/command');
        controlIndiv(jointMessages(i),topicName)
    end
end

function controlIndiv(joint,jointTopic)
    rosinit
    [pub,msg] = rospublisher(jointTopic,'std_msgs/Float64');
    pause(2) % Wait to ensure publisher is registered
    msg.Data = joint;
    send(pub,msg) % Publish message to joint topic
    rosshutdown
end