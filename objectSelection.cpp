#include "objectSelection.h"
#include "bitmaps/sphere.xpm"
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


BEGIN_EVENT_TABLE(ObjectSelection, wxDialog)
	EVT_LIST_ITEM_ACTIVATED(wxID_ANY,ObjectSelection::ObjectSelected)
END_EVENT_TABLE()

ObjectSelection::ObjectSelection(wxWindow *parent,const wxWindowID id, const wxPoint& pos,const wxSize& size)
:wxDialog(parent,id, wxT("Object Selection"), wxPoint(10,10), wxSize(640,400),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{
	ident=0;
	type=id;
	CreateList();
}



void ObjectSelection::CreateList(){

wxListItem item;

int index=0;

    
ob_list=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLC_REPORT);
wxImageList *im_list=new wxImageList(16,16);

wxIcon icons[12];
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




for(int i=0;i<WXSIZEOF(icons);i++)
	{
		im_list->Add(icons[i]);
	}

	
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
ob_list->SetColumnWidth(1,200);
SetSize(10,10,300,200);


ob_list->InsertItem(index,wxT("Laser Sensor 3D"),9);	ob_list->SetItem(index,1,wxT("Add a Laser Sensor 3D")); 
ob_list->InsertItem(index,wxT("Laser Sensor"),9);	ob_list->SetItem(index,1,wxT("Add a Laser Sensor")); 
ob_list->InsertItem(index,wxT("WheeledBase"),8);	ob_list->SetItem(index,1,wxT("Add a Wheeled Base")); 
ob_list->InsertItem(index,wxT("Prism"),4);		ob_list->SetItem(index,1,wxT("Add a prismatic object"));
ob_list->InsertItem(index,wxT("Cylinder"),2);	ob_list->SetItem(index,1,wxT("Add a cylindric object"));
ob_list->InsertItem(index,wxT("Sphere"),0);		ob_list->SetItem(index,1,wxT("Add a  spheric  object")); 



}
if(type==ID_ADDCOMP)
{
ob_list->InsertItem(index,wxT("NemoLaser Sensor 3D"),10);	ob_list->SetItem(index,1,wxT("Add NemoLaser Sensor 3D, a laser formed with the SickLMS200 and the PowerCube")); 
ob_list->InsertItem(index,wxT("PowerCube"),6);		ob_list->SetItem(index,1,wxT("Add PowerCube,it provides the basis for flexible combinatorics in automation")); 
ob_list->InsertItem(index,wxT("SickLMS200"),7);		ob_list->SetItem(index,1,wxT("Add SickLMS200, an extremely accurate distance measurement sensor")); 
ob_list->InsertItem(index,wxT("SickLMS100"),7);		ob_list->SetItem(index,1,wxT("Add SickLMS100, a  laser measurement system")); 
ob_list->InsertItem(index,wxT("Robot PATROLBOT"),8);ob_list->SetItem(index,1,wxT("Add PATROLBOT, a service robot which scan buildings and create floor plans")); 
ob_list->InsertItem(index,wxT("Robot NEO"),1);		ob_list->SetItem(index,1,wxT("Add PIONEER 3-AT, a versatile four wheel drive robotic platform")); 
ob_list->InsertItem(index,wxT("Robot ASEA"),11);		ob_list->SetItem(index,1,wxT("Add ASEA IRB 2000, a 6 degrees of freedom robot for welding aplications"));
ob_list->InsertItem(index,wxT("Robot PUMA"),5);		ob_list->SetItem(index,1,wxT("Add PUMA560, a 6 degrees of freedom robot due to 6 revolution joints"));
ob_list->InsertItem(index,wxT("Robot SCARA"),3);	ob_list->SetItem(index,1,wxT("Add ADEPTONE-MV,a robot with 3 rotational and 1 translational joints"));


}


}
void ObjectSelection::ObjectSelected(wxListEvent &event)
{
	long itemIndex=-1;
 
	itemIndex=ob_list->GetNextItem(itemIndex,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);

	if(type==ID_ADDOBJ)
	{
		if(itemIndex==0)ident=ID_ADDSPHERE;
		if(itemIndex==1)ident=ID_ADDCYL;
		if(itemIndex==2)ident=ID_ADDPRI;
		if(itemIndex==3)ident=ID_WHEEL;	
		if(itemIndex==4)ident=ID_LASER;
		if(itemIndex==5)ident=ID_LASER3D;
	}
	
	else if(type==ID_ADDCOMP )
	{
		if(itemIndex==8)ident=ID_NEMOLASER;
		if(itemIndex==7)ident=ID_POWERCUBE;
		if(itemIndex==6)ident=ID_LMS200;
		if(itemIndex==5)ident=ID_LMS100;
		if(itemIndex==4)ident=ID_PATROL;
		if(itemIndex==3)ident=ID_ADDNEO;
		if(itemIndex==2)ident=ID_ADDASEA;
		if(itemIndex==1)ident=ID_ADDPUMA;
		if(itemIndex==0)ident=ID_ADDSCARA;
	}
 
Close(true);
}


