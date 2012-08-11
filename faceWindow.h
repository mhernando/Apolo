#ifndef __APOLO__FACEWINDOW__H
#define __APOLO__FACEWINDOW__H

#include "mainWindow.h"
#include <wx/wx.h>
#include <wx/tglbtn.h>
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
	wxWindow *GetParentWorld(){ return parentCurrentWorld;};
	void CreatePanel();
	void GetPoint(wxCommandEvent& event);
	void AddVertex(wxCommandEvent &event);
	void FaceOrientation(wxCommandEvent& WXUNUSED(event));
	void CreateFace();
	void FaceButton(wxCommandEvent& event);
	void  ColorChanged(wxCommandEvent& event);
	
private:

	wxRadioBox *trans,*align;
	GenericSlider *roll,*pitch,*plane_dis;
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