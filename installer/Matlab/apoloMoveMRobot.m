
function ret = apoloMoveMRobot(robot,speeds,time,world)
if(nargin==3)world='';end;
ret=apoloMex('m',world,robot,[speeds time]);
return 