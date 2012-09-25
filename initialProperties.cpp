#include "initialProperties.h"
#include "tree.h"


BEGIN_EVENT_TABLE(InitialProperties, wxDialog)



	EVT_BUTTON(ID_ACCEPT, InitialProperties::OnButton)
	EVT_BUTTON(ID_CANCEL, InitialProperties::OnButton)
	EVT_BUTTON(ID_DEFAULT, InitialProperties::OnButton)
	EVT_BUTTON(ID_PRIBASE, InitialProperties::OnButton)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, InitialProperties::RefreshCanvas)
	EVT_CLOSE(InitialProperties::OnClose)
	//VT_WINDOW_CREATE(InitialProperties::Update)
END_EVENT_TABLE()

InitialProperties::InitialProperties(wxWindow *parent, NodeTree *obj, const wxString& title,wxWindowID id)
:wxDialog(parent,id, title, wxPoint(0,0), wxSize(300,500),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxMAXIMIZE_BOX ) 
{
	b_sel=true;
	world=obj->getSimu();
	pos=obj;
	wID=id;
	defName=world->tree->GetItemText(world->tree->GetLastChild(world->tree->GetSelection()));
	CreatePanel();
	
}

void InitialProperties::CreatePanel()
{

	
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *tbox=new wxBoxSizer(wxHORIZONTAL);
	tbox->Add(vbox,1,wxEXPAND);
	bool color=true;
	
	
	if(pos->getSimu()->mainWin->getToogleReference())
	{	Tree *tree=world->tree;
		pos->getSimu()->mainWin->Search(tree->GetLastChild(tree->GetSelection()),true);
	}
		
	if(dynamic_cast<ComposedEntity *>(pos->pointer.positionableentity))	
		color=false;
		
	
	if(wID==ID_ADDFACESET)
	{
		pos->pointer.facesetpart=dynamic_cast<FaceSetPart *>(pos->pointer.positionableentity);
		face=new FaceWindow(this,pos,wxEmptyString,wxDefaultPosition,wxDefaultSize);
		vbox->Add(face,0,wxEXPAND);	
	}
			
	else
	{
	

	dp=new DesignWidget(this,pos,wxEmptyString,wxDefaultPosition , wxDefaultSize,MainWindow::design_slider);
	
	pw=new PositionableWidget(this,pos,wxT("Positionable Parameters"),wxDefaultPosition,wxDefaultSize,MainWindow::slider,color);
	
	df = new wxButton(this,ID_DEFAULT,wxT("Create object with default parameters"),wxDefaultPosition,wxDefaultSize);
	
	vbox->Add(df,0,wxEXPAND | wxALL ,12);
	vbox->Add(pw,0,wxEXPAND);
	vbox->Add(dp,1,wxEXPAND | wxLEFT | wxRIGHT | wxTOP,5);
	vbox->AddSpacer(40);

	if(wID==ID_ADDIRRPRI)
		{

		wxStaticBoxSizer *pri=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Base Design"));
		
		base=new FaceWidget(this,world,wxDefaultPosition,wxDefaultSize,false);
		base->ChangeView(true);
		PointsList *points=new PointsList(this);
		wxButton *pri_base = new wxButton(this,ID_PRIBASE,wxT("Create"),wxDefaultPosition,wxSize(60,25));
		base->AssociatePointTable(points);
		
		pri->Add(base,1,wxEXPAND);
		pri->Add(points,0,wxEXPAND |wxALL, 5);
		pri->Add(pri_base,0,wxEXPAND |wxALL, 5);	
		tbox->Add(pri,1,wxEXPAND | wxALL,5);	
	
		}
	}

	

	////Buttom box///
	wxBoxSizer *b_box=new wxBoxSizer(wxHORIZONTAL);
	accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxSize(60,25));
	cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(60,25));
	b_box->Add(accept,1,wxALIGN_BOTTOM | wxALL,5);
	b_box->Add(cancel,1,wxALIGN_BOTTOM | wxALL ,5);	

	
	//Close Dialog Design//
	
	vbox->Add(b_box,1,wxEXPAND);
	
	SetSizer(tbox);
	tbox->SetSizeHints(this);
	
	
}

void InitialProperties::OnButton(wxCommandEvent& event)
{
	
	int id=event.GetId();
	
	if(id == ID_PRIBASE)	
	{
		pos->pointer.prismaticpart->setPolygonalBase(*(base->GetFace()));
		base->CreateFace();	
	}

	if(id == ID_ACCEPT)	Destroy();

	if(id == ID_DEFAULT)
	{
		t.position=pw->getDefPosition();
		Vector3D defOrientation=pw->getDefOrientation();
		t.orientation.setRPY(defOrientation.x,defOrientation.y,defOrientation.z);
		pos->pointer.positionableentity->setRelativeT3D(t);
		dp->SetSpecificValues(true);
		world->tree->SetItemText(world->tree->GetLastChild(world->tree->GetSelection()),defName);
		pos->pointer.solidentity->setColor(pw->getDefRedColor(),pw->getDefGreenColor(),pw->getDefBlueColor());
		Destroy();

	}
	if(id==ID_CANCEL)
		Close(true);
	
	
	

}

void InitialProperties::RefreshCanvas(wxCommandEvent &event)
{
	if(wID==ID_ADDIRRPRI)
		base->RefreshCanvas();
}

