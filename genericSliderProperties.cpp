#include "genericSliderProperties.h"
#include "genericSlider.h"

BEGIN_EVENT_TABLE(GenericSliderProperties, wxDialog)
	EVT_BUTTON(ID_ACCEPT2,GenericSliderProperties::OnButton)
	EVT_BUTTON(ID_CANCEL2,GenericSliderProperties::OnButton)
	EVT_BUTTON(ID_APPLY,GenericSliderProperties::OnButton)
	EVT_BUTTON(ID_COLOR2,GenericSliderProperties::OnButton)
	EVT_BUTTON(ID_INCRUP,GenericSliderProperties::OnButton)
	EVT_BUTTON(ID_INCRDOWN,GenericSliderProperties::OnButton)
	EVT_CHECKBOX(ID_CHECK,GenericSliderProperties::OnCheck)
	EVT_TEXT_ENTER(ID_ENTER,GenericSliderProperties::OnEnter)
END_EVENT_TABLE()

GenericSliderProperties::GenericSliderProperties(wxPanel *parent,wxWindowID id,const wxString& title)
:wxDialog(parent,id,title,wxPoint(500,200),wxSize(200,150),wxDEFAULT_DIALOG_STYLE & ~( wxMAXIMIZE_BOX |wxMINIMIZE_BOX)|wxSTAY_ON_TOP)
{
	winSlider = (GenericSlider *)parent;
	isDecimal=true;
	minOld = min = winSlider->getMin();
	maxOld = max = winSlider->getMax();
	tickOld = tick = winSlider->getTicks();
	colorPanelOld = colorPanel = winSlider->getValueText()->GetBackgroundColour();

	wxBitmap b[3];
	b[0]= wxBitmap(colour_xpm);
	b[1]= wxBitmap(cursorUp_xpm);
	b[2]= wxBitmap(cursorDown_xpm);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	accept = new wxButton(this,ID_ACCEPT2,wxT("Accept"),wxDefaultPosition,wxSize(40,25));
	cancel = new wxButton(this,ID_CANCEL2,wxT("Cancel"),wxDefaultPosition,wxSize(40,25));
	apply = new wxButton(this,ID_APPLY,wxT("Apply"),wxDefaultPosition,wxSize(40,25));
	hbox->Add(accept);
	hbox->AddSpacer(5);
	hbox->Add(cancel);
	hbox->AddSpacer(5);
	hbox->Add(apply);

	box = new wxCheckBox(this,ID_CHECK,wxT("Increase in decimal"));
	box->SetValue(true);
	min_text = new wxStaticText(this, wxID_ANY, wxT("Min Value Slider:"));
	min_box = new wxTextCtrl(this,ID_ENTER,wxEmptyString,wxDefaultPosition,wxSize(50,18), wxTE_PROCESS_ENTER|wxTE_CENTRE);
	*min_box<<minOld;// esto hace que solo entren 2 decimales
	max_text = new wxStaticText(this,wxID_ANY, wxT("Max Value Slider:"));
	max_box = new wxTextCtrl(this,ID_ENTER,wxEmptyString,wxDefaultPosition,wxSize(50,18),wxTE_PROCESS_ENTER|wxTE_CENTRE);
	*max_box<<maxOld;
	tick_text = new wxStaticText(this, wxID_ANY, wxT("Tick frequency:"));
	tick_box= new wxTextCtrl(this,ID_ENTER,wxString::Format(wxT("%d"),tickOld),wxDefaultPosition,wxSize(50,18),wxTE_PROCESS_ENTER|wxTE_CENTRE);

	color_text = new wxStaticText(this,wxID_ANY, wxT("Color numeric box:"));
	color_box = new wxBitmapButton(this,ID_COLOR2,b[0],wxDefaultPosition,wxSize(25,25));
	min_up = new wxBitmapButton(this,ID_INCRUP,b[1],wxDefaultPosition,wxSize(12,9));
	min_down = new wxBitmapButton(this,ID_INCRDOWN,b[2],wxDefaultPosition,wxSize(12,10));
	max_up = new wxBitmapButton(this,ID_INCRUP,b[1],wxDefaultPosition,wxSize(12,9));
	max_down = new wxBitmapButton(this,ID_INCRDOWN,b[2],wxDefaultPosition,wxSize(12,10));
	tick_up = new wxBitmapButton(this,ID_INCRUP,b[1],wxDefaultPosition,wxSize(12,9));
	tick_down = new wxBitmapButton(this,ID_INCRDOWN,b[2],wxDefaultPosition,wxSize(12,10));

	wxBoxSizer* vbutton1 = new wxBoxSizer(wxVERTICAL);
	vbutton1->Add(min_up);
	vbutton1->Add(min_down);
	wxBoxSizer* htext1 = new wxBoxSizer(wxHORIZONTAL);
	htext1->Add(min_box,0,wxRIGHT,2);
	htext1->Add(vbutton1);
	wxBoxSizer* vbutton2 = new wxBoxSizer(wxVERTICAL);
	vbutton2->Add(max_up);
	vbutton2->Add(max_down);
	wxBoxSizer* htext2 = new wxBoxSizer(wxHORIZONTAL);
	htext2->Add(max_box,0,wxRIGHT,2);
	htext2->Add(vbutton2);
	wxBoxSizer* vbutton3 = new wxBoxSizer(wxVERTICAL);
	vbutton3->Add(tick_up);
	vbutton3->Add(tick_down);
	wxBoxSizer* htext3 = new wxBoxSizer(wxHORIZONTAL);
	htext3->Add(tick_box,0,wxRIGHT,2);
	htext3->Add(vbutton3);

	wxGridSizer* grid = new wxGridSizer(2);
	grid->SetVGap(5);
	grid->Add(color_text);grid->Add(color_box);
	grid->Add(max_text);grid->Add(htext2);
	grid->Add(min_text);grid->Add(htext1);
	grid->Add(tick_text);grid->Add(htext3);
	grid->Add(box);grid->AddSpacer(1);
	grid->Add(hbox);
	
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(grid,0,wxALL,20);
	SetSizer(vbox);
	GetSizer()->SetSizeHints(this);

}
void GenericSliderProperties::OnEnter(wxCommandEvent& event)
{
	double val;
	wxObject* obj = event.GetEventObject();
	if(obj == min_box)
	{
		min_box->GetValue().ToDouble(&val);
		setMin(val);
	}
	if(obj == max_box)
	{
		max_box->GetValue().ToDouble(&val);
		setMax(val);
	}
	if(obj == tick_box)
	{
		tick_box->GetValue().ToDouble(&val);
		setTick(val);
	}
	event.Skip();
}
void GenericSliderProperties::OnCheck(wxCommandEvent& event)
{
	if(box->GetValue())isDecimal=true;
	else isDecimal=false;
}
void GenericSliderProperties::OnButton(wxCommandEvent& event)
{
	int id = event.GetId();
	wxObject *obj = event.GetEventObject();
	if(isDecimal)incr = 0.01;
	else incr = 1;
	if(id == ID_INCRUP)
	{
		if(obj == min_up)setMin(min+incr);
		if(obj == max_up)setMax(max+incr);
		if(obj == tick_up)setTick(tick+1);
	}
	if(id == ID_INCRDOWN)
	{
		if(obj == min_down)setMin(min-incr);
		if(obj == max_down)setMax(max-incr);
		if(obj == tick_down)setTick(tick-1);
	}
	if(id == ID_ACCEPT2)
	{
		winSlider->setCurrentMinMax(min,max);
		winSlider->setTicks(tick);
		winSlider->Refresh(false);
		Close(true);
	}
	if(id == ID_CANCEL2)
	{
		winSlider->setCurrentMinMax(minOld,maxOld);
		winSlider->setTicks(tickOld);
		winSlider->getValueText()->SetBackgroundColour(colorPanelOld);
		winSlider->Refresh(false);
		Close(true);
	}
	if(id == ID_APPLY)
	{
		winSlider->setCurrentMinMax(min,max);
		winSlider->setTicks(tick);
		winSlider->Refresh(false);
	}
	if(id == ID_COLOR2)
	{
		wxColourDialog dial(this);
		dial.SetTitle(wxT("Color Picker"));
		if(dial.ShowModal()!= wxID_OK)return;
		colorPanel = dial.GetColourData().GetColour();
		winSlider->getValueText()->SetBackgroundColour(colorPanel);
		winSlider->Refresh(false);
	}
	event.Skip();
}
void GenericSliderProperties::UpDateProperties()
{
	min_box->Clear();
	*min_box<<min;
	max_box->Clear();
	*max_box<<max;
	tick_box->Clear();
	*tick_box<<(int)tick;
}
void GenericSliderProperties::setMin(double val)
{
	bool limit;
	double minAbsolut,maxAbsolut;
	winSlider->getProperties(minAbsolut,maxAbsolut,limit);
	if(val)
	{
		if((limit)&&(val<minAbsolut))min=minAbsolut;
		else min=val;
	}
	else min=minOld;
	UpDateProperties();
}
void GenericSliderProperties::setMax(double val)
{
	bool limit;
	double minAbsolut,maxAbsolut;
	winSlider->getProperties(minAbsolut,maxAbsolut,limit);
	if(val)
	{
		if((limit)&&(val>maxAbsolut))max=maxAbsolut;
		else max=val;
	}
	else max=maxOld;
	UpDateProperties();
}
void GenericSliderProperties::setTick(double val)
{
	if(val)tick=val;
	else tick=tickOld;
	UpDateProperties();
}

