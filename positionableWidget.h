#ifndef __APOLO__POSITIONABLE_WIDGET__H
#define __APOLO__POSITIONABLE_WIDGET__H

#include <string>
#include "genericSlider.h"
using namespace std;

DECLARE_EVENT_TYPE(wxEVT_POSITIONABLE_WIDGET_CHANGE, -1)
DECLARE_EVENT_TYPE(wxEVT_POSITIONABLE_WIDGET_COLOR, -1)

class PositionableWidget: public wxPanel

{
public:
	PositionableWidget(wxWindow *window,const wxString label,const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxSize(60,200),bool sliders = false, bool solid=true);
	void GetValues(double &xp,double &yp,double &zp, double &rp,double &pp,double &ywp,string &textp);
	void  GetColor(double &red,double &green,double &blue);
	void OnValuesChanged(wxCommandEvent& event);
	void  ColorChanged(wxCommandEvent& event);
	void SliderLimits(double posup=20,double posdown=-20,double oriup=180,double oridown=-180,bool limit=false);
	void SliderInitial(double inpos=0.0,double inori=0.0);
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
	bool solid;
	bool slider;
	wxString title;
	wxColor color;
	double x,y,z,r,p,yw;
	wxWindow *parent;
	string text;
	wxString name;
	double cred,cgreen,cblue;
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__POSITIONABLE_WIDGET__H