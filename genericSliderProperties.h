#ifndef __APOLO__GENERIC_SLIDER_PROPERTIES__H
#define __APOLO__GENERIC_SLIDER_PROPERTIES__H

#include "definitions.h"
#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/colordlg.h>
#include <wx/checkbox.h>
#include "bitmaps/colour.xpm"
#include "bitmaps/cursorDown.xpm"
#include "bitmaps/cursorUp.xpm"

class GenericSlider;

class GenericSliderProperties: public wxDialog
{
public:
	GenericSliderProperties(wxPanel *parent,wxWindowID id,const wxString& title);
	void OnEnter(wxCommandEvent& event);
	void OnCheck(wxCommandEvent& event);
	void OnButton(wxCommandEvent& event);
	void UpDateProperties();
	void setMin(double val);
	void setMax(double val);
	void setTick(double val);
	
private:
	GenericSlider* winSlider;
	bool isDecimal;
	double val;
	double incr;
	double min,max,minOld,maxOld;
	int tick,tickOld;
	wxColour colorPanel,colorPanelOld;
	wxCheckBox* box;
	wxButton* accept;
	wxButton* cancel;
	wxButton* apply;
	wxStaticText* min_text;
	wxStaticText* max_text;
	wxStaticText* tick_text;
	wxStaticText* color_text;
	wxTextCtrl* min_box;
	wxTextCtrl* max_box;
	wxTextCtrl* tick_box;
	wxBitmapButton* min_up;
	wxBitmapButton* min_down;
	wxBitmapButton* max_up;
	wxBitmapButton* max_down;
	wxBitmapButton* tick_up;
	wxBitmapButton* tick_down;
	wxBitmapButton* color_box;

	DECLARE_EVENT_TABLE();
};

#endif //__APOLO__GENERIC_SLIDER_PROPERTIES__H