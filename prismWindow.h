#ifndef __APOLO__PRISMWINDOW__H
#define __APOLO__PRISMWINDOW__H



#include "globalView.h"
#include "designWidget.h"
#include "positionableWidget.h"


class ChildView;
class FaceControlButtons;
class PositionableWidget;


class PrismWindow : public wxPanel
{
public:
	PrismWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size);
	void CreatePanel();
	void OnButton(wxCommandEvent& event);
	void RefreshCanvas(wxCommandEvent &event);
	void ColorChanged(wxCommandEvent& event);
	void CopyDesign(wxCommandEvent& event);
	void PasteDesign(wxCommandEvent& event);
	~PrismWindow(void);


private:

	bool worldView;
	FaceWidget* base;
	PositionableWidget *pw;
	wxString defName;
	DesignWidget *dp;
	wxButton *cView;
	NodeTree *node;
	DECLARE_EVENT_TABLE();
};
#endif // __APOLO__PRISMWINDOW__H