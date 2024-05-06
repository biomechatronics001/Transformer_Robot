 function pairedjointcontrol2(joint1,joint2,joint3,joint6, joint8)
    currentJointValues = getJointStateVals()
    
    parfor i = 1:5
        disp("entered parfor loop")
        if i == 1
          controlJoint(1,joint1,currentJointValues)
        elseif i == 2
          controlJoint(2,joint2,currentJointValues)
        elseif i == 3
          controlJoint(3,joint3,currentJointValues)
        elseif i == 4
          controlJoint(6,joint6,currentJointValues)
        elseif i == 5
          controlJoint(8,joint8,currentJointValues)
        end
        
    end
end

function controlJoint(jointNum, jointInput, currentJointVals)
    rosshutdown
    disp("Entered control joint function")
    
    rosinit
    topicToPublishTo = strcat(strcat('/joint',int2str(jointNum)),'_position_controller/command')
    [pub,msg] = rospublisher(topicToPublishTo,'std_msgs/Float64');
    pause(2)
    
    incrementInterval = 30;
    jointValSigns = [1, -1, 1, -1, -1];
    if jointNum == 8 
        jointIndex = 5;
    elseif jointNum == 6
        jointIndex = 4;
    else
        jointIndex = jointNum;
    end
    incrementVal = (deg2rad(jointValSigns(jointIndex)*jointInput)-currentJointVals(jointIndex,1))/incrementInterval;
    
    for i = 1:incrementInterval
        % Increment current joint value by interval amount, store into msg
        msg.Data = currentJointVals(jointIndex,1) + incrementVal;
        % Update current joint values
        currentJointVals(jointIndex,1) = currentJointVals(jointIndex,1)+incrementVal;
        
        disp('about to send!');
        send(pub,msg)
        %pause(0.05)
        disp('sent!');
    end
    
    rosshutdown
end

function jointValues = getJointStateVals()
    % In case of crashes
    rosshutdown
    
    rosinit
    sub = rossubscriber('/joint_states','sensor_msgs/JointState');
    pause(1.5)
    
    allJointsReceived = 0;
    numberOfJoints = 5;
    while allJointsReceived == 0
        values = receive(sub,18); % receive(subscriberName,timeoutValue)
        
        if size(values.Position)<numberOfJoints
            % Retrieved position array is missing a joint. Continue while
            % loop until position array contains all Joint Position Values
            continue
        else
            allJointsReceived = 1; % Stop condition
        end
    end
    
    % Return joint values array and shutdown ros link
    jointValues = values.Position;
    rosshutdown 
end