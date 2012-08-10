#ifndef __APOLO__FACEWINDOW__H
#define __APOLO__FACEWINDOW__H

#include "mainWindow.h"
#include <wx/wx.h>
#include <wx/splitter.h>
#include "nodeTree.h"
#include "simulatedWorld.h"
#include "pointsList.h"
#include "canvas.h"
#include "genericSlider.h"
#include "mrcore.h"



class FaceWindow : public wxPanel
{
public:
	FaceWindow(wxWindow *parent,NodeTree *obj,SimulatedWorld *simu, const wxWindowID id, const wxString& title, const wxPoint& pos,const wxSize& size);
	///void OnSashDrag(wxSashEvent& event);
	void CreatePanel();
	void GetPoint(wxCommandEvent& event);
	wxWindow *GetParentWorld(){ return parentCurrentWorld;};
	void AddVertex(wxCommandEvent &event);
	void FaceOrientation(wxCommandEvent& WXUNUSED(event));
	void CreateFace();
	void FaceButton(wxCommandEvent& event);
	//void  RadioClicked(wxRadioEvent& event)
	void  ColorChanged(wxCommandEvent& event);
	
private:

	wxRadioBox *radio;
	GenericSlider *roll,*pitch;
	wxTextCtrl *transp_box;
	wxButton *af;
	wxPoint point;
	Face *face,*faceCopy;
	wxSplitterWindow *drawFace;
	SimulatedWorld *world;
	Canvas *canvas1,*canvas2;
	PointsList *points;
	wxWindow *parentCurrentWorld;
	NodeTree *node;
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWINDOW__H