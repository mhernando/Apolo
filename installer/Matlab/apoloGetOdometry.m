
function ret=apoloGetOdometry(robot, world)
if(nargin==1)world='';end;
ret=apoloMex('o',world,robot);
end 