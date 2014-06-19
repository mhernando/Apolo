#include "mainWindow.h"
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/msgdlg.h>
#include <wx/wfstream.h>





DEFINE_EVENT_TYPE(wxEVT_EDIT_CLOSED)

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
	EVT_MENU(ID_LOADWORLDXML, MainWindow::OnLoadWorldXML)
	EVT_MENU(ID_LOADOBJXML, MainWindow::OnLoadObjectXML)
	EVT_MENU(ID_SAVEOBJXML, MainWindow::OnSaveObjectXML)
	EVT_MENU(ID_SAVEWORLDXML, MainWindow::OnSaveWorldXML)
	EVT_MENU(ID_SHOWEDITXML, MainWindow::showXMLEditor)
	EVT_COMMAND(wxID_ANY,wxEVT_EDIT_CLOSED,MainWindow::CloseEditConsole)
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
	EVT_MENU(ID_CHANGEFORM, MainWindow::OnChangeForm)
	EVT_MENU(ID_MODIFYFACE, MainWindow::OnChangeForm)
	EVT_BUTTON(ID_SELECTEDFACE, MainWindow::OnChangeForm)
	EVT_BUTTON(ID_CANCELSELECTION, MainWindow::OnChangeForm)
	EVT_BUTTON(ID_ADDOWNFACE, MainWindow::OnChangeForm)
	EVT_BUTTON(ID_CANCELDESIGN, MainWindow::OnChangeForm)
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
	EVT_MENU(ID_ROBOTGOTO, MainWindow::OnRobotSimGoTo)
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
	EVT_MENU(ID_LINKTO,MainWindow::OnLinkTo)
	EVT_MENU(ID_UNLINK,MainWindow::OnLinkTo)
	EVT_MENU(ID_SHOWLINKS,MainWindow::OnLinkTo)
	EVT_MENU(ID_TREESTRUCTURE, MainWindow::UpdateTreeStructure)
	EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnCloseNotebook)
	EVT_UPDATE_UI(ID_LOADOBJ, MainWindow::UpdateUILoadObject)
	EVT_UPDATE_UI(ID_SAVEOBJ,MainWindow::UpdateUISaveObject)
	EVT_UPDATE_UI(ID_SAVEWORLD,MainWindow::UpdateUISaveWorld)
	EVT_UPDATE_UI(ID_LOADOBJXML, MainWindow::UpdateUILoadObjectXML)
	EVT_UPDATE_UI(ID_SAVEOBJXML,MainWindow::UpdateUISaveObjectXML)
	EVT_UPDATE_UI(ID_SAVEWORLDXML,MainWindow::UpdateUISaveWorldXML)
	EVT_SASH_DRAGGED(ID_DRAG, MainWindow::OnSashDrag)
	EVT_BUTTON(ID_COPYDESIGN, MainWindow::CopyPasteDesign)
	EVT_BUTTON(ID_PASTEDESIGN, MainWindow::CopyPasteDesign)
	EVT_MENU(ID_COPYDESIGN, MainWindow::CopyPasteDesign)
	EVT_MENU(ID_PASTEDESIGN, MainWindow::CopyPasteDesign)
END_EVENT_TABLE()




MainWindow::MainWindow(wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos,const wxSize& size, const long style)
: wxMDIParentFrame(parent, id, title, pos, size, style),note(0)
{
	Centre();
	treeVisible=true;
	xmlEditorVisible=false;
	editionVisible=false;
	slider=true;
	popmenu=true;
	design_slider=true;
	state=0;
	managewindow=new ManageWindows();
	treeStruc=false;

	

#if wxUSE_STATUSBAR
	CreateStatusBar();
	SetStatusText(wxT("Welcome to APOLO"));
#endif // wxUSE_STATUSBAR


	CreateMenuBar();
	SetIcon(wxIcon(apolo_xpm));
	GetClientSize(&w,&h);
	CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);
	InitToolBar(GetToolBar());

	
	wxSashLayoutWindow* aux=new wxSashLayoutWindow(this,wxID_ANY,wxDefaultPosition, wxDefaultSize,wxSW_NOBORDER);
	aux->SetDefaultSize(wxSize(25,35));
	aux->SetOrientation(wxLAYOUT_HORIZONTAL);
	aux->SetAlignment(wxLAYOUT_TOP);
	aux->SetSashVisible(wxSASH_TOP, false);

	s=new wxSashLayoutWindow(this, ID_DRAG,wxDefaultPosition, wxDefaultSize, wxSW_3D);
	s->SetDefaultSize(wxSize(w/4, h));
	s->SetOrientation(wxLAYOUT_VERTICAL);
	s->SetAlignment(wxLAYOUT_LEFT);
	s->SetSashVisible(wxSASH_RIGHT, true); 

	treeToolbar=new wxToolBar();
	treeToolbar->Create(aux,wxID_ANY,wxDefaultPosition,wxSize(100,30),wxDOUBLE_BORDER,wxT("Views"));
	wxBitmap bitmaps[2];
	bitmaps[0]=wxBitmap(Links_xpm);
	bitmaps[1]=wxBitmap(treeStructure_xpm);
	wxStaticText *text = new wxStaticText(treeToolbar, wxID_ANY, wxT("Tree view")); 
	treeToolbar->AddSeparator();
	treeToolbar->AddControl(text); 
	treeToolbar->AddSeparator();
	treeToolbar->AddCheckTool(ID_SHOWLINKS,wxT("Show Links"),bitmaps[0],wxNullBitmap);
	treeToolbar->AddCheckTool(ID_TREESTRUCTURE,wxT("Tree structure"),bitmaps[1],wxNullBitmap);
	treeToolbar->AddSeparator();
	treeToolbar->SetBackgroundColour(wxColour(200,230,255));
	treeToolbar->Realize();
	
	
	//make a Aui Notebook
	note = new wxAuiNotebook(s, wxID_ANY,wxDefaultPosition,wxDefaultSize, wxAUI_NB_TOP  | wxAUI_NB_TAB_SPLIT | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_TAB_EXTERNAL_MOVE);
	tree = new Tree(note, ID_TREE);
	CreatePanelXML();
	tree->m_mainWin = this;	
	tree->XMLPanel=NULL;
	m_root = tree->AddRoot(wxT("Universe"), 0, 47, new TreeItemData(wxT("Root item")));
	tree->Parent(m_root);

	note->AddPage(tree, wxT("Universe"));
    SetAutoLayout(true);
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


	wxMenuItem *itemXML0 = new wxMenuItem(menuFile,ID_LOADWORLDXML, wxT("Load world XML"), wxT("Load a file XML of world"));
	itemXML0->SetBitmap(loadworldxml_xpm);
	menuFile->Append(itemXML0);

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
	//menuFile2->AppendSeparator();

	wxMenuItem *c_itemXML1 = new wxMenuItem(menuFile2,ID_LOADWORLDXML, wxT("Load world XML"), wxT("Load a file XML of world"));
	c_itemXML1->SetBitmap(loadworldxml_xpm);
	menuFile2->Append(c_itemXML1);
	wxMenuItem *c_itemXML2 = new wxMenuItem(menuFile2,ID_LOADOBJXML, wxT("Load Object XML"), wxT("Add object to current World select"));
	c_itemXML2->SetBitmap(loadobjectxml_xpm);
	menuFile2->Append(c_itemXML2);
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

	wxMenuItem *c_itemXML3 = new wxMenuItem(menuFile2,ID_SAVEWORLDXML, wxT("Save World XML"),wxT("Save world select"));
	c_itemXML3->SetBitmap(saveworldxml_xpm);
	menuFile2->Append(c_itemXML3);
	wxMenuItem *c_itemXML4 = new wxMenuItem(menuFile2,ID_SAVEOBJXML, wxT("Save Object XML"), wxT("Save object select"));
	c_itemXML4->SetBitmap(saveobjectxml_xpm);
	menuFile2->Append(c_itemXML4);


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
	wxBitmap bitmaps[14];
	bitmaps[0] = wxBitmap (NewWorld_xpm);
	bitmaps[1] = wxBitmap (mainLWorld_xpm);
	bitmaps[2] = wxBitmap (loadObject_xpm);
	bitmaps[3] = wxBitmap (saveWorld_xpm);
	bitmaps[4] = wxBitmap (saveObject_xpm);
	bitmaps[5] = wxBitmap (box_xpm);
	bitmaps[6] = wxBitmap (axis_xpm);
	bitmaps[7] = wxBitmap (mainLxml_xpm);
	bitmaps[8] = wxBitmap (loadobjectxml_xpm);
	bitmaps[9] = wxBitmap (saveworldxml_xpm);
	bitmaps[10] = wxBitmap (saveobjectxml_xpm);
	bitmaps[11]= wxBitmap (xml_xpm);
	bitmaps[12]=wxBitmap(Links_xpm);
	bitmaps[13]=wxBitmap(treeStructure_xpm);
	toolbar->AddTool(ID_NEW, bitmaps[0], wxT("New World"));
	toolbar->AddTool(ID_LOADWORLD, bitmaps[1], wxT("Load World"));
	toolbar->AddTool(ID_LOADWORLDXML, bitmaps[7], wxT("Load World XML"));
	toolbar->AddSeparator();
	toolbar->AddCheckTool(ID_DRAWBOX,wxT("Draw Box"),bitmaps[5],wxNullBitmap,wxT("Show Item Selected"));
	toolbar->AddCheckTool(ID_COMPRS,wxT("Composed Reference System"),bitmaps[6],wxNullBitmap,wxT("Show Reference System of main objects and composed objects"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_SHOWEDITXML, wxT("View XML editor"), bitmaps[11],wxNullBitmap);
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

		wxT("Authors: \nMiguel Hernando Gutierrez 2010-2012\nFrancisco Ramirez de Anton Montoro 2012-2013\nCarlos Mateo Benito 2011-2012\nEsther LLorente Garcia 2010-2011\nHas been used MRCore Library License and wxWindows Library License:\nwxWidgets 2.9.3 (www.wxwidgets.org)\nCopyright (C) 1998-2005 Julian Smart, Robert Roebling et al."),

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
			int rb=0,smp=0,wh=0,rbgoto=0;
			vector <RobotSimPanel*> robot=managewindow->getVectorRobotSimPanel();
			vector <WheeledBasePanel*> wheeled=managewindow->getVectorWheeledBasePanel();
			vector <RobotSimGoTo*> robotsimgoto=managewindow->getVectorRobotSimGoTo();

			vector<int> sizes;
			sizes.push_back((int)robot.size());
			sizes.push_back((int)wheeled.size());
			sizes.push_back((int)robotsimgoto.size());
			


			wxTreeItemId itemid=tree->GetWorld(tree->GetSelection());

			wxTreeItemId itemId = tree->GetSelection();
			NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;

			for(unsigned int i=0;i<listWorlds.size();i++)
				if(listWorlds[i]->getTreeItem()==itemid)//algoritmo de fácil implementación
				{			
					int size=0;		
						
					for (int index=0;index<(int)sizes.size();index++) //Cogemos el de mayor tamaño para que no haya problemas con los vectores	
					{
						if (size<sizes[index])size=sizes[index];
					}

					for (int j=size-1;j>=0;j--) //Vamos de atrás a adelante para no dejarnos nada por eliminar
					{	
						
						rbgoto=rb=smp=wh=0; //Actualizamos los indicadores de tipo cada vuelta en el bucle
					
						if ((robot.empty()==false) && (j<(int)robot.size())) //Nos aseguramos de que no esté vacío el vector y el índice no sea mayor que su tamaño
							if (robot[j]->getItemNode ()== itemData)
								rb=1;				//ponemos a 1 si es de tipo robotsimpanel
							else if (robot[j]->getItemParentData()==itemData)
								smp=1;				//si es de tìpo simplejoint
						
						if ((robotsimgoto.empty()==false) && (j<robotsimgoto.size()))
							if (robotsimgoto[j]->getItemNode ()==itemData)
								rbgoto=1;

						if ((wheeled.empty()==false) && (j<wheeled.size()))
							if (wheeled[j]->getItemNode ()==itemData)
								wh=1;
								
						if (rb || smp) //Si los menus son de tipo simplejoint o robotsim
							robot[j]->Delete(); //Ya localizados los menús pertenecientes al padre los eliminamos todos

						else if(wh) //Si los menus son de tipo WheeledBasePanel
							wheeled[j]->Delete();
						
						else if (rbgoto)//Si los menus son de tipo RobotSimGoTo
							robotsimgoto[j]->Delete();
						
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



void MainWindow::OnChangeForm(wxCommandEvent& event)
{
	int id=event.GetId();
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	
	if ((id==ID_MODIFYFACE)&&(itemData->pointer.facesetpart))
	{
		typ=1;
		vsele=new FaceSelection(this,wxID_ANY,wxString("FaceSetPart"),itemData->pointer.facesetpart);
		vsele->Show(true);
		editionVisible=true;
		vsele->MakeModal(true);
		return;
	}

	if((id==ID_CHANGEFORM)&&(itemData->pointer.prismaticpart))
	{
		typ=2;
		view=new globalView(this,wxID_ANY,wxT("Edit Prism"));
		view->Show(true);
		editionVisible=true;
		view->MakeModal(true);
		view->LoadFace(itemData->pointer.prismaticpart->getPolygonalBase());
		return;
	}

	if(id==ID_SELECTEDFACE)
	{
		vsele->Show(false);
		vsele->MakeModal(false);
		view=new globalView(this,wxID_ANY,wxT("Edit"));
		view->Show(true);
		editionVisible=true;
		view->MakeModal(true);
		view->LoadFace(*(vsele->getFaceSelected()));
	}

	if(id==ID_CANCELSELECTION)
	{
		vsele->Show(false);
		editionVisible=false;
		vsele->MakeModal(false);
	}

	if (id==ID_CANCELDESIGN)
	{
		view->Show(false);
		editionVisible=false;
		view->MakeModal(false);
	}


	if ((id==ID_ADDOWNFACE)&&(typ==1))
	{
		itemData->pointer.facesetpart->ModifyFace(vsele->numSelected(),*(view->GetFace()));
		view->Show(false);
		view->MakeModal(false);
		editionVisible=false;
	}


	if ((id==ID_ADDOWNFACE)&&(typ==2))
	{
		itemData->pointer.prismaticpart->setPolygonalBase(*(view->GetFace()));
		view->Show(false);
		view->MakeModal(false);
		editionVisible=false;
		id=NULL;
	}
	event.Skip();
}


void MainWindow::CloseEditConsole(wxCommandEvent &event)
{
	editionVisible=false;
	view->MakeModal(false);
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

void MainWindow::OnRobotSimGoTo(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	if(itemData->pointer.robotsim)
	{
		if(managewindow->CheckWindowsExist(itemData))
		{
			RobotSimGoTo* robotSimGoTarget;
			robotSimGoTarget = new RobotSimGoTo(this,wxID_ANY,wxT("GO TO TARGET"),itemData);
			robotSimGoTarget->getTitle()->SetLabel(itemData->getNameTree());
			robotSimGoTarget->setManageWindow(managewindow);
			robotSimGoTarget->Show(true);	
			wxLogStatus(wxT("Robot Sim Go To Target"));
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

void MainWindow::OnLoadWorldXML(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog openFile(this, wxT("Load world file XML"), wxEmptyString, wxEmptyString,
			wxT("XML files (*.xml)|*.xml"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	
	if(openFile.ShowModal() == wxID_OK)
	{
		wxString file = openFile.GetPath();
		char filec[500];
		strcpy(filec, (const char*)file.mb_str(wxConvUTF8));

		XMLfile xml_file(filec);
		Object *test1 = xml_file.load(filec);
		World *test2 = dynamic_cast<World *>(test1);				
		//xml_file.save();

	
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


void MainWindow::OnLoadObjectXML(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	for(unsigned int i=0; i<listWorlds.size(); i++)
	{
		if(listWorlds[i]->getTreeItem()==itemId)
		{
			wxFileDialog openFile(this,wxT("Load object file XML"), wxEmptyString, wxEmptyString,
            wxT("XML files (*.xml)|*.xml"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);
			if(openFile.ShowModal() == wxID_OK)
			{
				wxString fileName = openFile.GetPath();
				char c_file[100];
				strcpy(c_file,(const char*)fileName.mb_str(wxConvUTF8));
				
				XMLfile xml_file(c_file);
				Object *obj = xml_file.load(c_file);
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
void MainWindow::OnSaveWorldXML(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	
	for(unsigned int i= 0; i<listWorlds.size(); i++)
	{
		if(listWorlds[i]->getTreeItem()==itemId)
		{
			wxFileDialog saveFile(this,wxT("Save World file XML "),wxEmptyString,wxEmptyString,
				wxT("XML files (*.xml)|*.xml"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
			if(saveFile.ShowModal() == wxID_OK)
			{
				wxString file = saveFile.GetPath();
				char filec[100];
				strcpy(filec, (const char*) file.mb_str(wxConvUTF8));
				XMLfile xml_file(filec);
				xml_file.write(listWorlds[i]->getWorld());
				xml_file.save();
				wxLogMessage(wxT("Successfully save in %s"), saveFile.GetPath().c_str());
			}

		}
		else wxLogMessage(wxT("Please, select a World."));
	}
}
void MainWindow::OnSaveObjectXML(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree* itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;

	if(itemData->menus.menu_positionable && m_root!=itemId)
	{
		wxFileDialog saveFile(this,wxT("Save Object file XML"), wxEmptyString, wxEmptyString,
        wxT("XML files (*.xml)|*.xml"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if(saveFile.ShowModal() == wxID_OK)
		{
			wxString fileName = saveFile.GetPath();
			char c_file[100];
			strcpy(c_file,(const char*)fileName.mb_str(wxConvUTF8));

			XMLfile xml_file(c_file);			
			itemData->pointer.positionableentity->setRelativeT3D(0);
			xml_file.write(itemData->pointer.positionableentity);
			xml_file.save();

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

void MainWindow::UpdateUILoadObjectXML(wxUpdateUIEvent& event)
{
	event.Enable(listWorlds.size()!=0);
}
void MainWindow::UpdateUISaveObjectXML(wxUpdateUIEvent& event)
{
	event.Enable(listWorlds.size()!=0);
}
void MainWindow::UpdateUISaveWorldXML(wxUpdateUIEvent& event)
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
		

void MainWindow::OnLinkTo(wxCommandEvent& event)
{
	if(listWorlds.size()>0)
	{
		int id=event.GetId();
		wxTreeItemId itemId = tree->GetSelection();
		NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
		

		if(itemData->pointer.positionableentity)
		{
			simuWorld=itemData->getSimu();
			if (id==ID_LINKTO)
			{
				if(simuWorld->CheckItemLinked(itemData->pointer.positionableentity))
				{
					return;
				}
				state=1;
				wxSetCursor(wxCURSOR_POINT_LEFT);
				simuWorld->SetEntityToLink(itemData->pointer.positionableentity);
				simuWorld->SetIdToLink(itemId);
				wxLogStatus(wxT("Select Item(Link to)"));
				id=NULL;
			}
			if (id==ID_UNLINK)
			{
				if (showLinks==true) tree->EraseMarks();
				if(treeStruc==true) 
				{
					tree->UnlinkNode(simuWorld,itemData->pointer.positionableentity);
				}
				simuWorld->EraseLinked(itemData->pointer.positionableentity,itemId);
				itemData->pointer.positionableentity->LinkTo(NULL); //Deslinkar
				if(treeStruc==true) 
				{
					simuWorld->getTree()->showTreeStructure(simuWorld,true);
				}
				wxLogStatus(wxT("Unlink done"));
				state=0;
			}
		}


		if (id==ID_SHOWLINKS)
		{
			showLinks=treeToolbar->GetToolState(id);
			if(showLinks) 
			{
				tree->setShowLinks(true);
				treeStruc=false;
				treeToolbar->EnableTool(ID_TREESTRUCTURE,false);
			}
			else
			{
				tree->setShowLinks(false);
				treeStruc=true;
				treeToolbar->EnableTool(ID_TREESTRUCTURE,true);
			}
		}
	}	
}



void MainWindow::UpdateTreeStructure(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_TREESTRUCTURE)
	{
		treeStruc=treeToolbar->GetToolState(id);
		if(treeStruc)
		{
			treeToolbar->EnableTool(ID_SHOWLINKS,false);
			if(listWorlds.size()>0)
			{
				for(int i=0;i<listWorlds.size();i++)
				{
					listWorlds[i]->getTree()->showTreeStructure(listWorlds[i],true);
				}
			}
			return;
		}
		else
		{
			treeToolbar->EnableTool(ID_SHOWLINKS,true);
			if(listWorlds.size()>0)
			{
				for(int i=0;i<listWorlds.size();i++)
				{
					listWorlds[i]->getTree()->showTreeStructure(listWorlds[i],false);
				}
			}
			return;
		}
	}
}



void MainWindow::CopyPasteDesign(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_COPYDESIGN)  //Para copiar el diseño que estemos realizando si abrimos la edición desde la mainwindow
	{
		simuWorld->CleanClipboard();
		if (view->GetScreen2D()->GetVector().size()==0) return;
	
		for(int i=0;i<view->GetScreen2D()->GetVector().size();i++)
		{
			simuWorld->SetCopiedDesign(view->GetScreen2D()->GetVector());
		}
	}

	if (id==ID_PASTEDESIGN)
	{
		if(simuWorld->GetCopiedDesign().size()==0) return;
		view->GetScreen2D()->clearAuxPoints();
		for(int i=0;i<simuWorld->GetCopiedDesign().size();i++)
		{
			view->GetScreen2D()->SetPaste(true);
			view->GetScreen2D()->SetAuxPoints(simuWorld->GetCopiedDesign()[i].x,simuWorld->GetCopiedDesign()[i].y);
			view->GetScreen2D()->DrawScene2D();
		}
	}
}


void MainWindow::CreatePanelXML()
{
	edit=new PanelXML(this,wxID_ANY,wxT("XML Edition"),tree);
}


void MainWindow::showXMLEditor(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_SHOWEDITXML)
	{
		edit->ShowPanel();
	}
}








