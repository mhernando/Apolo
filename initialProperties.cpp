#include "initialProperties.h"


BEGIN_EVENT_TABLE(InitialProperties, wxDialog)


	EVT_CLOSE(InitialProperties::OnClose)
	EVT_BUTTON(ID_ACCEPT, InitialProperties::OnButton)
	EVT_BUTTON(ID_CANCEL, InitialProperties::OnButton)
	EVT_BUTTON(ID_DEFAULT, InitialProperties::OnButton)
//	EVT_WINDOW_CREATE(InitialProperties::Update)
END_EVENT_TABLE()

InitialProperties::InitialProperties(wxWindow *parent, NodeTree *obj,SimulatedWorld *s_world, const wxString& title)
:wxDialog(parent,wxID_ANY, title, wxPoint(0,0), wxSize(300,500),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxMAXIMIZE_BOX ) 
{
	b_sel=true;
	world=s_world;
	pos=obj;
	defName=s_world->tree->GetItemText(s_world->tree->GetLastChild(s_world->tree->GetSelection()));
	CreatePanel();
	
}

void InitialProperties::CreatePanel()
{

	
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	bool color=true;
	
	
	if(pos->getSimu()->mainWin->getToogleReference())
		pos->getSimu()->mainWin->Search(world->getNewNode(),true);
		

	if(dynamic_cast<ComposedEntity *>(pos->pointer.positionableentity) || dynamic_cast<FaceSetPart *>(pos->pointer.positionableentity))	
		color=false;
		
	if(dynamic_cast<FaceSetPart *>(pos->pointer.positionableentity))
	{
		pos->pointer.facesetpart=dynamic_cast<FaceSetPart *>(pos->pointer.positionableentity);
		face=new FaceWindow(this,pos,world,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize);
		vbox->Add(face,0,wxEXPAND);	
	}
	
	else
	{
	
	dp=new DesignWidget(this,wxID_ANY,pos,world, wxEmptyString,wxDefaultPosition , wxDefaultSize,MainWindow::design_slider,false);
	
	pw=new PositionableWidget(this,pos,world,"Positionable Parameters",wxDefaultPosition,wxDefaultSize,MainWindow::slider,color);
	
	df = new wxButton(this,ID_DEFAULT,wxT("Create object with default parameters"),wxDefaultPosition,wxDefaultSize);
	
	vbox->Add(df,0,wxEXPAND | wxALL ,12);
	vbox->Add(pw,0,wxEXPAND);
	vbox->Add(dp,1,wxEXPAND | wxLEFT | wxRIGHT | wxTOP,5);

	vbox->AddSpacer(40);
	}
	
	
	////Buttom box///
	wxBoxSizer *b_box=new wxBoxSizer(wxHORIZONTAL);

	accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxSize(60,25));
	cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(60,25));
	b_box->Add(accept,1,wxALIGN_BOTTOM,5);
	b_box->Add(cancel,1,wxALIGN_BOTTOM,5);	

	
	//Close Dialog Design//
	
	vbox->Add(b_box,1,wxEXPAND | wxALL ,12);
	
	SetSizer(vbox);
	vbox->SetSizeHints(this);
	
	
}

void InitialProperties::OnButton(wxCommandEvent& event)
{
	
	int id=event.GetId();
	
	if(id == ID_ACCEPT)	
	{
		Close(true);
		b_sel=true;
	}


	if(id == ID_DEFAULT)
	{
		t.position=pw->getDefPosition();
		Vector3D defOrientation=pw->getDefOrientation();
		t.orientation.setRPY(defOrientation.x,defOrientation.y,defOrientation.z);
		pos->pointer.positionableentity->setRelativeT3D(t);
		dp->GetSpecificValues(true);
		world->tree->SetItemText(world->tree->GetLastChild(world->tree->GetSelection()),defName);
		pos->pointer.solidentity->setColor(pw->getDefRedColor(),pw->getDefGreenColor(),pw->getDefBlueColor());
		Destroy();			
	}
	
	if(id==ID_CANCEL )
		Close(true);
	
	

}

void InitialProperties::OnClose(wxCloseEvent& event)
{
	b_sel=false;
	if(dynamic_cast<FaceSetPart *>(pos->pointer.positionableentity))
	{
		world->getChild()->getCanvas()->Reparent(face->GetParentWorld());
		face->GetParentWorld()->SendSizeEvent();
		
	} 
	Destroy();
}