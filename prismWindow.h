#ifndef __APOLO__PRISMWINDOW__H
#define __APOLO__PRISMWINDOW__H


#include "pointsList.h"
#include <wx/wx.h>
#include "childView.h"
#include "globalView.h"


class ChildView;
class FaceControlButtons;

class PrismWindow : public wxPanel
{
public:
	PrismWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size);
	void CreatePanel();
	void OnButton(wxCommandEvent& event);
	void DrawBase(wxCommandEvent& event);
	void RefreshCanvas();
	void DesignAlign(wxCommandEvent& event);
	~PrismWindow(void);


private:
	double xi,yi,xf,yf;
	bool worldView;
	FaceWidget* base;
	wxButton *cView;
	NodeTree *node;
	PointsList *points; 
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__PRISMWINDOW__H