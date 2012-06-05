#include "positionableWidget.h"

DEFINE_EVENT_TYPE(wxEVT_POSITIONABLE_WIDGET_CHANGE)
DEFINE_EVENT_TYPE(wxEVT_POSITIONABLE_WIDGET_COLOR)

BEGIN_EVENT_TABLE(PositionableWidget, wxPanel)
	EVT_TEXT(ID_ENTER,PositionableWidget::OnValuesChanged)
	EVT_BUTTON(ID_COLOR, PositionableWidget::ColorChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, PositionableWidget::OnValuesChanged)
END_EVENT_TABLE()

PositionableWidget::PositionableWidget(wxWindow *window, const wxPoint& pos ,
									   const wxSize& size,bool sliders , bool orientation, bool solid): wxPanel( window, wxID_ANY, pos, size)
									   
{
	slider=sliders;
	parent=window;

	CreatePanel(sliders,orientation,solid);	

}

void PositionableWidget::CreatePanel(bool sliders, bool orientation,bool solid)
{
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	
	wxPanel *panel=new wxPanel(this,-1,wxDefaultPosition,wxDefaultSize,wxBORDER_NONE);
	if(sliders==false)
	{
		
		
		wxStaticBox *posi=new wxStaticBox(panel,-1,wxT("Position"),wxPoint(5,5),wxSize(305,50));
	
		x_text = new wxStaticText(panel, wxID_ANY, wxT("X :"),wxPoint(10,25),wxDefaultSize);
		x_box = new wxTextCtrl(panel,ID_ENTER,"0",wxPoint(45,25),wxSize(50,20), wxTE_PROCESS_ENTER|wxTE_CENTRE);
		y_text = new wxStaticText(panel, wxID_ANY, wxT("Y :"),wxPoint(110,25),wxDefaultSize);
		y_box = new wxTextCtrl(panel,ID_ENTER,"0",wxPoint(145,25),wxSize(50,20), wxTE_PROCESS_ENTER|wxTE_CENTRE);
		z_text = new wxStaticText(panel, wxID_ANY, wxT("Z :"),wxPoint(210,25),wxDefaultSize);
		z_box = new wxTextCtrl(panel,ID_ENTER,"0",wxPoint(245,25),wxSize(50,20), wxTE_PROCESS_ENTER|wxTE_CENTRE);

		wxStaticBox *ori=new wxStaticBox(panel,-1,wxT("Orientation"),wxPoint(5,65),wxSize(305,50));
	
		r_text = new wxStaticText(panel, wxID_ANY, wxT("Roll :"),wxPoint(10,85),wxDefaultSize);
		r_box = new wxTextCtrl(panel,ID_ENTER,"0",wxPoint(45,85),wxSize(50,20), wxTE_PROCESS_ENTER|wxTE_CENTRE);
		p_text = new wxStaticText(panel, wxID_ANY, wxT("Pitch :"),wxPoint(110,85),wxDefaultSize);
		p_box = new wxTextCtrl(panel,ID_ENTER,"0",wxPoint(145,85),wxSize(50,20), wxTE_PROCESS_ENTER|wxTE_CENTRE);
		yw_text = new wxStaticText(panel, wxID_ANY, wxT("Yaw :"),wxPoint(210,85),wxDefaultSize);
		yw_box = new wxTextCtrl(panel,ID_ENTER,"0",wxPoint(245,85),wxSize(50,20), wxTE_PROCESS_ENTER|wxTE_CENTRE);
		
		vbox->Add(panel,0);
	}

	else
	{
		wxBoxSizer *sbox;

		if(orientation)
			sbox=new wxBoxSizer(wxHORIZONTAL);
		else
			sbox=new wxBoxSizer(wxVERTICAL);
	
		xs=new GenericSlider(this,"X position",wxDefaultPosition,wxDefaultSize,orientation);
		ys=new GenericSlider(this,"Y position",wxDefaultPosition,wxDefaultSize,orientation);
		zs=new GenericSlider(this,"Z position",wxDefaultPosition,wxDefaultSize,orientation);

		rs=new GenericSlider(this,"Roll orientation",wxDefaultPosition,wxDefaultSize,orientation);
		ps=new GenericSlider(this,"Pitch orientation",wxDefaultPosition,wxDefaultSize,orientation);
		yws=new GenericSlider(this,"Yaw orientation",wxDefaultPosition,wxDefaultSize,orientation);

		SliderLimits();
		
		//SliderInitial();

		sbox->Add(xs,1,wxEXPAND);
		sbox->Add(ys,1,wxEXPAND);
		sbox->Add(zs,1,wxEXPAND);
		sbox->Add(rs,1,wxEXPAND);
		sbox->Add(ps,1,wxEXPAND);
		sbox->Add(yws,1,wxEXPAND);
		
		vbox->Add(sbox,1,wxEXPAND);
		
		}




	wxPanel *panel2=new wxPanel(this,-1,wxDefaultPosition,wxDefaultSize,wxBORDER_NONE);


	wxStaticBox *pers=new wxStaticBox(panel2,-1,wxT("Personalitation"),wxPoint(5,5),wxSize(305,50));
	name_text=new wxStaticText(panel2,wxID_ANY, wxT("Name :"),wxPoint(10,25),wxDefaultSize);
	name_box = new wxTextCtrl(panel2,ID_ENTER,wxEmptyString,wxPoint(60,25),wxSize(120,20),wxTE_PROCESS_ENTER);
	
	if (solid==true)
	{
	color_text = new wxStaticText(panel2,wxID_ANY, wxT("Color :"),wxPoint(210,25),wxDefaultSize);
	color_box = new wxBitmapButton(panel2,ID_COLOR,wxIcon(colour_xpm),wxPoint(260,20),wxSize(25,25));
	}


	
	vbox->Add(panel2,0,wxRIGHT | wxTOP,5);
	SetSizer(vbox);

}

void PositionableWidget::OnValuesChanged(wxCommandEvent& event)
{
	if(slider==false)
	{
		x_box->GetValue().ToDouble(&x);
		y_box->GetValue().ToDouble(&y);
		z_box->GetValue().ToDouble(&z);
		r_box->GetValue().ToDouble(&r);
		p_box->GetValue().ToDouble(&p);
		yw_box->GetValue().ToDouble(&yw);
	}
	else
	{
		x=xs->getValue();
		y=ys->getValue();
		z=zs->getValue();
		r=rs->getValue();
		p=ps->getValue();
		yw=yws->getValue();
	}
	text=name_box->GetValue();
	
	//report to the parent window... a value have changed
	wxCommandEvent positionableEvent( wxEVT_POSITIONABLE_WIDGET_CHANGE, GetId() );
    positionableEvent.SetEventObject( this );
	// Send it
    parent->GetEventHandler()->ProcessEvent( positionableEvent );
	
}

void PositionableWidget::GetValues(double &xp,double &yp,double &zp, double &rp,double &pp,double &ywp,string &textp)
{
		xp=x;
		yp=y;
		zp=z;
		rp=r;
		pp=p;
		ywp=yw;
		textp=text;
}

void  PositionableWidget::ColorChanged(wxCommandEvent& event)
{
	
		color=wxGetColourFromUser(this);
		if(color.IsOk())
		{
			
			cred = color.Red();
			cgreen = color.Green();
			cblue = color.Blue();

			//report to the parent window... a value have changed
			wxCommandEvent positionableEvent( wxEVT_POSITIONABLE_WIDGET_COLOR, GetId() );
			positionableEvent.SetEventObject( this );
			// Send it
			parent->GetEventHandler()->ProcessEvent( positionableEvent );
		}		
}

void  PositionableWidget::GetColor(double &red, double &green,double &blue)
{
blue=cblue;
red=cred;
green=cgreen;		
}

void  PositionableWidget::SliderLimits(double posup,double posdown,double oriup,double oridown,bool limit)
{
	xs->setProperties(posdown,posup,limit);
	ys->setProperties(posdown,posup,limit);
	zs->setProperties(posdown,posup,limit);
	rs->setProperties(oridown,oriup,limit);
	ps->setProperties(oridown,oriup,limit);
	yws->setProperties(oridown,oriup,limit);

}

void  PositionableWidget::SliderInitial(double inpos,double inori)
{
	xs->setValue(inpos);
	ys->setValue(inpos);
	zs->setValue(inpos);
	rs->setValue(inori);
	ps->setValue(inori);
	yws->setValue(inori);

}


	


