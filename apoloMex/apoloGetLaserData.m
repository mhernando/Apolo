
function ret=apoloGetLaserData(laser,world)
if(nargin==1)world='';end;
ret=apoloMex('l',world,laser);
pause(0.1)
end 