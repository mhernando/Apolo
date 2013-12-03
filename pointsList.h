#ifndef __APOLO__POINTS_LIST__H
#define __APOLO__POINTS_LIST__H

#include "mrcore.h"
#include "wx/grid.h"
#include "wx/wx.h"
#include "faceWidget.h"
#include "ChangeVertex.h"





class globalView;
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
	void AssociateDesign2D(globalView *Design2D);
	Vector2D getLastPointAdded(){return lastPoint;}
	Vector2D getChangePointAdded(){return changePoint;}
	Vector2D getDeletePointAdded(){return deletePoint;}
	Vector2D getMovedPointAdded(){return movedPoint;}

	double getAuxRow(){return auxrow;} 
	void ChangePoint();
	void OnMenuChangePoint(wxCommandEvent& event);
	void DeletePoint ();

	void DeletePoints();

	void DeletePointMarked(int r);
	void OnMenuDeletePoint(wxCommandEvent& event);
	void OnItemMenu(wxGridEvent& event);
	void RefreshGrid();
	int CheckPoint(double x,double y);
	void MovedPoint(int rowtochange,double x,double y);
	void InsertedPoint(int firts,int second,double x,double y);
	void MovedPoints();
	void MarkRow(int MarkedRow);
	void SetDesignAssociated(bool aux){designAssociated=aux;};
	wxGrid *grid;
	

private:
	FaceWidget *faces;
	globalView *Screen2D;
	bool facesAssociated,designAssociated;
	wxString name;
	wxWindow *parent;
	ChangeVertex *changeVertex;
	double row,col,auxrow; 
	Vector2D lastPoint, changePoint,deletePoint,movedPoint;

	
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__POINTS_LIST__H