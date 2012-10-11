#ifndef __APOLO__DESIGN_WIDGET__H
#define __APOLO__DESIGN_WIDGET__H


#include "nodeTree.h"
#include "simulatedWorld.h"
#include "genericSlider.h"
#include "canvas.h"


class DesignWidget: public wxPanel

{
public:
	DesignWidget(wxWindow *window,NodeTree *pose,const wxString label,const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,bool sliders = true,bool orient=false);
	void SetSpecificValues(bool def=false);
	void OnValueChanged(wxCommandEvent& event){SetSpecificValues();}
	void CreatePanel(bool sliders,bool orientation);
	void GetDefaultValues();


private:
	GenericSlider *c_radio;
	GenericSlider *c_height;
	GenericSlider *c_vertex;
	Canvas *base;
	TypeNode type;
	double defRadio,defHeight,defVertex;
	bool slider,orientation;
	double r,h,v;
	NodeTree *nodeW;
	wxStaticText *r_text;
	wxTextCtrl	*r_box;
	wxStaticText *h_text;
	wxTextCtrl	*h_box;
	wxStaticText *v_text;
	wxTextCtrl	*v_box;

	
	
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__DESIGN_WIDGET__H