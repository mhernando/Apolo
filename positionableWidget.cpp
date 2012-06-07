#include "positionableWidget.h"

DEFINE_EVENT_TYPE(wxEVT_POSITIONABLE_WIDGET_CHANGE)
DEFINE_EVENT_TYPE(wxEVT_POSITIONABLE_WIDGET_COLOR)

BEGIN_EVENT_TABLE(PositionableWidget, wxPanel)
	EVT_TEXT(ID_ENTER,PositionableWidget::OnValuesChanged)
	EVT_BUTTON(ID_COLOR, PositionableWidget::ColorChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, PositionableWidget::OnValuesChanged)
END_EVENT_TABLE()

PositionableWidget::PositionableWidget(wxWindow *window,const wxString label, const wxPoint& pos ,
									   const wxSize& size,bool sliders , bool orientation, bool solid): wxPanel( window, wxID_ANY, pos, size)
									   
{
	slider=sliders;
	parent=window;
	name=label;
	CreatePanel(sliders,orientation,solid);	

}

void PositionableWidget::CreatePanel(bool sliders, bool orientation,bool solid)
{
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer *pE=new wxStaticBoxSizer(wxVERTICAL,this,name);
	wxStaticBoxSizer *posi, *ori, *pers ;
	if(sliders==false)
	{
		
		
		wxBoxSizer *pbox=new wxBoxSizer(wxHORIZONTAL);
		posi= new wxStaticBoxSizer(wxVERTICAL,this,wxT("Orientation"));
		
		x_text = new wxStaticText(this, wxID_ANY, wxT("X :    "),wxDefaultPosition,wxDefaultSize);
		x_box = new wxTextCtrl(this,ID_ENTER,"0",wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		y_text = new wxStaticText(this, wxID_ANY, wxT("Y :      "),wxDefaultPosition,wxDefaultSize);
		y_box = new wxTextCtrl(this,ID_ENTER,"0",wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		z_text = new wxStaticText(this, wxID_ANY, wxT("Z :    "),wxDefaultPosition,wxDefaultSize);
		z_box = new wxTextCtrl(this,ID_ENTER,"0",wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);

		pbox->Add(x_text,0,wxCENTRE);
		pbox->AddSpacer(5);
		pbox->Add(x_box,1,wxCENTRE);
		pbox->AddSpacer(15);
		pbox->Add(y_text,0,wxCENTRE);
		pbox->AddSpacer(5);
		pbox->Add(y_box,1,wxCENTRE);
		pbox->AddSpacer(15);
		pbox->Add(z_text,0,wxCENTRE);
		pbox->AddSpacer(5);
		pbox->Add(z_box,1,wxCENTRE);
		posi->Add(pbox,1,wxEXPAND);


		wxBoxSizer *qbox=new wxBoxSizer(wxHORIZONTAL);
		ori= new wxStaticBoxSizer(wxVERTICAL,this,wxT("Orientation"));
		//wxStaticBox *ori=new wxStaticBox(panel,-1,wxT("Orientation"),wxPoint(5,65),wxSize(305,50));
	
		r_text = new wxStaticText(this, wxID_ANY, wxT("Roll :"),wxDefaultPosition,wxDefaultSize);
		r_box = new wxTextCtrl(this,ID_ENTER,"0",wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		p_text = new wxStaticText(this, wxID_ANY, wxT("Pitch :"),wxDefaultPosition,wxDefaultSize);
		p_box = new wxTextCtrl(this,ID_ENTER,"0",wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		yw_text = new wxStaticText(this, wxID_ANY, wxT("Yaw :"),wxDefaultPosition,wxDefaultSize);
		yw_box = new wxTextCtrl(this,ID_ENTER,"0",wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		
		qbox->Add(r_text,0,wxCENTRE);
		qbox->AddSpacer(5);
		qbox->Add(r_box,1,wxCENTRE);
		qbox->AddSpacer(15);
		qbox->Add(p_text,0,wxCENTRE);
		qbox->AddSpacer(5);
		qbox->Add(p_box,1,wxCENTRE);
		qbox->AddSpacer(15);
		qbox->Add(yw_text,0,wxCENTRE);
		qbox->AddSpacer(5);
		qbox->Add(yw_box,1,wxCENTRE);
		ori->Add(qbox,1,wxEXPAND);

		pE->Add(posi,0,wxEXPAND | wxALL,5); 
		pE->Add(ori,0,wxEXPAND | wxALL,5);
	
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
		
		pE->Add(sbox,1,wxEXPAND);
		
		}


	wxBoxSizer *rbox=new wxBoxSizer(wxHORIZONTAL);
	pers= new wxStaticBoxSizer(wxVERTICAL,this,wxT("Personalitation"));
	name_text=new wxStaticText(this,wxID_ANY, wxT("Name :"),wxDefaultPosition,wxDefaultSize);
	name_box = new wxTextCtrl(this,ID_ENTER,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	
	if (solid==true)
	{
	color_text = new wxStaticText(this,wxID_ANY, wxT("Color :"),wxDefaultPosition,wxDefaultSize);
	color_box = new wxBitmapButton(this,ID_COLOR,wxIcon(colour_xpm),wxDefaultPosition,wxDefaultSize);
	}

	rbox->Add(name_text,0,wxCENTRE);
	rbox->AddSpacer(5);
	rbox->Add(name_box,1,wxCENTRE);
	rbox->AddSpacer(40);
	rbox->Add(color_text,0,wxCENTRE);
	rbox->AddSpacer(5);
	rbox->Add(color_box,1,wxCENTRE);
	pers->Add(rbox,1,wxEXPAND);

	pE->Add(pers,0,wxEXPAND | wxALL,5);
	vbox->Add(pE,1,wxEXPAND | wxALL,5);
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


	


