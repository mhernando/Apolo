#ifndef __APOLO__INITIAL_PROPERTIES__H
#define __APOLO__INITIAL_PROPERTIES__H  


#include "positionableWidget.h"
#include "designWidget.h"
#include "prismWindow.h"
#include "canvas.h"
#include "faceWindow.h"


class PositionableWidget;
class MainWindow;
class PrismWindow;
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
	
	FaceWidget *base;
	wxButton *cView;
	bool b_sel, worldView;
	NodeTree *node;	
	DesignWidget *dp;
	PrismWindow *priW;
	PositionableWidget *pw;
	wxString defName;
	wxWindowID wID;
	MainWindow* mainWin;
	FaceWindow *face;
	

	DECLARE_EVENT_TABLE();
};


#endif