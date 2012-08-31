#ifndef __APOLO__ROBOT_CONNECTION__H
#define __APOLO__ROBOT_CONNECTION__H

#include "mrcore.h"
#include "definitions.h"
#include "connectionLog.h"
#include "nodeTree.h"
#include "wx/wx.h"




class RobotConnection :wxDialog

{
public:
	RobotConnection(wxWindow *parent,const wxString& name);
	void SendData(NodeTree *robot);
	void CloseServer(NodeTree *robot);
	void CreatePanel();
	void OnClose(wxCloseEvent& event);
	void OnButton(wxCommandEvent& event);
	static 	void *ConnectClient(void* client);
	bool getLogVisible(){return logVisible;}
	void ShowConnLog(bool showLog);
	void DisconnectClient(NodeTree *robot);
	void ReceiveData(NodeTree *robot);
	



private:
	bool cancel;
	bool logVisible;
	wxWindow *window;
	LaserSensorServer *laser_serv;
	LaserSensor3DServer *laser3D_serv;
	static ConnectionLog *connectionLog;
	static wxTextCtrl *port_box, *adress_box,*clients_box;
	wxButton *b_accept,*b_cancel,*b_default;
	wxDialog *dialog;
	wxString defPort,defAdress,defClients;
	
	DECLARE_EVENT_TABLE();
};

#endif // __APOLO__ROBOT_CONNECTION__H
