
function apoloResetOdometry(robot,pose,world)
if(nargin<=2)world='';end;
if(nargin==1)pose=[0 0 0];end;
apoloMex('R',world,robot,pose);
return 