#include "robotConnection.h"


RobotConnection::RobotConnection(wxWindow *parent,const wxString &name)
:wxDialog(parent,wxID_ANY,name,wxPoint(10,10),wxSize(200,400))
{

	window=parent;
	logVisible=true;
	connectionLog=new ConnectionLog(this,wxT("Connection Log"));

}

void RobotConnection::SendData(NodeTree *robot)
{
	
	ConnectionParameters *serverParam=new ConnectionParameters(this,true,wxT("Server Parameters"));
	
	serverParam->ShowModal();
	if(!serverParam->IsAccepted())
		return;

	wxString address_client=serverParam->GetAddress_Clients();
	wxString port=serverParam->GetPort();
	
	if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS200Sim || robot->getTipo()==N_LaserSensorSim )
		robot->server.laserSensor->init(atoi(port),atoi(address_client),true);
	else if(robot->getTipo()==N_LaserSensor3DSim) 
		robot->server.laserSensor3D->init(atoi(port),atoi(address_client),true);
	else if(robot->getTipo()==N_WheeledBaseSim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim) 
		robot->server.wheeledBase->init(atoi(port),atoi(address_client),true);
	else if(robot->getTipo()==N_CameraSim)
		robot->server.camera->init(atoi(port),atoi(address_client),true);
	else if(robot->getTipo()==N_KinectSim)
		robot->server.kinect->init(atoi(port),atoi(address_client),true);
	else if(robot->getTipo()==N_QuadrotorSim)
		robot->server.quadrotor->init(atoi(port),atoi(address_client),true);
	else return;
	
	robot->typeConnection=1;
	getIP(robot->server.Address);
	robot->server.Port=atoi(port);
	connectionLog->AddConnection(robot);
	
	server_Thid.Start(&RobotConnection::UpdateServerState,this,robot);
	ShowConnLog(logVisible);
}



void RobotConnection::ReceiveData(NodeTree *robot)
{
	ConnectionParameters *clientParam=new ConnectionParameters(this,false,wxT("Client Parameters")); 
	clientParam->ShowModal();
	if(!clientParam->IsAccepted())
		return;
	robot->client.Address=clientParam->GetAddress_Clients();
	robot->client.Port=atoi(clientParam->GetPort());
	robot->typeConnection=2;
	
	client_Thid.Start(&RobotConnection::ConnectClient,this,robot);
	ShowConnLog(logVisible);
	
}

void* RobotConnection::UpdateServerState(void *server)
{
	NodeTree* robot=(NodeTree*)server;
	bool isConnected=false;
	int clients=0;
	bool update=false;
	
	while(1)
	{
	
		if(robot->typeConnection==0)
			break;
	
		if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS200Sim || robot->getTipo()==N_LaserSensorSim )
		{
			if(clients!=robot->server.laserSensor->getNumClientsConnected())
			{
				robot->server.Clients=robot->server.laserSensor->getNumClientsConnected();
				clients=robot->server.Clients;
				if(clients>0)isConnected=true; 
				else isConnected=false;
				update=true;
			}		
		}
	/*else if(robot->getTipo()==N_LaserSensor3DSim) 
	{
		if(robot->server.laserSensor3D->hasClients())
		{ 
			isConnected=true; 
			robot->server.Clients=robot->server.laserSensor3D->getNumClientsConnected();
		}
	}	
	else if(robot->getTipo()==N_WheeledBaseSim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim) 
	{
		if(robot->server.wheeledBase->hasClients()) 
		{ 
			isConnected=true; 
			robot->server.Clients=robot->server.wheeledBase->getNumClientsConnected();
		}
	}
	else if(robot->getTipo()==N_CameraSim)
	{
		if(robot->server.camera->hasClients()) 
		{ 
			isConnected=true; 
			robot->server.Clients=robot->server.camera->getNumClientsConnected();
		}
	}	
	else if(robot->getTipo()==N_KinectSim)
	{
		if(robot->server.kinect->hasClients()) 
		{ 
			isConnected=true; 
			robot->server.Clients=robot->server.kinect->getNumClientsConnected();
		}
	}
	else if(robot->getTipo()==N_QuadrotorSim)
	{
		if(robot->server.quadrotor->hasClients())
		{ 
			isConnected=true; 
			robot->server.Clients=robot->server.quadrotor->getNumClientsConnected();
		}
	}
*/
	if(update)
	{
		connectionLog->StateConnection(robot,isConnected);
		update=false;
	}
	
	}
	return NULL;

}
void*  RobotConnection::ConnectClient(void *client)
{
	NodeTree* robot=(NodeTree*)client;
	
	
	if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS200Sim || robot->getTipo()==N_LaserSensorSim )
	{
		
		connectionLog->AddConnection(robot);
		LaserData data,rdata;
	
		while(1)
		{
			if(robot->client.laserSensor->connect(robot->client.Address,robot->client.Port,false))
			{
				robot->client.Host=robot->client.laserSensor->getHost();
				connectionLog->StateConnection(robot,true);
				robot->pointer.lasersensorsim->setActive(false);
				while(1)
				{
					if(!robot->client.laserSensor->isConnected())
					{
						connectionLog->StateConnection(robot,false);
						break;
					}
					
					robot->client.laserSensor->getData(data);
					robot->pointer.lasersensorsim->getData(rdata);
					robot->pointer.lasersensorsim->setData(data);
					robot->pointer.lasersensorsim->setDrawGLMode(rdata.drawGLMode);
					data.clear();
					
				
				}
				
				
			}
			if(robot->typeConnection==0)
					break;
		}

	}


	else if(robot->getTipo()==N_LaserSensor3DSim )
	{
		
		connectionLog->AddConnection(robot);
		LaserData3D data;
		
		
		while(1)
		{
			if(robot->client.laserSensor->connect(robot->client.Address,robot->client.Port,false))
			{
				robot->client.Host=robot->client.laserSensor3D->getHost();
				connectionLog->StateConnection(robot,true);
				//robot->pointer.lasersensor3dsim->setActive(false);
				while(1)
				{
					if(!robot->client.laserSensor3D->isConnected())
					{
						connectionLog->StateConnection(robot,false);
						break;
					}
					
					robot->client.laserSensor3D->getData(data);
					
					
					robot->pointer.lasersensor3dsim->setData(data);
		
					
				
				}
				
				
			}
			if(robot->typeConnection==0)
					break;
		}

	}
		
	else if(robot->getTipo()==N_CameraSim)
	{
		
		connectionLog->AddConnection(robot);
		Image image;
		
		
		while(1)
		{
			if(robot->client.camera->connect(robot->client.Address,robot->client.Port,false))
			{
				robot->client.Host=robot->client.camera->getHost();
				connectionLog->StateConnection(robot,true);
				//robot->pointer.cameraSim->setActive(false);
				while(1)
				{
					if(!robot->client.camera->isConnected())
					{
						connectionLog->StateConnection(robot,false);
						break;
					}
					
					robot->client.camera->getImage(image);
					Sleep(1000);
					
					robot->pointer.cameraSim->setImage(image);
					
					
				
				}
				
				
			}
			if(robot->typeConnection==0)
					break;
		}

	}
	
	else if(robot->getTipo()==N_KinectSim)
	{
		
		connectionLog->AddConnection(robot);
		PointCloud cloud;
		
		
		while(1)
		{
			if(robot->client.kinect->connect(robot->client.Address,robot->client.Port,false))
			{
				robot->client.Host=robot->client.kinect->getHost();
				connectionLog->StateConnection(robot,true);
				//robot->pointer.kinectSim->setActive(false);
				while(1)
				{
					if(!robot->client.kinect->isConnected())
					{
						connectionLog->StateConnection(robot,false);
						break;
					}
					
					robot->client.kinect->getData(cloud);
					Sleep(1000);
					
					robot->pointer.kinectSim->setData(cloud);
					
					
				
				}
				
				
			}
			if(robot->typeConnection==0)
					break;
		}

	}
	else if(robot->getTipo()==N_QuadrotorSim )
	{
		
		connectionLog->AddConnection(robot);
		QuadrotorData data;
		
		
		while(1)
		{
			if(robot->client.quadrotor->connect(robot->client.Address,robot->client.Port,false))
			{
				robot->client.Host=robot->client.laserSensor->getHost();
				connectionLog->StateConnection(robot,true);
				//robot->pointer.quadrotorSim->setActive(false);
				while(1)
				{
					if(!robot->client.quadrotor->isConnected())
					{
						connectionLog->StateConnection(robot,false);
						break;
					}
					
				//	robot->client.quadrotor->getData(data);
				//	Sleep(1000);
					
				//	robot->pointer.quadrotorSim->setData(data);
					
					
				
				}
				
				
			}
			if(robot->typeConnection==0)
					break;
		}

	}
	
	else if(robot->getTipo()==N_WheeledBaseSim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim)
	{
		
		connectionLog->AddConnection(robot);
		Odometry odo;
		
		
		while(1)
		{
			if(robot->client.wheeledBase->connect(robot->client.Address,robot->client.Port,false))
			{
				robot->client.Host=robot->client.wheeledBase->getHost();
				connectionLog->StateConnection(robot,true);
				//robot->pointer.wheeledbasesim->setActive(false);
				while(1)
				{
					if(!robot->client.wheeledBase->isConnected())
					{
						connectionLog->StateConnection(robot,false);
						break;
					}
					
					robot->client.wheeledBase->getOdometry(odo);
					
					
					robot->pointer.wheeledbasesim->setLocation(odo.pose);
					
					
				
				}
				
				
			}
			if(robot->typeConnection==0)
					break;
		}

	}


	else return NULL;

	return NULL;
}






void RobotConnection::CloseServer(NodeTree *robot)
{
		if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS200Sim || robot->getTipo()==N_LaserSensorSim) robot->server.laserSensor->close();
		else if(robot->getTipo()==N_LaserSensor3DSim)robot->server.laserSensor3D->close();	
		else if(robot->getTipo()==N_WheeledBaseSim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim ) robot->server.wheeledBase->close(); 
		else if(robot->getTipo()==N_CameraSim)robot->server.camera->close();
		else if(robot->getTipo()==N_KinectSim)robot->server.kinect->close();
		else if(robot->getTipo()==N_QuadrotorSim)robot->server.quadrotor->close();
		robot->typeConnection=0;
		connectionLog->DeleteConnection(robot);
}

void RobotConnection::DisconnectClient(NodeTree *robot)
{
		if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LaserSensorSim ) robot->client.laserSensor->close();
		else if(robot->getTipo()==N_LaserSensor3DSim)robot->client.laserSensor3D->close();	
		else if(robot->getTipo()==N_WheeledBaseSim  || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim ) robot->client.wheeledBase->close(); 
		else if(robot->getTipo()==N_CameraSim)robot->client.camera->close();
		else if(robot->getTipo()==N_KinectSim)robot->client.kinect->close();
		else if(robot->getTipo()==N_QuadrotorSim)robot->client.quadrotor->close();
		robot->typeConnection=0;
		connectionLog->DeleteConnection(robot);
		
}



void RobotConnection::ShowConnLog(bool showLog)
{
	logVisible=showLog;
	
	if(logVisible)	
		connectionLog->Show();
	else	
		connectionLog->Show(false);
	
	window->SetFocus();
}

void RobotConnection::getIP(wxString &ip)
{
	wxIPV4address addr;
	addr.Hostname(wxGetFullHostName());
	ip = addr.IPAddress();
} 