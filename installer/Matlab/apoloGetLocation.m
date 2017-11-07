
function ret=apoloGetLocation(robot,world)
if(nargin==1)world='';end;
ret=apoloMex('G',world,robot);
end 