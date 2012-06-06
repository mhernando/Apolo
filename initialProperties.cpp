#include "initialProperties.h"


BEGIN_EVENT_TABLE(InitialProperties, wxDialog)
	EVT_COMMAND(wxID_ANY, wxEVT_POSITIONABLE_WIDGET_CHANGE, InitialProperties::OnValueChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_POSITIONABLE_WIDGET_COLOR, InitialProperties::ChangeColor)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, InitialProperties::OnValueChanged)
	EVT_TEXT(ID_ENTER,InitialProperties::OnValueChanged)
	EVT_CLOSE(InitialProperties::OnClose)
	EVT_BUTTON(ID_ACCEPT, InitialProperties::OnButton)
	EVT_BUTTON(ID_CANCEL, InitialProperties::OnButton)
	EVT_BUTTON(ID_DEFAULT, InitialProperties::OnButton)
	EVT_BUTTON(ID_COLOR, InitialProperties::OnButton)
END_EVENT_TABLE()

InitialProperties::InitialProperties(wxWindow *parent,wxWindowID id, PositionableEntity *obj,SimulatedWorld *s_world, const wxString& title)
:wxDialog(parent,id, title, wxPoint(0,0), wxSize(300,500),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxRESIZE_BORDER) 
{
	b_sel=0;
	S_G=true;
	winId=id;

	world=s_world;
	pos=obj;
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	
	pw=new PositionableWidget(this,"Positionable Parameters",wxDefaultPosition,wxDefaultSize,MainWindow::slider,MainWindow::orientation);

	df = new wxButton(this,ID_DEFAULT,wxT("Create object with default parameters"),wxDefaultPosition,wxDefaultSize);
	
	vbox->Add(df,0,wxEXPAND | wxALL ,12);
	
	
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
	
	vbox->Add(pw,0,wxEXPAND);
	vbox->Add(pbox,1,wxEXPAND | wxLEFT | wxRIGHT | wxTOP,5 );
	vbox->AddSpacer(40);
	vbox->Add(b_box,1,wxEXPAND | wxALL ,12);
	
	
	
	
	vbox->SetSizeHints(this);
	SetSizer(vbox);

}

void InitialProperties::OnValueChanged(wxCommandEvent& event)
{

	pw->GetValues(x,y,z,r,p,yw,text);
	t.position=Vector3D(x,y,z);
	t.orientation.setRPY(r,p,yw);
	pos->setRelativeT3D(t);
	GetSetSpecificValues(winId,object);
	world->getChild()->UpdateWorld();
}

void InitialProperties::ChangeColor(wxCommandEvent& event)
{
	pw->GetColor(red,green,blue);
	object.solidentity->setColor(red/255,green/255,blue/255);
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
	
	if(id==ID_CANCEL )
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
