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
	delete serverParam;
	getIP(robot->server.Address);
	robot->server.Port=wxAtoi(port);

	if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS200Sim || robot->getTipo()==N_LaserSensorSim )
		robot->server.laserSensor->init(robot->server.Port,wxAtoi(address_client),true);
	else if(robot->getTipo()==N_LaserSensor3DSim) 
		robot->server.laserSensor3D->init(robot->server.Port,wxAtoi(address_client),true);
	else if(robot->getTipo()==N_WheeledBaseSim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim) 
		robot->server.wheeledBase->init(robot->server.Port,wxAtoi(address_client),true);
	else if(robot->getTipo()==N_CameraSim)
		robot->server.camera->init(robot->server.Port,wxAtoi(address_client),true);
	else if(robot->getTipo()==N_KinectSim)
		robot->server.kinect->init(robot->server.Port,wxAtoi(address_client),true);
	else if(robot->getTipo()==N_QuadrotorSim)
		robot->server.quadrotor->init(robot->server.Port,wxAtoi(address_client),true);
	else return ;

	robot->typeConnection=1;
	connectionLog->AddConnection(robot);
	Thread<RobotConnection> thid;
	HandleThreads(thid,robot);
	ShowConnLog(logVisible);
}



void RobotConnection::ReceiveData(NodeTree *robot)
{
	ConnectionParameters *clientParam=new ConnectionParameters(this,false,wxT("Client Parameters")); 
	clientParam->ShowModal();
	if(!clientParam->IsAccepted()) return;
	robot->client.Address=clientParam->GetAddress_Clients();
	robot->client.Port=wxAtoi(clientParam->GetPort());
	delete clientParam;
	robot->typeConnection=2;
	Thread<RobotConnection>thid;
	HandleThreads(thid,robot,false,false);
	ShowConnLog(logVisible);
	
}

void* RobotConnection::UpdateServerState(void *server)
{
	NodeTree* robot=(NodeTree*)server;
	bool isConnected=false;
	bool update=false;
	int clients=0;
	while(1)
	{
	
		if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS200Sim || robot->getTipo()==N_LaserSensorSim )
		{
			if(robot->typeConnection==0)
			{
						robot->server.laserSensor->close();
						break;
			}
			
			if(clients!=robot->server.laserSensor->getNumClientsConnected())
			{
				robot->server.Clients=robot->server.laserSensor->getNumClientsConnected();
				update=true;
			}		
		}

		
		else if(robot->getTipo()==N_WheeledBaseSim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim) 
		{
			if(robot->typeConnection==0)
			{
				
				robot->server.wheeledBase->close();
						break;
			}
			
			if(clients!=robot->server.wheeledBase->getNumClientsConnected())
			{
				robot->server.Clients=robot->server.wheeledBase->getNumClientsConnected();
				update=true;
			}		
		}
	else if(robot->getTipo()==N_LaserSensor3DSim) 
	{
		if(robot->typeConnection==0)
			{
				
				robot->server.laserSensor3D->close();
						break;
			}
			
		if(clients!=robot->server.laserSensor3D->getNumClientsConnected())
			{
				robot->server.Clients=robot->server.laserSensor3D->getNumClientsConnected();
				update=true;
			}		
	}	

	else if(robot->getTipo()==N_CameraSim)
	{
		if(robot->typeConnection==0)
			{
				
						robot->server.camera->close();
						break;
			}
			
		if(clients!=robot->server.camera->getNumClientsConnected())
			{
				robot->server.Clients=robot->server.camera->getNumClientsConnected();
				update=true;
			}		
	}	
	
	else if(robot->getTipo()==N_KinectSim)
	{
		if(robot->typeConnection==0)
			{
				
						robot->server.kinect->close();
						break;
			}
			
		if(clients!=robot->server.kinect->getNumClientsConnected())
			{
				robot->server.Clients=robot->server.kinect->getNumClientsConnected();
				update=true;
			}		
	}
	
	else if(robot->getTipo()==N_QuadrotorSim)
	{
		if(robot->typeConnection==0)
			{
				
						robot->server.quadrotor->close();
						break;
			}
		if(clients!=robot->server.wheeledBase->getNumClientsConnected())
			{
				robot->server.Clients=robot->server.quadrotor->getNumClientsConnected();
				update=true;
			}		
	}

		if(update)
		{
			clients=robot->server.Clients;
			if(clients>0)	isConnected=true; 
			else			isConnected=false;
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
		while(robot->typeConnection>0)
		{
			
			if(robot->client.laserSensor->connect(robot->client.Address.char_str(),robot->client.Port,false))
			{
				robot->client.Host=wxString(robot->client.laserSensor->getHost());
				connectionLog->StateConnection(robot,true);
				LaserData data,rdata;
				robot->pointer.lasersensorsim->setActive(false);
				while(robot->client.laserSensor->isConnected()==1)
				{
					
						if(robot->typeConnection==0)
							break;
			
						robot->pointer.lasersensorsim->getData(rdata);
						if(robot->client.laserSensor->getData(data))
							robot->pointer.lasersensorsim->setData(data);	
						robot->pointer.lasersensorsim->setDrawGLMode(rdata.drawGLMode);
						data.clear();
					
				}
				connectionLog->StateConnection(robot,false);
				robot->pointer.lasersensorsim->setActive(true);
				
			}
			
		}
		robot->client.laserSensor->close();
	}
	
	else if(robot->getTipo()==N_LaserSensor3DSim )
	{	
		connectionLog->AddConnection(robot);
		while(robot->typeConnection>0)
		{
			
			if(robot->client.laserSensor3D->connect(robot->client.Address.char_str(),robot->client.Port,false))
			{
				robot->client.Host=wxString(robot->client.laserSensor3D->getHost());
				connectionLog->StateConnection(robot,true);
				//robot->pointer.lasersensor3dsim->setActive(false);
				LaserData3D data,rdata;
				while(robot->client.laserSensor3D->isConnected()==1)
				{
					
						if(robot->typeConnection==0)
							//robot->pointer.lasersensor3dsim->setActive(true);
							break;
						

						robot->pointer.lasersensor3dsim->getData(rdata);
						if(robot->client.laserSensor3D->getData(data))
							robot->pointer.lasersensor3dsim->setData(data);	
						robot->pointer.lasersensor3dsim->setDrawGLMode(rdata.drawGLMode);
					
				}
				connectionLog->StateConnection(robot,false);
				//robot->pointer.lasersensor3dsim->setActive(true);
			}
		}	
		robot->client.laserSensor3D->close();
	}
	else if(robot->getTipo()==N_CameraSim)
	{	
		connectionLog->AddConnection(robot);
		while(robot->typeConnection>0)
		{
			
			if(robot->client.camera->connect(robot->client.Address.char_str(),robot->client.Port,false))
			{
				robot->client.Host=wxString(robot->client.camera->getHost());
				connectionLog->StateConnection(robot,true);
				Image image;
				while(robot->client.camera->isConnected()==1)
				{
					
						if(robot->typeConnection==0)
							break;
						

						if(robot->client.camera->getImage(image))
							robot->pointer.cameraSim->setImage(image);	
						
				}
				connectionLog->StateConnection(robot,false);
			}
		}
		robot->client.camera->close();
	}
		
	else if(robot->getTipo()==N_KinectSim)
	{	
		connectionLog->AddConnection(robot);
		while(robot->typeConnection>0)
		{
			
			if(robot->client.kinect->connect(robot->client.Address.char_str(),robot->client.Port,false))
			{
				robot->client.Host=wxString(robot->client.kinect->getHost());
				connectionLog->StateConnection(robot,true);
				PointCloud cloud;
				while(robot->client.kinect->isConnected()==1)
				{
					
						if(robot->typeConnection==0)
							break;
						
					if(robot->client.kinect->getData(cloud))
						robot->pointer.kinectSim->setData(cloud);
							
					
				}
				connectionLog->StateConnection(robot,false);
			}
		}	
		robot->client.kinect->close();			
	}
	
	else if(robot->getTipo()==N_QuadrotorSim)
	{	
		connectionLog->AddConnection(robot);
		while(robot->typeConnection>0)
		{
			
			if(robot->client.kinect->connect(robot->client.Address.char_str(),robot->client.Port,false))
			{
				robot->client.Host=wxString(robot->client.quadrotor->getHost());
				connectionLog->StateConnection(robot,true);
				QuadrotorData data;
				while(robot->client.quadrotor->isConnected()==1)
				{
					
						if(robot->typeConnection==0)
							break;
						
					//	robot->client.quadrotor->getData(data);
				//	robot->pointer.quadrotorSim->setData(data);
							
					
				}
				connectionLog->StateConnection(robot,false);
			}
		}
		robot->client.quadrotor->close();
	}
	
	else if(robot->getTipo()==N_WheeledBaseSim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_PatrolbotSim)
	{	
		connectionLog->AddConnection(robot);
		while(robot->typeConnection>0)
		{
			
			if(robot->client.kinect->connect(robot->client.Address.char_str(),robot->client.Port,false))
			{
				robot->client.Host=wxString(robot->client.wheeledBase->getHost());
				connectionLog->StateConnection(robot,true);
				Odometry odo;
				double speed=0;
				double rot=0;
				while(robot->client.wheeledBase->isConnected()==1)
				{
					
						if(robot->typeConnection==0)
							break;
						
			
						robot->pointer.wheeledbasesim->getSpeed(speed,rot);
						robot->client.wheeledBase->move(speed,rot);				
						//robot->pointer.wheeledbasesim->setLocation(odo.pose);	
				}
				connectionLog->StateConnection(robot,false);
			}
		}
		robot->client.wheeledBase->close();
	}


	else return NULL;
	return NULL;
}



void RobotConnection::Disconnect(NodeTree *robot)
{

		robot->typeConnection=0;
		Thread<RobotConnection>thid;
		HandleThreads(thid,robot,true);
		connectionLog->DeleteConnection(robot);
		
}



void RobotConnection::ShowConnLog(bool showLog)
{
	logVisible=showLog;
	
	if(logVisible)	connectionLog->Show();
	else			connectionLog->Show(false);
	
	window->SetFocus();
}

void RobotConnection::getIP(wxString &ip)
{
	wxIPV4address addr;
	addr.Hostname(wxGetFullHostName());
	ip = addr.IPAddress();
} 

void RobotConnection::HandleThreads(Thread<RobotConnection> thid,NodeTree *robot,bool close,bool server)
{
	if(!close)
	{
	v_threads.push_back(thid);
	r_connected.push_back(robot);
	if(server)	
		v_threads.back().Start(&RobotConnection::UpdateServerState,this,robot);
	else
		v_threads.back().Start(&RobotConnection::ConnectClient,this,robot);
	}
	else
	{	
	
		for(unsigned int i=0;i<r_connected.size();i++)
		{
			if(r_connected[i]==robot)
			{
				v_threads[i].Join();
				v_threads.erase(v_threads.begin()+i);
				r_connected.erase(r_connected.begin()+i);
			}
		}
				
			
	}

} 
