#ifndef __APOLO__PRISMWINDOW__H
#define __APOLO__PRISMWINDOW__H

#include "faceWidget.h"
#include "pointsList.h"
#include <wx/wx.h>


class PrismWindow : public wxPanel
{
public:
	PrismWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size);
	void CreatePanel();
	void OnButton(wxCommandEvent& event);
	void DrawBase(wxCommandEvent& event);
	void RefreshCanvas();
	~PrismWindow(void);

private:
	bool worldView;
	FaceWidget *base;
	wxButton *cView;
	NodeTree *node;
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__PRISMWINDOW__H