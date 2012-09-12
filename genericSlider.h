#ifndef __APOLO__GENERIC_SLIDER__H
#define __APOLO__GENERIC_SLIDER__H

#include "definitions.h"
#include "genericSliderProperties.h"

#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/window.h>
#include <wx/string.h>

DECLARE_EVENT_TYPE(wxEVT_GENERIC_SLIDER_CHANGE, -1)

class GenericSlider: public wxPanel
{
public:
	GenericSlider(wxWindow *window,  wxString label, const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxSize(60,200),bool orientation = true);
	
	void OnEnter(wxCommandEvent& event);
	void OnButtonCtrl(wxCommandEvent& event);
	void OnSliderProperties(wxCommandEvent& event);
	void OnScroll(wxScrollEvent& event);

	void createPanel(bool orientation);
	void setValue(double val);
	void setTicks(int t){ticks = t;}
	void setCurrentMinMax(double _min, double _max);
	void setProperties(double min, double max, bool islimited=true);
	void updateMinMax(double _currentMin, double _currentMax);
	double getValue(void){return value;}
	double getMin(){return currentMin;}
	double getMax(){return currentMax;}
	int getTicks(){return ticks;}
	void getProperties(double &_min, double &_max, bool &_islimited){_min=min;_max=max;_islimited=limited;}
	wxTextCtrl* getValueText(){return m_value;}

private:
	GenericSliderProperties* sliderProperties;
	//wxwidgets components
	wxStaticBox *m_box;
	wxSlider* m_slider; 
	wxTextCtrl*	m_value;
	wxStaticText *m_max;
	wxStaticText *m_min;
	wxButton* m_properties;

	wxString title;
	double max; //absolute max value
	double min; //absolute min value
	bool limited; //the absolute limits are activated
	bool scroll;

	double currentMax; //current maximun slider max
	double currentMin; //current maximun slider min
	double value; //the computed value
	int ticks; //number of slider ticks
	wxWindow *parent;
	int V,H;

public:
	DECLARE_EVENT_TABLE()
};

#endif //__APOLO__GENERIC_SLIDER__H
