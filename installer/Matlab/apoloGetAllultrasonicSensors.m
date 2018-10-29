
function ret=apoloGetAllultrasonicSensors(object,world)
if(nargin==1)world='';end;
ret=apoloMex('d',world,object);

end 