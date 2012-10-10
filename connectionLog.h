#ifndef __APOLO__CONNECTION_LOG__H
#define __APOLO__CONNECTION_LOG__H

#include "mrcore.h"
#include "nodeTree.h"
#include "wx/grid.h"
#include "wx/wx.h"
#include "definitions.h"




class ConnectionLog: public wxDialog

{
public:
	ConnectionLog(wxWindow *window,wxString name);
	void CreatePanel();
	void AddConnection(NodeTree *robot);
	void StateConnection(NodeTree *robot,bool connected, bool errorData=false);
	void DeleteConnection(NodeTree *robot);
	bool IsObjectConnected();
	wxGrid *grid;
	

private:
	vector<NodeTree*>log;
	wxString tittle;
	wxWindow *parent;
	double row,col;
	Mutex mutex;
	
	
};

#endif // __APOLO__CONNECTION_LOG__H