#ifndef __APOLO__ROBOT_CONNECTION__H
#define __APOLO__ROBOT_CONNECTION__H

#include "mrcore.h"
#include "definitions.h"
#include "connectionLog.h"
#include "connectionParameters.h"
#include "nodeTree.h"
#include "wx/wx.h"
#include <wx/socket.h>




class RobotConnection :wxDialog

{
public:
	RobotConnection(wxWindow *parent,const wxString& name);
	void SendData(NodeTree *robot);
	void* ConnectClient(void* client);
	void* UpdateServerState(void *server);
	bool getLogVisible(){return logVisible;}
	ConnectionLog *getLog(){return connectionLog;}
	void ShowConnLog(bool showLog);
	void HandleThreads(Thread <RobotConnection>thid,NodeTree *robot,bool close=false,bool server=true);
	void getIP(wxString &ip);
	void Disconnect(NodeTree *robot);
	void ReceiveData(NodeTree *robot);
	



private:
	
	bool logVisible;
	wxWindow *window;
	vector<Thread<RobotConnection> > v_threads;
	vector<NodeTree*> r_connected;
	ConnectionLog *connectionLog;
	
	
	
	
};

#endif // __APOLO__ROBOT_CONNECTION__H
