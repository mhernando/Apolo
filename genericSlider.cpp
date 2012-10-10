#include "genericSlider.h"
#include "changeLocationCtrl.h"

DEFINE_EVENT_TYPE(wxEVT_GENERIC_SLIDER_CHANGE)

BEGIN_EVENT_TABLE(GenericSlider, wxPanel)
	EVT_TEXT_ENTER(ID_NUM, GenericSlider::OnEnter)
	EVT_BUTTON(ID_EDIT, GenericSlider::OnSliderProperties)
	EVT_SCROLL(GenericSlider::OnScroll)
END_EVENT_TABLE()


GenericSlider::GenericSlider(wxWindow *window,wxString label,const wxPoint& pos,const wxSize& size,bool orientation)
	: wxPanel( window, wxID_ANY, pos, size)
{
	title = label;
	limited = false;
	scroll=false;
	value=0;
	ticks=10;
	parent = window;

	currentMax=max=100;
	currentMin=min=0;
	createPanel(orientation);
	
	
		
}
void GenericSlider::OnEnter(wxCommandEvent& event)
{
	
	int a = event.GetId();
	scroll=true ;
	double val=0;
	if(m_value->GetValue().ToDouble(&val))	setValue(val); 
		else 
		{
			wxLogMessage(wxT("Write a number"));
			setValue(value);
		}
	
		
}
void GenericSlider::OnSliderProperties(wxCommandEvent& event)
{
	wxString text;
	text<<wxT("Edit")<<wxT(" ")<<title;
	sliderProperties= new GenericSliderProperties(this,wxID_ANY,text);
	sliderProperties->ShowModal();
}
void GenericSlider::OnScroll(wxScrollEvent& event)
{
	scroll=true ;
	value=currentMin+((m_slider->GetValue())*(currentMax-currentMin)/100.0);
	setValue(value);
	
}
void GenericSlider::createPanel(bool orientation)
{
	
	if(orientation){V=8;H=4;}//(POSITION VERTICAL)
	else {V=4;H=8;} //(POSITION HORIZONTAL)
	
	wxBoxSizer *topsizer = new wxBoxSizer(V);
	wxStaticBoxSizer *sbs= new wxStaticBoxSizer(V,this,title);
	m_box=sbs->GetStaticBox();
	
	wxBoxSizer *horizontal_sizer = new wxBoxSizer(H);
	
	if(orientation)  m_slider = new wxSlider(this, ID_SLIDER, 0,0,100,wxDefaultPosition, wxSize(30,-1),wxSL_VERTICAL | wxSL_INVERSE);
	else m_slider = new wxSlider(this, ID_SLIDER, 0,0,100,wxDefaultPosition, wxSize(30,-1));
		
	horizontal_sizer->Add(m_slider,wxSizerFlags(0).Expand().Border(wxALL, 1));

	wxBoxSizer *minmaxbs=new wxBoxSizer(V);
	if(orientation)
	{
		m_min=new wxStaticText(this,wxID_ANY,wxT("()"),wxDefaultPosition,wxSize(34,20),wxALIGN_LEFT);
		m_max=new wxStaticText(this,wxID_ANY,wxT("()"),wxDefaultPosition,wxSize(34,20),wxALIGN_LEFT);
		minmaxbs->Add(m_max,wxSizerFlags(1).Align(wxALIGN_LEFT));
		minmaxbs->Add(m_min,wxSizerFlags(0).Align(wxALIGN_LEFT));
		
	}
	else
	{
		m_min=new wxStaticText(this,wxID_ANY,wxT("()"),wxDefaultPosition,wxSize(45,20),wxALIGN_LEFT);//original
		m_max=new wxStaticText(this,wxID_ANY,wxT("()"),wxDefaultPosition,wxSize(40,20),wxALIGN_RIGHT);
		minmaxbs->Add(m_min,wxSizerFlags(1).Align(wxALIGN_RIGHT));
		minmaxbs->Add(m_max,wxSizerFlags(1).Align(wxALIGN_LEFT));
	}
	 
	horizontal_sizer->Add(minmaxbs,wxSizerFlags(1).Expand());
	m_value = new wxTextCtrl(this, ID_NUM, wxEmptyString,wxDefaultPosition,wxSize(60,20),wxTE_PROCESS_ENTER|wxTE_CENTRE );
	if(orientation) sbs->Add(m_value,wxSizerFlags(0).Align(0).Border(wxALL,1));
	m_properties=new wxButton(this, ID_EDIT, wxT("..."), wxDefaultPosition,wxSize(60,20));
	sbs->Add(horizontal_sizer,wxSizerFlags(1).Expand());
	if(orientation)sbs->Add(m_properties,wxSizerFlags(0).Align(wxALIGN_BOTTOM));
	else
	{
		wxBoxSizer* vertical_sizer= new wxBoxSizer(wxVERTICAL);
		vertical_sizer->Add(m_value,wxSizerFlags(0).Align(wxALIGN_RIGHT));
		vertical_sizer->Add(m_properties,wxSizerFlags(0).Align(wxALIGN_RIGHT));
		sbs->AddSpacer(6);
		sbs->Add(vertical_sizer);
	}
	topsizer->Add(sbs,wxSizerFlags(1).Expand().Border(wxALL, 1));
	SetSizer( topsizer );
}
void GenericSlider::setValue(double val)
{
	//out of limits
	if((limited)&&((val>max)||(val<min)))return;
	if(val>currentMax)currentMax=val;
	if(val<currentMin)currentMin=val;
	value=val;
	m_slider->SetValue((int)100*(val-currentMin)/(currentMax-currentMin));
	
	wxString w=wxString::Format(wxT("%.2f"),value);
	m_value->ChangeValue(wxEmptyString);
	m_value->ChangeValue(w);
	
	


		
	if(scroll) // For Security (only propagate the event when  a scroll event is issued)
	{
	//report to the parent window... a value have changed
	wxCommandEvent sliderEvent( wxEVT_GENERIC_SLIDER_CHANGE,GetId() );
	sliderEvent.ResumePropagation(2);
	sliderEvent.SetEventObject( parent);
	
	// Send it
	parent->GetEventHandler()->ProcessEvent( sliderEvent );
	parent->GetParent()->GetEventHandler()->ProcessEvent( sliderEvent );
	scroll=false;
	}

}
void GenericSlider::setCurrentMinMax(double _min, double _max)
{
	
	if((limited)&&(_max>max))_max=max;
	if((limited)&&(_min<min))_min=min;
	if(_max<_min)return;
	currentMax=_max;
	currentMin=_min;
	updateMinMax(currentMin,currentMax);

}
void GenericSlider::setProperties(double _min, double _max, bool islimited)
{
	currentMax=max=_max; 
	currentMin=min=_min;
	limited=islimited;
	setCurrentMinMax(currentMin,currentMax);

}
void GenericSlider::updateMinMax(double m_currentMin, double m_currentMax)
{
	wxString aux1,aux2;
	wxString precis="%.2f";
	if(m_currentMax-m_currentMin>25)precis[2]='0';
	if(m_currentMax-m_currentMin>15 && m_currentMax-m_currentMin<24)precis[2]='1';
	if(m_currentMax-m_currentMin<14)precis[2]='2';

	aux1.Printf(precis,m_currentMin);
	aux2.Printf(precis,m_currentMax);
	m_min->SetLabel(aux1);
	m_max->SetLabel(aux2);
}



