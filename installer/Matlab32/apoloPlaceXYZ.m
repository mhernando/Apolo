function apoloPlaceXYZ(object,x,y,z,world)
if(nargin==4)world='';end;
apoloMex('P',world,object,[x y z 0 0 0])
return 