#ifndef __APOLO__DESIGN_WIDGET__H
#define __APOLO__DESIGN_WIDGET__H


#include "nodeTree.h"
#include "simulatedWorld.h"
#include "genericSlider.h"


class DesignWidget: public wxPanel

{
public:
	DesignWidget(wxWindow *window,wxWindowID id,NodeTree *pose,SimulatedWorld *world,const wxString label,const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,bool sliders = true, bool orientation=false);
	void GetSpecificValues(bool def=false);
	void OnValueChanged(wxCommandEvent& event);
	void CreatePanel(bool sliders, bool orientation);
	void ObjectConversion();


private:
	GenericSlider *c_radio;
	GenericSlider *c_height;
	GenericSlider *c_vertex;
	SimulatedWorld *simu;
	double defRadio,defHeight,defVertex;
	bool auxS,auxC,auxP;
	bool slider;
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