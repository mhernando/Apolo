#ifndef __APOLO__INITIAL_PROPERTIES__H
#define __APOLO__INITIAL_PROPERTIES__H  

#include "mainWindow.h"
#include "positionableWidget.h"
#include "designWidget.h"

class PositionableWidget;

class InitialProperties:public wxDialog
{

public:
	InitialProperties(wxWindow *parent,NodeTree *obj,SimulatedWorld *s_world, const wxString& title);
	void OnButton(wxCommandEvent& event);
	int GetButtom(){return b_sel;};
	void OnClose(wxCloseEvent& event){b_sel=false; Destroy();};
//	void Update(wxWindowCreateEvent& event);
	void CreatePanel();

private:
	
	bool b_sel;
	wxButton *accept;
	wxButton *cancel;
	wxButton *df;
	SimulatedWorld *world;
	NodeTree *pos;
	Transformation3D t;
	string text;
	double x,y,z,r,p,yw;
	PositionableWidget *pw;
	double red,green,blue;
	

	DECLARE_EVENT_TABLE();
};


#endif