#ifndef __APOLO__POINTS_LIST__H
#define __APOLO__POINTS_LIST__H

#include "mrcore.h"
#include "wx/grid.h"
#include "wx/wx.h"
#include "faceWidget.h"
#include "changeVertex.h"

//DECLARE_EVENT_TYPE(wxEVT_POINT_ADDED, -1)
class FaceWidget;
class ChangeVertex;
class PointsList: public wxPanel
{
public:
	PointsList(wxWindow *window,const wxString label=wxEmptyString,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize);
	void CreatePanel();
	wxWindow *getParent(){return parent;}
	void SetPoints(double x, double y);
	void SetVertex(int r);
	void OnChange(wxGridEvent& event);
	void AssociateFace(FaceWidget *face);
	Vector2D getLastPointAdded(){return lastPoint;}
	Vector2D getChangePointAdded(){return changePoint;}
	Vector2D getDeletePointAdded(){return deletePoint;}
	double getAuxRow(){return auxrow;} 
	void ChangePoint();
	void OnMenuChangePoint(wxCommandEvent& event);
	void DeletePoint ();
	void OnMenuDeletePoint(wxCommandEvent& event);
	void OnItemMenu(wxGridEvent& event);
	void RefreshGrid();
	wxGrid *grid;
	

private:
	FaceWidget *faces;
	bool facesAssociated;
	wxString name;
	wxWindow *parent;
	ChangeVertex *changeVertex;
	double row,col,auxrow; 
	Vector2D lastPoint, changePoint,deletePoint;

	
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__POINTS_LIST__H
