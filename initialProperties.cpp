
#include "initialProperties.h"
#include "tree.h"


BEGIN_EVENT_TABLE(InitialProperties, wxDialog)



	EVT_BUTTON(ID_ACCEPT, InitialProperties::OnButton)
	EVT_BUTTON(ID_CANCEL, InitialProperties::OnButton)
	EVT_BUTTON(ID_DEFAULT, InitialProperties::OnButton)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, InitialProperties::RefreshCanvas)
	EVT_CLOSE(InitialProperties::OnClose)
END_EVENT_TABLE()

InitialProperties::InitialProperties(wxWindow *parent, NodeTree *obj, const wxString& title,wxWindowID id)
:wxDialog(parent,id, title, wxPoint(0,0), wxSize(600,800),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxMAXIMIZE_BOX ) 
{
	mainWin=(MainWindow*)parent;
	b_sel=true;
	worldView=true;
	node=obj;
	wID=id;
	defName=node->getSimu()->tree->GetItemText(node->getSimu()->tree->GetLastChild(node->getSimu()->tree->GetSelection()));
	CreatePanel();
	
}


void InitialProperties::CreatePanel()
{
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *tbox=new wxBoxSizer(wxHORIZONTAL);
	tbox->Add(vbox,1,wxEXPAND);
	bool color=true;
	
	
	if(node->getSimu()->mainWin->getToogleReference())
	{	Tree *tree=node->getSimu()->tree;
		node->getSimu()->mainWin->Search(tree->GetLastChild(tree->GetSelection()),true);
	}
		
	if(dynamic_cast<ComposedEntity *>(node->pointer.positionableentity))	
		color=false;
		
	
	if(wID==ID_ADDFACESET)
	{
		node->pointer.facesetpart=dynamic_cast<FaceSetPart *>(node->pointer.positionableentity);
		face=new FaceWindow(this,node,wxEmptyString,wxDefaultPosition,wxSize(1040,700));
		vbox->Add(face,0,wxEXPAND);	
	}
			
	else
	{
	

	dp=new DesignWidget(this,node,wxEmptyString,wxDefaultPosition , wxDefaultSize,mainWin->getDesignValue());
	
		if (wID!=ID_ADDIRRPRI)
			pw=new PositionableWidget(this,node,wxT("Positionable Parameters"),wxDefaultPosition,wxDefaultSize,mainWin->getSliderValue(),color);
		else
			pw=new PositionableWidget(this,node,wxT("Positionable Parameters"),wxDefaultPosition,wxDefaultSize,mainWin->getSliderValue(),color,ID_ADDIRRPRI);

	wxButton *df = new wxButton(this,ID_DEFAULT,wxT("Create object with default parameters"),wxDefaultPosition,wxDefaultSize);
	
	vbox->Add(df,0,wxEXPAND);
	vbox->Add(pw,0,wxEXPAND);
	vbox->Add(dp,0,wxEXPAND);
	//vbox->AddSpacer(40);

		if(wID==ID_ADDIRRPRI)

		{
			priW=new PrismWindow(this,node,wxEmptyString,wxDefaultPosition,wxDefaultSize);
			tbox->Add(priW,0,wxEXPAND);	
		}
	}

	

	////Buttom box///
	wxBoxSizer *b_box=new wxBoxSizer(wxHORIZONTAL);
	wxButton *accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxDefaultSize);
	wxButton *cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxDefaultSize);
	b_box->Add(accept,1,wxALIGN_BOTTOM);
	b_box->Add(cancel,1,wxALIGN_BOTTOM);


	
	//Close Dialog Design//
	
	vbox->Add(b_box,1,wxEXPAND);
	vbox->SetMinSize(vbox->GetMinSize());
	SetSizer(tbox);
	tbox->SetSizeHints(this);
}



void InitialProperties::OnButton(wxCommandEvent& event)
{
	
	int id=event.GetId();
	

	if(id == ID_ACCEPT)	
	{	
		if(wID==ID_ADDFACESET)
		{
			face->AddFace();
		}
			
		
		Destroy();

	}

	if(id == ID_DEFAULT)
	{
		Transformation3D t;
		t.position=pw->getDefPosition();
		Vector3D defOrientation=pw->getDefOrientation();
		t.orientation.setRPY(defOrientation.x,defOrientation.y,defOrientation.z);
		node->pointer.positionableentity->setRelativeT3D(t);
		dp->SetSpecificValues(true);
		node->getSimu()->tree->SetItemText(node->getSimu()->tree->GetLastChild(node->getSimu()->tree->GetSelection()),defName);
		node->pointer.solidentity->setColor(pw->getDefRedColor(),pw->getDefGreenColor(),pw->getDefBlueColor());
		Destroy();

	}
	if(id==ID_CANCEL)
		Close(true);
	
	
	

}

void InitialProperties::RefreshCanvas(wxCommandEvent &event)
{
	if(wID==ID_ADDIRRPRI)
		priW->RefreshCanvas();
}

