function A= testVelocidad(robot,mi,mj  )
%TESTVELOCIDAD Summary of this function goes here
%robot, grid size (mi *mj)
%   Detailed explanation goes here
A=zeros(mi,mj);
for i = 1:mi
    for  j=1:mj
        A(i,j)=apoloCheckRobotJoints(robot,[2.5*2*(i/mi -0.5) 2.5*2*(j/mj -0.5) -1.05]);

    end
end
figure;
imagesc(A);
figure(gcf)
end

