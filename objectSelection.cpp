#include "objectSelection.h"
#include "bitmaps/sphere.xpm"
#include "bitmaps/faceSetPart.xpm"
#include "bitmaps/cylindrical.xpm"
#include "bitmaps/prismatic.xpm"
#include "bitmaps/pioneer.xpm"
#include "bitmaps/scara.xpm"
#include "bitmaps/robotSim.xpm"
#include "bitmaps/powercube.xpm"
#include "bitmaps/sickLms.xpm"
#include "bitmaps/wheeledBase.xpm"
#include "bitmaps/laser.xpm"
#include "bitmaps/nemolaser.xpm"
#include "bitmaps/asea.xpm"
#include "bitmaps/positionable.xpm"


BEGIN_EVENT_TABLE(ObjectSelection, wxDialog)
	EVT_LIST_ITEM_ACTIVATED(wxID_ANY,ObjectSelection::ObjectSelected)
	EVT_LIST_ITEM_SELECTED(wxID_ANY,ObjectSelection::PreliminarView)
	EVT_CLOSE(ObjectSelection::OnClose)
END_EVENT_TABLE()



ObjectSelection::ObjectSelection(wxWindow *parent,const wxWindowID id, const wxPoint& pos,const wxSize& size)
:wxDialog(parent,id, wxT("Object Selection"), wxPoint(10,10),wxSize(800,400) /*wxSize(640,400)*/,wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxRESIZE_BORDER) 
{
	window=parent;
	ident=0;
	type=id;

	face.addVertex(-1.5,1);face.addVertex(-0.5,1);face.addVertex(0,2);face.addVertex(0.5,1);face.addVertex(1.5,1);face.addVertex(1,0);face.addVertex(1.5,-1);
	face.addVertex(0.5,-1);face.addVertex(0,-2);face.addVertex(-0.5,-1);face.addVertex(-1.5,-1);face.addVertex(-1,0);face.addVertex(-1.5,1);face.addVertex(-0.5,1);
			
	trans.orientation.setRPY(0,0,0);	face1.setBase(trans);
	face1.addVertex(-1,-1);face1.addVertex(-1,1);face1.addVertex(1,1);face1.addVertex(1,-1);	set.addFace(face1);
	trans.position.x=1; trans.position.z=1; trans.orientation.setRPY(0,deg2rad(-90),0);	face2.setBase(trans);
	face2.addVertex(-1,-1);face2.addVertex(-1,1);face2.addVertex(1,1);face2.addVertex(1,-1);	set.addFace(face2);
	trans.position.x=0;trans.position.y=1;	trans.orientation.setRPY(deg2rad(90),0,0);		face3.setBase(trans);	
	face3.addVertex(-1,-1);face3.addVertex(-1,1);face3.addVertex(1,1);face3.addVertex(1,-1);	set.addFace(face3);
	preliminar=NULL;
	CreateList();
}

void ObjectSelection::PreliminarView(wxListEvent &event)
{	
	long itemIndex=-1;
	wxListItem id=event.GetItem();

	itemIndex=ob_list->GetNextItem(itemIndex,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	
	
	preView->ClearObjects();
	delete preliminar;
	
	
	if(type==ID_ADDOBJ)
	{
		preView->SetViewCenter(0,0,0.5); 
		
		if(itemIndex<5)
		{
			preView->SetViewPoint(6,0,30);
			if(itemIndex==0) {preliminar=new FaceSetPart(set); object=ID_ADDFACESET;}
			else if(itemIndex==1){preView->SetViewCenter(0,0,0); preliminar=new SpherePart();object=ID_ADDSPHERE;}
			else if(itemIndex==2){preliminar= new CylindricalPart();object=ID_ADDCYL;}
			else if(itemIndex==3){p.setRegularPolygonBase(1,4);	p.setColor(0.5,0.5,0.5);	preliminar= new PrismaticPart(p); object=ID_ADDPRI;}
			else if(itemIndex==4){p.setPolygonalBase(face); p.setColor(1,1,0);	preliminar= new PrismaticPart(p); 	object=ID_ADDIRRPRI;}
		}
		else
		{
			preView->SetViewCenter(0,0,0);
			preView->SetViewPoint(2,0,30);
			if(itemIndex==5){preView->SetViewCenter(0,0,0.1);	preliminar = new WheeledBaseSim();	object=ID_WHEEL;}
			else if(itemIndex==6){preliminar=new KinectSim(); preliminar->setDrawReferenceSystem(false); object=ID_KINECT;}
			else if(itemIndex==7){preliminar=new CameraSim();object=ID_CAMERA;}
			else if(itemIndex==8){preliminar = new LaserSensorSim();object=ID_LASER;}
			else if(itemIndex==9){preliminar = new LaserSensor3DSim();object=ID_LASER3D;}
		}

	}
	
	else if(type==ID_ADDCOMP )
	{	
		preView->SetViewCenter(0,0,0);
		preView->SetViewPoint(2,0,30);
		
		if(itemIndex<3)
		{
			preView->SetViewCenter(0,0,0.75);
			preView->SetViewPoint(4,0,0);
			if(itemIndex==0){preliminar=new AdeptOneSim;	object=ID_ADDSCARA;}
			else if(itemIndex==1){preliminar=new  Puma560Sim;	object=ID_ADDPUMA;}
			else if(itemIndex==2){preliminar=new AseaIRB2000Sim;	object=ID_ADDASEA;}
			
		}
		else if(itemIndex<6)
		{	
			if(itemIndex==3){preliminar=new  Pioneer3ATSim;	object=ID_ADDNEO;}
			else if(itemIndex==4){preliminar=new PatrolbotSim;	object=ID_PATROL;}
			else if(itemIndex==5){preliminar=new QuadrotorSim;object=ID_QUADROTOR;}
		}
			
		
		else
		{
			preView->SetViewPoint(1.5,0,30);
			if(itemIndex==6){preView->SetViewCenter(0,0,-0.1);preliminar=new LMS100Sim;	object=ID_LMS100;}
			else if(itemIndex==7){preliminar=new LMS200Sim;	object=ID_LMS200;}
			else if(itemIndex==8){preliminar=new PowerCube70Sim; preliminar->setDrawReferenceSystem(false);	object=ID_POWERCUBE;}
			else if(itemIndex==9){preliminar=new NemoLaserSensor3DSim; preliminar->setDrawReferenceSystem(false); object=ID_NEMOLASER;}
			else if(itemIndex==10){preView->SetViewCenter(0,0,0.75); preView->SetViewPoint(4,0,0); preliminar=new PersonSim;object=ID_PERSON;}
			//else if(itemIndex==11){preliminar=new MobileRobot("Robot") ;object=ID_MOBILEROBOT;}
		}
		
		
		
	}
	
	preView->AddObject(preliminar);
	
}


void ObjectSelection::CreateList()
{

	wxListItem item;
	int index=0;

	wxBoxSizer *rbox=new wxBoxSizer(wxHORIZONTAL);
	preView=new Canvas(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,true);  
	preView->SetShowFrame(false);
	ob_list=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLC_REPORT);


	wxImageList *im_list=new wxImageList(16,16);
	wxIcon icons[14];
	icons[0]=wxIcon(sphere_xpm);
	icons[1]=wxIcon(pioneer_xpm);
	icons[2]=wxIcon(cylindrical_xpm);
	icons[3]=wxIcon(scara_xpm);
	icons[4]=wxIcon(prismatic_xpm);
	icons[5]=wxIcon(robotSim_xpm);
	icons[6]=wxIcon(powercube_xpm);
	icons[7]=wxIcon(sickLms_xpm);
	icons[8]=wxIcon(wheeledBase_xpm);
	icons[9]=wxIcon(laser_xpm);
	icons[10]=wxIcon(nemolaser_xpm);
	icons[11]=wxIcon(asea_xpm);
	icons[12]=wxIcon(faceSetPart_xpm);
	icons[13]=wxIcon(positionable_xpm);

	for(int i=0;i<WXSIZEOF(icons);i++)
		im_list->Add(icons[i]);
	

	
	ob_list->SetImageList(im_list, wxIMAGE_LIST_SMALL);
	item.SetText(wxT("Object"));
	item.SetAlign(wxLIST_FORMAT_LEFT);
	item.SetWidth(160);
	ob_list->InsertColumn(0,item);

	item.SetText(wxT("Description"));
	item.SetAlign(wxLIST_FORMAT_LEFT);
	item.SetWidth(500);
	ob_list->InsertColumn(1,item);

	if(type==ID_ADDOBJ)
	{
	ob_list->SetColumnWidth(0,130);
	ob_list->SetColumnWidth(1,300);
	


	//ob_list->InsertItem(index,wxT("Laser Sensor 3D"),9);	ob_list->SetItem(index,1,wxT("Add a Laser Sensor 3D")); 
	//ob_list->InsertItem(index,wxT("Laser Sensor"),9);		ob_list->SetItem(index,1,wxT("Add a Laser Sensor")); 
	//ob_list->InsertItem(index,wxT("Camera"),0);				ob_list->SetItem(index,1,wxT("Add a  Kinect  object")); 
	//ob_list->InsertItem(index,wxT("Kinect"),0);				ob_list->SetItem(index,1,wxT("Add a  camera object")); 
	ob_list->InsertItem(index,wxT("WheeledBase"),8);		ob_list->SetItem(index,1,wxT("Add a Wheeled Base")); 
	ob_list->InsertItem(index,wxT("Irregular Prism"),4);	ob_list->SetItem(index,1,wxT("Add a irregular prismatic object"));
	ob_list->InsertItem(index,wxT("Prism"),4);				ob_list->SetItem(index,1,wxT("Add a prismatic object"));
	ob_list->InsertItem(index,wxT("Cylinder"),2);			ob_list->SetItem(index,1,wxT("Add a cylindric object"));
	ob_list->InsertItem(index,wxT("Sphere"),0);				ob_list->SetItem(index,1,wxT("Add a  spheric  object")); 
	ob_list->InsertItem(index,wxT("FaceSet"),12);			ob_list->SetItem(index,1,wxT("Add a  set of faces")); 


	}	
	
	if(type==ID_ADDCOMP)
	{
		SetSize(1250,600);
		//ob_list->InsertItem(index,wxT("MobileRobot"),10);			ob_list->SetItem(index,1,wxT("Add MobileRobot"));
		ob_list->InsertItem(index,wxT("Person"),13);				ob_list->SetItem(index,1,wxT("Add Human Figure"));   
		ob_list->InsertItem(index,wxT("NemoLaser Sensor 3D"),10);	ob_list->SetItem(index,1,wxT("Add NemoLaser Sensor 3D, a laser formed with the SickLMS200 and the PowerCube")); 
		ob_list->InsertItem(index,wxT("PowerCube"),6);				ob_list->SetItem(index,1,wxT("Add PowerCube,it provides the basis for flexible combinatorics in automation")); 
		ob_list->InsertItem(index,wxT("SickLMS200"),7);				ob_list->SetItem(index,1,wxT("Add SickLMS200, an extremely accurate distance measurement sensor")); 
		ob_list->InsertItem(index,wxT("SickLMS100"),7);				ob_list->SetItem(index,1,wxT("Add SickLMS100, a  laser measurement system")); 
		ob_list->InsertItem(index,wxT("Quadrotor"),10);				ob_list->SetItem(index,1,wxT("Add Quadrotor"));
		ob_list->InsertItem(index,wxT("Robot PATROLBOT"),8);		ob_list->SetItem(index,1,wxT("Add PATROLBOT, a service robot which scan buildings and create floor plans")); 
		ob_list->InsertItem(index,wxT("Robot PIONNER"),1);			ob_list->SetItem(index,1,wxT("Add PIONEER 3-AT, a versatile four wheel drive robotic platform")); 	
		ob_list->InsertItem(index,wxT("Robot ASEA"),5);				ob_list->SetItem(index,1,wxT("Add ASEA IRB 2000, a 6 degrees of freedom robot for welding aplications"));
		ob_list->InsertItem(index,wxT("Robot PUMA"),5);				ob_list->SetItem(index,1,wxT("Add PUMA560, a 6 degrees of freedom robot due to 6 revolution joints"));
		ob_list->InsertItem(index,wxT("Robot SCARA"),3);			ob_list->SetItem(index,1,wxT("Add ADEPTONE-MV,a robot with 3 rotational and 1 translational joints"));

	}
	
	Thread<ObjectSelection> rotationalView_Thid;
	rotationalView_Thid.Start(&ObjectSelection::RotationalView,this,preView);
	
	rbox->Add(ob_list,1,wxEXPAND);
	rbox->Add(preView,1,wxEXPAND);
	SetSizer(rbox);


}
void*  ObjectSelection::RotationalView(void* canvas)
{
	Canvas * preView=(Canvas *)canvas;
	double d,rot,elv;
	
	while(1)
	{
		preView->GetViewPoint(d,rot,elv);
		preView->SetViewPoint(d,++rot,elv);
		if(rot==360)rot=0;
		Sleep(30);
		if(preView->IsBeingDeleted())
			break;
		preView->Refresh();
	}

	return NULL;
}

void ObjectSelection::ObjectSelected(wxListEvent &event)
{
	ident=object;
	Close();
}

void ObjectSelection::OnClose(wxCloseEvent &event)
{
	delete preliminar;
	//pthread_cancel(thid);
	Destroy();
}
