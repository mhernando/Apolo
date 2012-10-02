#ifndef __APOLO__DESIGN_PROPERTIES__H
#define __APOLO__DESIGN_PROPERTIES__H  



#include "definitions.h"
#include "nodeTree.h"
#include "simulatedWorld.h"
//#include "designWidget.h"

class DesignWidget;
class MainWindow;
//

class DesignProperties:public wxDialog
{

public:
	DesignProperties(wxWindow *parent,NodeTree *obj,const wxString& title);
	void OnButton(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void CreatePanel();

private:
	
	DesignWidget *dign;
	wxButton *accept;
	wxButton *cancel;
	NodeTree *node;
	SimulatedWorld *world;
	MainWindow* mainWin;

	
	

	DECLARE_EVENT_TABLE();
};


#endif