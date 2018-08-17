function A= testLaserData(laser,robot,n)
%TESTLaserData 

for i = 1:n
    apoloMoveMRobot(robot, [0.1, 0.2], 0.1);
    a=apoloGetLaserData(laser);
    b=size(a);
    t = 1:b(2);
    t = t*(1.5*pi/b(2));
    polarplot(t, a);
    
    
    pause(0.5)
   
    apoloUpdate()
end
A=a



end

