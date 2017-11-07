
function ret=apoloGetLocationMRobot(robot,world)
if(nargin==1)world='';end;
ret=apoloMex('g',world,robot);
end 