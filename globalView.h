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
#include "bitmaps/zoom.xpm"
#include "bitmaps/align.xpm"
#include "bitmaps/SceneSize.xpm"
#include "bitmaps/DesignSize.xpm"
#include "bitmaps/CancelIcon.xpm"
#include "bitmaps/AcceptIcon.xpm"


DECLARE_EVENT_TYPE(wxEVT_EDIT_CLOSED, -1)

class PointsList;

class globalView : public wxFrame
{
	
public:

	globalView(wxWindow *parent,wxWindowID id,const wxString& title);

	void CreatePanel();
	void CreateFace();
	void InitToolbar();
	void InsertSizes();
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
	void OnClose(wxCloseEvent& event);
	void LoadFace(Face loaded);
	Face* GetFace(){return face;};
	void ManageButtons(wxCommandEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnChangeZoom(wxCommandEvent& event);
	void DeleteFace();
	void PasteDesign(vector<Vector2D> CopiedFace);
	void SetCanvasSize(wxCommandEvent& event);
	void UpdateSlidersValues();
	void ManageSliders();


private:
	wxPanel* Panel;
	DesignMine* Screen2D;
	PointsList *points;
	wxToolBar* Toolbar;
	wxSpinCtrl* radioGrid;
	wxSlider* Vertical;
	wxSlider* Horizontal;
	wxSlider *slidZoom;
	Face *face;
	wxWindow* window;
	wxBitmapButton* Accept,*Cancel;
	int newSize;
	wxTextCtrl *ZoomValue;
	bool initializing;
	int xSize,ySize;
	wxComboBox* selectSize;
	wxArrayString Sizes;

	DECLARE_EVENT_TABLE();
};


#endif // __APOLO__GLOBALVIEW__H