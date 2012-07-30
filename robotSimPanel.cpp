#include "robotSimPanel.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(RobotSimPanel, ApoloPanel)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, RobotSimPanel::OnValueChanged)
END_EVENT_TABLE()

RobotSimPanel::RobotSimPanel(wxWindow *parent, wxWindowID id,NodeTree* itemData)
: ApoloPanel(itemData,parent,id)
{
	itemnode = itemData;
	title = new wxStaticText(this,wxID_ANY,wxEmptyString, wxDefaultPosition, wxDefaultSize);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(title,0,wxLEFT|wxTOP,5);

	numJoints = itemnode->pointer.robotsim->getNumJoints();
	RobotSim* robot = itemnode->pointer.robotsim;
	for(int i=0;i<numJoints;i++)
	{	
		
		nameJoint.Printf(wxT("Joint %d"),i);
		joint = new GenericSlider(this,nameJoint,wxDefaultPosition,wxDefaultSize/*wxSize(300,300)*/,false);

		listJoints.push_back(joint);
		vbox->Add(joint,0,wxEXPAND|wxALL,5);
		
	}
	for(int i=0;i<numJoints;i++)
	{
		double max,min,val;
		robot->getJointLimits(i,max,min);
		listJoints[i]->setProperties(min,max,true);
		robot->getJointValue(i,val);
		listJoints[i]->setValue(val);
	}
	SetSizer(vbox);
	vbox->SetSizeHints( this );	
}
void RobotSimPanel::OnValueChanged(wxCommandEvent& event)
{
	
	for(int i=0;i<numJoints;i++)
	{
		if(listJoints[i]->IsMouseInWindow())
		{
			double value = listJoints[i]->getValue();
			itemnode->pointer.robotsim->setJointValue(i,value);
			itemnode->getSimu()->getChild()->RefreshChild();
		}
	}
	event.Skip();
}

