#include "objectSelection.h"
#include "bitmaps/sphere.xpm"
#include "bitmaps/cylindrical.xpm"
#include "bitmaps/prismatic.xpm"
#include "bitmaps/pioneer.xpm"
#include "bitmaps/scara.xpm"
#include "bitmaps/robotSim.xpm"

BEGIN_EVENT_TABLE(ObjectSelection, wxDialog)
	EVT_LIST_ITEM_ACTIVATED(wxID_ANY,ObjectSelection::ObjectSelected)
END_EVENT_TABLE()

ObjectSelection::ObjectSelection(wxWindow *parent,const wxWindowID id, const wxPoint& pos,const wxSize& size)
:wxDialog(parent,id, wxT("Object Selection"), wxPoint(10,10), wxSize(520,400),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
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

wxIcon icons[6];
icons[0]=wxIcon(sphere_xpm);
icons[1]=wxIcon(pioneer_xpm);
icons[2]=wxIcon(cylindrical_xpm);
icons[3]=wxIcon(scara_xpm);
icons[4]=wxIcon(prismatic_xpm);
icons[5]=wxIcon(robotSim_xpm);

for(int i=0;i<WXSIZEOF(icons);i++)
	{
		im_list->Add(icons[i]);
	}

	
ob_list->SetImageList(im_list, wxIMAGE_LIST_SMALL);
item.SetText(wxT("Object"));
item.SetAlign(wxLIST_FORMAT_LEFT);
item.SetWidth(100);
ob_list->InsertColumn(0,item);



item.SetText(wxT("Description"));
item.SetAlign(wxLIST_FORMAT_LEFT);
item.SetWidth(420);
ob_list->InsertColumn(1,item);


if(type==ID_ADDOBJ)
{
ob_list->InsertItem(index,wxT("Prism"),4);		ob_list->SetItem(index,1,wxT("Add a prismatic object"));
ob_list->InsertItem(index,wxT("Cylinder"),2);	ob_list->SetItem(index,1,wxT("Add a cylindric object"));
ob_list->InsertItem(index,wxT("Sphere"),0);		ob_list->SetItem(index,1,wxT("Add a  spheric  object")); 



}
if(type==ID_ADDCOMP)
{

ob_list->InsertItem(index,wxT("Robot ASEA"),5);		ob_list->SetItem(index,1,wxT("Add ASEA IRB 2000, a 6 degrees of freedom robot for welding aplications"));
ob_list->InsertItem(index,wxT("Robot PUMA"),5);		ob_list->SetItem(index,1,wxT("Add PUMA560, a 6 degrees of freedom robot due to 6 revolution joints"));
ob_list->InsertItem(index,wxT("Robot SCARA"),3);	ob_list->SetItem(index,1,wxT("Add ADEPTONE-MV,a robot with 3 rotational and 1 translational joints"));
ob_list->InsertItem(index,wxT("Robot NEO"),1);		ob_list->SetItem(index,1,wxT("Add PIONEER 3-AT, a versatile four wheel drive robotic platform")); 

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
	}
	
	else if(type==ID_ADDCOMP )
	{
		if(itemIndex==0)ident=ID_ADDNEO;
		if(itemIndex==1)ident=ID_ADDSCARA;
		if(itemIndex==2)ident=ID_ADDPUMA;
		if(itemIndex==3)ident=ID_ADDASEA;
	}
 
Close(true);
}


