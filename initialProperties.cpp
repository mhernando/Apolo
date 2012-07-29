#include "initialProperties.h"


BEGIN_EVENT_TABLE(InitialProperties, wxDialog)


	EVT_CLOSE(InitialProperties::OnClose)
	EVT_BUTTON(ID_ACCEPT, InitialProperties::OnButton)
	EVT_BUTTON(ID_CANCEL, InitialProperties::OnButton)
	EVT_BUTTON(ID_DEFAULT, InitialProperties::OnButton)
//	EVT_WINDOW_CREATE(InitialProperties::Update)
END_EVENT_TABLE()

InitialProperties::InitialProperties(wxWindow *parent, NodeTree *obj,SimulatedWorld *s_world, const wxString& title)
:wxDialog(parent,wxID_ANY, title, wxPoint(0,0), wxSize(300,500),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxRESIZE_BORDER) 
{
	b_sel=true;
	world=s_world;
	pos=obj;
	CreatePanel();
	
}

void InitialProperties::CreatePanel()
{
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	bool color=true;
	
	
	if(pos->getSimu()->mainWin->getToogleReference())
		pos->getSimu()->mainWin->Search(world->getNewNode(),true);
		

	if(dynamic_cast<ComposedEntity *>(pos->pointer.positionableentity))	
		color=false;
		
	
		
	
	
	DesignWidget *dp=new DesignWidget(this,wxID_ANY,pos,world, wxEmptyString,wxDefaultPosition , wxDefaultSize,MainWindow::design_slider,false);
	
	pw=new PositionableWidget(this,pos,world,"Positionable Parameters",wxDefaultPosition,wxDefaultSize,MainWindow::slider,color);
	
	df = new wxButton(this,ID_DEFAULT,wxT("Create object with default parameters"),wxDefaultPosition,wxDefaultSize);
	
	vbox->Add(df,0,wxEXPAND | wxALL ,12);
	
	////Buttom box///
	wxBoxSizer *b_box=new wxBoxSizer(wxHORIZONTAL);

	accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxSize(60,25));
	cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(60,25));
	b_box->Add(accept,1,wxALIGN_BOTTOM,5);
	b_box->Add(cancel,1,wxALIGN_BOTTOM,5);	

	
	//Close Dialog Design//
	
	vbox->Add(pw,0,wxEXPAND);
	vbox->Add(dp,1,wxEXPAND | wxLEFT | wxRIGHT | wxTOP,5);
	
	vbox->AddSpacer(40);
	vbox->Add(b_box,1,wxEXPAND | wxALL ,12);
	
	
	vbox->SetSizeHints(this);
	SetSizer(vbox);
}

void InitialProperties::OnButton(wxCommandEvent& event)
{
	int id=event.GetId();
	
	if(id == ID_ACCEPT)
		Destroy();
	

	if(id == ID_DEFAULT)
	{
		t.position=Vector3D(0,0,0);
		t.orientation.setRPY(0,0,0);
		pos->pointer.positionableentity->setRelativeT3D(t);
		pos->pointer.positionableentity->setName(pos->pointer.positionableentity->getName());
		pos->pointer.solidentity->setColor(128.0/255,128.0/255,128.0/255);
		Destroy();			
	}
	
	if(id==ID_CANCEL )
		Close(true);	
	

}

