#include "mainWindow.h"


BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(ID_NEW, MainWindow::OnNewWorld)
	EVT_MENU(ID_LNSERVER, MainWindow::OnConnection)
	EVT_MENU(ID_STSERVER, MainWindow::OnConnection)
	EVT_MENU(ID_LNCLIENT, MainWindow::OnConnection)
	EVT_MENU(ID_STCLIENT, MainWindow::OnConnection)
	EVT_MENU(ID_ADDOBJ, MainWindow::AddObject)
	EVT_MENU(ID_ADDCOMP, MainWindow::AddObject)
	EVT_MENU(ID_ADDSPHERE, MainWindow::AddObject)
	EVT_MENU(ID_ADDCUSTOM, MainWindow::AddObject)
	EVT_MENU(ID_ADDSCARA, MainWindow::AddObject)
	EVT_MENU(ID_ADDPUMA, MainWindow::AddObject)
	EVT_MENU(ID_ADDASEA, MainWindow::AddObject)
	EVT_MENU(ID_ADDNEO, MainWindow::AddObject)
	EVT_MENU(ID_ADDCYL, MainWindow::AddObject)
	EVT_MENU(ID_ADDPRI, MainWindow::AddObject)
	EVT_MENU(ID_ADDIRRPRI, MainWindow::AddObject)
	EVT_MENU(ID_ADDFACESET, MainWindow::AddObject)
	EVT_MENU(ID_POWERCUBE, MainWindow::AddObject)
	EVT_MENU(ID_LMS200, MainWindow::AddObject)
	EVT_MENU(ID_PATROL, MainWindow::AddObject)
	EVT_MENU(ID_LMS100, MainWindow::AddObject)
	EVT_MENU(ID_WHEEL, MainWindow::AddObject)
	EVT_MENU(ID_LASER, MainWindow::AddObject)
	EVT_MENU(ID_LASER3D, MainWindow::AddObject)
	EVT_MENU(ID_NEMOLASER, MainWindow::AddObject)
	EVT_MENU(ID_CAMERA, MainWindow::AddObject)
	EVT_MENU(ID_KINECT, MainWindow::AddObject)
	EVT_MENU(ID_MOBILEROBOT, MainWindow::AddObject)
	EVT_MENU(ID_PERSON, MainWindow::AddObject)
	EVT_MENU(ID_QUADROTOR, MainWindow::AddObject)
	EVT_MENU(ID_DELOBJ, MainWindow::DeleteObject)
	EVT_MENU(ID_LASMOD0, MainWindow::OnLaserStyle)
	EVT_MENU(ID_LASMOD1, MainWindow::OnLaserStyle)
	EVT_MENU(ID_LASMOD2, MainWindow::OnLaserStyle)
	EVT_MENU(ID_LASMOD3, MainWindow::OnLaserStyle)
	EVT_MENU(wxID_EXIT, MainWindow::OnQuit)
	EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
	EVT_MENU(ID_LOADWORLD, MainWindow::OnLoadWorld)
	EVT_MENU(ID_LOADMESH, MainWindow::OnLoadMesh)
	EVT_MENU(ID_LOADOBJ, MainWindow::OnLoadObject)
	EVT_MENU(ID_SAVEOBJ, MainWindow::OnSaveObject)
	EVT_MENU(ID_SAVEWORLD, MainWindow::OnSaveWorld)
	EVT_MENU(ID_DELETE, MainWindow::OnDeleteWorld)
	EVT_MENU(ID_DRAWBOX,MainWindow::ShowSelection)
	EVT_MENU(ID_COMPRS,MainWindow::ShowReferenceComposed)
	EVT_CLOSE(MainWindow::OnClose)
	EVT_SIZE(MainWindow::OnSize)
	EVT_MENU(ID_VIS_TREE,MainWindow::OnVisibleTree)
	EVT_MENU(ID_VIS_CONNLOG,MainWindow::OnVisibleConnectionLog)
	EVT_MENU(SLI_VERT,MainWindow::PropertiesDisplay)
	EVT_MENU(CONT_MENU,MainWindow::PropertiesDisplay)
	EVT_MENU(DROP_MENU,MainWindow::PropertiesDisplay)
	EVT_MENU(POP_MENU,MainWindow::PropertiesDisplay)
	EVT_MENU(DIS_SLI,MainWindow::PropertiesDisplay)
	EVT_MENU(DIS_CONT,MainWindow::PropertiesDisplay)
	EVT_MENU(ID_VIS_TREE,MainWindow::OnVisibleTree)
	EVT_MENU(ID_ORI, MainWindow::OnChangeLocationCtrl)
	EVT_MENU(ID_POSIT, MainWindow::OnChangeLocationCtrl)
	EVT_MENU(ID_DIS, MainWindow::OnDesign)
	EVT_MENU(ID_COLOR, MainWindow::OnColor)
	EVT_MENU(ID_BOX, MainWindow::OnDrawBox)
	EVT_MENU(ID_REFER, MainWindow::OnDrawReference)
	EVT_MENU(ID_NAME, MainWindow::OnNameItemTree)
	EVT_MENU(ID_CONVERMESH, MainWindow::OnConverter)
	EVT_MENU(ID_CONVER, MainWindow::OnConverter)
	EVT_MENU(ID_MOVE, MainWindow::OnWheeledBasePanelCtrl)
	EVT_MENU(ID_ROBOT, MainWindow::OnRobotSimPanelCtrl)
	EVT_MENU(ID_SIMPLEJOINT, MainWindow::OnSimpleJointMove)
	EVT_MENU(ID_SPLITHF,MainWindow::HandleChildViews)
	EVT_MENU(ID_SPLITHS, MainWindow::HandleChildViews)
	EVT_MENU(ID_SPLITVF, MainWindow::HandleChildViews)
	EVT_MENU(ID_SPLITVS, MainWindow::HandleChildViews)
	EVT_MENU(ID_UNSPLITF, MainWindow::HandleChildViews)
	EVT_MENU(ID_UNSPLITS, MainWindow::HandleChildViews)
	EVT_MENU(ID_PLAY, MainWindow::HandleChildViews)
	EVT_MENU(ID_STOP2,MainWindow::HandleChildViews)
	EVT_MENU(ID_CANVASCOLOR,MainWindow::HandleChildViews)
	EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnCloseNotebook)
	EVT_UPDATE_UI(ID_LOADOBJ, MainWindow::UpdateUILoadObject)
	EVT_UPDATE_UI(ID_SAVEOBJ,MainWindow::UpdateUISaveObject)
	EVT_UPDATE_UI(ID_SAVEWORLD,MainWindow::UpdateUISaveWorld)
	EVT_SASH_DRAGGED(ID_DRAG, MainWindow::OnSashDrag)

END_EVENT_TABLE()




MainWindow::MainWindow(wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos,const wxSize& size, const long style)
: wxMDIParentFrame(parent, id, title, pos, size, style),note(0)
{
	
	Centre();
	treeVisible=true;
	slider=true;
	popmenu=true;
	design_slider=true;
	managewindow=new ManageWindows();
	

#if wxUSE_STATUSBAR
	CreateStatusBar();
	SetStatusText(wxT("Welcome to APOLO"));
#endif // wxUSE_STATUSBAR



	CreateMenuBar();
	
	SetIcon(wxIcon(apolo_xpm));
	GetClientSize(&w,&h);
	CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);
	InitToolBar(GetToolBar());
	s=new wxSashLayoutWindow(this, ID_DRAG,wxDefaultPosition, wxDefaultSize, wxSW_3D | wxCLIP_CHILDREN);
	
	s->SetDefaultSize(wxSize(w/4, h));
	s->SetOrientation(wxLAYOUT_VERTICAL);
	s->SetAlignment(wxLAYOUT_LEFT);
	s->SetSashVisible(wxSASH_RIGHT, true); 
	
	//make a Aui Notebook
	note = new wxAuiNotebook(s, wxID_ANY,wxDefaultPosition,wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_TAB_EXTERNAL_MOVE);
	
	tree = new Tree(note, ID_TREE);
	tree->m_mainWin = this;	
	m_root = tree->AddRoot(wxT("Universe"), 0, 47, new TreeItemData(wxT("Root item")));
	tree->Parent(m_root);

	note->AddPage(tree, wxT("Universe"));
	
	rToogle=false;
	SimulatedWorld::tree = tree;
	SimulatedWorld::mainWin = this;
	port=new ApoloPort(12000,&listWorlds);
	connection=new RobotConnection(this,wxT("Server Configuration"));
	
	
}
	
void MainWindow::CreateMenuBar()
{
	//make main Menu//
	menuFile = new wxMenu;
	wxMenuItem *item1 = new wxMenuItem(menuFile, ID_NEW, wxT("New world"),wxT("Create a new world"));
	item1->SetBitmap(new_xpm);
	menuFile->Append(item1);
	wxMenuItem *item2 = new wxMenuItem(menuFile,ID_LOADWORLD, wxT("Load world"), wxT("Load a file of world"));
	item2->SetBitmap(loadWorld_xpm);
	menuFile->Append(item2);
	menuFile->AppendSeparator();
	menuFile->AppendCheckItem(ID_VIS_TREE, wxT("Unvisible tree"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, wxT("E&xit"), wxT("Quit the program"));
	
	menuAbout = new wxMenu;
	menuAbout->Append(wxID_ABOUT, wxT("&About..."), wxT("Information program"));
	menuSettings=new wxMenu;

	ipro=new wxMenu;
	osel=new wxMenu;
	dwid=new wxMenu;
	menuSettings->AppendSubMenu(ipro,wxT("Positionable Properties"),wxT("Change display configuration of Positionable Properties"));
	ipro->AppendCheckItem(CONT_MENU,wxT("Contextual Menu"));
	ipro->AppendCheckItem(SLI_VERT,wxT("Vertical Sliders"));
	menuSettings->AppendSubMenu(osel,wxT("Object Selection"),wxT("Change display configuration of Positionable Properties"));
	osel->AppendCheckItem(POP_MENU,wxT("Pop-Up Menu"));
	osel->AppendCheckItem(DROP_MENU,wxT("Drop-Down Menu"));
	menuSettings->AppendSubMenu(dwid,wxT("Design Widget"),wxT("Change display configuration of Design Properties"));
	dwid->AppendCheckItem(DIS_SLI,wxT("Slider Menu"));
	dwid->AppendCheckItem(DIS_CONT,wxT("Contextual Menu"));
	CheckProperties();

	menubar = new wxMenuBar;
	menubar->Append(menuFile, wxT("&File"));
	menubar->Append(menuAbout, wxT("About"));
	menubar->Append(menuSettings, wxT("Settings"));
	SetMenuBar(menubar);
	

	
	
		
	//make a child menu
	menuFile2 = new wxMenu;
	wxMenuItem *c_item1 = new wxMenuItem(menuFile2, ID_NEW, wxT("New world"),wxT("Create a new world"));
	c_item1->SetBitmap(new_xpm);
	menuFile2->Append(c_item1);
	wxMenuItem *c_item2 = new wxMenuItem(menuFile2,ID_LOADWORLD, wxT("Load world"), wxT("Load a file of world"));
	c_item2->SetBitmap(loadWorld_xpm);
	menuFile2->Append(c_item2);
	wxMenuItem *c_item3 = new wxMenuItem(menuFile2,ID_LOADOBJ, wxT("Load Object"), wxT("Add object to current World select"));
	c_item3->SetBitmap(loadObject_xpm);
	menuFile2->Append(c_item3);
	menuFile2->AppendSeparator();
	menuFile2->Append(ID_LOADMESH, wxT("Import .stl"), wxT("Import .stl file"));
	menuFile2->AppendSeparator();
	menuFile2->Append(ID_CONVER, wxT("Converter .stl"), wxT("Converter .stl file"));
	menuFile2->AppendSeparator();
	wxMenuItem *c_item4 = new wxMenuItem(menuFile2,ID_SAVEWORLD, wxT("Save World"),wxT("Save world select"));
	c_item4->SetBitmap(saveWorld_xpm);
	menuFile2->Append(c_item4);
	wxMenuItem *c_item5 = new wxMenuItem(menuFile2,ID_SAVEOBJ, wxT("Save Object"), wxT("Save object select"));
	c_item5->SetBitmap(saveObject_xpm);
	menuFile2->Append(c_item5);
	menuFile2->Append(ID_DELETE, wxT("Delete world"),wxT("Delete world select"));
	menuFile2->AppendSeparator();
	menuFile2->AppendCheckItem(ID_VIS_TREE,wxT("Unvisible Tree"));
	menuFile2->AppendCheckItem(ID_VIS_CONNLOG, wxT("Unvisible ConnectionLog"));
	menuFile2->AppendSeparator();
	menuFile2->Append(wxID_EXIT, wxT("Close all"), wxT("Quit the program"));

	menuView = new wxMenu;
	wxMenuItem *itemV1 = new wxMenuItem(menuView, ID_SPLITHF, wxT("Double horizontal"));
	itemV1->SetBitmap(V1_xpm);
	wxMenuItem *itemV2 = new wxMenuItem(menuView, ID_SPLITVF, wxT("Double vertical"));
	itemV2->SetBitmap(V2_xpm);
	wxMenuItem *itemV3 = new wxMenuItem(menuView, ID_SPLITHS, wxT("Triple horizontal"));
	itemV3->SetBitmap(V4_xpm);
	wxMenuItem *itemV4 = new wxMenuItem(menuView, ID_SPLITVS, wxT("Triple vertical"));
	itemV4->SetBitmap(V3_xpm);
	wxMenuItem *itemV5 = new wxMenuItem(menuView, ID_UNSPLITF, wxT("Simple"));
	itemV5->SetBitmap(V5_xpm);
	menuView->Append(itemV1);
	menuView->Append(itemV2);
	menuView->Append(itemV3);
	menuView->Append(itemV4);
	menuView->Append(itemV5);
	menuView->Append(ID_UNSPLITS, wxT("Double"));
	menuView->AppendSeparator();
	menuView->Append(ID_CANVASCOLOR, wxT("Change background color"));
	menuSimulator = new wxMenu;
	wxMenuItem *Iplay = new wxMenuItem(menuSimulator, ID_PLAY, wxT("Play Simulator"));
	Iplay->SetBitmap(play_xpm);
	wxMenuItem *Istop = new wxMenuItem(menuSimulator, ID_STOP2, wxT("Stop Simulator"));
	Istop->SetBitmap(stop2_xpm);
	menuSimulator->Append(Iplay);
	menuSimulator->Append(Istop);
	
}

void MainWindow::OnReplaceMenuBar()
{
	
	if(listWorlds.size()==0)
	{	
		
		while(menubar->GetMenuCount()>0)GetMenuBar()->Remove(0);
		menubar->Append(menuFile,wxT("File"));
		menubar->Append(menuAbout,wxT("About"));
		menubar->Append(menuSettings,wxT("Settings"));
	
	}

	
	else if(menubar->GetMenuCount()==3)
	{
		menubar->Replace(0,menuFile2,wxT("&File"));
		menubar->Replace(1,menuView,wxT("Views"));
		menubar->Replace(2,menuSimulator,wxT("Run"));
		menubar->Append(menuAbout,wxT("About"));
		menubar->Append(menuSettings, wxT("Settings") );
	}



}
void MainWindow::OnSashDrag(wxSashEvent& event)
{
  
    s->SetDefaultSize(wxSize(event.GetDragRect().width, h));
           
	#if wxUSE_MDI_ARCHITECTURE
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
	#endif // wxUSE_MDI_ARCHITECTURE

    // Leaves bits of itself behind sometimes
    GetClientWindow()->Refresh();
	
    }
void MainWindow::OnClose(wxCloseEvent& event)
{

		if(connection->getLog()->IsObjectConnected())
			wxMessageBox(wxT("Please stop all connections"),wxT("Cannot close the window"));
		else
		{
			wxString msg;
			msg.Printf(wxT("Are you sure close window?"));
			if ( wxMessageBox(msg, _T("Please confirm"),wxICON_QUESTION | wxYES_NO) != wxYES )	 
				 return;
		Destroy();
		
		}
  }
void MainWindow::InitToolBar(wxToolBar *tool)
{
	toolbar=tool;
	wxBitmap bitmaps[7];
	bitmaps[0] = wxBitmap (new_xpm);
	bitmaps[1] = wxBitmap (loadWorld_xpm);
	bitmaps[2] = wxBitmap (loadObject_xpm);
	bitmaps[3] = wxBitmap (saveWorld_xpm);
	bitmaps[4] = wxBitmap (saveObject_xpm);
	bitmaps[5] = wxBitmap (box_xpm);
	bitmaps[6] = wxBitmap (positionable_xpm);

	toolbar->AddTool(ID_NEW, bitmaps[0], wxT("New World"));
	toolbar->AddTool(ID_LOADWORLD, bitmaps[1], wxT("Load World"));
	toolbar->AddTool(ID_SAVEWORLD, bitmaps[3], wxT("Save World")); 
	toolbar->AddTool(ID_LOADOBJ, bitmaps[2], wxT("Load Object"));
	toolbar->AddTool(ID_SAVEOBJ, bitmaps[4], wxT("Save Object"));
	toolbar->AddSeparator();
	toolbar->AddCheckTool(ID_DRAWBOX,wxT("Draw Box"),bitmaps[5],wxNullBitmap,wxT("Show Item Selected"));
	toolbar->AddCheckTool(ID_COMPRS,wxT("Composed Reference System"),bitmaps[6],wxNullBitmap,wxT("Show Reference System of main objects and composed objects"));
	toolbar->AddSeparator();
	toolbar->Realize();
}
void MainWindow::OnSize(wxSizeEvent& WXUNUSED (event))
{
	#if wxUSE_MDI_ARCHITECTURE
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
	#endif 

}
void MainWindow::OnCloseNotebook(wxAuiNotebookEvent& event)
{
	wxAuiNotebook* m_note = (wxAuiNotebook *) event.GetEventObject();
	if(m_note->GetSelection()== 0)event.Veto();
}
void MainWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	
	wxMessageBox(wxT("Apolo Simulator\n")
		wxT("Authors: \nMiguel Hernando Gutierrez 2010-2012\nCarlos Mateo Benito 2011-2012\nEsther LLorente Garcia 2010-2011\nHas been used MRCore Library License and wxWindows Library License:\nwxWidgets 2.9.3 (www.wxwidgets.org)\nCopyright (C) 1998-2005 Julian Smart, Robert Roebling et al."),
				 wxT("Information"),wxOK | wxICON_INFORMATION, this);
}

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
		Close(true);
	
}
void MainWindow::HandleChildViews(wxCommandEvent &event)
{
			
	for(unsigned int i=0;i<listWorlds.size();i++)
	{
		
		if(listWorlds[i]->getChild()->getIsActivated())
		{

			switch(event.GetId())
			{
			case ID_UNSPLITF:
				listWorlds[i]->getChild()->UnSplitFirst();
				break;

				case ID_UNSPLITS:
					listWorlds[i]->getChild()->UnSplitSecond();
					break;

				case ID_SPLITVF:
					listWorlds[i]->getChild()->SplitVerticalFirst();
					break;
				
				case ID_SPLITVS:
					listWorlds[i]->getChild()->SplitVerticalSecond();
					break;

				case ID_SPLITHF:
					listWorlds[i]->getChild()->SplitHorizontalFirst();
					break;

				case ID_SPLITHS:
					listWorlds[i]->getChild()->SplitHorizontalSecond();
					break;
					

				case ID_PLAY:
					listWorlds[i]->getChild()->OnSimulator(ID_PLAY);
					break;

				case ID_STOP2:
					listWorlds[i]->getChild()->OnSimulator(ID_STOP2);
					break;
				case ID_CANVASCOLOR:
					listWorlds[i]->getChild()->ChangeBackgroundColor();
					break;

			}
			
		}
	}
			

}

void MainWindow::OnVisibleTree(wxCommandEvent& WXUNUSED(event))
{
	if(treeVisible)
	{
		s->Show(false);
		treeVisible=false;
		wxLogStatus(wxT("Visible tree"));
	}
	else 
	{
		s->Show(true);
		treeVisible=true;
		wxLogStatus(wxT("Unvisible tree"));
	}
	
	#if wxUSE_MDI_ARCHITECTURE
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
	#endif // wxUSE_MDI_ARCHITECTURE
}

void MainWindow::OnVisibleConnectionLog(wxCommandEvent& event)
{
	if(connection->getLogVisible())
		connection->ShowConnLog(false);
	else
		connection->ShowConnLog(true);
}

void MainWindow::AddObject(wxCommandEvent& event)
{
	int id= event.GetId();
		
	if(id!=ID_ADDCUSTOM && popmenu==true)
	{
		ObjectSelection *ob_sel=new ObjectSelection(this,id,wxDefaultPosition,wxDefaultSize);
		ob_sel->ShowModal();
		id=ob_sel->GetObject();
			if(id==0)return;
	}
	for(unsigned int i=0;i<listWorlds.size();i++)	
		if (listWorlds[i]->getTreeItem() == tree->GetWorld(tree->GetSelection()))
			listWorlds[i]->AddObject(id);
	
	
	
}

void MainWindow::DeleteObject(wxCommandEvent& WXUNUSED(event))
{


	wxString msg;
        msg.Printf(wxT("Are you sure you want to delete this object?"));
        if ( wxMessageBox(msg, _T("Please confirm"),wxICON_QUESTION | wxYES_NO) != wxYES ) return;
        else
		{	
			int rb=0,smp=0,wh=0;
			vector <RobotSimPanel*> robot=managewindow->getVectorRobotSimPanel();
			vector <WheeledBasePanel*> wheeled=managewindow->getVectorWheeledBasePanel();

			wxTreeItemId itemid=tree->GetWorld(tree->GetSelection());

			wxTreeItemId itemId = tree->GetSelection();
			NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;

			for(unsigned int i=0;i<listWorlds.size();i++)
				if(listWorlds[i]->getTreeItem()==itemid)
				{
					int size=0;

					if (robot.size()>wheeled.size())//Comprobamos cuál es el de mayor tamaño para que no haya problemas con los vectores
						size=robot.size();
					else
						size=wheeled.size(); //Si tuviésemos mas ventanas es fácil de implementar

						for (int j=size-1;j>=0;j--) //Vamos de atrás a adelante para no dejarnos nada por eliminar
						{	
							
							rb=smp=wh=0; //Actualizamos los indicadores de tipo cada vuelta en el bucle
						
							if ((robot.empty()==false) && (j<robot.size())) //Nos aseguramos de que no esté vacío el vector y el índice no sea mayor que su tamaño
								if (robot[j]->getItemNode ()== itemData)
									rb=1;				//ponemos a 1 si es de tipo robotsimpanel
								else if (robot[j]->getItemParentData()==itemData)
									smp=1;				//si es de tìpo simplejoint

							if ((wheeled.empty()==false) && (j<wheeled.size()))
								if (wheeled[j]->getItemNode ()==itemData)
									wh=1;
									
							if (rb || smp) //Si los menus son de tipo simplejoint o robotsim
								robot[j]->Delete(); //Ya localizados los menús pertenecientes al padre los eliminamos todos

							else if(wh) //Si los menus son de tipo WheeledBasePanel
								wheeled[j]->Delete();
							
						}
					
					listWorlds[i]->DeleteObject(tree->GetSelection()); // Borramos el icono del árbol del mundo y el objeto en sí
				}
		}
			
}


/*bool MainWindow::checkPanelExist(NodeTree* node)
{
	int numPage = note->GetPageCount();
	for(int i=0;i<numPage;i++)
	{
		if(((ApoloPanel *)note->GetPage(i))->getItemNode() == node)
		{
			return false;
		}
	}
	return true;
}*/



void MainWindow::OnNameItemTree(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId  = tree->GetSelection();
	NodeTree *itemData = itemId .IsOk() ? (NodeTree *)tree->GetItemData(itemId ):NULL;
    static wxString s_text;
    s_text = wxGetTextFromUser(wxT("New name:"), wxT("Change Name Item Tree"),s_text, this);
	char text[100];
	strcpy(text,(const char*)s_text.mb_str(wxConvUTF8));
	if(!s_text.empty() && itemData->menus.menu_world)
	{
		tree->SetItemText(itemId, s_text);
		itemData->getSimu()->setName(s_text.ToStdString());
	}
    else if ( !s_text.empty() && itemData->pointer.positionableentity)
    {
		itemData->pointer.positionableentity->setName(text);
		tree->SetItemText(itemId, s_text);
    }
}
void MainWindow::OnConverter(wxCommandEvent& event)
{
	int id = event.GetId();
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	Converter* conver;
	if(id == ID_CONVERMESH)
	{
		
		conver = new Converter(this,ID_CONVERMESH,wxT("CPP CODE"));
		conver->OnlyRead(itemData->pointer.meshpart);
		conver->ShowModal();
		wxLogStatus(wxT("See cpp code"));
		
	}
	if(id == ID_CONVER)
	{
		
		conver = new Converter(this,ID_CONVER,wxT("Converter .stl"));
		conver->ShowModal();
		wxLogStatus(wxT("Converter"));
	}

	delete conver;
}
void MainWindow::OnChangeLocationCtrl(wxCommandEvent& event)
{
	int id = event.GetId();
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	if(itemData->pointer.positionableentity)
	{
		ChangeLocationCtrl* locationCtrl;
		if(id == ID_ORI)locationCtrl = new ChangeLocationCtrl(this, ID_ORI, wxT("Change Orientation"));
		else locationCtrl = new ChangeLocationCtrl(this, ID_POSIT, wxT("Change Position"));
		
		locationCtrl->setItemData(itemData);
		locationCtrl->ShowModal();	
		wxLogStatus(wxT("Change Location"));
		delete locationCtrl;
	}
	
	event.Skip();
}
void MainWindow::OnWheeledBasePanelCtrl(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;

	if(itemData->pointer.wheeledbasesim)
	{
		if(managewindow->CheckWindowsExist(itemData))
		{


			WheeledBasePanel* wheeledBaseCtrl;
			wheeledBaseCtrl = new WheeledBasePanel(this,wxID_ANY,wxT(" Move"),itemData);
			wheeledBaseCtrl->getTitle()->SetLabel(wxString(itemData->getNameTree()));
			wheeledBaseCtrl->setManageWindow(managewindow);
			wheeledBaseCtrl->Show(true);
			wxLogStatus(wxT("Wheeled Base Panel"));
		}
	}
}
void MainWindow::OnRobotSimPanelCtrl(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	if(itemData->pointer.robotsim)
	{
		if(managewindow->CheckWindowsExist(itemData))
		{
			RobotSimPanel* robotSimCtrl;
			robotSimCtrl = new RobotSimPanel(this,wxID_ANY,wxT("Move all Joints"),itemData);
			robotSimCtrl->getTitle()->SetLabel(wxString(itemData->getNameTree()));
			robotSimCtrl->setManageWindow(managewindow);
			robotSimCtrl->Show(true);	
			wxLogStatus(wxT("Robot Sim Panel"));
		}
		
	}
}
void MainWindow::OnSimpleJointMove( wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;

	wxTreeItemId parentItem=tree->GetItemParent(itemId);
	NodeTree *parentData = parentItem.IsOk() ? (NodeTree *) tree->GetItemData(parentItem):NULL;

	if(itemData->pointer.simplejoint)
	{
		if(managewindow->CheckWindowsExist(itemData))
		{
			RobotSimPanel* robotSimCtrl;
			robotSimCtrl = new RobotSimPanel(this,wxID_ANY,wxT ("Move only one Joint"),itemData,parentData,true);
			robotSimCtrl->getTitle()->SetLabel(wxString(itemData->getNameTree()));
			robotSimCtrl->setManageWindow(managewindow);
			robotSimCtrl->Show(true);	
			wxLogStatus(wxT("Robot Sim Panel/Joint"));
		}
		
	}
}

void MainWindow::OnDrawBox(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId .IsOk() ? (NodeTree *)tree->GetItemData(itemId ):NULL;
	if(itemData->pointer.solidentity->getDrawBox())ShowBox(true);
	else ShowBox(false);
}
void MainWindow::OnDrawReference(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *)tree->GetItemData(itemId):NULL;
	if(itemData->pointer.positionableentity->getDrawReferenceSystem())ShowReference(true);
	else ShowReference(false);
}

void MainWindow::OnDesign(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	DesignProperties *design=new DesignProperties(this,itemData,wxT("Design Properties"));
	design->ShowModal();
	delete design;
}

void MainWindow::OnColor(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	if(itemData->pointer.solidentity) 
	{
		double _r,_g,_b;
		itemData->pointer.solidentity->getColor(_r,_g,_b);
		wxColour c = wxColour(_r*255,_g*255,_b*255);
		wxColour color = wxGetColourFromUser(0);
		if(color.IsOk())
		{
			double r = color.Red();
			double g = color.Green();
			double b = color.Blue();
			itemData->pointer.solidentity->setColor(r/255,g/255,b/255);
			for(unsigned int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();
		}
		for(unsigned int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();		
	}
}
void MainWindow::OnNewWorld(wxCommandEvent& WXUNUSED(event))
{
	World *w = new World();
	simuWorld = new SimulatedWorld(w);
	listWorlds.push_back(simuWorld);
	tree->Expand(tree->GetRootItem());
	OnReplaceMenuBar();

}
void MainWindow::OnLoadWorld(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog openFile(this, wxT("Load world file"), wxEmptyString, wxEmptyString,
			wxT("MWF files (*.mwf)|*.mwf"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	
	if(openFile.ShowModal() == wxID_OK)
	{
		wxString file = openFile.GetPath();
		char filec[500];
		strcpy(filec, (const char*)file.mb_str(wxConvUTF8));

		StreamFile myfile(filec,true);
		Object *test1 = myfile.read();
		World *test2 = dynamic_cast<World *>(test1);
	
		if(test2)
		{
			simuWorld = new SimulatedWorld(test2);
			listWorlds.push_back(simuWorld);
		}
		else 
		{
			delete test1;
		}
	}
	wxFileInputStream input(openFile.GetPath());
	if(!input.IsOk())
	{
		wxLogError(wxT("Cannot open file %s"), openFile.GetPath().c_str());
		return;
	}
	tree->Expand(tree->GetRootItem());
	tree->Expand(tree->GetLastChild(tree->GetRootItem()));
	Search(tree->GetLastChild(tree->GetRootItem()),toolbar->GetToolState(ID_COMPRS));
	OnReplaceMenuBar();
}
void MainWindow::OnLoadMesh(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	
	for(unsigned int i= 0; i<listWorlds.size(); i++)
	{
		if(listWorlds[i]->getTreeItem()==itemId)
		{
			wxFileDialog openFileDialog(this,wxT("Load meshPart file"), wxEmptyString, wxEmptyString,
            wxT("STL files (*.stl)|*.stl"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);

			if (openFileDialog.ShowModal() == wxID_OK)
			{
				wxString fileName = openFileDialog.GetPath();
				char file[100];
				strcpy(file,(const char*)fileName.mb_str(wxConvUTF8));
				
				MeshPart* mesh;
				mesh = new MeshPart;
					
				if(mesh->loadAsciiSTL(file))(*listWorlds[i]->getWorld())+=mesh;
				else if(mesh->loadBinarySTL(file))(*listWorlds[i]->getWorld())+=mesh;
				else 
				{
					wxLogError(wxT("Cannot load file %s."),openFileDialog.GetPath().c_str());
					return;
				}
				listWorlds[i]->getChild()->UpdateWorld();
				tree->AddNode(mesh, listWorlds[i]->getTreeItem(),listWorlds[i]);
			}
			wxFileInputStream input(openFileDialog.GetPath());
			if (!input.IsOk())
			{
				 wxLogError(wxT("Cannot open file %s."), openFileDialog.GetPath().c_str());
				 return;
			}
		}
		else wxLogMessage(wxT("Please, select a World."));
	}
}
void MainWindow::OnLoadObject(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	for(unsigned int i=0; i<listWorlds.size(); i++)
	{
		if(listWorlds[i]->getTreeItem()==itemId)
		{
			wxFileDialog openFile(this,wxT("Load object file"), wxEmptyString, wxEmptyString,
            wxT("MOF files (*.mof)|*.mof"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);
			if(openFile.ShowModal() == wxID_OK)
			{
				wxString fileName = openFile.GetPath();
				char c_file[100];
				strcpy(c_file,(const char*)fileName.mb_str(wxConvUTF8));
				StreamFile s_file(c_file,true);
				Object *obj = s_file.read();
				PositionableEntity* p_obj = dynamic_cast<PositionableEntity *>(obj);
				if(obj)
				{
					(*listWorlds[i]->getWorld())+=p_obj;
					listWorlds[i]->getChild()->UpdateWorld();
					tree->AddNode(p_obj, listWorlds[i]->getTreeItem(),listWorlds[i]);
				}
			}
			wxFileInputStream input(openFile.GetPath());
			if(!input.IsOk())
			{
				wxLogError(wxT("Cannot open file %s"),openFile.GetPath().c_str());
				return;
			}
		}
		else wxLogMessage(wxT("Please, select a World."));
	}
}
void MainWindow::OnSaveWorld(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	
	for(unsigned int i= 0; i<listWorlds.size(); i++)
	{
		if(listWorlds[i]->getTreeItem()==itemId)
		{
			wxFileDialog saveFile(this,wxT("Save World file "),wxEmptyString,wxEmptyString,
				wxT("MWF files (*.mwf)|*.mwf"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
			if(saveFile.ShowModal() == wxID_OK)
			{
				wxString file = saveFile.GetPath();
				char filec[100];
				strcpy(filec, (const char*) file.mb_str(wxConvUTF8));
				
				StreamFile myfile(filec,false);
				myfile.write(listWorlds[i]->getWorld());

				wxLogMessage(wxT("Successfully save in %s"), saveFile.GetPath().c_str());
			}
		}
		else wxLogMessage(wxT("Please, select a World."));
	}
}
void MainWindow::OnSaveObject(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree* itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;

	if(itemData->menus.menu_positionable && m_root!=itemId)
	{
		wxFileDialog saveFile(this,wxT("Save Object file"), wxEmptyString, wxEmptyString,
        wxT("MOF files (*.mof)|*.mof"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if(saveFile.ShowModal() == wxID_OK)
		{
			wxString fileName = saveFile.GetPath();
			char c_file[100];
			strcpy(c_file,(const char*)fileName.mb_str(wxConvUTF8));
			StreamFile s_file(c_file,false);
			itemData->pointer.positionableentity->setRelativeT3D(0);
			s_file.write(itemData->pointer.positionableentity);

			wxLogMessage(wxT("Successfully save in %s"), saveFile.GetPath().c_str());
		}
	}
	else wxLogMessage(wxT("Please select a Object."));
}


void MainWindow::OnDeleteWorld(wxCommandEvent& WXUNUSED(event))
{	
	wxTreeItemId itemId = tree->GetSelection();
	int deletePartWorld=0;
	
	for(int i=listWorlds.size()-1; i>=0; i--)
	{
		if(listWorlds[i]->getTreeItem()==itemId)
		{
			deletePartWorld = i;
			int numPage = note->GetPageCount();
			for(int j=numPage-1;j>=1; j--)
			{
				SimulatedWorld* a=((WheeledBasePanel *)note->GetPage(j))->getItemNode()->getSimu();
				SimulatedWorld* b = listWorlds[i];
				if(a == b)note->DeletePage(j);
			}
			
		}
	}
	delete (listWorlds[deletePartWorld]);
	listWorlds.erase(listWorlds.begin()+deletePartWorld);
	OnReplaceMenuBar();


}
void MainWindow::UpdateUILoadObject(wxUpdateUIEvent& event)
{
	event.Enable(listWorlds.size()!=0);
}
void MainWindow::UpdateUISaveObject(wxUpdateUIEvent& event)
{
	event.Enable(listWorlds.size()!=0);
}
void MainWindow::UpdateUISaveWorld(wxUpdateUIEvent& event)
{
	event.Enable(listWorlds.size()!=0);
}


void MainWindow::ShowBox(bool box)
{
	drawBox = box;
	wxTreeItemId itemId  = tree->GetSelection();
	NodeTree *itemData = itemId .IsOk() ? (NodeTree *)tree->GetItemData(itemId ):NULL;
	if(itemData->pointer.solidentity)
	{
		if(drawBox)itemData->pointer.solidentity->setDrawBox(false);
		else itemData->pointer.solidentity->setDrawBox(true);
		for(unsigned int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();	
	}	
}
void MainWindow::ShowReference(bool refer)
{
	referVisible = refer;
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *)tree->GetItemData(itemId):NULL;
	if(itemData->pointer.positionableentity)
	{
		if(referVisible)itemData->pointer.positionableentity->setDrawReferenceSystem(false);
		else itemData->pointer.positionableentity->setDrawReferenceSystem(true);
		for(unsigned int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();
	}	
}

void MainWindow::PropertiesDisplay(wxCommandEvent& event)
{
	int id=event.GetId();

	if(id==SLI_VERT)slider=true;
	if(id==CONT_MENU)slider=false;
	if(id==POP_MENU)popmenu=true;
	if(id==DROP_MENU)popmenu=false;
	if(id==DIS_CONT)design_slider=false;
	if(id==DIS_SLI)design_slider=true;
	CheckProperties();
}

void MainWindow::CheckProperties()
{

	if(popmenu==true)
	{
		osel->Check(POP_MENU,true);
		osel->Check(DROP_MENU,false);
	}
	else
	{
		osel->Check(DROP_MENU,true);
		osel->Check(POP_MENU,false);
		
	}
	if(design_slider==true)
	{
		dwid->Check(DIS_SLI,true);
		dwid->Check(DIS_CONT,false);
	}
	else
	{
		dwid->Check(DIS_SLI,false);
		dwid->Check(DIS_CONT,true);
		
	}
	
	if(slider==true)
	{
		ipro->Check(SLI_VERT,true);
		ipro->Check(CONT_MENU,false);
	}
	else
	{
		ipro->Check(SLI_VERT,false);
		ipro->Check(CONT_MENU,true);
	}
}
void MainWindow::ShowSelection(wxCommandEvent& event)
{
	int id=event.GetId();
	bool d_box=toolbar->GetToolState(id);
	NodeTree *itemData = tree->GetSelection().IsOk() ? (NodeTree *) tree->GetItemData(tree->GetSelection())
										:NULL;
		
	if(d_box)
	{
		tree->SetShowSelection(true);
		if(!tree->GetSelection().IsOk()) return;
		if(itemData->menus.menu_solid  && tree->GetSelection()!=tree->GetRootItem())
		{
			itemData->pointer.solidentity->setDrawBox(true);
			itemData->getSimu()->getChild()->Refresh();
		}
	}
	else
	{
		
		tree->SetShowSelection(false);
		if(!tree->GetSelection().IsOk()) return;
		if(itemData->menus.menu_solid && tree->GetSelection()!=tree->GetRootItem())
		{
			itemData->pointer.solidentity->setDrawBox(false);
			itemData->getSimu()->getChild()->Refresh();
		}
		
	}
	
		
}

void MainWindow::ShowReferenceComposed(wxCommandEvent& event)
{
	int id=event.GetId();
	rToogle=toolbar->GetToolState(id);
	wxTreeItemIdValue value;
	if(tree->HasChildren(tree->GetRootItem()))
	{
		Search(tree->GetFirstChild(tree->GetRootItem(),value),rToogle);
		(this)->GetSimulated()->getChild()->Refresh();
	}
	
}

void  MainWindow::Search(wxTreeItemId search,bool toogle)
{
	wxTreeItemIdValue cookie;
	NodeTree *itemData = search.IsOk() ? (NodeTree *) tree->GetItemData(search)
		:NULL;

	if(itemData->getTipo()==N_World )
		if(itemData->pointer.world->getNumObjects()!=0)
			Search(tree->GetFirstChild(search,cookie),toogle);
	
	if(itemData->menus.menu_positionable)
	{
		if(itemData->menus.menu_composed || itemData->pointer.positionableentity->getOwner()->getClassName()=="World")
		{
			if(toogle)
				itemData->pointer.positionableentity->setDrawReferenceSystem();
			else
				itemData->pointer.positionableentity->setDrawReferenceSystem(false);
		
			if(itemData->menus.menu_composed)
				if(itemData->pointer.composedentity->getNumObjects()>0)
					Search(tree->GetFirstChild(search,cookie),toogle);
		}
	}
	if(tree->GetLastChild(tree->GetItemParent(search))==search)
		return;
	
	Search(tree->GetNextSibling(search),toogle);

}
	
void MainWindow::OnConnection(wxCommandEvent& event)
{

	int id=event.GetId();
	wxTreeItemId item=tree->GetSelection();
	NodeTree *object = item.IsOk() ? (NodeTree *) tree->GetItemData(item)
		:NULL;

	if(id==ID_LNSERVER)
	{
		
		connection->SendData(object);
		if(object->typeConnection==1)tree->SetItemTextColour(item,*wxRED);
		
	}


	
	else if(id==ID_LNCLIENT)
	{
		
		connection->ReceiveData(object);
		if(object->typeConnection==2)tree->SetItemTextColour(item,*wxGREEN);
	}

	else if(id==ID_STCLIENT || id==ID_STSERVER)
	{
		
		connection->Disconnect(object);
		tree->SetItemTextColour(item,*wxBLACK);
	}

}
		
void MainWindow::OnLaserStyle(wxCommandEvent& event)
{
	wxTreeItemId itemId  = tree->GetSelection();
	NodeTree *itemData = itemId .IsOk() ? (NodeTree *)tree->GetItemData(itemId ):NULL;
	
	if(event.GetId()==ID_LASMOD0)
		itemData->pointer.lasersensorsim->setDrawGLMode(0);
	else if(event.GetId()==ID_LASMOD1)
		itemData->pointer.lasersensorsim->setDrawGLMode(1);
	else if(event.GetId()==ID_LASMOD2)
		itemData->pointer.lasersensorsim->setDrawGLMode(2);
	else if(event.GetId()==ID_LASMOD3)
		itemData->pointer.lasersensorsim->setDrawGLMode(3);
	
}
		
	










