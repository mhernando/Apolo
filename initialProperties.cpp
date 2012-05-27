#include "initialProperties.h"


BEGIN_EVENT_TABLE(InitialProperties, wxDialog)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, InitialProperties::OnValueChanged)
	EVT_TEXT(ID_ENTER,InitialProperties::OnValueChanged)
	EVT_BUTTON(ID_ACCEPT, InitialProperties::OnButton)
	EVT_BUTTON(ID_CANCEL, InitialProperties::OnButton)
	EVT_BUTTON(ID_DEFAULT, InitialProperties::OnButton)
	EVT_BUTTON(ID_COLOR, InitialProperties::OnButton)
END_EVENT_TABLE()

InitialProperties::InitialProperties(wxWindow *parent,wxWindowID id, PositionableEntity *obj,SimulatedWorld *s_world, const wxString& title)
:wxDialog(parent,id, title, wxPoint(200,50), wxSize(500,500),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{
	b_sel=0;
	S_G=true;
	winId=id;
	world=s_world;
	pos=obj;

	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);

	df = new wxButton(this,ID_DEFAULT,wxT("Create object with default parameters"),wxDefaultPosition,wxDefaultSize);
	vbox->Add(df,0,wxEXPAND | wxALL ,12);
	
	//PositionableEntity`s Properties//
	wxPanel *panel=new wxPanel(this,-1,wxDefaultPosition,wxDefaultSize,wxBORDER_NONE);
	
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
	
	wxStaticBox *pers=new wxStaticBox(panel,-1,wxT("Personalitation"),wxPoint(5,125),wxSize(305,50));
	name_text=new wxStaticText(panel,wxID_ANY, wxT("Name :"),wxPoint(10,145),wxDefaultSize);
	name_box = new wxTextCtrl(panel,ID_ENTER,pos->getName(),wxPoint(60,145),wxSize(120,20),wxTE_PROCESS_ENTER);
	//name_box->SetDefaultStyle(wxTextAttr(*wxBLUE)
	color_text = new wxStaticText(panel,wxID_ANY, wxT("Color :"),wxPoint(210,145),wxDefaultSize);
	color_box = new wxBitmapButton(panel,ID_COLOR,wxIcon(colour_xpm),wxPoint(260,140),wxSize(25,25));
	
	vbox->Add(panel,0 ,wxRIGHT,5);
	vbox->AddSpacer(5);


	
	
	
	/////Object's Properties///
	wxBoxSizer *pbox=new wxBoxSizer(wxVERTICAL);

	c_radio=new GenericSlider(this,"Radio",wxDefaultPosition,wxDefaultSize,false);
	c_height=new GenericSlider(this,"Height",wxDefaultPosition,wxDefaultSize,false);
	c_vertex=new GenericSlider(this,"BaseVertex",wxDefaultPosition,wxDefaultSize,false);

	if(id==ID_ADDSPHERE || id==ID_ADDCYL || id==ID_ADDPRI)
	{
		c_radio->setProperties(0,10,false);
		c_radio->setValue(1);
		pbox->Add(c_radio,0,wxEXPAND );
	}

	if(id==ID_ADDCYL || id==ID_ADDPRI)
	{
		c_height->setProperties(0,10,false);
		c_height->setValue(1);
		pbox->Add(c_height,0,wxEXPAND);
		
	}

	if(id==ID_ADDPRI)
	{	
		c_vertex->setProperties(0,12,false);
		c_vertex->setValue(6);
		pbox->Add(c_vertex,0,wxEXPAND);
	}
	
	///Hide generic Slider if they aren't used, otherwise they will appear on the top of the Dialog/////
	if(c_vertex->GetContainingSizer()==NULL)
		c_vertex->Hide();
	if(c_height->GetContainingSizer()==NULL)
		c_height->Hide();
	if(c_radio->GetContainingSizer()==NULL)
		c_radio->Hide();

	////Buttom box///
	wxBoxSizer *b_box=new wxBoxSizer(wxHORIZONTAL);

	accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxSize(60,25));
	cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(60,25));
	b_box->Add(accept,1,wxALIGN_BOTTOM,5);
	b_box->Add(cancel,1,wxALIGN_BOTTOM,5);	

	//Close Dialog Design//
	vbox->Add(pbox,0,wxEXPAND | wxLEFT | wxRIGHT,5 );
	vbox->AddSpacer(40);
	vbox->Add(b_box,0,wxEXPAND | wxALL ,12);
	vbox->SetSizeHints(this);
	SetSizer(vbox);

}

void InitialProperties::OnValueChanged(wxCommandEvent& event)
{
	
	
	double x,y,z,r,p,yw;
	

	x_box->GetValue().ToDouble(&x);
	y_box->GetValue().ToDouble(&y);
	z_box->GetValue().ToDouble(&z);
	r_box->GetValue().ToDouble(&r);
	p_box->GetValue().ToDouble(&p);
	yw_box->GetValue().ToDouble(&yw);
	t.position=Vector3D(x,y,z);
	t.orientation.setRPY(r,p,yw);

	text=name_box->GetValue();
	pos->setRelativeT3D(t);
		

GetSetSpecificValues(winId,object);

world->getChild()->UpdateWorld();

}

void InitialProperties::OnButton(wxCommandEvent& event)
{
	int id=event.GetId();
	
	if(id == ID_ACCEPT)
	{	
		pos->setName(text);
		b_sel=1;
		Close(true);
	}

	if(id == ID_DEFAULT)
	{
		S_G=false;
		t.position=Vector3D(0,0,0);
		t.orientation.setRPY(0,0,0);
		pos->setRelativeT3D(t);
		pos->setName(pos->getName());
		object.solidentity->setColor(128.0/255,128.0/255,128.0/255);
		GetSetSpecificValues(winId,object);
		b_sel=1;
		Close(true);			
	}

	if(id == ID_COLOR)
	{
		wxColor color=wxGetColourFromUser(this);
		if(color.IsOk())
		{
			double red,green,blue;
			red = color.Red();
			green = color.Green();
			blue = color.Blue();
			object.solidentity->setColor(red/255,green/255,blue/255);
		}
	}
	
	if(id==ID_CANCEL)
	{
		b_sel=2;
		Close(true);	
	}

}


void InitialProperties::GetSetSpecificValues(wxWindowID id, Object &tobject)
{
if(id==ID_ADDSPHERE)
		{
			if(S_G)
				{
				tobject.spherepart=dynamic_cast<SpherePart *>(pos);
				tobject.spherepart->setRadius(c_radio->getValue());
				}
			else
				tobject.spherepart->setRadius(1);
			
		}
if(id==ID_ADDCYL)
{
			if(S_G)
			{
				tobject.cylindricalpart=dynamic_cast<CylindricalPart *>(pos);
				tobject.cylindricalpart->setHeightAndRadius(c_height->getValue(),c_radio->getValue());
			}
			else
				tobject.cylindricalpart->setHeightAndRadius(1,1);
		}
if(id==ID_ADDPRI)
		{
				tobject.prismaticpart=dynamic_cast<PrismaticPart *>(pos);
				tobject.prismaticpart->setRegularPolygonBase(c_radio->getValue(),c_vertex->getValue());
				tobject.prismaticpart->setHeight(c_height->getValue());			
		}

}