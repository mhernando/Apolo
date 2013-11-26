#include "robotSimPanel.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(RobotSimPanel, wxFrame)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, RobotSimPanel::OnValueChanged)
	EVT_CLOSE(RobotSimPanel::OnClose)
END_EVENT_TABLE()

RobotSimPanel::RobotSimPanel(wxWindow *parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData,
							 NodeTree *parentData, bool onlyjoint)
: wxFrame(parent, id,title_frame, wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{
	this->SetIcon(wxIcon(joint_xpm));
	itemnode = itemData;
	itemParentData=parentData;
	simplejoint=onlyjoint;


	wxPanel* panel=new wxPanel(this, wxID_ANY);
	title = new wxStaticText(panel,wxID_ANY,wxEmptyString, wxDefaultPosition, wxDefaultSize);
	
	wxBoxSizer *nbox = new wxBoxSizer(wxHORIZONTAL);
	nbox->Add(title,0,wxEXPAND|wxALL,5);
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);


	
	if (!simplejoint)
	{
			numJoints = itemnode->pointer.robotsim->getNumJoints();
			RobotSim* robot = itemnode->pointer.robotsim;
			for(int i=0;i<numJoints;i++)
			{	
				
				nameJoint.Printf(wxT("Joint %d"),i);
				joint = new GenericSlider(panel,nameJoint,wxDefaultPosition,wxDefaultSize,false);

				listJoints.push_back(joint);
				vbox->Add(joint,0,wxEXPAND|wxALL,5);
				vbox->AddSpacer(5);
				
			}
			for(int i=0;i<numJoints;i++)
			{
				double max,min,val;
				robot->getJointLimits(i,max,min);
				listJoints[i]->setProperties(min,max,true);
				robot->getJointValue(i,val);
				listJoints[i]->setValue(val);
			}
	}
	else
	{
				SimpleJoint* simplejoint = itemnode->pointer.simplejoint;
				nameJoint.Printf(wxT("Joint"));
				joint = new GenericSlider(panel,nameJoint,wxDefaultPosition,wxDefaultSize,false);
				vbox->Add(joint,0,wxEXPAND|wxALL,5);
				
				double max1,min1,val1;
				simplejoint->getMaxMin(max1,min1);
				joint->setProperties(min1,max1,true);
				val1=simplejoint->getValue();
				joint->setValue(val1);

	}
	wxBoxSizer *tbox = new wxBoxSizer(wxVERTICAL);
	tbox->Add(nbox,0,wxEXPAND|wxALIGN_TOP,5);
	tbox->Add(vbox,0,wxEXPAND|wxALIGN_BOTTOM,5);


	panel->SetSizer(tbox);
	tbox->SetSizeHints( this );
	
}
void RobotSimPanel::OnValueChanged(wxCommandEvent& event)
{
	
	if (!simplejoint)
	
		for(int i=0;i<numJoints;i++)
		{

				double value = listJoints[i]->getValue();
				itemnode->pointer.robotsim->setJointValue(i,value);
				itemnode->getSimu()->getChild()->RefreshChild();

		}
	
	else
	{
		double value = joint->getValue();
		itemnode->pointer.simplejoint->setValue(value);
		itemnode->getSimu()->getChild()->RefreshChild();
	}
	event.Skip();
}

void RobotSimPanel::setManageWindow (ManageWindows* mg)
{
	managewindow=mg;
	mg->addWindowRobotSim(this);

}

void RobotSimPanel::Delete()
{

	managewindow->WindowRobotSimIsClosed(this);
	Destroy();	

}



void RobotSimPanel::OnClose(wxCloseEvent& event)
{

	managewindow->WindowRobotSimIsClosed(this);
	Destroy();	

}