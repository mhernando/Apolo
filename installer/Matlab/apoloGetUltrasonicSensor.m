
function ret=apoloGetUltrasonicSensor(sensor,world)
if(nargin==1)world='';end;
ret=apoloMex('u',world,sensor);

end 