#ifndef __APOLO__POSITIONABLE_WIDGET__H
#define __APOLO__POSITIONABLE_WIDGET__H


#include "nodeTree.h"
#include "simulatedWorld.h"
#include "genericSlider.h"


class PositionableWidget: public wxPanel

{
public:
	PositionableWidget(wxWindow *window,NodeTree *obj,const wxString label,const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxSize(60,200),bool sliders = false, bool color_w=true,wxWindowID winid = wxID_ANY);
	
	void OnValuesChanged(wxCommandEvent& event);
	void ColorChanged(wxCommandEvent& event);
	void SliderLimits(double posup=20,double posdown=-20,double oriup=180,double oridown=-180,bool limit=false);
	void CreatePanel(bool sliders, bool solid);
	Vector3D getDefPosition(){return defPosition;};
	Vector3D getDefOrientation(){return defOrientation;};
	double getDefRedColor(){return defRed;};
	double getDefGreenColor(){return defGreen;};
	double getDefBlueColor(){return defBlue;};


private:
	
	wxStaticText *x_text,*y_text,*z_text,*r_text ,*p_text,*yw_text,*name_text,*color_text;
	wxTextCtrl	*x_box,*y_box,*z_box,*r_box,*p_box,*yw_box,*name_box;
	wxBitmapButton *color_box;
	GenericSlider *xs,*ys,*zs,*rs,*ps,*yws;
	wxColor color;
	bool slider;
	double defRed,defGreen,defBlue;
	double cred,cgreen,cblue;
	Vector3D defPosition,defOrientation,position,orientation;
	wxWindow *parent;
	wxString text,name,title;
	NodeTree *node;
	wxWindowID wID;
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__POSITIONABLE_WIDGET__H