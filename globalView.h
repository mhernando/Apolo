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
#include "genericSlider.h"

#include "bitmaps/halfwaypoint.xpm"
#include "bitmaps/move.xpm"
#include "bitmaps/erase.xpm"
#include "bitmaps/icon_copy.xpm"
#include "bitmaps/PasteIcon.xpm"
#include "bitmaps/grid.xpm"



class PointsList;

class globalView : public wxFrame
{
	
public:
	globalView(wxWindow *parent,wxWindowID id,const wxString& title);
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
	void LoadFace(Face* loaded);
	Face* GetFace(){return face;};
	void ManageButtons(wxCommandEvent& event);
	void Paint(wxPaintEvent& event);
	void OnChangeZoom(wxCommandEvent& event);
	void DeleteFace();
	void PasteDesign(vector<Vector2D> CopiedFace);

private:
	wxPanel* Panel;
	DesignMine* Screen2D;
	PointsList *points;
	wxSpinCtrl* radioGrid;
	wxSlider* Vertical;
	wxSlider* Horizontal;
	GenericSlider* Zoom;
	Face *face;
	wxWindow* window;
	wxButton* Finish,*Cancel;
	DECLARE_EVENT_TABLE();
};


#endif // __APOLO__GLOBALVIEW__H