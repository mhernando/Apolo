#include "designProperties.h"


BEGIN_EVENT_TABLE(DesignProperties, wxDialog)
	EVT_BUTTON(ID_ACCEPT, DesignProperties::OnButton)
	EVT_BUTTON(ID_CANCEL, DesignProperties::OnButton)
	EVT_CLOSE(DesignProperties::OnClose)
END_EVENT_TABLE()

DesignProperties::DesignProperties(wxWindow *parent, NodeTree *itemData,SimulatedWorld *s_world, const wxString& title)
:wxDialog(parent,wxID_ANY,title, wxPoint(10,10), wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{
	node=itemData;
	world=s_world;
	CreatePanel();
	

}

void DesignProperties::CreatePanel()
{
	DesignWidget *design=new DesignWidget(this,wxID_ANY,node,world,wxEmptyString,wxDefaultPosition,wxDefaultSize,MainWindow::design_slider,true);
	
	
	wxBoxSizer *vbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *b_box=new wxBoxSizer(wxHORIZONTAL);

	accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxSize(60,25));
	cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(60,25));
	b_box->Add(accept,1,wxALIGN_BOTTOM,5);
	b_box->Add(cancel,1,wxALIGN_BOTTOM,5);	

	vbox->Add(design,0,wxEXPAND| wxALL,6 );
	vbox->AddSpacer(20);
	vbox->Add(b_box,0,wxEXPAND | wxALL ,6);
	
	vbox->SetSizeHints(this);
	SetSizer(vbox);
}

void DesignProperties::OnButton(wxCommandEvent& event)
{
	int id=event.GetId();
	
	if(id == ID_ACCEPT)
		Destroy();

	
	if(id==ID_CANCEL)
		Close(true);	
	

}

void DesignProperties::OnClose(wxCloseEvent& event)
{
//design->GetSpecificValues(true);
	Destroy();
}