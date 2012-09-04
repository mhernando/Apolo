#ifndef __APOLO__ROBOT_CONNECTION__H
#define __APOLO__ROBOT_CONNECTION__H

#include "mrcore.h"
#include "definitions.h"
#include "connectionLog.h"
#include "connectionParameters.h"
#include "nodeTree.h"
#include "wx/wx.h"




class RobotConnection :wxDialog

{
public:
	RobotConnection(wxWindow *parent,const wxString& name);
	void SendData(NodeTree *robot);
	void CloseServer(NodeTree *robot);
	void *ConnectClient(void* client);
	bool getLogVisible(){return logVisible;}
	ConnectionLog *getLog(){return connectionLog;}
	void ShowConnLog(bool showLog);
	void DisconnectClient(NodeTree *robot);
	void ReceiveData(NodeTree *robot);
	



private:
	
	bool logVisible;
	wxWindow *window;
	LaserSensorServer *laser_serv;
	Thread<RobotConnection> client_Thid;

	LaserSensor3DServer *laser3D_serv;
	ConnectionLog *connectionLog;
	wxDialog *dialog;
	
	
	
};

#endif // __APOLO__ROBOT_CONNECTION__H
