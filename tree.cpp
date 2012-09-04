#include "tree.h"
#include "mainWindow.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(Tree, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_TREE, Tree::OnItemMenu)
	EVT_TREE_SEL_CHANGED(wxID_ANY,Tree::ShowSelection)
	EVT_LEFT_DCLICK(Tree::OnShowCanvas)
END_EVENT_TABLE()

Tree::Tree(wxWindow * parent, const wxWindowID id)
: wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize)
{
	wxImageList *images = new wxImageList(16,16,true);
	// should correspond to "bit" & "bitsel" enum 
	wxIcon icons[48];
	icons[0] = wxIcon (universe_xpm); //universe
	icons[1] = wxIcon (joint_xpm); //simple joint
	icons[2] = wxIcon (jointSelect_xpm); //simple joint select 
	icons[3] = wxIcon (joint_xpm); //joint 
	icons[4] = wxIcon (jointSelect_xpm); //joint select 
	icons[5] = wxIcon (cylindrical_xpm); //cylindrical
	icons[6] = wxIcon (cylindricalSelect_xpm); //cylindrical select
	icons[7] = wxIcon (prismatic_xpm); //prismatic
	icons[8] = wxIcon (prismaticSelect_xpm); //prismatic select
	icons[9] = wxIcon (sickLms_xpm); //sick lms 200
	icons[10] = wxIcon (sickLmsSelect_xpm); //sick lms 200 select
	icons[11] = wxIcon (sickLms_xpm); //sick lms 100
	icons[12] = wxIcon (sickLmsSelect_xpm);//sick lms 100 select
	icons[13] = wxIcon (faceSetPart_xpm); //face set part 
	icons[14] = wxIcon (faceSetPartSelect_xpm); //face set part Select 
	icons[15] = wxIcon (icon1_xpm); //patrol bot sim
	icons[16] = wxIcon (icon2_xpm); //patrol bot sim select
	icons[17] = wxIcon (pioneer_xpm); //pioneer
	icons[18] = wxIcon (pioneerSelect_xpm); //pioneer select
	icons[19] = wxIcon (powercube_xpm); //powercube 
	icons[20] = wxIcon (powercubeSelect_xpm); //powercube select
	icons[21] = wxIcon (nemolaser_xpm); //nemo laser 
	icons[22] = wxIcon (nemolaserSelect_xpm); //nemo laser select
	icons[23] = wxIcon (laser_xpm); //laser sensor3d
	icons[24] = wxIcon (laserSelect_xpm); //laser sensor3d select
	icons[25] = wxIcon (laser_xpm); //laser sensor
	icons[26] = wxIcon (laserSelect_xpm); //laser sensor select
	icons[27] = wxIcon (meshpart_xpm); //mesh part 
	icons[28] = wxIcon (meshpartSelect_xpm); //mesh part select
	icons[29] = wxIcon (wheeledBase_xpm); //wheeled base 
	icons[30] = wxIcon (wheeledBaseSelect_xpm); //wheeled base select
	icons[31] = wxIcon (composed_xpm); //composed entity
	icons[32] = wxIcon (composedSelect_xpm); //composed entity select
	icons[33] = wxIcon (solidEntity_xpm); //solidentity
	icons[34] = wxIcon (solidEntitySelect_xpm); //solidentity select
	icons[35] = wxIcon (positionable_xpm); //positionable entity 
	icons[36] = wxIcon (positionableSelect_xpm); //positionabla entity select
	icons[37] = wxIcon (world_xpm); //world
	icons[38] = wxIcon (worldSelect_xpm); //world select
	icons[39] = wxIcon (robotSim_xpm); //robotSim
	icons[40] = wxIcon (robotSimSelect_xpm); //robotSim select
	icons[41] = wxIcon (scara_xpm); //adeptone
	icons[42] = wxIcon (scaraSelect_xpm); //adeptone select
	icons[43] = wxIcon (sphere_xpm);
    icons[44] = wxIcon (sphereSelect_xpm);
	icons[45] = wxIcon (asea_xpm);
	icons[46] = wxIcon (aseaSelect_xpm);
	icons[47] = wxIcon (universeSelect_xpm);

	for(int i=0;i<WXSIZEOF(icons);i++)
	{
		images->Add(icons[i]);
	}
	AssignImageList(images);
	sel=false;
}
 wxTreeItemId Tree::GetWorld(wxTreeItemId i)
{
	wxTreeItemId world=i;
	if(GetItemParent(world)==root)
		return world;
	//else if(GetItemParent(world)==NULL)
	//	return 0;
	GetWorld(GetItemParent(world));
}


void Tree::Parent(wxTreeItemId r)
{
	root = r;
}
wxTreeItemId Tree::GenerateSubTree(World* w,SimulatedWorld* simu)
{
	static int num = 0;
	text.Printf(wxT("World %d"),++num);
	NodeTree *auxW = new NodeTree(w,simu);
	wxTreeItemId node = AppendItem(root, text, 37, 38,auxW);
	auxW->SetId(node);
	for (int i=0;i<w->getNumObjects();i++)
	{
		AddNode((*w)[i],node,simu);
	}
	return node;
}



wxTreeItemId Tree::AddNode(PositionableEntity* pos, wxTreeItemId parent,SimulatedWorld *simu)
{
	NodeTree *auxP = new NodeTree(pos,simu);
	
	wxTreeItemId mynode = AppendItem(parent, auxP->getNameTree(), auxP->bit, auxP->bitsel, auxP);
	auxP->SetId(mynode);
	ComposedEntity * auxC = dynamic_cast<ComposedEntity *>(pos);
	if(auxC)
		for(int j=0;j<auxC->getNumObjects();j++)
			AddNode((*auxC)[j],mynode,simu);
		
	
	return mynode;

	
	
}



void Tree::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId itemId = GetSelection();
	itemId = event.GetItem();
	SelectItem(itemId);
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) GetItemData(itemId)
										:NULL;
	wxPoint pt = event.GetPoint();
	wxString title;
	if(itemData == NULL)return;
	if(itemId.IsOk() && itemId == m_mainWin->getRoot())
	{
		wxMenu menuUniverse(wxT("Menu of Universe"));
		menuUniverse.Append(ID_NEW, wxT("New world"));
		menuUniverse.Append(ID_LOADWORLD, wxT("Load world"));
		PopupMenu(&menuUniverse,pt);
	}
	else if(itemId.IsOk() && itemData->menus.menu_world)
	{
		wxMenu menuWorld(wxT("Menu of World"));
		wxMenu *subMenu=new wxMenu;
		wxMenu *basic=new wxMenu;
		wxMenu *composed=new wxMenu;

		menuWorld.Append(ID_NAME, wxT("Change name"));
		menuWorld.AppendSeparator();
		menuWorld.Append(ID_LOADOBJ, wxT("Load object"));
		menuWorld.Append(ID_LOADMESH, wxT("Import .stl"));
		menuWorld.Append(ID_SAVEWORLD, wxT("Save world"), wxT("Save this world"));
		menuWorld.Append(ID_DELETE,wxT("Delete world"));
		menuWorld.AppendSeparator();
		
		if(MainWindow::popmenu)
		{
		menuWorld.Append(ID_ADDOBJ,wxT("Add Simples Entities"));
		menuWorld.Append(ID_ADDCOMP,wxT("Add Complex Entities"));
		}
		else
		{
		menuWorld.AppendSubMenu(basic,wxT("Add Simples Entities"));
		menuWorld.AppendSubMenu(composed,wxT("Add Complex Entities"));
		
		///Number of items for Simple Entities Menu///
		#define NUMBER_S 10
		#define NUMBER_C 12
		m_item simples[NUMBER_S];
		m_item compos[NUMBER_C];
		wxMenuItem *item_s[NUMBER_S];
		wxMenuItem *item_c[NUMBER_C];
		
		simples[0]=SimplyItems(ID_ADDFACESET,"Face Set",wxIcon(faceSetPart_xpm));
		simples[1]=SimplyItems(ID_ADDSPHERE,"Sphere",wxIcon(sphere_xpm));
		simples[2]=SimplyItems(ID_ADDCYL,"Cylinder",wxIcon(cylindrical_xpm));
		simples[3]=SimplyItems(ID_ADDPRI,"Prism",wxIcon(prismatic_xpm));
		simples[4]=SimplyItems(ID_ADDIRRPRI,"Irregular Prism",wxIcon(prismatic_xpm));	
		simples[5]=SimplyItems(ID_WHEEL,"Wheeled Base",wxIcon(wheeledBase_xpm));
		simples[6]=SimplyItems(ID_KINECT,"Kinect",wxIcon(laser_xpm));
		simples[7]=SimplyItems(ID_CAMERA,"Camera",wxIcon(laser_xpm));
		simples[8]=SimplyItems(ID_LASER,"Laser",wxIcon(laser_xpm));
		simples[9]=SimplyItems(ID_LASER3D,"Laser3D",wxIcon(laser_xpm));
		
		

		compos[0]=SimplyItems(ID_ADDSCARA,"Robot SCARA",wxIcon(scara_xpm));
		compos[1]=SimplyItems(ID_ADDNEO,"Robot PIONEER",wxIcon(pioneer_xpm));
		compos[2]=SimplyItems(ID_ADDPUMA,"Robot PUMA",wxIcon(robotSim_xpm));
		compos[3]=SimplyItems(ID_ADDASEA,"Robot ASEA",wxIcon(asea_xpm));
		compos[4]=SimplyItems(ID_PATROL,"Robot PatrolBot",wxIcon(wheeledBase_xpm));
		compos[5]=SimplyItems(ID_LMS100,"LMS100",wxIcon(sickLms_xpm));
		compos[6]=SimplyItems(ID_LMS200,"LMS200",wxIcon(sickLms_xpm));
		compos[7]=SimplyItems(ID_POWERCUBE,"PowerCube",wxIcon(powercube_xpm));
		compos[8]=SimplyItems(ID_NEMOLASER,"NemoLaser Sensor 3D ",wxIcon(nemolaser_xpm));
		compos[9]=SimplyItems(ID_QUADROTOR,"Quadrotor",wxIcon(powercube_xpm));
		compos[10]=SimplyItems(ID_PERSON,"Person",wxIcon(laser_xpm));
		compos[11]=SimplyItems(ID_MOBILEROBOT,"Mobile Robot",wxIcon(nemolaser_xpm));			
		
		for( int i=0; i<NUMBER_S;i++)
		{
		
			item_s[i] = new wxMenuItem(basic,simples[i].id,simples[i].name);
			item_s[i]->SetBitmap(simples[i].icon);
			basic->Append(item_s[i]);

			
		}

		for( int j=0; j<NUMBER_C;j++)
		{
		
			item_c[j] = new wxMenuItem(composed,compos[j].id,compos[j].name);
			item_c[j]->SetBitmap(compos[j].icon);
			composed->Append(item_c[j]);
		}	
		}
		menuWorld.Append(ID_ADDCUSTOM,wxT("Add Composed Entity"));
	
		PopupMenu(&menuWorld,pt);
	}
	else
	{
		if(itemId.IsOk())
		{
			wxString text = itemData ? itemData->getNameTree() : GetItemText(itemId) ;
			title<<wxT("Menu for ") << text;
		}
		else 
		{
			title = wxT("Menu empty");
		}
		
		wxMenu menuTree(title);
		if(itemData->menus.menu_composed) menuTree.Append(ID_ADDOBJ,wxT("Add Simple Object"));
		if(itemData->menus.menu_composed) menuTree.Append(ID_ADDCOMP,wxT("Add Complex Object"));
		if(itemData->menus.menu_composed) menuTree.Append(ID_ADDCUSTOM,wxT("Add Composed Object"));
		if(itemData->menus.menu_positionable) menuTree.Append(ID_SAVEOBJ, wxT("Save object"));
		if(itemData->menus.menu_positionable) menuTree.Append(ID_DELOBJ, wxT("Delete Object"));
		if(itemData->menus.menu_positionable)menuTree.AppendSeparator();
		if(itemData->menus.menu_positionable) menuTree.Append(ID_POSIT, wxT("Change position"));
		if(itemData->menus.menu_positionable) menuTree.Append(ID_ORI, wxT("Change orientation"));
		if(itemData->menus.menu_solid && itemData->menus.menu_composed==false && itemData->getTipo()!=N_FaceSetPart) menuTree.Append(ID_DIS, wxT("Change design properties"));
		if(itemData->menus.menu_positionable && itemData->pointer.positionableentity->getDrawReferenceSystem()==1) menuTree.Append(ID_REFER, wxT("Undraw Reference System"));
		if(itemData->menus.menu_positionable && itemData->pointer.positionableentity->getDrawReferenceSystem()==0) menuTree.Append(ID_REFER, wxT("Draw Reference System"));
		if(itemData->menus.menu_positionable) menuTree.Append(ID_NAME, wxT("Change name"));
		if(itemData->menus.menu_positionable) menuTree.AppendSeparator();
		if(itemData->menus.menu_solid)menuTree.Append(ID_COLOR, wxT("Change color"));
		if(itemData->menus.menu_solid && itemData->pointer.solidentity->getDrawBox()==1)menuTree.Append(ID_BOX, wxT("Undraw box"));
		if(itemData->menus.menu_solid && itemData->pointer.solidentity->getDrawBox()==0)menuTree.Append(ID_BOX, wxT("Draw box"));
		if(itemData->menus.menu_wheeledbased)menuTree.AppendSeparator();
		if(itemData->menus.menu_wheeledbased) menuTree.Append(ID_MOVE, wxT("Move control"));
		if(itemData->menus.menu_robotsim) menuTree.AppendSeparator();
		if(itemData->menus.menu_robotsim) menuTree.Append(ID_ROBOT, wxT("Move joints"));
		if(itemData->menus.menu_meshpart) menuTree.AppendSeparator();
		if(itemData->menus.menu_meshpart) menuTree.Append(ID_CONVERMESH, wxT("See Cpp Code"));
		if(itemData->menus.menu_server) menuTree.AppendSeparator();
		if(itemData->menus.menu_server && itemData->typeConnection==0) menuTree.Append(ID_LNSERVER, wxT("Launch Server"));
		if(itemData->menus.menu_server && itemData->typeConnection==1) menuTree.Append(ID_STSERVER, wxT("Stop Sending Data"));
		if(itemData->menus.menu_server && itemData->typeConnection==0) menuTree.Append(ID_LNCLIENT, wxT("Launch Client"));
		if(itemData->menus.menu_server && itemData->typeConnection==2) menuTree.Append(ID_STCLIENT, wxT("Stop Receiving Data"));


		PopupMenu(&menuTree,pt);
	}
	event.Skip();
}

void Tree::OnShowCanvas(wxMouseEvent& event)
{
	wxTreeItemId id = HitTest(event.GetPosition());
	if(id.IsOk())
	{
		wxLogStatus(wxT("Item seleccionado"));
		m_mainWin->OnShowCanvas();
	}	
	else wxLogStatus(wxT("No item under mouse"));

	event.Skip();
}

Tree::m_item Tree::SimplyItems(int id,string name, wxIcon icon)
{
			struct  m_item item;
			item.id=id;
			item.name=name;
			item.icon=icon;
			return item;
}

void Tree::ShowSelection(wxTreeEvent& event)
{

	if(sel)
	{
	wxTreeItemId itemId = GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) GetItemData(itemId)
										:NULL;

	if(itemData->menus.menu_solid && root!=itemId)
		{
			itemData->pointer.solidentity->setDrawBox();
			itemData->getSimu()->getChild()->UpdateWorld();
		}
	if(event.GetOldItem()!=NULL)
		{
			itemId=event.GetOldItem();
			itemData = itemId.IsOk() ? (NodeTree *) GetItemData(itemId)
										:NULL;
			if(itemData->menus.menu_solid && root!=itemId)
			{
				itemData->pointer.solidentity->setDrawBox(false);
				itemData->getSimu()->getChild()->UpdateWorld();
			}
		}
	}

}




		
	
	
	

