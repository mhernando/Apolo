#ifndef __APOLO__FACEWINDOW__H
#define __APOLO__FACEWINDOW__H

#include "mainWindow.h"
#include "mrcore.h"
#include "faceWidget.h"

#include "nodeTree.h"
#include "simulatedWorld.h"


#include "canvas.h"
#include "genericSlider.h"
#include <wx/grid.h>
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/splitter.h>

class FaceWidget;
class PointsList;
class MainWindow;


class FaceWindow : public wxPanel
{
public:
	FaceWindow(wxWindow *parent,NodeTree *obj, const wxString& title, const wxPoint& pos,const wxSize& size);
	void CreatePanel();
	void FaceOrientation(wxCommandEvent& event);
	void FaceButton(wxCommandEvent& event);
	void  ColorChanged(wxCommandEvent& event);
	void FaceAlign(wxCommandEvent& event);
	
private:
	
	bool worldView;
	double red,green,blue;
	wxRadioBox *align;
	FaceWidget *canvas;
	GenericSlider *roll,*pitch,*plane_dis,*transparency;
	wxTextCtrl *transp_box;
	wxButton *af,*cView;
	wxPoint point;
	PointsList *points;
	wxSplitterWindow *drawFace;
	SimulatedWorld *world;
	NodeTree *node;
	MainWindow* mainWin;
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWINDOW__H