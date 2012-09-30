#ifndef __APOLO__APOLO_PORT__H
#define __APOLO__APOLO_PORT__H

#include "mrcore.h"
#include "simulatedWorld.h"

class ApoloPort 

{
	PositionableEntity *getElement(char *nworld,char *name,int *worldindex);
public:
	ApoloPort(int port,vector<SimulatedWorld*>*listWorlds);
	void *handleConnections(void *server);
	



private:
	Socket *sock;
	vector<SimulatedWorld*>*world;
	Thread<ApoloPort> Thid;
	bool closeServer;
	
	
	
	
};

#endif // __APOLO__APOLO_PORT__H
