#include "tree.h"
#include "mainWindow.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(Tree, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_TREE, Tree::OnItemMenu)
	EVT_TREE_SEL_CHANGED(ID_TREE,Tree::ShowSelection)
	EVT_LEFT_DCLICK(Tree::OnShowCanvas)
	EVT_MOTION(Tree::ControlMouse)
END_EVENT_TABLE()

Tree::Tree(wxWindow * parent, const wxWindowID id)
: wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize)
{
	wxImageList *images = new wxImageList(16,16,true);
	// should correspond to "bit" & "bitsel" enum 
	wxIcon icons[50];
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
	icons[48]=	wxIcon (link_xpm);
	icons[49]=	wxIcon(link2_xpm);

	for(int i=0;i<WXSIZEOF(icons);i++)
	{
		images->Add(icons[i]);
	}
	AssignImageList(images);
	sel=false;
	showLinks=false;
	PreviousitemId=NULL;
}


 wxTreeItemId Tree::GetWorld(wxTreeItemId i)
{
	wxTreeItemId world=i;
	if(GetItemParent(world)!=root)
		world=GetWorld(GetItemParent(world));
	
	return world;
}


void Tree::Parent(wxTreeItemId r)
{
	root = r;
}

wxTreeItemId Tree::GenerateSubTree(SimulatedWorld* simu)
{
	static int num = 0;
	World* w=simu->getWorld();
	text.Printf(wxT("World %d"),++num);
	NodeTree *auxW = new NodeTree(simu);
	wxTreeItemId node = AppendItem(root, text, 37, 38,auxW);
	auxW->SetId(node);
	for (int i=0;i<w->getNumObjects();i++)
	{
		wxTreeItemId aux=AddNode((*w)[i],node,simu)->GetId();
		simu->InsertLoadedId(aux);
	}
	return node;
}



NodeTree* Tree::AddNode(PositionableEntity* pos, wxTreeItemId parent,SimulatedWorld *simu)
{
	NodeTree *auxP = new NodeTree(pos,simu);
	
	wxTreeItemId mynode = AppendItem(parent,auxP->getNameTree(), auxP->bit, auxP->bitsel, auxP);
	auxP->SetId(mynode);
	ComposedEntity * auxC = dynamic_cast<ComposedEntity *>(pos);
	if(auxC)
		for(int j=0;j<auxC->getNumObjects();j++)
			AddNode((*auxC)[j],mynode,simu);
		
	
	return auxP;

}



void Tree::OnItemMenu(wxTreeEvent& event)
{
	if(m_mainWin->getState()==0)
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
		menuUniverse.Append(ID_LOADWORLDXML, wxT("Load world XML"));
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
		menuWorld.Append(ID_LOADOBJXML, wxT("Load object XML"));
		menuWorld.Append(ID_LOADMESH, wxT("Import .stl"));
		menuWorld.Append(ID_SAVEWORLD, wxT("Save world"), wxT("Save this world"));
		menuWorld.Append(ID_SAVEWORLDXML, wxT("Save world XML"), wxT("Save this world in XML file"));
		if(!itemData->getSimu()->IsObjectConnected())
			menuWorld.Append(ID_DELETE,wxT("Delete world"));
		menuWorld.AppendSeparator();
		
		if(m_mainWin->getPopMenuValue())
		{
		menuWorld.Append(ID_ADDOBJ,wxT("Add Simples Entities"));
		menuWorld.Append(ID_ADDCOMP,wxT("Add Complex Entities"));
		}
		else
		{
		menuWorld.AppendSubMenu(basic,wxT("Add Simples Entities"));
		menuWorld.AppendSubMenu(composed,wxT("Add Complex Entities"));
		
		///Number of items for Simple Entities Menu///
		#define NUMBER_S 6
		#define NUMBER_C 11
		m_item simples[NUMBER_S];
		m_item compos[NUMBER_C];
		wxMenuItem *item_s[NUMBER_S];
		wxMenuItem *item_c[NUMBER_C];
		
		simples[0]=SimplyItems(ID_ADDFACESET,wxT("Face Set"),wxIcon(faceSetPart_xpm));
		simples[1]=SimplyItems(ID_ADDSPHERE,wxT("Sphere"),wxIcon(sphere_xpm));
		simples[2]=SimplyItems(ID_ADDCYL,wxT("Cylinder"),wxIcon(cylindrical_xpm));
		simples[3]=SimplyItems(ID_ADDPRI,wxT("Prism"),wxIcon(prismatic_xpm));
		simples[4]=SimplyItems(ID_ADDIRRPRI,wxT("Irregular Prism"),wxIcon(prismatic_xpm));	
		simples[5]=SimplyItems(ID_WHEEL,wxT("Wheeled Base"),wxIcon(wheeledBase_xpm));
	//	simples[6]=SimplyItems(ID_KINECT,wxT("Kinect"),wxIcon(laser_xpm));
	//	simples[7]=SimplyItems(ID_CAMERA,wxT("Camera"),wxIcon(laser_xpm));
	//	simples[8]=SimplyItems(ID_LASER,wxT("Laser"),wxIcon(laser_xpm));
		//simples[9]=SimplyItems(ID_LASER3D,wxT("Laser3D"),wxIcon(laser_xpm));
		
		

		compos[0]=SimplyItems(ID_ADDSCARA,wxT("Robot SCARA"),wxIcon(scara_xpm));
		compos[1]=SimplyItems(ID_ADDNEO,wxT("Robot PIONEER"),wxIcon(pioneer_xpm));
		compos[2]=SimplyItems(ID_ADDPUMA,wxT("Robot PUMA"),wxIcon(robotSim_xpm));
		compos[3]=SimplyItems(ID_ADDASEA,wxT("Robot ASEA"),wxIcon(robotSim_xpm));
		compos[4]=SimplyItems(ID_PATROL,wxT("Robot PatrolBot"),wxIcon(wheeledBase_xpm));
		compos[5]=SimplyItems(ID_LMS100,wxT("LMS100"),wxIcon(sickLms_xpm));
		compos[6]=SimplyItems(ID_LMS200,wxT("LMS200"),wxIcon(sickLms_xpm));
		compos[7]=SimplyItems(ID_POWERCUBE,wxT("PowerCube"),wxIcon(powercube_xpm));
		compos[8]=SimplyItems(ID_NEMOLASER,wxT("NemoLaser Sensor 3D "),wxIcon(nemolaser_xpm));
		compos[9]=SimplyItems(ID_QUADROTOR,wxT("Quadrotor"),wxIcon(powercube_xpm));
		compos[10]=SimplyItems(ID_PERSON,wxT("Person"),wxIcon(positionable_xpm));
	//	compos[11]=SimplyItems(ID_MOBILEROBOT,wxT("Mobile Robot"),wxIcon(nemolaser_xpm));			
		
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
			wxString text = itemData ? wxString(itemData->getNameTree()) : GetItemText(itemId) ;
			title<<wxT("Menu for ") << text;
		}
		else 
		{
			title = wxT("Menu empty");
		}
		
		wxMenu menuTree(title);
		
		//Submenu for laser styles///
		wxMenu *menuLaser=new wxMenu();
		menuLaser->Append(ID_LASMOD0, wxT("Points"));
		menuLaser->Append(ID_LASMOD1, wxT("Rays"));
		menuLaser->Append(ID_LASMOD2, wxT("Contour"));
		menuLaser->Append(ID_LASMOD3, wxT("Detection"));
		///////
		
		if(itemData->menus.menu_composed && !itemData->menus.menu_connection) menuTree.Append(ID_ADDOBJ,wxT("Add Simple Object"));
		if(itemData->menus.menu_composed && !itemData->menus.menu_connection) menuTree.Append(ID_ADDCOMP,wxT("Add Complex Object"));
		if(itemData->menus.menu_composed && !itemData->menus.menu_connection) menuTree.Append(ID_ADDCUSTOM,wxT("Add Composed Object"));
		if(itemData->menus.menu_positionable && itemData->typeConnection==0) menuTree.Append(ID_SAVEOBJ, wxT("Save object"));
		if(itemData->menus.menu_positionable && itemData->typeConnection==0) menuTree.Append(ID_SAVEOBJXML, wxT("Save object XML"));
		if(itemData->menus.menu_positionable && itemData->typeConnection==0) menuTree.Append(ID_DELOBJ, wxT("Delete Object"));
		if(itemData->menus.menu_positionable && itemData->typeConnection==0)menuTree.AppendSeparator();
		if((itemData->menus.menu_positionable)&&(itemData->pointer.positionableentity->getLinkedTo()==NULL)) menuTree.Append(ID_LINKTO, wxT("Link to"));
		if((itemData->menus.menu_positionable)&&(itemData->pointer.positionableentity->getLinkedTo()!=0)&&(itemData->getSimu()->CheckItemLinked(itemData->pointer.positionableentity)))
			menuTree.Append(ID_UNLINK, wxT("UnLink"));
		if(itemData->menus.menu_prismaticpart) menuTree.Append(ID_CHANGEFORM, wxT("Edit Prism"));
		if(itemData->menus.menu_facesetpart) menuTree.Append(ID_MODIFYFACE, wxT("Edit FaceSetPart"));
		if(itemData->menus.menu_positionable) menuTree.Append(ID_POSIT, wxT("Change position"));
		if(itemData->menus.menu_positionable) menuTree.Append(ID_ORI, wxT("Change orientation"));
		if(itemData->menus.menu_design) menuTree.Append(ID_DIS, wxT("Change design properties"));
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
		if(itemData->menus.menu_robotsim) 
		{
			menuTree.Append(ID_ROBOTGOTO, wxT("Go to target"));//simulate robot goes to a target
			menuTree.Enable(ID_ROBOTGOTO,false);
			if (itemData->getSimu()->getChild()->getPlaysimu() && !itemData->pointer.robotsim->checkRobotIsMoving())//We have to play Simulate
				menuTree.Enable(ID_ROBOTGOTO,true);
		}
		if(itemData->menus.menu_robotsim) 
		{
			menuTree.Append(ID_ROBOT, wxT("Move joints"));
			menuTree.Enable(ID_ROBOT,true);
			if (itemData->getSimu()->getChild()->getPlaysimu())//We have to play Simulate
				menuTree.Enable(ID_ROBOT,false);
		}
		if(itemData->menus.menu_simplejoint)menuTree.Append(ID_SIMPLEJOINT, wxT("Move joint"));//Move one only joint
		if(itemData->menus.menu_laser) menuTree.AppendSeparator();
		if(itemData->menus.menu_laser) menuTree.AppendSubMenu(menuLaser,wxT("Laser Styles"));
		if(itemData->menus.menu_meshpart) menuTree.AppendSeparator();
		if(itemData->menus.menu_meshpart) menuTree.Append(ID_CONVERMESH, wxT("See Cpp Code"));
		if(itemData->menus.menu_connection) menuTree.AppendSeparator();
		if(itemData->menus.menu_connection && itemData->typeConnection==0) menuTree.Append(ID_LNSERVER, wxT("Launch Server"));
		if(itemData->menus.menu_connection && itemData->typeConnection==1) menuTree.Append(ID_STSERVER, wxT("Stop Sending Data"));
		if(itemData->menus.menu_connection && itemData->typeConnection==0) menuTree.Append(ID_LNCLIENT, wxT("Launch Client"));
		if(itemData->menus.menu_connection && itemData->typeConnection==2) menuTree.Append(ID_STCLIENT, wxT("Stop Receiving Data"));
		PopupMenu(&menuTree,pt);
		
	}
	
	event.Skip();
	}
}


///Función recursiva para comprobar que no se linka con un linker de nivel superior
bool Tree::CheckLink(PositionableEntity* selected,PositionableEntity* n)
{
	if (selected->getLinkedTo()==n) return true;
	else if (selected->getLinkedTo()==NULL) return false;
	else CheckLink(selected->getLinkedTo(),n);
}



void Tree::OnShowCanvas(wxMouseEvent& event)
{
	if (m_mainWin->GetState()==0)
	{
	wxTreeItemId itemId;

	if( GetSelection()==root)
		return;
	else
		itemId=GetSelection();

	NodeTree *itemData = itemId .IsOk() ? (NodeTree *)GetItemData(itemId ):NULL;
	if(itemId.IsOk())
	{
		if(itemData->getTipo()==N_World)
		{
			if(!itemData->getSimu()->getChild()->IsShown())
				itemData->getSimu()->getChild()->Show();
			wxLogStatus(wxT("Item seleccionado"));
			itemData->getSimu()->getChild()->Maximize(!itemData->getSimu()->getChild()->IsMaximized());
			itemData->getSimu()->getChild()->Update();
		}
		else
		{
			if(itemData->pointer.solidentity)
			{
				bool box=itemData->pointer.solidentity->getDrawBox();
				itemData->pointer.solidentity->setDrawBox(!box);
				itemData->getSimu()->getChild()->Refresh();
			}
		}
	}
	else  wxLogStatus(wxT("No item under mouse"));

	event.StopPropagation();
	}
	
	
	if (m_mainWin->GetState()==1)
	{
		wxSetCursor(wxCURSOR_POINT_LEFT);
		wxTreeItemId itemId;
		if(GetSelection()==root)
		return;
		else
		itemId=GetSelection();
		bool aux=false;
		NodeTree *itemData = itemId .IsOk() ? (NodeTree *)GetItemData(itemId ):NULL;
		if(itemId.IsOk())
		{
			//Para evitar el autolinkado del elemento que hemos seleccionado 
			if(itemData->pointer.positionableentity!=m_mainWin->GetSimulated()->GetEntityToLink())
			{
				aux=CheckLink(m_mainWin->GetSimulated()->GetEntityToLink(),itemData->pointer.positionableentity);
				if(aux==false)
				{
					if(ValidateNodeToLink(itemData->getTipo()))
					{
						m_mainWin->GetSimulated()->GetEntityToLink()->LinkTo(itemData->pointer.positionableentity);
						m_mainWin->GetSimulated()->InsertLinkerEntity(itemData->pointer.positionableentity,itemId);
						m_mainWin->GetSimulated()->InsertLinkedEntity(itemData->pointer.positionableentity,m_mainWin->GetSimulated()->GetEntityToLink(),m_mainWin->GetSimulated()->GetIdToLink());
						wxSetCursor(wxCURSOR_ARROW);
						m_mainWin->SetState(0);
						wxLogStatus(wxT("Link done"));
					}
					else 
					{
						Tcp* t=itemData->pointer.composedentity->getTcp();
						m_mainWin->GetSimulated()->GetEntityToLink()->LinkTo(t);
						m_mainWin->GetSimulated()->InsertLinkerEntity(itemData->pointer.composedentity,itemId);
						m_mainWin->GetSimulated()->InsertLinkedEntity(itemData->pointer.positionableentity,m_mainWin->GetSimulated()->GetEntityToLink(),m_mainWin->GetSimulated()->GetIdToLink());
						wxSetCursor(wxCURSOR_ARROW);
						m_mainWin->SetState(0);
						wxLogStatus(wxT("Link done"));
					}
					if(m_mainWin->GetTreeStructureState())
					{
						SimulatedWorld * simuWorld=m_mainWin->GetSimulated();
						Restructure(simuWorld,GetWorld(itemId));
						showTreeStructure(simuWorld,true);
					}
				}
				else wxSetCursor(wxCURSOR_NO_ENTRY);
			}
			else wxSetCursor(wxCURSOR_NO_ENTRY);
		}	
	}
}



Tree::m_item Tree::SimplyItems(int id,wxString name, wxIcon icon)
{
			struct  m_item item;
			item.id=id;
			item.name=name;
			item.icon=icon;
			return item;
}



void Tree::ShowSelection(wxTreeEvent& event)
{
	if(m_mainWin->listWorlds.size()>0)
	if (showLinks==true)
	{
		int aux;
		bool check;
		wxTreeItemId itemId;
		itemId=GetSelection();

		wxTreeItemId old=event.GetOldItem();
		if(root==old)
		return;
		//eliminar las marcas del item anterior 
		if(old.IsOk())
		{
			NodeTree *itemData = old .IsOk() ? (NodeTree *)GetItemData(old ):NULL;
			SimulatedWorld* w;
			w=itemData->getSimu();
			SetItemBold(old,false);
			SetItemTextColour(old,*wxBLACK);
			aux=w->getLinkerPositionable(itemData->pointer.positionableentity); //Se comprueba si era linker
			if(aux!=-1)
			{
				if(w->getLinksId().size()>0)
				{
					if(w->getLinksId()[aux].size()>0)
					{
						for(int i=0;i<w->getLinksId()[aux].size();i++)  //Búsqueda de los linked asociados
						{
							SetItemBackgroundColour(w->getItemID(aux,i),*wxWHITE);
							RestoreItemImage(w->getItemID(aux,i));
						}
					}
				}
			}	

			check=w->CheckItemLinked(itemData->pointer.positionableentity);  //Se comprueba si era linked
			if(check==true)
			{
				RestoreItemImage(old);
				PositionableEntity* pos=w->getLinker(itemData->pointer.positionableentity);
				if(w->GetLinkersPos().size()>0)
				{
					for(int i=0;i<w->GetLinkersPos().size();i++)
					{
						aux=w->getLinkerPositionable(pos); //Búsqueda del indice del linker correspondiente
					}
					SetItemBackgroundColour(w->getIdLinkersId()[aux],*wxWHITE);
					RestoreItemImage(w->getIdLinkersId()[aux]);
				}
			}
		}

		//marcar los linkados del nuevo item
		NodeTree *itemData = itemId .IsOk() ? (NodeTree *)GetItemData(itemId ):NULL;
		PreviousitemId=itemId;
		if(root==itemId)
		return;
		
		if(itemId.IsOk())
		{
			SimulatedWorld* w;
			w=itemData->getSimu();
			SetItemBold(GetSelection());
			SetItemTextColour(GetSelection(),*wxBLUE);
			
			aux=w->getLinkerPositionable(itemData->pointer.positionableentity); //Se comprueba si es linker
			if(aux!=-1)
			{
				if(w->getLinksId().size()>0)
				{
					if(w->getLinksId()[aux].size()>0)
					{
						for(int i=0;i<w->getLinksId()[aux].size();i++)  //Búsqueda de los linked asociados
						{
							SetItemBackgroundColour(w->getItemID(aux,i),*wxGREEN);
							SetItemImage(w->getItemID(aux,i),48);
						}
					}	
				}
			}
			check=w->CheckItemLinked(itemData->pointer.positionableentity);  //Se comprueba si es linked
			if(check==true)
			{
				PositionableEntity* pos=w->getLinker(itemData->pointer.positionableentity);
				if(w->getIdLinkersId().size()>0)
				{
					for(int i=0;i<w->getIdLinkersId().size();i++)
					{
						aux=w->getLinkerPositionable(pos); //Búsqueda del indice del linker correspondiente
					}
					SetItemBackgroundColour(w->getIdLinkersId()[aux],*wxRED);
					SetItemImage(w->getIdLinkersId()[aux],49);
				}
			}
		}
	}



	wxTreeItemId itemId = GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) GetItemData(itemId)
										:NULL;
	if(root==itemId)
		return;
	
	if(sel)
	{
	
	if(itemData->menus.menu_solid)
		{
			itemData->pointer.solidentity->setDrawBox();
			itemData->getSimu()->getChild()->UpdateWorld();
		}
	
	wxTreeItemId old=event.GetOldItem();
	if(old.IsOk())
		{
			itemId=event.GetOldItem();
			itemData = itemId.IsOk() ? (NodeTree *) GetItemData(itemId)
										:NULL;
			if(itemData->menus.menu_solid)
			{
				itemData->pointer.solidentity->setDrawBox(false);
				itemData->getSimu()->getChild()->UpdateWorld();
			}
		}
	}


	for(unsigned int i=0;i<m_mainWin->listWorlds.size();i++)	
		m_mainWin->listWorlds[i]->getChild()->SetIsActivated(false);
	if(!itemData->getSimu()->getChild()->IsShown())
			itemData->getSimu()->getChild()->Show();
	itemData->getSimu()->getChild()->Activate();
	itemData->getSimu()->getChild()->SetIsActivated(true);
}


	
void Tree::ControlMouse(wxMouseEvent& event)
{
	if(m_mainWin->getState()==1)
	{
		wxSetCursor(wxCURSOR_POINT_LEFT);
	}
	else wxSetCursor(wxCURSOR_ARROW);

}
	

void Tree::RestoreItemImage(wxTreeItemId Id)
{
	NodeTree *itemData = (NodeTree *)GetItemData(Id);
	TypeNode typ=itemData->getTipo();
	if(typ==N_SimpleJoint) SetItemImage(Id,1);
	if(typ==N_Joint) SetItemImage(Id,3);
	if(typ==N_CylindricalPart) SetItemImage(Id,5);
	if(typ==N_PrismaticPart) SetItemImage(Id,7);
	if(typ==N_IrregularPrismaticPart) SetItemImage(Id,7);
	if(typ==N_LMS200Sim) SetItemImage(Id,9);
	if(typ==N_LMS100Sim) SetItemImage(Id,11);
	if(typ==N_FaceSetPart) SetItemImage(Id,13);
	if(typ==N_PatrolbotSim) SetItemImage(Id,15);
	if(typ==N_Pioneer3ATSim) SetItemImage(Id,17);
	if(typ==N_PowerCube70Sim) SetItemImage(Id,19);
	if(typ==N_NemoLaserSensor3DSim) SetItemImage(Id,21);
	if(typ==N_LaserSensor3DSim) SetItemImage(Id,23);
	if(typ==N_LaserSensorSim) SetItemImage(Id,25);
	if(typ==N_MeshPart) SetItemImage(Id,27);
	if(typ==N_WheeledBaseSim) SetItemImage(Id,29);
	if(typ==N_ComposedEntity) SetItemImage(Id,31);
	if(typ==N_SolidEntity) SetItemImage(Id,33);
	if(typ==N_PositionableEntity) SetItemImage(Id,35);
	if(typ==N_World) SetItemImage(Id,37);
	if(typ==N_RobotSim) SetItemImage(Id,39);
	if(typ==N_AdeptOneSim) SetItemImage(Id,41);
	if(typ==N_SpherePart) SetItemImage(Id,43);
	if(typ==N_CameraSim) SetItemImage(Id,9);
	if(typ==N_KinectSim) SetItemImage(Id,9);
	if(typ==N_MobileRobot) SetItemImage(Id,9);
	if(typ==N_PersonSim) SetItemImage(Id,9);
	if(typ==N_QuadrotorSim) SetItemImage(Id,9);
	if(typ==N_AseaIRB2000) SetItemImage(Id,39);
}



void Tree::UpdateTree(SimulatedWorld *N)
{
	World* w=N->getWorld();
	wxTreeItemId main=N->getTreeItem();
	N->getTree()->DeleteChildren(main);
	for (int i=0;i<w->getNumObjects();i++)
	{
		AddNode((*w)[i],main,N);
	}
}



void Tree::showTreeStructure(SimulatedWorld* sim,bool view)
{
	World* w=sim->getWorld();
	wxTreeItemId main=sim->getTreeItem();
	if(view==true)
	{
		bool aux=false;
		bool linked;
		int linker;

		for(int x=0;x<sim->getWorld()->getNumObjects();x++)
		{
			linker=-1;
			linked=false;
			aux=false;

			PositionableEntity* pos=(*w)[x];

			///Se comprueba que ese positionable es un linker
			linker=sim->getLinkerPositionable(pos);
			if(linker!=-1) aux=true;

			//Se comprueba si ese positionable es linked
			linked=sim->CheckItemLinked(pos);

			if(aux==true)
			{
				if((linker!=-1)&&(linked==false))////Ese item es un linker que no está linkado a nada
				{ 
					sim->getTree()->Delete(sim->getIdLinkersId()[linker]);
					Structure(sim,linker,sim->GetLinkersPos()[linker],main);
				}
			}
		}
	}

	if(view==false)
	{
		Restructure(sim,main);
	}
}




void Tree::Structure(SimulatedWorld* sim,int linker,PositionableEntity* pos,wxTreeItemId nod)
{
	bool check;
	NodeTree* node;
	node=AddNode(pos,nod,sim);
	wxTreeItemId id=node->GetId();
	for(int n=0;n<sim->getLinksPos()[linker].size();n++)
	{
		check=false;
		for(int y=0;y<sim->GetLinkersPos().size();y++)
		{
			if(sim->getLinksPos()[linker][n]==sim->GetLinkersPos()[y])
			{
				Delete(sim->getIdLinkersId()[y]);
				Structure(sim,y,sim->GetLinkersPos()[y],id);
				check=true;
			}
		}
		if(check==false)
		{
			sim->getTree()->Delete(sim->getLinksId()[linker][n]);
			AddNode(sim->getLinksPos()[linker][n],id,sim);
		}
	}
}


void Tree::Restructure(SimulatedWorld* sim,wxTreeItemId main)
{
	World* w=sim->getWorld();
	bool linker,linked;
	int aux;
	for(int x=0;x<sim->getWorld()->getNumObjects();x++)
	{
		linker=false;
		linked=false;
		PositionableEntity* pos=(*w)[x];
		aux=sim->getLinkerPositionable(pos);
		if(aux!=-1) linker=true;
		linked=sim->CheckItemLinked(pos);
		if(linked==true)
		{
			Delete(sim->getPositionableId(pos));
			AddNode(pos,main,sim);
		}
	}
}


void Tree::setShowLinks(bool sh)
{
	if (sh==true) showLinks=true;
	
	else
	{
		showLinks=false;
		//Se eliminan las marcas de la última selección activa antes de anular esta opción
		int aux;
		bool check;
		if(PreviousitemId.IsOk())
		{
			NodeTree *itemData = PreviousitemId .IsOk() ? (NodeTree *)GetItemData(PreviousitemId ):NULL;
			SimulatedWorld* w;
			w=itemData->getSimu();
			SetItemBold(PreviousitemId,false);
			SetItemTextColour(PreviousitemId,*wxBLACK);
			aux=w->getLinkerPositionable(itemData->pointer.positionableentity); //Se comprueba si era linker
			if(aux!=-1)
			{
				if(w->getLinksId().size()>0)
				{
					if(w->getLinksId()[aux].size()>0)
					{
						for(int i=0;i<w->getLinksId()[aux].size();i++)  //Búsqueda de los linked asociados
						{
							SetItemBackgroundColour(w->getItemID(aux,i),*wxWHITE);
							RestoreItemImage(w->getItemID(aux,i));
						}
					}
				}
			}	

			check=w->CheckItemLinked(itemData->pointer.positionableentity);  //Se comprueba si era linked
			if(check==true)
			{
				RestoreItemImage(PreviousitemId);
				PositionableEntity* pos=w->getLinker(itemData->pointer.positionableentity);
				if(w->GetLinkersPos().size()>0)
				{
					for(int i=0;i<w->GetLinkersPos().size();i++)
					{
						aux=w->getLinkerPositionable(pos); //Búsqueda del indice del linker correspondiente
					}
					SetItemBackgroundColour(w->getIdLinkersId()[aux],*wxWHITE);
					RestoreItemImage(w->getIdLinkersId()[aux]);
				}
			}
		}
	}
}


void Tree::EraseMarks()  //Eliminar marca del linkado al realizar el unlink
{
	int aux;
	wxTreeItemId itemId = GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) GetItemData(itemId):NULL;
	SimulatedWorld* w;
	w=itemData->getSimu();
	SetItemBold(itemId,false);
	SetItemTextColour(itemId,*wxBLACK);
	RestoreItemImage(itemId);
	PositionableEntity* pos=itemData->pointer.positionableentity->getLinkedTo();
	if(w->GetLinkersPos().size()>0)
	{
		for(int i=0;i<w->GetLinkersPos().size();i++)
		{
			aux=w->getLinkerPositionable(pos); //Búsqueda del indice del linker correspondiente
		}
		SetItemBackgroundColour(w->getIdLinkersId()[aux],*wxWHITE);
		RestoreItemImage(w->getIdLinkersId()[aux]);
	}
}


bool Tree::ValidateNodeToLink(TypeNode type)
{
	if((type==N_AseaIRB2000)||(type==N_RobotSim)||(type==N_PatrolbotSim)||(type==N_Pioneer3ATSim)) return false;
	else return true;
}