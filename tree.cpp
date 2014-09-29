#include "tree.h"
#include "mainWindow.h"
#include "simulatedWorld.h"

BEGIN_EVENT_TABLE(Tree, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_TREE, Tree::OnItemMenu)
	EVT_TREE_SEL_CHANGED(ID_TREE,Tree::ShowSelection)
	EVT_LEFT_DCLICK(Tree::ManageTreeSelection)
	EVT_MOTION(Tree::ControlMouse)
END_EVENT_TABLE()

Tree::Tree(wxWindow * parent, const wxWindowID id)
: wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize)
{
	wxImageList *images = new wxImageList(16,16,true);
	// should correspond to "bit" & "bitsel" enum 
	wxIcon icons[51];
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
	icons[50]=  wxIcon(euitiBot_xpm);


	for(int i=0;i<WXSIZEOF(icons);i++)
	{
		images->Add(icons[i]);
	}
	AssignImageList(images);
	sel=false;
	showLinks=false;
	PreviousitemId=NULL;
	num=0;
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
	World* w=simu->getWorld();
	text.Printf(wxT("World %d"),++num);
	NodeTree *auxW = new NodeTree(simu);
	wxTreeItemId node = AppendItem(root, text, 37, 38,auxW);
	auxW->SetId(node);
	for (int i=0;i<w->getNumObjects();i++)
	{
		AddNode((*w)[i],node,simu)->GetId();
	}
	return node;
}



NodeTree* Tree::AddNode(PositionableEntity* pos, wxTreeItemId parent,SimulatedWorld *simu)
{
	NodeTree *auxP = new NodeTree(pos,simu);
	
	wxTreeItemId mynode = AppendItem(parent,auxP->getNameTree(), auxP->bit, auxP->bitsel, auxP);
	auxP->SetId(mynode);
	if (simu->checkAddedItem(pos)==false)
	{
		simu->InsertId(mynode);
		simu->InsertItem(auxP->pointer.positionableentity);
	}

	ComposedEntity * auxC = dynamic_cast<ComposedEntity *>(pos);
	if(auxC)
	{
		for(int j=0;j<auxC->getNumObjects();j++)
		{
			NodeTree* aux;
			aux=AddNode((*auxC)[j],mynode,simu);
			wxTreeItemId auxnode=aux->GetId();
			if (simu->checkAddedItem((*auxC)[j])==false)
			{
				simu->InsertId(auxnode);
				simu->InsertItem((*auxC)[j]);
			}
		}
	}
	return auxP;

}



void Tree::OnItemMenu(wxTreeEvent& event)
{
	if(m_mainWin->GetState()==0)
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
		#define NUMBER_C 12
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
		compos[11]=SimplyItems(ID_EUITIBOT,wxT("Euitibot"),wxIcon(euitiBot_xpm));
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
		if((itemData->menus.menu_positionable)&&(itemData->pointer.positionableentity->getLinkedTo()!=0)&&(itemData->pointer.positionableentity->getLinkedTo()))
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
		if((itemData->menus.menu_robotsim)&&(itemData->pointer.robotsim)) menuTree.Append(ID_SAVEASROBOT, wxT("Save as RobotSim"));
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


void Tree::ManageTreeSelection(wxMouseEvent& event)
{
	if(XMLPanel!=NULL)
	{
		if(XMLPanel->IsShown()) 
		{
			wxTreeItemId itemId;
			if( GetSelection()==root)
				return;
			else
				itemId=GetSelection();

			NodeTree *itemData = itemId .IsOk() ? (NodeTree *)GetItemData(itemId ):NULL;
			if (itemData->getTipo()==N_World) 
			{
				XMLPanel->SetInformationFromTree(itemData,N_World);
			}
			else 
			{
				XMLPanel->SetInformationFromTree(itemData,N_PositionableEntity);
			}
		}
	}

	else 
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
			itemId=GetSelection();
			bool aux=false;
			NodeTree *itemData = itemId .IsOk() ? (NodeTree *)GetItemData(itemId ):NULL;
			if(GetSelection()==root)
			{	
				wxLogStatus(wxT("Invalid node"));
				return;
			}
			else
			{
				aux=isWorldNode(itemId);
				if((itemId.IsOk())&&(aux==false))
				{
					//Para evitar el autolinkado del elemento que hemos seleccionado 
					if(itemData->pointer.positionableentity!=m_mainWin->GetSimulated()->GetEntityToLink())
					{
						aux=m_mainWin->GetSimulated()->checkLink(itemData->pointer.positionableentity,m_mainWin->GetSimulated()->GetEntityToLink());
						if(aux==true)
						{
							if(dynamic_cast<Tcp *>(itemData->pointer.positionableentity))
							{
								Tcp *Owner=dynamic_cast<Tcp *>(itemData->pointer.positionableentity);
								m_mainWin->GetSimulated()->GetEntityToLink()->LinkTo(Owner);
								wxSetCursor(wxCURSOR_ARROW);
								m_mainWin->SetState(0);
								wxLogStatus(wxT("Link done"));
							}
							else
							{
								if(ValidateNodeToLink(itemData->getTipo()))
								{
									m_mainWin->GetSimulated()->GetEntityToLink()->LinkTo(itemData->pointer.positionableentity);
									wxSetCursor(wxCURSOR_ARROW);
									m_mainWin->SetState(0);
									wxLogStatus(wxT("Link done"));
								}
								else 
								{
									Tcp* t=itemData->pointer.composedentity->getTcp();
									m_mainWin->GetSimulated()->GetEntityToLink()->LinkTo(t);
									wxSetCursor(wxCURSOR_ARROW);
									m_mainWin->SetState(0);
									wxLogStatus(wxT("Link done"));
								}
							}
							if(m_mainWin->GetTreeStructureState()==true)
							{
								m_mainWin->GetSimulated()->InitializeItemsVectors();
								UpdateTree(m_mainWin->GetSimulated());
								showTreeStructure(true);
							}
						}
						else 
						{
							wxSetCursor(wxCURSOR_NO_ENTRY);
							wxLogStatus(wxT("Illogical link"));
						}
					}
					else wxSetCursor(wxCURSOR_NO_ENTRY);
				}
				else
				{
					wxLogStatus(wxT("Invalid node"));
					return;
				}
			}
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
		if(itemId==old) return;
		if(root==old)
		return;

		//eliminar las marcas del item anterior 
		if(old.IsOk())
		{
			NodeTree *itemData = old .IsOk() ? (NodeTree *)GetItemData(old ):NULL;
			if(itemData->getTipo()==N_World) return;
			SimulatedWorld* w;
			w=itemData->getSimu();
			SetItemBold(old,false);
			SetItemTextColour(old,*wxBLACK);
			for(int i=0;i<w->getItems().size();i++)
			{
				if(w->getItems()[i]->getLinkedTo()==itemData->pointer.positionableentity)
				{
					SetItemBackgroundColour(w->getItemsId()[i],*wxWHITE);
					SetItemBold(w->getItemsId()[i],false);
					RestoreItemImage(w->getItemsId()[i]);
				}
			}

			for(int i=0;i<w->getItems().size();i++)
			{
				if(w->getItems()[i]==itemData->pointer.positionableentity->getLinkedTo())
				{
					SetItemBackgroundColour(w->getItemsId()[i],*wxWHITE);
					SetItemBold(w->getItemsId()[i],false);
					RestoreItemImage(w->getItemsId()[i]);
				}
			}
		}

		//marcar los linkados del nuevo item
		NodeTree *itemData = itemId .IsOk() ? (NodeTree *)GetItemData(itemId ):NULL;
		PreviousitemId=itemId;
		if(root==itemId)
		return;
		if(itemData->getTipo()==N_World) return;
		
		if(itemId.IsOk())
		{
			SimulatedWorld* w;
			w=itemData->getSimu();
			SetItemBold(GetSelection());
			SetItemTextColour(GetSelection(),*wxBLUE);
			for(int i=0;i<w->getItems().size();i++)
			{
				if(w->getItems()[i]->getLinkedTo()==itemData->pointer.positionableentity)
				{
					SetItemBackgroundColour(w->getItemsId()[i],*wxGREEN);
					SetItemBold(w->getItemsId()[i]);
					SetItemImage(w->getItemsId()[i],48);
				}
			}

			for(int i=0;i<w->getItems().size();i++)
			{
				if(w->getItems()[i]==itemData->pointer.positionableentity->getLinkedTo())
				{
					SetItemBackgroundColour(w->getItemsId()[i],*wxRED);
					SetItemBold(w->getItemsId()[i]);
					SetItemImage(w->getItemsId()[i],49);
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
	if(m_mainWin!=NULL)
	{
		if(m_mainWin->GetState()==1)
		{
			wxSetCursor(wxCURSOR_POINT_LEFT);
		}
		else wxSetCursor(wxCURSOR_ARROW);
	}
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


void Tree::CreateLinksSubTree(PositionableEntity *pos,wxTreeItemId r,SimulatedWorld* simu)
{
	for(int i=0;i<simu->getItems().size();i++)
	{
		if(simu->getItems()[i]->getLinkedTo()==pos)
		{
			Delete(simu->getItemsId()[i]);
			NodeTree* aux;
			aux=AddNode(simu->getItems()[i],r,simu);
			simu->getItemsId()[i]=aux->GetId();
			CreateLinksSubTree(simu->getItems()[i],simu->getItemsId()[i],simu);
		}
	}
}




void Tree::showTreeStructure(bool val)
{
	if (val==true)
	{
		for(int i=0;i<m_mainWin->listWorlds.size();i++)
		{
			for(int j=0;j<m_mainWin->listWorlds[i]->getItems().size();j++)
			{
				if(m_mainWin->listWorlds[i]->getItems()[j]->getLinkedTo()==NULL)
				{
					CreateLinksSubTree(m_mainWin->listWorlds[i]->getItems()[j],m_mainWin->listWorlds[i]->getItemsId()[j],m_mainWin->listWorlds[i]);
				}
			}
		}
	}
	else
	{
		for(int i=0;i<m_mainWin->listWorlds.size();i++)
		{
			m_mainWin->listWorlds[i]->InitializeItemsVectors();
			UpdateTree(m_mainWin->listWorlds[i]);
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
		
		if(PreviousitemId.IsOk())
		{
			NodeTree *itemData = PreviousitemId .IsOk() ? (NodeTree *)GetItemData(PreviousitemId ):NULL;
			if(itemData->getTipo()==N_World) return;
			SimulatedWorld* w;
			w=itemData->getSimu();
			SetItemBold(PreviousitemId,false);
			SetItemTextColour(PreviousitemId,*wxBLACK);
			for(int i=0;i<w->getItems().size();i++)
			{
				if(w->getItems()[i]->getLinkedTo()==itemData->pointer.positionableentity)
				{
					SetItemBackgroundColour(w->getItemsId()[i],*wxWHITE);
					SetItemBold(w->getItemsId()[i],false);
					RestoreItemImage(w->getItemsId()[i]);
				}
			}

			for(int i=0;i<w->getItems().size();i++)
			{
				if(w->getItems()[i]==itemData->pointer.positionableentity->getLinkedTo())
				{
					SetItemBackgroundColour(w->getItemsId()[i],*wxWHITE);
					SetItemBold(w->getItemsId()[i],false);
					RestoreItemImage(w->getItemsId()[i]);
				}
			}
		}
	}
}


bool Tree::ValidateNodeToLink(TypeNode type)
{
	if((type==N_AseaIRB2000)||(type==N_RobotSim)||(type==N_PatrolbotSim)||(type==N_Pioneer3ATSim)||(type==N_ComposedEntity)) return false;
	else return true;
}


bool Tree::isWorldNode(wxTreeItemId itemId)
{
	NodeTree *itemData = itemId .IsOk() ? (NodeTree *)GetItemData(itemId ):NULL;
	for(int i=0;i<m_mainWin->listWorlds.size();i++)
	{
		if(itemData->pointer.world==m_mainWin->listWorlds[i]->getWorld()) return true;
	}
	return false;
}


void Tree::EraseMarks(NodeTree* unlinkedItem)
{
	SimulatedWorld* w=unlinkedItem->getSimu();
	for(int i=0;i<w->getItems().size();i++)
	{
		if(w->getItems()[i]==(unlinkedItem->pointer.positionableentity->getLinkedTo()))
		{
			SetItemBackgroundColour(w->getItemsId()[i],*wxWHITE);
			RestoreItemImage(w->getItemsId()[i]);
		}
	}
}