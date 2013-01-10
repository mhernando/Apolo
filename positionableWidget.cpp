#include "positionableWidget.h"



BEGIN_EVENT_TABLE(PositionableWidget, wxPanel)
	EVT_TEXT(wxID_ANY,PositionableWidget::OnValuesChanged)
	EVT_BUTTON(ID_COLOR, PositionableWidget::ColorChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, PositionableWidget::OnValuesChanged)
END_EVENT_TABLE()

PositionableWidget::PositionableWidget(wxWindow *window,NodeTree *obj,const wxString label, const wxPoint& pos ,
									   const wxSize& size,bool sliders ,bool color_w,wxWindowID winid): wxPanel( window, wxID_ANY, pos, size)
									   
{
	wID=winid;
	slider=sliders;
	parent=window;
	name=label;
	defPosition=obj->pointer.positionableentity->getRelativePosition();
	obj->pointer.positionableentity->getRelativeOrientation(defOrientation.x,defOrientation.y,defOrientation.z);
	if(color_w)
		obj->pointer.solidentity->getColor(defRed,defGreen,defBlue);
	node=obj;
	CreatePanel(sliders,color_w);	
	

}

void PositionableWidget::CreatePanel(bool sliders, bool color_w)
{
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer *pE=new wxStaticBoxSizer(wxVERTICAL,this,name);
	wxStaticBoxSizer *posi, *ori, *pers ;
	
	
	wxBoxSizer *rbox=new wxBoxSizer(wxHORIZONTAL);
	pers= new wxStaticBoxSizer(wxVERTICAL,this,wxT("Personalitation"));
	name_text=new wxStaticText(this,wxID_ANY, wxT("Name :"),wxDefaultPosition,wxDefaultSize);
	name_box = new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	
	
	if(sliders==false)
	{
		wxString defValue;
	
		
		
		
		wxBoxSizer *pbox=new wxBoxSizer(wxHORIZONTAL);
		posi= new wxStaticBoxSizer(wxVERTICAL,this,wxT("Orientation"));
		
		x_text = new wxStaticText(this, wxID_ANY, wxT("X :    "),wxDefaultPosition,wxDefaultSize);
		x_box = new wxTextCtrl(this,wxID_ANY,defValue<<position.x,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		y_text = new wxStaticText(this, wxID_ANY, wxT("Y :      "),wxDefaultPosition,wxDefaultSize);
		y_box = new wxTextCtrl(this,wxID_ANY,defValue<<position.y,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		z_text = new wxStaticText(this, wxID_ANY, wxT("Z :    "),wxDefaultPosition,wxDefaultSize);
		z_box = new wxTextCtrl(this,wxID_ANY,defValue<<position.z,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		

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
		pE->Add(posi,0,wxEXPAND | wxALL,5); 
	

		
		wxBoxSizer *qbox=new wxBoxSizer(wxHORIZONTAL);
		ori= new wxStaticBoxSizer(wxVERTICAL,this,wxT("Orientation"));
		
	
		r_text = new wxStaticText(this, wxID_ANY, wxT("Roll :"),wxDefaultPosition,wxDefaultSize);
		r_box = new wxTextCtrl(this,wxID_ANY,defValue<<orientation.x,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		p_text = new wxStaticText(this, wxID_ANY, wxT("Pitch :"),wxDefaultPosition,wxDefaultSize);
		p_box = new wxTextCtrl(this,wxID_ANY,defValue<<orientation.y,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		yw_text = new wxStaticText(this, wxID_ANY, wxT("Yaw :"),wxDefaultPosition,wxDefaultSize);
		yw_box = new wxTextCtrl(this,wxID_ANY,defValue<<orientation.z,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		
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
		pE->Add(ori,0,wxEXPAND | wxALL,5);
		
		
		
	
	
	}
	else
	{
		wxBoxSizer *sbox;
		

	
		
		
		if (wID!=ID_ADDIRRPRI)
		{
			sbox=new wxBoxSizer(wxHORIZONTAL);
			xs=new GenericSlider(this,wxT("X position"),wxDefaultPosition,wxDefaultSize,true);
			ys=new GenericSlider(this,wxT("Y position"),wxDefaultPosition,wxDefaultSize,true);
			zs=new GenericSlider(this,wxT("Z position"),wxDefaultPosition,wxDefaultSize,true);

			rs=new GenericSlider(this,wxT("Roll orientation"),wxDefaultPosition,wxDefaultSize,true);
			ps=new GenericSlider(this,wxT("Pitch orientation"),wxDefaultPosition,wxDefaultSize,true);
			yws=new GenericSlider(this,wxT("Yaw orientation"),wxDefaultPosition,wxDefaultSize,true);
		}
		else
		{
			sbox=new wxBoxSizer(wxVERTICAL);
			xs=new GenericSlider(this,wxT("X position"),wxDefaultPosition,wxDefaultSize,false);
			ys=new GenericSlider(this,wxT("Y position"),wxDefaultPosition,wxDefaultSize,false);
			zs=new GenericSlider(this,wxT("Z position"),wxDefaultPosition,wxDefaultSize,false);

			rs=new GenericSlider(this,wxT("Roll orientation"),wxDefaultPosition,wxDefaultSize,false);
			ps=new GenericSlider(this,wxT("Pitch orientation"),wxDefaultPosition,wxDefaultSize,false);
			yws=new GenericSlider(this,wxT("Yaw orientation"),wxDefaultPosition,wxDefaultSize,false);


		}
		
		SliderLimits();
		
		xs->setValue(position.x);
		ys->setValue(position.y);
		zs->setValue(position.z);
		rs->setValue(orientation.x);
		ps->setValue(orientation.y);
		yws->setValue(orientation.z);

		sbox->Add(xs,0,wxEXPAND);
		sbox->Add(ys,0,wxEXPAND);
		sbox->Add(zs,0,wxEXPAND);

		sbox->Add(rs,0,wxEXPAND);
		sbox->Add(ps,0,wxEXPAND);
		sbox->Add(yws,0,wxEXPAND);
		
		pE->Add(sbox,0,wxEXPAND);
		}

	rbox->Add(name_text,0,wxCENTRE);
	rbox->AddSpacer(5);
	rbox->Add(name_box,0,wxCENTRE);
	
	
	if (color_w==true)
	{
	color_text = new wxStaticText(this,wxID_ANY, wxT("Color :"),wxDefaultPosition,wxDefaultSize);
	color_box = new wxBitmapButton(this,ID_COLOR,wxIcon(colour_xpm),wxDefaultPosition,wxDefaultSize);

	rbox->AddSpacer(40);
	rbox->Add(color_text,0,wxCENTRE);
	rbox->AddSpacer(5);
	rbox->Add(color_box,0,wxCENTRE);
	}

	
	
	
	pers->Add(rbox,0,wxEXPAND);
	pE->Add(pers,0,wxEXPAND | wxALL,5);
	vbox->Add(pE,0,wxEXPAND | wxALL,5);
	vbox->SetSizeHints(this);
	SetSizer(vbox);

}

void PositionableWidget::OnValuesChanged(wxCommandEvent& event)
{
	
	if(slider==false)
	{
		x_box->GetValue().ToDouble(&position.x);
		y_box->GetValue().ToDouble(&position.y);
		z_box->GetValue().ToDouble(&position.z);
		r_box->GetValue().ToDouble(&orientation.x);
		p_box->GetValue().ToDouble(&orientation.y);
		yw_box->GetValue().ToDouble(&orientation.z);
	}
	else
	{
		position.x=xs->getValue();
		position.y=ys->getValue();
		position.z=zs->getValue();
		orientation.x=rs->getValue();
		orientation.y=ps->getValue();
		orientation.z=yws->getValue();
	}
	if(name_box->GetValue()!=wxEmptyString)
	{
		text=name_box->GetValue();
		wxTreeItemId id;
		Tree *tree=node->getSimu()->tree;
		tree->SetItemText(tree->GetLastChild(tree->GetSelection()),text);
		node->pointer.positionableentity->setName(text.ToStdString());
	}
	Transformation3D t;
	t.position=position;
	t.orientation.setRPY(deg2rad(orientation.x),deg2rad(orientation.y),deg2rad(orientation.z));
	node->pointer.positionableentity->setRelativeT3D(t);
	
	node->getSimu()->getChild()->UpdateWorld();

}

void  PositionableWidget::ColorChanged(wxCommandEvent& event)
{
	
		color=wxGetColourFromUser(this);
		if(color.IsOk())
		{
			double red,green,blue;
			red = color.Red();
			green = color.Green();
			blue = color.Blue();

			node->pointer.solidentity->setColor(red/255,green/255,blue/255);
			node->getSimu()->getChild()->UpdateWorld();
		
		}		
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



	




	


