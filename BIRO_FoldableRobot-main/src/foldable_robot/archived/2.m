function parallelJointControl(joint1,joint2,joint3,joint4,joint5,joint6,joint7,joint8)
    % Array of joint messages
    jointMessages = [joint1, joint2, joint3, joint4, joint5, joint6, joint7, joint8];
    parfor i = 1:8
        topicName = strcat(strcat('/joint',num2str(i)),'_position_controller/command');
        controlIndiv(jointMessages(i),topicName)
    end
end