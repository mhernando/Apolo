function apoloUpdate(world)
% UPDATE(world), where world is the name of the world thats being redraw 
% If world is not specified, all the worlds of apolo are updated.
    if(nargin == 1)
        apoloMex('U',world);
    else
        apoloMex('U','');
    end
end 