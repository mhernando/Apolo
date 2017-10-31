
function ret=apoloCheckRobotJoints(robot,values,world)
if(nargin==2)world='';end;
ret=apoloMex('j',world,robot,values);
return 