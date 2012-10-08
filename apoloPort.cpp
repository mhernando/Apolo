
#include "apoloPort.h"
#include "ApoloMessage.h"
ApoloPort::ApoloPort(int port,vector<SimulatedWorld*>*listWorlds)
{
	world=listWorlds;
	sock=new Socket();
	sock->initServer(port);
	Thid.Start(&ApoloPort::handleConnections,this,sock);	

}




PositionableEntity *ApoloPort::getElement(char *nworld,char *name,int *worldindex)
{
	//if nworld=0 busca en todos los mundos
	PositionableEntity *elem=0;
	*worldindex=-1;
	if(nworld==0){
		if(name==0)return 0;
		for(int i=0; i<world->size();i++){
			elem=(*world)[i]->getWorld()->getObjectByName(name);
			if(elem){
				*worldindex=i;
				return elem;								
			}
		}
	}
	else
	{
		for(int i=0; i<world->size();i++){
			const char *waux=(*world)[i]->getName().c_str();
			if(strcmp(waux,nworld)==0)
			{
				*worldindex=i;
				return ((*world)[i]->getWorld()->getObjectByName(name));
			}
		}
	}
	//world & element not found
	return 0;
}
void *ApoloPort::handleConnections(void *server)
{
	Socket *aux_sock= (Socket*) server;
	Socket socket=*aux_sock;
	PositionableEntity *pos;
	RobotSim *robot;
	static int valid=0, total=0;
	
	while(1)
	{
		Socket *temp=socket.acceptConnection();
		
		
		while(temp->IsConnected())
		{	
			
			
			char buffer[10000];
			int size=0;
			if(0<(size=temp->Receive(buffer,10000,-1)))
			{
				
				ApoloMessage *m=0;
				char *pbuffer=buffer;//recorrepbuffer
				while(m=ApoloMessage::getApoloMessage(&pbuffer,size))
				{
					size-=m->getSize();
					total++;
					char *nworld=m->getWorld();
					char *name=m->getObjectName();
					int worldindex=0;
					PositionableEntity *element=getElement(nworld,name,&worldindex);

					switch(m->getType())
					{
						case AP_CHECKJOINTS:
						case AP_SETJOINTS:
							if(robot=dynamic_cast<RobotSim*>(element)){
							int numJoints=m->getCharAt(0);
							for(int i=0;i<numJoints;i++)
							robot->setJointValue(i,m->getDoubleAt(1+8*i));
							if(m->getType()==AP_CHECKJOINTS){
								bool res=robot->checkRobotColision();
								char resp[50];
								int tam=ApoloMessage::writeBOOL(resp,res);
								temp->Send(resp,tam);
							}

							valid++;
							}
							
							break;
						case AP_PLACE:
							if(element){
								double d[6];
								for(int i=0;i<6;i++)d[i]=m->getDoubleAt(8*i);
								element->setAbsoluteT3D(Transformation3D(d[0],d[1],d[2],d[3],d[4],d[5]));
								valid++;
							}
							break;
						case AP_UPDATEWORLD:
							if(worldindex>=0){//updates one world
								(*world)[worldindex]->getChild()->RefreshChild();
							}else //updates all worlds
							{
								for(int i=0; i<world->size();i++)
									(*world)[i]->getChild()->RefreshChild();
							}
							valid++;
							break;
					}

					delete m;//aseguro limpieza
				}
				char messageLog[150];
				sprintf(messageLog,"Commands Executed: %d/%d",valid,total);
				wxLogStatus(messageLog);
				//temp->Send(request,50);

					
	
								
			}		
		}

		temp->close();
		delete temp;
	
	}
	

}