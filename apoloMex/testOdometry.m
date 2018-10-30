function A= testOdometry(robot,n)
%TESTOdometry 
pos=apoloGetOdometry(robot)
A=[pos]
for i = 1:n
    apoloMoveMRobot(robot, [0.05, 0.05], 0.1);
    apoloUpdate()
    a=apoloGetOdometry(robot);
    A=[A ; a];
   
    
end
plot(A(:,1), A(:,2),'b')




end

