#ifndef __APOLO__POSITIONABLE_WIDGET__H
#define __APOLO__POSITIONABLE_WIDGET__H


#include "nodeTree.h"
#include "simulatedWorld.h"
#include <string>
#include "genericSlider.h"


class PositionableWidget: public wxPanel

{
public:
	PositionableWidget(wxWindow *window,NodeTree *obj,SimulatedWorld *world,const wxString label,const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxSize(60,200),bool sliders = false, bool color_w=true);
	
	void OnValuesChanged(wxCommandEvent& event);
	void ColorChanged(wxCommandEvent& event);
	void SliderLimits(double posup=20,double posdown=-20,double oriup=180,double oridown=-180,bool limit=false);
	void CreatePanel(bool sliders, bool solid);


private:
	
	wxStaticText *x_text;
	wxTextCtrl	*x_box;
	wxStaticText *y_text;
	wxTextCtrl	*y_box;
	wxStaticText *z_text;
	wxTextCtrl	*z_box;
	wxStaticText *r_text;
	wxTextCtrl	*r_box;
	wxStaticText *p_text;
	wxTextCtrl	*p_box;
	wxStaticText *yw_text;
	wxTextCtrl	*yw_box;
	wxStaticText *name_text;
	wxTextCtrl	*name_box;
	wxStaticText *color_text;
	wxBitmapButton *color_box;
	GenericSlider *xs;
	GenericSlider *ys;
	GenericSlider *zs;
	GenericSlider *rs;
	GenericSlider *ps;
	GenericSlider *yws;
	bool slider;
	wxString title;
	wxColor color;
	Vector3D position,orientation;
	wxWindow *parent;
	string text;
	NodeTree *node;
	SimulatedWorld *s_world;
	wxString name;
	double cred,cgreen,cblue;
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__POSITIONABLE_WIDGET__H