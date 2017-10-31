
function apoloSetRobotJoints(robot,values,world)
if(nargin==2)world='';end;
apoloMex('J',world,robot,values)
return 