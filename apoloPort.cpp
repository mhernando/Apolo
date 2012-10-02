#include "apoloPort.h"
#include "ApoloMessage.h"
ApoloPort::ApoloPort(int port,vector<SimulatedWorld*>*listWorlds)
{
	world=listWorlds;
	sock=new Socket();
	sock->initServer(port);
	Thid.Start(&ApoloPort::handleConnections,this,sock);	

}




/*void *ApoloPort::handleConnections(void *server)
{
	Socket *aux_sock= (Socket*) server;
	Socket socket=*aux_sock;
	PositionableEntity *pos;
	LaserData data;
	LaserSensorSim *sensor;
	RobotSim *robot;
	
	union double2byteConversor
{
		unsigned char bytes[8];
		double real;
};

	union int2byteConversor
{
		unsigned char bytes[4];
		int integer;
};
	
	
	int n=0;
	char order;
	unsigned int index=0;
	bool inex=false;
	char nameWorld[30];
	char nameObject[30];
	unsigned char max=0;
	unsigned char numJoints=0;
	char request[6000]="\nSucces\n";
	double objectPosition[3]={0,0,0};
	double objectOrientation[3]={0,0,0};
	unsigned char jointN[6]={0,0,0,0,0,0};
	double jointOrientation[6]={0,0,0,0,0,0};
	
	
	Transformation3D trans;
	Vector3D orientation;
	double2byteConversor dConv;
	int2byteConversor iConv;
	
	while(1)
	{
		Socket *temp=socket.acceptConnection();
		
		
		while(temp->IsConnected())
		{	
			n=0;
			inex=false;
			char buffer[500];
			if(0<temp->Receive(buffer,500,-1))
			{

					
					order=buffer[n++];
					max=buffer[n++];
					for(int i=0;i<max;i++)
						nameWorld[i]=buffer[n++];
					
					max=buffer[n++];
					for(int i=0;i<max;i++)
						nameObject[i]=buffer[n++];
					
					
					for(index=0; index<world->size();index++)
					{
						char worldXName[30];
						sprintf(worldXName,"%s",(*world)[index]->getName().c_str());
						
						if(strcmp(nameWorld,"0")==0 )
						{
							if((*world)[index]->getWorld()->getObjectByName(nameObject))
							{
								pos=(*world)[index]->getWorld()->getObjectByName(nameObject);
								break;
								
							}
							if(index==world->size()-1)
							{
								sprintf(request,"\nThe object doesn`t exit\n");
								inex=true;
								break;
							}
						}
						
						
						else if(strcmp(nameWorld,worldXName)==0)
						{
							if((*world)[index]->getWorld()->getObjectByName(nameObject))
							{
								pos=(*world)[index]->getWorld()->getObjectByName(nameObject);
								break;


							}
							if(index==world->size()-1)
							{
								sprintf(request,"\nThe object doesn`t exit\n");
								inex=true;
								break;
							}
						}
							
						if(index==world->size()-1)
						{
							sprintf(request,"\nThe world doesn`t exit\n");
							inex=true;
							break;
						}



					}
					if(!inex)
					{

						if(order=='p')
						{
	
						for(int i=0;i<6;i++)
						{
							for(int z=0;z<8;z++)
								dConv.bytes[z]=buffer[n++];
						
							if(i=0)trans.position.x=dConv.real;	
							if(i=1)trans.position.y=dConv.real;
							if(i=2)trans.position.z=dConv.real;
							if(i=4)orientation.x=dConv.real;	
							if(i=5)orientation.y=dConv.real;
							if(i=6)orientation.z=dConv.real;
						}

						trans.orientation.setRPY(orientation.x,orientation.y,orientation.z);
						pos->setAbsoluteT3D(trans);
					
						}
					
						else if(order=='j')
						{
							robot=dynamic_cast<RobotSim*>(pos);
							numJoints=buffer[n++];
							for(int i=0;i<numJoints;i++) jointN[i]=buffer[n++];
							for(int i=0;i<numJoints;i++)
							{
								for(int z=0;z<8;z++)
									dConv.bytes[z]=buffer[n++];
							
									jointOrientation[i]=dConv.real;
									robot->setJointValue(jointN[i],jointOrientation[i]);
							}
						}
						else if(order=='g')
						{
							n=0;
							sensor=dynamic_cast<LaserSensorSim*>(pos);
							sensor->getData(data);
							iConv.integer=data.size();
							for(int z=0;z<4;z++)
								request[n++]=iConv.bytes[z];
							for(int i=0;i<data.size();i++)
							{
								dConv.real=data.getRange(i);
								for(int z=0;z<8;z++)
									request[n++]=dConv.bytes[z];
							}
						
						
							if(0<temp->Send(request,6000))
								temp->Receive(request,100,-1);
							
						}


						else
							sprintf(request,"\nThis order doesn`t exit\n");

					
					

					}

					temp->Send(request,50);
					(*world)[index]->getChild()->RefreshChild();
								
			}
			
			
			
		}

		temp->close();
		delete temp;
	
	}
	
}*/
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
							if(element){
							robot=dynamic_cast<RobotSim*>(element);
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