#ifndef __APOLO__GLOBALVIEW__H
#define __APOLO__GLOBALVIEW__H

#include <wx/button.h>
#include "canvas.h"
#include <wx/panel.h>
#include "faceWidget.h"
#include "nodeTree.h"
#include "simulatedWorld.h"
#include "mrcore.h"
#include "designMine.h"

#include "bitmaps/halfwaypoint.xpm"
#include "bitmaps/move.xpm"
#include "bitmaps/erase.xpm"


class PointsList;

class globalView : public wxFrame
{
	
public:
	globalView(wxWindow *parent, wxWindowID id,const wxString& title);
	void CreatePanel();
	void CreateFace();
	DesignMine* GetScreen2D(){return Screen2D;};
	void ChangeZoom(wxCommandEvent& event);
	void FaceAlign(wxCommandEvent& event);
	void ChangeGridSize(wxSpinEvent& event);
	void ChangePosition(wxCommandEvent& event);
	void AddedMousePoint(wxCommandEvent& event);
	void InsertPoint(wxCommandEvent& event);
	void ChangePoint(wxCommandEvent& event);
	void DeletePoint(wxCommandEvent& event);
	void ChangePolygonPosition(wxCommandEvent& event);
	void ManagePoints(bool addPoint=true, bool changePoint=false, bool deletePoint=false, int deleteRow=0);
	Face* GetFace(){return face;};
	void ManageButtons(wxCommandEvent& event);
	void Paint(wxPaintEvent& event);

private:
	wxPanel* Panel;
	DesignMine* Screen2D;
	PointsList *points;
	wxSlider* Zoom; 
	wxSpinCtrl* radioGrid;
	wxSlider* Vertical;
	wxSlider* Horizontal;
	Face *face;
	wxWindow* window;
	wxButton* Finish;

	DECLARE_EVENT_TABLE();
};


#endif // __APOLO__GLOBALVIEW__H