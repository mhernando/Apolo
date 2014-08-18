#ifndef __APOLO__INITIAL_PROPERTIES__H
#define __APOLO__INITIAL_PROPERTIES__H  




#include "prismWindow.h"
#include "canvas.h"
#include "faceWindow.h"
#include "CreateComposed.h"
#include "designWidget.h"



class MainWindow;
class PrismWindow;
class FaceWindow;
class CreateComposed;
class PositionableWidget;


class InitialProperties:public wxDialog
{

public:
	InitialProperties(wxWindow *parent,NodeTree *obj, const wxString& title,wxWindowID id=0);
	void OnButton(wxCommandEvent& event);
	bool GetButtom(){return b_sel;}
	void OnClose(wxCloseEvent &event){	b_sel=false; Destroy();}
	void CreatePanel();
	FaceWindow* getface(){return face;}

private:

	wxButton *cView;
	bool b_sel, worldView;
	NodeTree *node;	
	PrismWindow *priW;
	wxWindowID wID;
	MainWindow* mainWin;
	FaceWindow *face;
	CreateComposed* comp;
	PositionableWidget *pw;
	DesignWidget *dp;
	wxString defName;


	DECLARE_EVENT_TABLE();
};


#endif