
function apoloMoveMRobot(robot,speeds,time,world)
if(nargin==3)world='';end;
apoloMex('m',world,robot,[speeds time]);
return 