function apoloLinkToRobotTCP(robot,object,world)
if(nargin==2)world='';end;
apoloMex('L',world,robot,object)
return 