#ifndef __APOLO__PRISMWINDOW__H
#define __APOLO__PRISMWINDOW__H

#include "faceWidget.h"
#include "pointsList.h"
#include <wx/wx.h>
#include "childView.h"


class ChildView;
class PrismWindow : public wxPanel
{
public:
	PrismWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size);
	void CreatePanel();
	void OnButton(wxCommandEvent& event);
	void DrawBase(wxCommandEvent& event);
	void ChangeZoom(wxCommandEvent& event);
	void Move(wxCommandEvent& event);
	void RefreshCanvas();
	~PrismWindow(void);


private:
	double xi,yi,xf,yf;
	bool worldView;
	FaceWidget* base;   
	wxButton *cView,*Up,*Down,*Left,*Right;
	wxSlider* Zoom; 
	NodeTree *node;
	PointsList *points;  
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__PRISMWINDOW__H