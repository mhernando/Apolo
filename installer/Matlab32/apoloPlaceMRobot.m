
function ret=apoloPlaceMRobot(robot,pose,angle,world)
if(nargin==3)world='';end;
ret=apoloMex('p',world,robot,[pose angle]);
return 