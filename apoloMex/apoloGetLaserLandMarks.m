
function ret=apoloGetLaserLandMarks(laser,world)
if(nargin==1)world='';end;
ret=apoloMex('B',world,laser);

end 