function setconfig(config)
    if config == 0
        pairedjointcontrol2(0,0,0,0,0)
    elseif config == 1
        pairedjointcontrol2(0,0,180,0,0)
    elseif config == 2
        pairedjointcontrol2(0,180,180,180,180)
    elseif config == 3
        pairedjointcontrol2(0,180,0,180,180)
    elseif config == 4
        pairedjointcontrol2(180,180,0,180,180)
    elseif config == 5
        pairedjointcontrol2(180,0,0,0,0)
    elseif config == 6
        pairedjointcontrol2(90,90,90,0,0)
    elseif config == 7
        pairedjointcontrol2(90,90,90,90,90)
    elseif config == 8 % Execute Animation
        wait = 3.15;
        
        % Config 0
        pairedjointcontrol2(0,0,0,0,0)
        pause(wait)
        
        % Config 1
        pairedjointcontrol2(0,0,180,0,0)
        pause(wait)
        
        % Config 2
        pairedjointcontrol2(0,180,180,180,180)
        pause(wait)
        
        % Config 3
        pairedjointcontrol2(0,180,0,180,180)
        pause(wait)
        
        % Config 4
        pairedjointcontrol2(180,180,0,180,180)
        pause(wait)
        
        % Config 5
        pairedjointcontrol2(180,0,0,0,0)
        pause(wait)
        
        % Config 0
        pairedjointcontrol2(0,0,0,0,0)
        pause(wait)
        
        % Config 6
        pairedjointcontrol2(90,90,90,0,0)
        pause(wait)
        
        % Config 7
        pairedjointcontrol2(90,90,90,90,90)
        pause(wait)
        
        % Config 0
        pairedjointcontrol2(0,0,0,0,0)
        
        % Config 0
        pairedjointcontrol2(0,0,0,0,0)
    end
end