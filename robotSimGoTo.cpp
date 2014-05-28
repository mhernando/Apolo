#include "robotSimGoTo.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(RobotSimGoTo, wxFrame)
	EVT_BUTTON(ID_ACCEPT, RobotSimGoTo::OnButton)
	EVT_BUTTON(ID_CANCEL, RobotSimGoTo::OnButton)
	EVT_CLOSE(RobotSimGoTo::OnClose)
END_EVENT_TABLE()

RobotSimGoTo::RobotSimGoTo(wxWindow *parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData,
							 NodeTree *parentData)
: wxFrame(parent, id, title_frame, wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE) 
{

	itemnode=itemData;
	noDelete=false;
	this->SetIcon(wxIcon(goto_xpm));

	wxPanel* panel = new wxPanel(this, wxID_ANY);

//general declaration wxBoxsizer	
	
	wxBoxSizer *parentbox=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *childbox=new wxBoxSizer(wxHORIZONTAL);
	
//title
	wxBoxSizer *titlebox = new wxBoxSizer(wxHORIZONTAL);//container
	title = new wxStaticText(panel,wxID_ANY,wxEmptyString, wxDefaultPosition, wxDefaultSize);
	//title->SetFont(wxFont(14,wxFONTFAMILY_ROMAN));
	titlebox->AddSpacer(15);
	titlebox->Add(title,0,wxEXPAND|wxALL);



//robot location information and its Tcp one too

	wxStaticBoxSizer *inforAll=new wxStaticBoxSizer (wxHORIZONTAL,panel,wxT("POSITION AND ORIENTATION"));

	Vector3D pos,orient;
	pos=itemData->pointer.robotsim->getRelativePosition();
	itemData->pointer.robotsim->getRelativeOrientation(orient.x,orient.y,orient.z);
	Transformation3D tcpLocatRelative, tcpLocatAbsolute;
	tcpLocatRelative=itemnode->pointer.robotsim->getTcpLocation();
	tcpLocatAbsolute=itemnode->pointer.robotsim->getTcpAbsLocation();
	
	//robot
	stringstream str,str1,str2;
	str<<"Position (absolute): {x="<<pos.x<<", y="<<pos.y<<", z="<<pos.z<<'}';
	str1<<"Orientation: {roll="<<orient.x<<", pitch="<<orient.y<<", yaw="<<orient.z<<'}';

	wxStaticBoxSizer *infor=new wxStaticBoxSizer (wxVERTICAL,panel,wxT("Robot"));
	wxStaticText* positionRobot=new wxStaticText(panel,wxID_ANY, wxString(str.str()),wxDefaultPosition,wxDefaultSize);
	wxStaticText* orientationRobot=new wxStaticText(panel,wxID_ANY, wxString(str1.str()),wxDefaultPosition,wxDefaultSize);
	infor->AddSpacer(10);
	infor->Add(positionRobot,0,wxEXPAND|wxALL);
	infor->AddSpacer(10);
	infor->Add(orientationRobot,0,wxEXPAND|wxALL);

	inforAll->Add(infor,0,wxEXPAND|wxALL);
	
	//tcp
	str.str(string());
	str1.str(string());
	Vector3D tcpOrient;
	tcpLocatRelative.orientation.getRPY(tcpOrient.x,tcpOrient.y,tcpOrient.z);
	str<<"Position (absolute): {x="<<tcpLocatAbsolute.position.x<<", y="<<tcpLocatAbsolute.position.y<<", z="<<tcpLocatAbsolute.position.z<<'}';
	str1<<"Position (relative): {x="<<tcpLocatRelative.position.x<<", y="<<tcpLocatRelative.position.y<<", z="<<tcpLocatRelative.position.z<<'}';
	str2<<"Orientation: {roll="<<tcpOrient.x<<", pitch="<<tcpOrient.y<<", yaw="<<tcpOrient.z<<'}';

	wxStaticBoxSizer *inforTcp=new wxStaticBoxSizer (wxVERTICAL,panel,wxT("TCP"));
	wxStaticText* positionTcpAbs=new wxStaticText(panel,wxID_ANY, wxString(str.str()),wxDefaultPosition,wxDefaultSize);
	wxStaticText* positionTcpRel=new wxStaticText(panel,wxID_ANY, wxString(str1.str()),wxDefaultPosition,wxDefaultSize);
	wxStaticText* orientationTcp=new wxStaticText(panel,wxID_ANY, wxString(str2.str()),wxDefaultPosition,wxDefaultSize);
	inforTcp->AddSpacer(10);
	inforTcp->Add(positionTcpAbs,0,wxEXPAND|wxALL);
	inforTcp->AddSpacer(10);
	inforTcp->Add(positionTcpRel,0,wxEXPAND|wxALL);
	inforTcp->AddSpacer(10);
	inforTcp->Add(orientationTcp,0,wxEXPAND|wxALL);
	
	inforAll->Add(inforTcp,0,wxEXPAND|wxALL);

//target box 

	wxStaticBoxSizer *childRightbox=new wxStaticBoxSizer(wxVERTICAL,panel,wxT("TARGET"));//container

	//RadioBox choose parts robot to move
	wxArrayString strings3;
	strings3.Add(wxT("Cartesian"));
	strings3.Add(wxT("Articular"));

	move = new wxRadioBox(panel, ID_CHOOSETOMOVE, wxT("MOVE"), wxDefaultPosition, wxDefaultSize, strings3, 1, wxRA_SPECIFY_ROWS);

	childRightbox->Add(move,0,wxEXPAND|wxALL);
	childRightbox->AddSpacer(10);

	//points XYZ to fill
	wxBoxSizer *targets=new wxBoxSizer(wxHORIZONTAL);//container

	wxStaticBoxSizer *targ=new wxStaticBoxSizer(wxVERTICAL,panel,wxT("CARTESIAN"));
	wxStaticBoxSizer *_X=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("X coordinate"));//Create a new wxStaticSizer
	coorX = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_X->Add(coorX,0,wxEXPAND|wxALL);

	wxStaticBoxSizer *_Y=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("Y coordinate"));//Create a new wxStaticSizer
	coorY = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);	
	_Y->Add(coorY,0,wxEXPAND|wxALL);
	
	wxStaticBoxSizer *_Z=new wxStaticBoxSizer (wxHORIZONTAL,panel,wxT("Z coordinate"));//Create a new wxStaticSizer
	coorZ = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_Z->Add(coorZ,0,wxEXPAND|wxALL);

	wxStaticBoxSizer *_roll=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("Roll"));//Create a new wxStaticSizer
	roll = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_roll->Add(roll,0,wxEXPAND|wxALL);

	wxStaticBoxSizer *_pitch=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("Pitch"));//Create a new wxStaticSizer
	pitch = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);	
	_pitch->Add(pitch,0,wxEXPAND|wxALL);
	
	wxStaticBoxSizer *_yaw=new wxStaticBoxSizer (wxHORIZONTAL,panel,wxT("Yaw"));//Create a new wxStaticSizer
	yaw = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	_yaw->Add(yaw,0,wxEXPAND|wxALL);
	
	targ->AddSpacer(5);
	//position
	targ->Add(_X,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_Y,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_Z,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);

	//orientation
	targ->Add(_roll,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_pitch,0,wxEXPAND|wxALL);
	targ->AddSpacer(5);
	targ->Add(_yaw,0,wxEXPAND|wxALL);

	targets->Add(targ,0,wxEXPAND|wxALL);


	//Articular values
	wxBoxSizer *_joints=new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer *all_joints=new wxStaticBoxSizer(wxHORIZONTAL,panel,wxT("ARTICULAR"));

	int numJoints = itemnode->pointer.robotsim->getNumJoints();

	for(int i=0;i<numJoints;i++)
	{		
		wxString nameJoint;
		nameJoint.Printf(wxT("Joint %d"),i);
		joint = new GenericSlider(panel,nameJoint,wxDefaultPosition,wxDefaultSize,false);

		listJoints.push_back(joint);
		_joints->Add(joint,0,wxEXPAND|wxALL,5);
		_joints->AddSpacer(5);

		if (i==(numJoints-1))
			all_joints->Add(_joints,0,wxEXPAND|wxALL);
		else if (((i+1)%3)==0 && i!=0)
		{
			all_joints->Add(_joints,0,wxEXPAND|wxALL);
			_joints=new wxBoxSizer(wxVERTICAL);	
		}

		double max,min,val;
		itemnode->pointer.robotsim->getJointLimits(i,max,min);
		listJoints[i]->setProperties(min,max,true);
		itemnode->pointer.robotsim->getJointValue(i,val);
		listJoints[i]->setValue(val);
	}

	targets->Add(all_joints,0,wxEXPAND|wxALL);
	childRightbox->Add(targets,0,wxEXPAND|wxALL);


//RadioBox type trajectory
	wxBoxSizer *childLeftbox=new wxBoxSizer(wxVERTICAL);//container

	wxArrayString strings0;
	strings0.Add(wxT("Synchronous Joints"));
	strings0.Add(wxT("Linear Path"));

	typetraj = new wxRadioBox(panel, ID_CHOOSETYPETRAJECTORY, wxT("TYPE TRAJECTORY"), wxDefaultPosition, wxDefaultSize, strings0, 1, wxRA_SPECIFY_COLS);

	childLeftbox->Add(typetraj,0,wxEXPAND|wxALL);
	childLeftbox->AddSpacer(15);

//RadioBox type movement

	wxArrayString strings1;
	strings1.Add(wxT("Trapezoidal Velocity Profile"));
	strings1.Add(wxT("Cubic Polinomial Trajectory"));
	strings1.Add(wxT("Spline"));

	typemov = new wxRadioBox(panel, ID_CHOOSETYPEMOVEMENT, wxT("TYPE MOVEMENT"), wxDefaultPosition, wxDefaultSize, strings1, 1, wxRA_SPECIFY_COLS);

	childLeftbox->Add(typemov,0,wxEXPAND|wxALL);
	childLeftbox->AddSpacer(15);

//RadioBox system reference 

	wxArrayString strings2;
	strings2.Add(wxT("Robot (Relative)"));
	strings2.Add(wxT("World (Absolute)"));

	refsys = new wxRadioBox(panel, ID_CHOOSESYSREFERENCE, wxT("REFERENCE SYSTEM"), wxDefaultPosition, wxDefaultSize, strings2, 1, wxRA_SPECIFY_COLS);

	childLeftbox->Add(refsys,0,wxEXPAND|wxALL);
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


void RobotSimGoTo::checkSelections()
{
	wxString selection = "";

	selection = move->GetString(move->GetSelection());
	if (selection=="Cartesian")
		cartesian = true;
	else //if (selection=="Articular")
		cartesian = false;

	selection = refsys->GetString(refsys->GetSelection());
	if (selection=="World (Absolute)")
		coordAbsolute = true;
	else //if (selection=="Robot (Relative)")
		coordAbsolute = false;

	selection = typemov->GetString(typemov->GetSelection());
	if (selection=="Cubic Polinomial Trajectory")
		itemnode->pointer.robotsim->setPositionInterpolator(CPT);

	else if (selection=="Trapezoidal Velocity Profile") 
		itemnode->pointer.robotsim->setPositionInterpolator(TVP);

	else //if (selection=="Spline") 
		itemnode->pointer.robotsim->setPositionInterpolator(SPLINE);

	selection = typetraj->GetString(typetraj->GetSelection());
	if (selection=="Linear Path")
		itemnode->pointer.robotsim->setPathType(LINEAR);

	else //if (selection=="Default") 
		itemnode->pointer.robotsim->setPathType(SYNC_JOINT);

}

void RobotSimGoTo::OnButton(wxCommandEvent& event)
{
	int id = event.GetId();

	if(id == ID_ACCEPT)
	{
		checkSelections(); //Checking the selections in radio boxes
		OnValueChanges();
		if (!noDelete)
			Delete();
		noDelete=false;
	}
	
	else if(id == ID_CANCEL)
		Delete();

	event.Skip();
}

void RobotSimGoTo::OnValueChanges()
{

	if (!cartesian)
	{
		vector<double> target;
		for(int i=0;i<(int)listJoints.size();i++)
		{
			double value = listJoints[i]->getValue();
			target.push_back(value);
		}

		itemnode->pointer.robotsim->moveTo(target);
	}

	else if (cartesian && !checkEmptyStringBoxes())
	{	
		double x=0.0, y=0.0, z=0.0, ro=0.0, pi=0.0, ya=0.0;
		wxString value;

		//position
		value=coorX->GetValue();
		value.ToDouble(&x);
		value.Clear();
		value=coorY->GetValue();
		value.ToDouble(&y);
		value.Clear();
		value=coorZ->GetValue();
		value.ToDouble(&z);
		value.Clear();

		//orientation
		value=roll->GetValue();
		value.ToDouble(&ro);
		value.Clear();
		value=pitch->GetValue();
		value.ToDouble(&pi);
		value.Clear();
		value=yaw->GetValue();
		value.ToDouble(&ya);
		value.Clear();

		Transformation3D target(x,y,z,ro,pi,ya);
		
		if (coordAbsolute)
			itemnode->pointer.robotsim->computeTrajectoryToAbs(target);
		else 
			itemnode->pointer.robotsim->computeTrajectoryTo(target);
	}
	else
	{
		wxString msg;
		msg.Printf(wxT("You have to fill all the gaps -> TARGET = {X, Y, Z, Roll, Pitch, Yaw}"));
		wxMessageBox(msg, wxT("Please confirm"));
		noDelete=true;
	}
}

bool RobotSimGoTo::checkEmptyStringBoxes()
{
	if (coorX->GetValue()==wxEmptyString ||
		coorY->GetValue()==wxEmptyString ||
		coorZ->GetValue()==wxEmptyString ||
		roll->GetValue()==wxEmptyString ||
		pitch->GetValue()==wxEmptyString ||
		yaw->GetValue()==wxEmptyString)
		return true;
	return false;
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
