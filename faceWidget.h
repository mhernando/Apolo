#ifndef __APOLO__FACEWIDGET__H
#define __APOLO__FACEWIDGET__H

#include "mainWindow.h"
#include <wx/wx.h>
#include <wx/splitter.h>
#include "simulatedWorld.h"
#include "pointsList.h"
#include "canvas.h"
#include "mrcore.h"



class FaceWidget : public wxPanel
{
public:
	FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal=true);
	void CreatePanel();
	void RefreshCanvas();
	void GetPoint(wxCommandEvent& event);
	void AddVertex();
	void OnChangeSplitter(wxSplitterEvent &event);
	void SetAlign(bool al){align=al;};
	bool GetAlign(){return align;};
	Canvas *GetCanvas3D(){return canvas2;}
	void AssociatePointTable(PointsList *point);
	void CreateFace();
	Face* GetFace() {return face;};

	

	
private:
	bool created;
	bool tableAssociated;
	bool align,h;
	wxWindow *window;
	float x,y;
	Face *face,*faceCopy;
	PointsList *points;
	SimulatedWorld *world;
	wxPoint point;
	Canvas *canvas1,*canvas2;
	wxSplitterWindow *canvas;
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWIDGET__H