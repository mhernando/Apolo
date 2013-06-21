#include "robotSimGoTo.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(RobotSimGoTo, wxFrame)
	EVT_BUTTON(ID_ACCEPT, RobotSimGoTo::OnButton)
	EVT_BUTTON(ID_CANCEL, RobotSimGoTo::OnButton)
	EVT_CLOSE(RobotSimGoTo::OnClose)
END_EVENT_TABLE()

RobotSimGoTo::RobotSimGoTo(wxWindow *parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData,
							 NodeTree *parentData)
: wxFrame(parent, id,title_frame, wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{
	itemnode=itemData;
	noDelete=false;

	this->SetIcon(wxIcon(goto_xpm));

	wxPanel* panel = new wxPanel(this, wxID_ANY);
//declaration general wxBoxsizer	
	
	wxBoxSizer *parentbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *childbox=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *childLeftbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *childRightbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *informationRobot=new wxBoxSizer(wxVERTICAL);
	
//title
	title = new wxStaticText(panel,wxID_ANY,wxEmptyString, wxDefaultPosition, wxDefaultSize);
	title->SetFont(wxFont(14,wxFONTFAMILY_ROMAN));
	wxBoxSizer *titlebox = new wxBoxSizer(wxHORIZONTAL);
	titlebox->AddSpacer(15);
	titlebox->Add(title,0,wxEXPAND|wxALL);

//robot location information and its Tcp location too
	Vector3D pos,orient;
	pos=itemData->pointer.robotsim->getRelativePosition();
	itemData->pointer.robotsim->getRelativeOrientation(orient.x,orient.y,orient.z);
	Transformation3D tcpLocat;
	tcpLocat=itemnode->pointer.robotsim->getTcpLocation();

	stringstream str,str1;
	str<<"Position: {x="<<pos.x<<", y="<<pos.y<<", z="<<pos.z<<'}';
	str1<<"Orientation: {roll="<<orient.x<<", pitch="<<orient.y<<", yaw="<<orient.z<<'}';

	wxStaticBoxSizer *infor=new wxStaticBoxSizer (wxVERTICAL,panel,wxT("LOCATION ROBOT INFORMATION"));
	wxStaticText* positionRobot=new wxStaticText(panel,wxID_ANY, wxString(str.str()),wxDefaultPosition,wxDefaultSize);
	wxStaticText* orientationRobot=new wxStaticText(panel,wxID_ANY, wxString(str1.str()),wxDefaultPosition,wxDefaultSize);
	infor->AddSpacer(15);
	infor->Add(positionRobot,0,wxEXPAND|wxALL);
	infor->AddSpacer(15);
	infor->Add(orientationRobot,0,wxEXPAND|wxALL);

	childLeftbox->Add(infor,0,wxEXPAND|wxALL);
	
//tcp location

	str.str(string());
	str1.str(string());
	Vector3D tcpOrient;
	tcpLocat.orientation.getRPY(tcpOrient.x,tcpOrient.y,tcpOrient.z);
	str<<"Position: {x="<<tcpLocat.position.x<<", y="<<tcpLocat.position.y<<", z="<<tcpLocat.position.z<<'}';
	str1<<"Orientation: {roll="<<tcpOrient.x<<", pitch="<<tcpOrient.y<<", yaw="<<tcpOrient.z<<'}';

	wxStaticBoxSizer *inforTcp=new wxStaticBoxSizer (wxVERTICAL,panel,wxT("LOCATION TCP INFORMATION"));
	wxStaticText* positionTcp=new wxStaticText(panel,wxID_ANY, wxString(str.str()),wxDefaultPosition,wxDefaultSize);
	wxStaticText* orientationTcp=new wxStaticText(panel,wxID_ANY, wxString(str1.str()),wxDefaultPosition,wxDefaultSize);
	inforTcp->AddSpacer(15);
	inforTcp->Add(positionTcp,0,wxEXPAND|wxALL);
	inforTcp->AddSpacer(15);
	inforTcp->Add(orientationTcp,0,wxEXPAND|wxALL);
	
	childLeftbox->Add(inforTcp,0,wxEXPAND|wxALL);

//buttons

	wxButton* accept = new wxButton(panel,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxDefaultSize);
	wxButton* cancel = new wxButton(panel,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxDefaultSize);
	childLeftbox->AddSpacer(70);
	childLeftbox->Add(accept,0,wxEXPAND|wxALIGN_BOTTOM);
	childLeftbox->AddSpacer(5);
	childLeftbox->Add(cancel,0,wxEXPAND|wxALIGN_BOTTOM);

//points XYZ to fill

	wxStaticBoxSizer *targ=new wxStaticBoxSizer(wxVERTICAL,panel,wxT("TARGET"));

	wxStaticBoxSizer *_X=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("X COORDINATE"));//Creacion de un nuevo wxStaticSizer
	coorX = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_X->Add(coorX,0,wxEXPAND|wxALL);

	wxStaticBoxSizer *_Y=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("Y COORDINATE"));//Creacion de un nuevo wxStaticSizer
	coorY = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);	
	_Y->Add(coorY,0,wxEXPAND|wxALL);
	
	wxStaticBoxSizer *_Z=new wxStaticBoxSizer (wxHORIZONTAL,panel,wxT("Z COORDINATE"));//Creacion de un nuevo wxStaticSizer
	coorZ = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_Z->Add(coorZ,0,wxEXPAND|wxALL);
	
	targ->AddSpacer(5);
	targ->Add(_X,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_Y,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_Z,0,wxEXPAND|wxALL);

	childRightbox->Add(targ,0,wxEXPAND|wxALL);


//joining all
	childbox->AddSpacer(15);
	childbox->Add(childLeftbox,0,wxEXPAND|wxALL);
	childbox->AddSpacer(15);
	childbox->Add(childRightbox,0,wxEXPAND|wxALL);
	childbox->AddSpacer(15);


	parentbox->Add(titlebox,0,wxEXPAND|wxALL);
	parentbox->AddSpacer(20);
	parentbox->Add(childbox,0,wxEXPAND|wxALL);
	parentbox->AddSpacer(20);


	panel->SetSizer(parentbox);
	parentbox->SetSizeHints(this);

	
}

void RobotSimGoTo::OnButton(wxCommandEvent& event)
{
	int id = event.GetId();


	if(id == ID_ACCEPT)
	{
		OnValueChanges();
		if (!noDelete)
			Delete();
		noDelete=false;
	}
	
	if(id == ID_CANCEL)
	{
		Delete();

	}
	event.Skip();
}

void RobotSimGoTo::OnValueChanges()
{

	double x=0.0,y=0.0,z=0.0;
	wxString value;

	if (coorX->GetValue()!=wxEmptyString && coorY->GetValue()!=wxEmptyString && coorZ->GetValue()!=wxEmptyString)
	{	
		value=coorX->GetValue();
		value.ToDouble(&x);
		value.Clear();
		value=coorY->GetValue();
		value.ToDouble(&y);
		value.Clear();
		value=coorZ->GetValue();
		value.ToDouble(&z);
		value.Clear();
		Transformation3D t(x,y,z);
		target=t;
//		itemnode->pointer.robotsim->goTo(target);
	}
	else
	{
		wxString msg;
		msg.Printf(wxT("You have to fill all the gaps -> TARGET = {X,Y,Z}"));
		wxMessageBox(msg, wxT("Please confirm"));
		noDelete=true;
	}

}

void RobotSimGoTo::setManageWindow (ManageWindows* mg)
{
	managewindow=mg;
	mg->addWindowRobotSimGoTo(this);

}

void RobotSimGoTo::Delete()
{

	managewindow->WindowRobotSimGoToIsClosed(this);
	Destroy();	

}

void RobotSimGoTo::OnClose(wxCloseEvent& event)
{

	managewindow->WindowRobotSimGoToIsClosed(this);
	Destroy();	

}
