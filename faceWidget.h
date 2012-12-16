#ifndef __APOLO__FACEWIDGET__H
#define __APOLO__FACEWIDGET__H

#include "mainWindow.h"
#include <wx/wx.h>
#include <wx/splitter.h>
#include "simulatedWorld.h"
#include "pointsList.h"
#include "canvas.h"
#include "mrcore.h"
#include "faceDesign.h"

DECLARE_EVENT_TYPE(wxEVT_POINT_ADDED, -1)

class FaceWidget : public wxPanel
{
public:
	FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal=true, bool pre=false);
	void CreatePanel();
	void RefreshCanvas();
	void GetPoint(wxCommandEvent& event);
	void SetVertex(bool addvertex=true,bool changevertex=false,bool deletevertex=false,int deleteRow=0);
	void OnChangeSplitter(wxSplitterEvent &event);
	void SetAlign(bool al){align=al;};
	bool GetAlign(){return align;};
	Canvas *GetCanvas3D(){return canvas2;}
	void AssociatePointTable(PointsList *point);
	void ChangeView(bool world);
	void CreateFace();
	Face* GetFace() {return face;};
	Canvas *canvas2;
	FaceDesign* design1;

	
private:
	bool tableAssociated;
	bool align,h;
	bool worldView;
	bool noPreliminar3D;
	double x,y;
	Face *face,*faceCopy;
	PointsList *points;
	SimulatedWorld *world;
	//Poner las canvas aquí después de la corrección
	wxSplitterWindow* canvas;
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWIDGET__H