#include "robotConnection.h"
#include "simulatedWorld.h"
#include "pthread.h"

BEGIN_EVENT_TABLE(RobotConnection,wxDialog)
	EVT_BUTTON(ID_ACCEPT, RobotConnection::OnButton)
	EVT_BUTTON(ID_CANCEL, RobotConnection::OnButton)
	EVT_BUTTON(ID_DEFAULT, RobotConnection::OnButton)
	EVT_CLOSE(RobotConnection::OnClose)
END_EVENT_TABLE()

wxTextCtrl *RobotConnection::adress_box;
wxTextCtrl *RobotConnection::port_box;
wxTextCtrl *RobotConnection::clients_box;
ConnectionLog *RobotConnection::connectionLog;


//void* RobotConnection::ConnectClient(void* client);


RobotConnection::RobotConnection(wxWindow *parent,const wxString &name)
:wxDialog(parent,wxID_ANY,name,wxPoint(10,10),wxSize(200,400))
{
	cancel=false;
	window=parent;
	defPort=wxT("80");
	defAdress=wxT("127.0.0.1");
	defClients=wxT("5");
	logVisible=true;
	connectionLog=new ConnectionLog(this,wxT("Connection Log"));
	CreatePanel();
	
	
	
	

}
void RobotConnection::CreatePanel()
{
	wxStaticText *adress_text,*clients_text,*port_text;
	
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bbox = new wxBoxSizer(wxHORIZONTAL);
	b_default = new wxButton(this,ID_DEFAULT,wxT("Default Values"),wxDefaultPosition,wxDefaultSize);
	
	port_text = new wxStaticText(this, wxID_ANY, wxT("Port :    "),wxDefaultPosition,wxDefaultSize);
	port_box = new wxTextCtrl(this,wxID_ANY,defPort,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
	
	adress_text = new wxStaticText(this, wxID_ANY, wxT("Adress :      "),wxDefaultPosition,wxDefaultSize);
	adress_box = new wxTextCtrl(this,wxID_ANY,defAdress,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);

	clients_text = new wxStaticText(this, wxID_ANY, wxT("Max. Clients :    "),wxDefaultPosition,wxDefaultSize);
	clients_box = new wxTextCtrl(this,wxID_ANY,defClients,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);

	b_accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxDefaultSize);
	b_cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxDefaultSize);
	
	
	
	vbox->Add(b_default,0,wxEXPAND|wxALL,10);
	vbox->Add(port_text,0,wxLEFT,10);
	vbox->Add(port_box,0,wxEXPAND |wxALL,10);
	vbox->Add(adress_text,0,wxLEFT,10);
	vbox->Add(adress_box,0,wxEXPAND | wxALIGN_LEFT|wxALL,10);
	vbox->Add(clients_text,0,wxLEFT,10);
	vbox->Add(clients_box,0,wxEXPAND | wxALIGN_LEFT|wxALL,10);
	bbox->Add(b_accept,1);
	bbox->Add(b_cancel,1);
	vbox->Add(bbox,1,wxEXPAND |wxALL,5);
	
	

	SetSizer(vbox);
	vbox->SetSizeHints(this);
	
	
}
void RobotConnection::SendData(NodeTree *robot)
{
	cancel=false;
	ShowModal();
	if(cancel)
		return;
	
	if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LaserSensorSim )
		robot->server.laserSensor->init(atoi(port_box->GetValue()),atoi(clients_box->GetValue()),true,adress_box->GetValue());
	if(robot->getTipo()==N_LaserSensor3DSim) 
		robot->server.laserSensor3D->init(atoi(port_box->GetValue()),atoi(clients_box->GetValue()),true,adress_box->GetValue());
	if(robot->getTipo()==N_WheeledBaseSim) 
		robot->server.wheeledBase->init(atoi(port_box->GetValue()),atoi(clients_box->GetValue()),true,adress_box->GetValue());
	if(robot->getTipo()==N_CameraSim)
		robot->server.camera->init(atoi(port_box->GetValue()),atoi(clients_box->GetValue()),true,adress_box->GetValue());
	if(robot->getTipo()==N_KinectSim)
		robot->server.kinect->init(atoi(port_box->GetValue()),atoi(clients_box->GetValue()),true,adress_box->GetValue());
	if(robot->getTipo()==N_QuadrotorSim)
		robot->server.quadrotor->init(atoi(port_box->GetValue()),atoi(clients_box->GetValue()),true,adress_box->GetValue());
	
	robot->typeConnection=1;
	connectionLog->AddConnection(robot);
	ShowConnLog(logVisible);

}

void RobotConnection::ReceiveData(NodeTree *robot)
{
	cancel=false;
	ShowModal();
	if(cancel)
		return;
	
	robot->typeConnection=2;
	pthread_t thid;
	pthread_create(&thid,NULL,ConnectClient,robot);
	ShowConnLog(logVisible);
	
	
}
 void*  RobotConnection::ConnectClient(void *client)
{
	NodeTree* robot=(NodeTree*)client;
	
	if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LaserSensorSim )
	{
		robot->pointer.lms100sim->setLaserProperties(0,0,0,0,0);
		connectionLog->AddConnection(robot);
		LaserData data;
		while(1)
		{
			if(robot->client.laserSensor->connect(adress_box->GetValue(),atoi(port_box->GetValue()),false))
			{
				robot->client.getAddress=robot->client.laserSensor->getAddress();
				robot->client.getPort=robot->client.laserSensor->getPort();
				robot->client.getHost=robot->client.laserSensor->getHost();
				connectionLog->StateConnection(robot,true);
				
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
					
				
				}
				
				if(robot->typeConnection==0)
				break;
			}
			
		}

	}

		
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
			Pose3D pose3D;
			while(1)
			{
				if(robot->client.wheeledBase->isConnected()) break;
					
			//	robot->pointer.wheeledbasesim->setData(robot->client.wheeledBase->getPose3D(pose3D));
			//	añadir odometry

			}
			robot->client.wheeledBase->close();		
			
		
		}
	

	}
	
	
	

return NULL;
}






void RobotConnection::CloseServer(NodeTree *robot)
{
		if(robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LMS100Sim || robot->getTipo()==N_LaserSensorSim ) robot->server.laserSensor->close();
		if(robot->getTipo()==N_LaserSensor3DSim)robot->server.laserSensor3D->close();	
		if(robot->getTipo()==N_WheeledBaseSim) robot->server.wheeledBase->close(); 
		if(robot->getTipo()==N_CameraSim)robot->server.camera->close();
		if(robot->getTipo()==N_KinectSim)robot->server.kinect->close();
		if(robot->getTipo()==N_QuadrotorSim)robot->server.quadrotor->close();
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

void RobotConnection::OnButton(wxCommandEvent& event)
{
	int id=event.GetId();
	if(id==ID_ACCEPT) Show(false);
	if(id==ID_CANCEL) Close();	
	if(id==ID_DEFAULT)
	{
		port_box->SetValue(defPort);
		adress_box->SetValue(defAdress);
		clients_box->SetValue(defClients);
		Show(false);
	}
}

void RobotConnection::OnClose(wxCloseEvent& event)
{
	cancel=true;
	Show(false);
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