
function ret=apoloGetOdometry(robot, lastodom, noise, world)
if(nargin<4)world='';end;
if(nargin==2)noise=0.0;end;
ret=apoloMex('o',world,robot,[lastodom noise]);
end 