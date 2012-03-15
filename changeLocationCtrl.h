#ifndef __APOLO__CHANGE_LOCATION_CTRL__H
#define __APOLO__CHANGE_LOCATION_CTRL__H

#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/button.h>
#include "genericSlider.h"

class NodeTree;

class ChangeLocationCtrl : public wxDialog
{
public:
	ChangeLocationCtrl(wxWindow *parent, wxWindowID id, const wxString& title);
	void OnQuit(wxCommandEvent& event);
	void setItemData(NodeTree * node);
	void OnButton(wxCommandEvent& event);
	void OnValueChanged(wxCommandEvent& event);
	wxWindowID GetwinId(){return winId;}

private:
	wxWindowID winId;
	GenericSlider *control0;
	GenericSlider *control1;
	GenericSlider *control2;
	NodeTree * itemnode;
	wxPanel *panel;
	wxButton *accept;
	wxButton *cancel;
	double initialValues[3]; 
	DECLARE_EVENT_TABLE();
};

#endif // __APOLO__CHANGE_LOCATION_CTRL__H