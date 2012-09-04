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
	
	if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_Pioneer3ATSim || robot->getTipo()==N_LaserSensorSim )
		robot->server.laserSensor->init(atoi(port),atoi(address_client),true);
	if(robot->getTipo()==N_LaserSensor3DSim) 
		robot->server.laserSensor3D->init(atoi(port),atoi(address_client),true);
	if(robot->getTipo()==N_WheeledBaseSim) 
		robot->server.wheeledBase->init(atoi(port),atoi(address_client),true);
	if(robot->getTipo()==N_CameraSim)
		robot->server.camera->init(atoi(port),atoi(address_client),true);
	if(robot->getTipo()==N_KinectSim)
		robot->server.kinect->init(atoi(port),atoi(address_client),true);
	if(robot->getTipo()==N_QuadrotorSim)
		robot->server.quadrotor->init(atoi(port),atoi(address_client),true);
	
	robot->typeConnection=1;
	connectionLog->AddConnection(robot);
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
void*  RobotConnection::ConnectClient(void *client)
{
	NodeTree* robot=(NodeTree*)client;
	
	
	if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS200Sim || robot->getTipo()==N_LaserSensorSim )
	{
		
		connectionLog->AddConnection(robot);
		LaserData data;
		
		
		while(1)
		{
			if(robot->client.laserSensor->connect(robot->client.Address,robot->client.Port,false))
			{
				robot->client.Host=robot->client.laserSensor->getHost();
				connectionLog->StateConnection(robot,true);
				robot->pointer.lms100sim->setActive(false);
				while(1)
				{
					if(!robot->client.laserSensor->isConnected())
					{
						connectionLog->StateConnection(robot,false);
						break;
					}
					
					robot->client.laserSensor->getData(data);
					Sleep(1000);
					
					robot->pointer.lms100sim->setData(data);
					data.clear();
					
				
				}
				
				
			}
			if(robot->typeConnection==0)
					break;
		}

	}

/*		
	else if(robot->getTipo()==N_LaserSensor3DSim)
	{
		robot->client.laserSensor3D->connect(adress_box->GetValue(),atoi(port_box->GetValue()));
		{	LaserData3D data3D;
			while(1)
			{
				if(!robot->client.laserSensor3D->isConnected()) break;
				robot->client.laserSensor3D->getData(data3D);	
				robot->pointer.lasersensor3dsim->setData(data3D);
			}
			robot->client.laserSensor3D->close();		
		
		}
	}
	else if(robot->getTipo()==N_CameraSim)
	{
		robot->client.camera->connect(adress_box->GetValue(),atoi(port_box->GetValue()));
		{	Image image;
			while(1)
			{
				if(!robot->client.camera->isConnected()) break;
				robot->client.camera->getImage(image);	
				robot->pointer.cameraSim->setImage(image);
			}
			robot->client.camera->close();		
		
		}
	}
	else if(robot->getTipo()==N_KinectSim)
	{
		robot->client.kinect->connect(adress_box->GetValue(),atoi(port_box->GetValue()));
		{	PointCloud cloud;
			while(1)
			{
				if(!robot->client.kinect->isConnected()) break;
				robot->client.kinect->getData(cloud);
				robot->pointer.kinectSim->setData(cloud);
			}
			robot->client.kinect->close();		
		
		}

	}
	if(robot->getTipo()==N_QuadrotorSim)
	{
		robot->client.quadrotor->connect(adress_box->GetValue(),atoi(port_box->GetValue()));
		{	QuadrotorData q_data;
			while(1)
			{
				if(!robot->client.quadrotor->isConnected()) break;
					
				//robot->pointer.quadrotorSim->setData(robot->client.quadrotor->getData(q_data))
			}
			robot->client.quadrotor->close();
			//return;
		
		}
	}
	if(robot->getTipo()==N_WheeledBaseSim)
	{
		robot->client.wheeledBase->connect(adress_box->GetValue(),atoi(port_box->GetValue()));
		{	
			Odometry odo;
			Pose3D pose3D;
			
			while(1)
			{
				if(robot->client.wheeledBase->isConnected()) break;
					
				//robot->pointer.wheeledbasesim->setData(robot->client.wheeledBase->getPose3D(pose3D));
				//añadir odometry

			}
			robot->client.wheeledBase->close();		
			
		
		}
	

	}
	
	
	*/

return NULL;
}






void RobotConnection::CloseServer(NodeTree *robot)
{
		if(robot->getTipo()==N_LMS100Sim) robot->server.laserSensor->close();
		else if(robot->getTipo()==N_LaserSensor3DSim)robot->server.laserSensor3D->close();	
		else if(robot->getTipo()==N_WheeledBaseSim) robot->server.wheeledBase->close(); 
		else if(robot->getTipo()==N_CameraSim)robot->server.camera->close();
		else if(robot->getTipo()==N_KinectSim)robot->server.kinect->close();
		else if(robot->getTipo()==N_QuadrotorSim)robot->server.quadrotor->close();
		robot->typeConnection=0;
		connectionLog->DeleteConnection(robot);
}

void RobotConnection::DisconnectClient(NodeTree *robot)
{
		if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LaserSensorSim ) robot->client.laserSensor->close();
		if(robot->getTipo()==N_LaserSensor3DSim)robot->client.laserSensor3D->close();	
		if(robot->getTipo()==N_WheeledBaseSim) robot->client.wheeledBase->close(); 
		if(robot->getTipo()==N_CameraSim)robot->client.camera->close();
		if(robot->getTipo()==N_KinectSim)robot->client.kinect->close();
		if(robot->getTipo()==N_QuadrotorSim)robot->client.quadrotor->close();
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

