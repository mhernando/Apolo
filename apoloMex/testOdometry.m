function A= testOdometry(robot,n)
%TESTOdometry 
pos=apoloGetOdometry(robot,[0 0 0],0)
A=[pos]
for i = 1:n
    apoloMoveMRobot(robot, [0.1, 0.02], 0.1);
    apoloUpdate()
    a=apoloGetOdometry(robot,pos,0.05);
    pos=pos+a;
    A=[A ; pos];
   
    
end




end

