function A= testVelocidad(mi,mj  )
%TESTVELOCIDAD Summary of this function goes here
%   Detailed explanation goes here
A=zeros(mi,mj);
for i = 1:mi
    for  j=1:mj
        A(i,j)=apoloCheckRobotJoints('Puma 560',[2.5*2*(i/mi -0.5) 2.5*2*(j/mj -0.5) 0.5]);

    end
end
figure;
imagesc(A);
figure(gcf)
end

