#include "apoloPort.h"

ApoloPort::ApoloPort(int port,vector<SimulatedWorld*>*listWorlds)
{
	world=listWorlds;
	sock=new Socket();
	sock->initServer(port);
	Thid.Start(&ApoloPort::handleConnections,this,sock);	

}




void *ApoloPort::handleConnections(void *server)
{
	Socket* sock= (Socket*) server;
	Socket socket=*sock;
	PositionableEntity *pos;
	
	char message[100];
	int numWorld=0;
	int worldSize=0;
	char name[20];
	float x=0;
	float y=0;
	float z=0;
	float u=0;
	float v=0;
	float w=0;
	char order='0';
	
	while(1)
	{
		
		Socket* temp=socket.acceptConnection();
		while(1)
		{
		
			
			if(!temp || !temp->IsConnected())
			{
				LOG_ERROR("Accept returned invalid socket");
				break;
			}
		
			else
			{		
		
				bool ret=temp->ReceiveBytes(message,100,-1);
		
				if(ret)
				{
					LOG_ERROR("Receive error");//sock.Receive outputs LOG
					break;
				}
		
				else
				{
			
					sscanf(message,"%s %c %d %f %f %f %f %f %f",name,&order,&numWorld,&x,&y,&z,&u,&v,&w);

					for(int i=0;name[i]!='\0';i++)
					{
						if(name[i]=='_')
							name[i]=' ';
					}
					worldSize=(*world).size()-1;
					if(numWorld<0 || numWorld >worldSize)
					{
						temp->Send("This world doesn't exit",100);
						break;
					}
					
					pos=(*world)[numWorld]->getWorld()->getObjectByName(name);
		
					
					if(!(*world)[numWorld]->getWorld()->hasObject(pos))
					{
						temp->Send("This object doesn't exit",50);
						break;
					}

		
					if(order=='p')
					{
						Vector3D vector;
						vector.x=x;
						vector.y=y;
						vector.z=z;
						pos->setRelativePosition(vector);
						char response[50];
						sprintf(response,"The object has been moved to coordenates %.2f %.2f %.2f",x,y,z);
						temp->Send(response,100);
					}
		

					else if(order=='g')
					{
						LaserSensorSim *sim;
						LaserData data;
				
						sim=dynamic_cast<LaserSensorSim*>(pos);
						sim->getData(data);
						char laserString[5000];
						for(int i=0; i<data.size()*5;i=i+6)
							sprintf(laserString+i,"%.2f\t",data.getRange(i));
						temp->Send(laserString,5000);
					}

					else if(order=='r')
					{
						RobotSim *robot;
						robot=dynamic_cast<RobotSim*>(pos);
						int joints=robot->getNumJoints();
						float values[6];
						values[0]=x;
						values[1]=y;
						values[2]=z;
						values[3]=u;
						values[4]=v;
						values[5]=w;

						for(int i=0;i<joints;i++)
							robot->setJointValue(i,values[i]);
						temp->Send("Robot articulations have been moved",100);


					}
					(*world)[numWorld]->getChild()->RefreshChild();
				}
			
	
			}
		}
	


	temp->close();
	}
}