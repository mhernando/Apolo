#ifndef __APOLO__INITIAL_PROPERTIES__H
#define __APOLO__INITIAL_PROPERTIES__H  


#include "positionableWidget.h"
#include "designWidget.h"
#include "canvas.h"
#include "faceWindow.h"


class PositionableWidget;
class MainWindow;
class FaceWindow;
class FaceWidget;

class InitialProperties:public wxDialog
{

public:
	InitialProperties(wxWindow *parent,NodeTree *obj, const wxString& title,wxWindowID id=0);
	void OnButton(wxCommandEvent& event);
	bool GetButtom(){return b_sel;}
	void RefreshCanvas(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event){	b_sel=false; Destroy();}
	void CreatePanel();

private:
	
	FaceWindow *face;
	FaceWidget *base;
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
	wxWindowID wID;
	MainWindow* mainWin;
	

	DECLARE_EVENT_TABLE();
};


#endif