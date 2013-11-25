#include "robotSimGoTo.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(RobotSimGoTo, wxFrame)
	EVT_BUTTON(ID_ACCEPT, RobotSimGoTo::OnButton)
	EVT_BUTTON(ID_CANCEL, RobotSimGoTo::OnButton)
	//EVT_BUTTON(ID_TVP, RobotSimGoTo::OnButton)
	//EVT_BUTTON(ID_CPT, RobotSimGoTo::OnButton)
	EVT_CLOSE(RobotSimGoTo::OnClose)
END_EVENT_TABLE()

RobotSimGoTo::RobotSimGoTo(wxWindow *parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData,
							 NodeTree *parentData)
: wxFrame(parent, id, title_frame, wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{

	itemnode=itemData;
	noDelete=false;
	this->SetIcon(wxIcon(goto_xpm));

	wxPanel* panel = new wxPanel(this, wxID_ANY);

//declaration general wxBoxsizer	
	
	wxBoxSizer *parentbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *childbox=new wxBoxSizer(wxHORIZONTAL);
	
//title
	wxBoxSizer *titlebox = new wxBoxSizer(wxHORIZONTAL);//container
	title = new wxStaticText(panel,wxID_ANY,wxEmptyString, wxDefaultPosition, wxDefaultSize);
	title->SetFont(wxFont(14,wxFONTFAMILY_ROMAN));

	titlebox->AddSpacer(15);
	titlebox->Add(title,0,wxEXPAND|wxALL);



//robot location information and its Tcp one too

	wxStaticBoxSizer *inforAll=new wxStaticBoxSizer (wxHORIZONTAL,panel,wxT("POSITION AND ORIENTATION"));

	Vector3D pos,orient;
	pos=itemData->pointer.robotsim->getRelativePosition();
	itemData->pointer.robotsim->getRelativeOrientation(orient.x,orient.y,orient.z);
	Transformation3D tcpLocat;
	tcpLocat=itemnode->pointer.robotsim->getTcpLocation();

	stringstream str,str1;
	str<<"Position: {x="<<pos.x<<", y="<<pos.y<<", z="<<pos.z<<'}';
	str1<<"Orientation: {roll="<<orient.x<<", pitch="<<orient.y<<", yaw="<<orient.z<<'}';

	wxStaticBoxSizer *infor=new wxStaticBoxSizer (wxVERTICAL,panel,wxT("Robot"));
	wxStaticText* positionRobot=new wxStaticText(panel,wxID_ANY, wxString(str.str()),wxDefaultPosition,wxDefaultSize);
	wxStaticText* orientationRobot=new wxStaticText(panel,wxID_ANY, wxString(str1.str()),wxDefaultPosition,wxDefaultSize);
	infor->AddSpacer(15);
	infor->Add(positionRobot,0,wxEXPAND|wxALL);
	infor->AddSpacer(15);
	infor->Add(orientationRobot,0,wxEXPAND|wxALL);

	inforAll->Add(infor,0,wxEXPAND|wxALL);
	
	//tcp orientation
	str.str(string());
	str1.str(string());
	Vector3D tcpOrient;
	tcpLocat.orientation.getRPY(tcpOrient.x,tcpOrient.y,tcpOrient.z);
	str<<"Position: {x="<<tcpLocat.position.x<<", y="<<tcpLocat.position.y<<", z="<<tcpLocat.position.z<<'}';
	str1<<"Orientation: {roll="<<tcpOrient.x<<", pitch="<<tcpOrient.y<<", yaw="<<tcpOrient.z<<'}';

	wxStaticBoxSizer *inforTcp=new wxStaticBoxSizer (wxVERTICAL,panel,wxT("TCP"));
	wxStaticText* positionTcp=new wxStaticText(panel,wxID_ANY, wxString(str.str()),wxDefaultPosition,wxDefaultSize);
	wxStaticText* orientationTcp=new wxStaticText(panel,wxID_ANY, wxString(str1.str()),wxDefaultPosition,wxDefaultSize);
	inforTcp->AddSpacer(15);
	inforTcp->Add(positionTcp,0,wxEXPAND|wxALL);
	inforTcp->AddSpacer(15);
	inforTcp->Add(orientationTcp,0,wxEXPAND|wxALL);
	
	inforAll->Add(inforTcp,0,wxEXPAND|wxALL);


//points XYZ to fill
	wxBoxSizer *childRightbox=new wxBoxSizer(wxHORIZONTAL);//container
	wxStaticBoxSizer *targetAll=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("TARGET"));

	wxStaticBoxSizer *targ=new wxStaticBoxSizer(wxVERTICAL,panel,wxT("CARTESIAN"));

	wxStaticBoxSizer *_X=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("X coordinate"));//Creacion de un nuevo wxStaticSizer
	coorX = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_X->Add(coorX,0,wxEXPAND|wxALL);

	wxStaticBoxSizer *_Y=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("Y coordinate"));//Creacion de un nuevo wxStaticSizer
	coorY = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);	
	_Y->Add(coorY,0,wxEXPAND|wxALL);
	
	wxStaticBoxSizer *_Z=new wxStaticBoxSizer (wxHORIZONTAL,panel,wxT("Z coordinate"));//Creacion de un nuevo wxStaticSizer
	coorZ = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_Z->Add(coorZ,0,wxEXPAND|wxALL);
	
	targ->AddSpacer(5);
	targ->Add(_X,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_Y,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_Z,0,wxEXPAND|wxALL);

	targetAll->Add(targ,0,wxEXPAND|wxALL);


//Articular values

	wxStaticBoxSizer *_joints=new wxStaticBoxSizer(wxVERTICAL,panel);
	wxStaticBoxSizer *all_joints=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("ARTICULAR"));
	int aux_counter = 0;

	int numJoints = itemnode->pointer.robotsim->getNumJoints();

	for(int i=0;i<numJoints;i++)
	{		
		wxString nameJoint;
		nameJoint.Printf(wxT("Joint %d"),i);
		joint = new GenericSlider(panel,nameJoint,wxDefaultPosition,wxDefaultSize,false);

		listJoints.push_back(joint);
		_joints->Add(joint,0,wxEXPAND|wxALL,5);
		_joints->AddSpacer(5);
		aux_counter++;
		if (aux_counter==3)
		{
			aux_counter=0;
			all_joints->Add(_joints,0,wxEXPAND|wxALL);
			_joints=new wxStaticBoxSizer(wxVERTICAL,panel);	
		}
		else if (i==numJoints)
			all_joints->Add(_joints,0,wxEXPAND|wxALL);
	//}

	//for(int i=0;i<numJoints;i++)
	//{
		double max,min,val;
		itemnode->pointer.robotsim->getJointLimits(i,max,min);
		listJoints[i]->setProperties(min,max,true);
		itemnode->pointer.robotsim->getJointValue(i,val);
		listJoints[i]->setValue(val);
	}

	targetAll->Add(all_joints,0,wxEXPAND|wxALL);
	childRightbox->Add(targetAll,0,wxEXPAND|wxALL);


//RadioBox type trajectory
	wxBoxSizer *childLeftbox=new wxBoxSizer(wxVERTICAL);//container

	wxArrayString strings0;
	strings0.Add(wxT("Linear Path"));
	strings0.Add(wxT("Default"));

	wxRadioBox* typetraj = new wxRadioBox(panel, ID_CHOOSETYPETRAJECTORY, wxT("TYPE TRAJECTORY"), wxDefaultPosition, wxDefaultSize, strings0, 1, wxRA_SPECIFY_COLS);

	childLeftbox->Add(typetraj,0,wxEXPAND|wxALL);
	childLeftbox->AddSpacer(15);

//RadioBox type movement

	wxArrayString strings1;
	strings1.Add(wxT("Cubic Polinomial Trajectory"));
	strings1.Add(wxT("Trapezoidal Velocity Profile"));
	strings1.Add(wxT("Spline"));

	wxRadioBox* typemov = new wxRadioBox(panel, ID_CHOOSETYPEMOVEMENT, wxT("TYPE MOVEMENT"), wxDefaultPosition, wxDefaultSize, strings1, 1, wxRA_SPECIFY_COLS);

	childLeftbox->Add(typemov,0,wxEXPAND|wxALL);
	childLeftbox->AddSpacer(15);

//RadioBox system reference 
//	wxBoxSizer *refmove=new wxBoxSizer(wxVERTICAL);//container

	wxArrayString strings2;
	strings2.Add(wxT("World (Relative)"));
	strings2.Add(wxT("Robot (Absolute)"));

	wxRadioBox* refsys = new wxRadioBox(panel, ID_CHOOSESYSREFERENCE, wxT("REFERENCE SYSTEM"), wxDefaultPosition, wxDefaultSize, strings2, 1, wxRA_SPECIFY_COLS);

	childLeftbox->Add(refsys,0,wxEXPAND|wxALL);
	childLeftbox->AddSpacer(15);

//RadioBox choose parts robot to move

	wxArrayString strings3;
	strings3.Add(wxT("Cartesian"));
	strings3.Add(wxT("Articular"));

	wxRadioBox* move = new wxRadioBox(panel, ID_CHOOSETOMOVE, wxT("MOVE"), wxDefaultPosition, wxDefaultSize, strings3, 1, wxRA_SPECIFY_COLS);

	childLeftbox->Add(move,0,wxEXPAND|wxALL);
	childLeftbox->AddSpacer(15);

//buttons 
	wxBoxSizer *buttonsbox=new wxBoxSizer(wxHORIZONTAL);//container

	wxButton* accept = new wxButton(panel,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxDefaultSize);
	wxButton* cancel = new wxButton(panel,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxDefaultSize);

	buttonsbox->AddSpacer(20);
	buttonsbox->Add(accept,0,wxEXPAND|wxALL);
	buttonsbox->AddSpacer(5);
	buttonsbox->Add(cancel,0,wxEXPAND|wxALL);

//joining all
	childbox->AddSpacer(15);
	childbox->Add(childLeftbox,0,wxEXPAND|wxALL);
	childbox->AddSpacer(15);
	childbox->Add(childRightbox,0,wxEXPAND|wxALL);
	childbox->AddSpacer(15);


	parentbox->Add(titlebox,0,wxEXPAND|wxALL);
	parentbox->AddSpacer(20);
	parentbox->Add(inforAll,0,wxEXPAND|wxALL);
	parentbox->AddSpacer(20);
	parentbox->Add(childbox,0,wxEXPAND|wxALL);
	parentbox->AddSpacer(20);
	parentbox->Add(buttonsbox,1,wxEXPAND|wxALL);
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
	//if(id == ID_TVP)
	//{
	//	trajectSelected->SetLabel("Trapezoidal Velocity Profile selected");
	//	itemnode->pointer.robotsim->setTrajectoryType(TVP);
	//}
	//
	//if(id == ID_CPT)
	//{
	//	trajectSelected->SetLabel("Cubic Polinomial Trajectory selected");
	//	itemnode->pointer.robotsim->setTrajectoryType(CPT);

	//}
	/*if(id == ID_SPLINE)
	{
		trajectSelected->SetLabel("Spline Trajectory selected");
		itemnode->pointer.robotsim->setTypeTrajectory(SPLINE);
	}*/

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
		
		//vector<double> target;

		//target.push_back(2.00);
		//target.push_back(1.25);
		//target.push_back(-0.68);
		//target.push_back(2.43);
		//target.push_back(1.67);
		//target.push_back(-2.98);

		//target.push_back(0.79);//pto (1,1,1) inverseKinematics
		//target.push_back(1.03);
		//target.push_back(0.53);
		//target.push_back(3.14);
		//target.push_back(2.07);
		//target.push_back(2.36);
		

		itemnode->pointer.robotsim->goToAbs(target);
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
