#include "simulatedWorld.h"
#include "nodeTree.h"
#include "changeLocationCtrl.h"

BEGIN_EVENT_TABLE(ChangeLocationCtrl, wxDialog)
	EVT_BUTTON(ID_ACCEPT, ChangeLocationCtrl::OnButton)
	EVT_BUTTON(ID_CANCEL, ChangeLocationCtrl::OnButton)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, ChangeLocationCtrl::OnValueChanged)
END_EVENT_TABLE()

ChangeLocationCtrl::ChangeLocationCtrl(wxWindow *parent, wxWindowID id, const wxString& title)
:wxDialog(parent, id, title, wxDefaultPosition, wxSize(525,300),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{
	itemnode = 0;
	winId = id;

	panel = new wxPanel(this, wxID_ANY);

	accept = new wxButton(panel,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxSize(50,25));
	cancel = new wxButton(panel,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(50,25));

	if(id == ID_ORI)//change orientation
	{
		control0 = new GenericSlider(panel,wxT("Roll"),wxDefaultPosition,wxDefaultSize,true);//true = vertical
		control1 = new GenericSlider(panel,wxT("Pitch"),wxDefaultPosition,wxDefaultSize,true);
		control2 = new GenericSlider(panel,wxT("Yaw"),wxDefaultPosition,wxDefaultSize,true);
		control0->setProperties(-180,180);
		control1->setProperties(-180,180);
		control2->setProperties(-180,180);
		
	}
	else//change position
	{
		control0 = new GenericSlider(panel,wxT("X (m)"),wxDefaultPosition,wxDefaultSize,true);
		control1 = new GenericSlider(panel,wxT("Y (m)"),wxDefaultPosition,wxDefaultSize,true);
		control2 = new GenericSlider(panel,wxT("Z (m)"),wxDefaultPosition,wxDefaultSize,true);
		control0->setProperties(-10,10);
		control1->setProperties(-10,10);
		control2->setProperties(-10,10);
		
	}

	
	
	
	
	wxBoxSizer *hbbox = new wxBoxSizer(wxHORIZONTAL);
	hbbox->Add(accept,0,wxALIGN_LEFT);
	hbbox->AddSpacer(5);
	hbbox->Add(cancel,0,wxALIGN_RIGHT);

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(control0,0,wxEXPAND|wxALL,5);
	hbox->Add(control1,0,wxEXPAND|wxALL,5);
	hbox->Add(control2,0,wxEXPAND|wxALL,5);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(hbox,1,wxEXPAND|wxALL,5);
	vbox->Add(hbbox,wxSizerFlags(0).Align(wxALIGN_BOTTOM));
	panel->SetSizer(vbox);
	vbox->SetMinSize(300,300);
	vbox->SetSizeHints(this);
}
void ChangeLocationCtrl::OnValueChanged(wxCommandEvent& event)
{
	Transformation3D t=itemnode->pointer.positionableentity->getRelativeT3D();
	
	if(winId == ID_ORI)
	{
		double roll,pitch,yaw;
		roll=deg2rad(control0->getValue());
		pitch=deg2rad(control1->getValue());
		yaw=deg2rad(control2->getValue());
		t.orientation.setRPY(roll,pitch,yaw);
	}
	else 
	{//POS
		double x,y,z;
		x=control0->getValue();
		y=control1->getValue();
		z=control2->getValue();
		t.position=Vector3D(x,y,z);
	}
	

	itemnode->pointer.positionableentity->setRelativeT3D(t);
	itemnode->getSimu()->getChild()->RefreshChild();
	event.Skip();
}
void ChangeLocationCtrl::OnButton(wxCommandEvent& event)
{
	int id = event.GetId();
	wxObject *obj = event.GetEventObject();

	if(id == ID_ACCEPT)
	{
		Close(true);
	}
	if(id == ID_CANCEL)
	{
		control0->setValue(initialValues[0]);
		control1->setValue(initialValues[1]);
		control2->setValue(initialValues[2]);
		Close(true);
	}
	event.Skip();
}
void ChangeLocationCtrl::setItemData(NodeTree * node)
{
	itemnode = node;
	if(winId == ID_ORI)
	{
		Transformation3D t = itemnode->pointer.positionableentity->getRelativeT3D();
		double roll,pitch,yaw;
		t.orientation.getRPY(roll,pitch,yaw);
		initialValues[0]=roll;
		initialValues[1]=pitch;
		initialValues[2]=yaw;
		control0->setValue(rad2deg(roll));
		control1->setValue(rad2deg(pitch));
		control2->setValue(rad2deg(yaw));
	}
	if(winId == ID_POSIT)
	{
		Transformation3D t = itemnode->pointer.positionableentity->getRelativeT3D();
		initialValues[0]=t.position.x;
		initialValues[1]=t.position.y;
		initialValues[2]=t.position.z;
		control0->setProperties(t.position.x-5,t.position.x+5,false);
		control0->setValue(t.position.x);
		control1->setProperties(t.position.y-5,t.position.y+5,false);
		control1->setValue(t.position.y);
		control2->setProperties(t.position.z-5,t.position.z+5,false);
		control2->setValue(t.position.z);
	}
	
}
