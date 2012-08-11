#ifndef __APOLO__INITIAL_PROPERTIES__H
#define __APOLO__INITIAL_PROPERTIES__H  

#include "mainWindow.h"
#include "positionableWidget.h"
#include "designWidget.h"
#include "canvas.h"
#include "faceWindow.h"


class PositionableWidget;
class FaceWindow;
class InitialProperties:public wxDialog
{

public:
	InitialProperties(wxWindow *parent,NodeTree *obj,SimulatedWorld *s_world, const wxString& title);
	void OnButton(wxCommandEvent& event);
	int GetButtom(){return b_sel;};
	void OnClose(wxCloseEvent& event);
//	void Update(wxWindowCreateEvent& event);
	void CreatePanel();

private:
	
	FaceWindow *face;
	Canvas *canvas1,*canvas2;
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
	DesignWidget *dp;
	wxString defName;
	double red,green,blue;
	

	DECLARE_EVENT_TABLE();
};


#endif