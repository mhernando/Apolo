#ifndef __APOLO__POINTS_LIST__H
#define __APOLO__POINTS_LIST__H

#include "mrcore.h"
#include "wx/grid.h"
#include "wx/wx.h"


DECLARE_EVENT_TYPE(wxEVT_POINT_ADDED, -1)

class PointsList: public wxPanel

{
public:
	PointsList(wxWindow *window,wxWindowID id,const wxString label,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize);
	void CreatePanel();
	void SetPoints(double x, double y);
	void SetVertex(int r);
	void OnChange(wxGridEvent& event);
	Vector2D getLastPointAdded(){return lastPoint;};
	void RefreshGrid();
	wxGrid *grid;


private:
	wxString name;
	wxWindow *parent;
	double row,col;
	Vector2D lastPoint;
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__POINTS_LIST__H