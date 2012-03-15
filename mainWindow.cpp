#include "mainWindow.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(ID_NEW, MainWindow::OnNewWorld)
	EVT_MENU(wxID_EXIT, MainWindow::OnQuit)
	EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
	EVT_MENU(ID_LOADWORLD, MainWindow::OnLoadWorld)
	EVT_MENU(ID_LOADMESH, MainWindow::OnLoadMesh)
	EVT_MENU(ID_LOADOBJ, MainWindow::OnLoadObject)
	EVT_MENU(ID_SAVEOBJ, MainWindow::OnSaveObject)
	EVT_MENU(ID_SAVEWORLD, MainWindow::OnSaveWorld)
	EVT_MENU(ID_DELETE, MainWindow::OnDeleteWorld)
	EVT_CLOSE(MainWindow::OnClose)
	EVT_SIZE(MainWindow::OnSize)
	EVT_MENU(ID_VIS_TREE,MainWindow::OnVisibleTree)
	EVT_MENU(ID_ORI, MainWindow::OnChangeLocationCtrl)
	EVT_MENU(ID_POSIT, MainWindow::OnChangeLocationCtrl)
	EVT_MENU(ID_COLOR, MainWindow::OnColor)
	EVT_MENU(ID_BOX, MainWindow::OnDrawBox)
	EVT_MENU(ID_REFER, MainWindow::OnDrawReference)
	EVT_MENU(ID_NAME, MainWindow::OnNameItemTree)
	EVT_MENU(ID_CONVERMESH, MainWindow::OnConverter)
	EVT_MENU(ID_CONVER, MainWindow::OnConverter)
	EVT_MENU(ID_MOVE, MainWindow::OnWheeledBasePanelCtrl)
	EVT_MENU(ID_ROBOT, MainWindow::OnRobotSimPanelCtrl)
	EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnCloseNotebook)
	EVT_UPDATE_UI(ID_LOADOBJ, MainWindow::UpdateUILoadObject)
	EVT_UPDATE_UI(ID_SAVEOBJ,MainWindow::UpdateUISaveObject)
	EVT_UPDATE_UI(ID_SAVEWORLD,MainWindow::UpdateUISaveWorld)

END_EVENT_TABLE()


MainWindow::MainWindow(wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos,const wxSize& size, const long style)
: wxMDIParentFrame(parent, id, title, pos, size, style)
{
	Centre();
	treeVisible=true;

#if wxUSE_STATUSBAR
	CreateStatusBar();
	SetStatusText(wxT("Welcome to APOLO"));
#endif // wxUSE_STATUSBAR

	SetIcon(wxIcon(apolo_xpm));

	CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);
	InitToolBar(GetToolBar());

	//make a Aui Notebook
	note = new wxAuiNotebook(this, wxID_ANY,wxDefaultPosition,wxSize(300,300), wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ACTIVE_TAB);

	tree = new Tree(note, ID_TREE);
	tree->m_mainWin = this;
	m_root = tree->AddRoot(wxT("Universe"), 0, 43, new TreeItemData(wxT("Root item")));
	tree->Parent(m_root);

	note->AddPage(tree, wxT("Universe"));

	SimulatedWorld::tree = tree;
	SimulatedWorld::mainWin = this;
	
}
void MainWindow::OnClose(wxCloseEvent& event)
{
	if ( event.CanVeto())
    {
        wxString msg;
        msg.Printf(wxT("Are you sure close window?"));
        if ( wxMessageBox(msg, _T("Please confirm"),
                          wxICON_QUESTION | wxYES_NO) != wxYES )
        {
            event.Veto();
            return;
        }
    }
	Destroy();
	event.Skip();
}
void MainWindow::InitToolBar(wxToolBar *toolbar)
{
	wxBitmap bitmaps[5];
	bitmaps[0] = wxBitmap (new_xpm);
	bitmaps[1] = wxBitmap (loadWorld_xpm);
	bitmaps[2] = wxBitmap (loadObject_xpm);
	bitmaps[3] = wxBitmap (saveWorld_xpm);
	bitmaps[4] = wxBitmap (saveObject_xpm);

	toolbar->AddTool(ID_NEW, bitmaps[0], wxT("New World"));
	toolbar->AddTool(ID_LOADWORLD, bitmaps[1], wxT("Load World"));
	toolbar->AddTool(ID_SAVEWORLD, bitmaps[3], wxT("Save World")); 
	toolbar->AddTool(ID_LOADOBJ, bitmaps[2], wxT("Load Object"));
	toolbar->AddTool(ID_SAVEOBJ, bitmaps[4], wxT("Save Object"));
	toolbar->Realize();
}
void MainWindow::OnSize(wxSizeEvent& WXUNUSED (event))
{
	int w, h;
	GetClientSize(&w,&h);
	note->SetSize(0,0,230,h);
	GetClientWindow()->SetSize(230,0,w-230,h);
	Refresh(false);
}
void MainWindow::OnCloseNotebook(wxAuiNotebookEvent& event)
{
	wxAuiNotebook* m_note = (wxAuiNotebook *) event.GetEventObject();
	if(m_note->GetSelection()== 0)event.Veto();
}
void MainWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxT("Apolo Simulator\n")
		wxT("Author:Esther LLorente 2010-2011\nHas been used MRCore Library License and wxWindows Library License:\nwxWidgets 2.8.11 (www.wxwidgets.org)\nCopyright (C) 1998-2005 Julian Smart, Robert Roebling et al."),
				 wxT("Information"),wxOK | wxICON_INFORMATION, this);
}
bool MainWindow::checkPanelExist(NodeTree* node)
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
}
void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	 wxString msg;
     msg.Printf(wxT("Are you sure close window?"));
     if ( wxMessageBox(msg, wxT("Please confirm"),wxICON_QUESTION | wxYES_NO) != wxYES ) return;
	Close(true);
}
void MainWindow::OnVisibleTree(wxCommandEvent& WXUNUSED(event))
{
	if(treeVisible)showTree(false);
	else showTree(true);
}
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
		itemData->getSimu()->getChild()->SetTitle(s_text);
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
	if(id == ID_CONVERMESH)
	{
		Converter* conver;
		conver = new Converter(this,ID_CONVERMESH,wxT("CPP CODE"));
		conver->OnlyRead(itemData->pointer.meshpart);
		conver->ShowModal();
		wxLogStatus(wxT("See cpp code"));
		
	}
	if(id == ID_CONVER)
	{
		Converter* conver;
		conver = new Converter(this,ID_CONVER,wxT("Converter .stl"));
		conver->ShowModal();
		wxLogStatus(wxT("Converter"));
	}

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
		else if(id == ID_POSIT)locationCtrl = new ChangeLocationCtrl(this, ID_POSIT, wxT("Change Position"));
		locationCtrl->setItemData(itemData);
		locationCtrl->ShowModal();	
		wxLogStatus(wxT("Change Location"));
	}
	event.Skip();
}
void MainWindow::OnWheeledBasePanelCtrl(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;

	if(itemData->pointer.wheeledbasesim)
	{
		if(checkPanelExist(itemData))
		{
			wxString textOutside;
			textOutside<< itemData->getSimu()->getChild()->GetTitle()<< wxT(" Move");
			WheeledBasePanel* wheeledBaseCtrl;
			wheeledBaseCtrl = new WheeledBasePanel(this,wxID_ANY,itemData);
			wheeledBaseCtrl->getTitle()->SetLabel((wxString)itemData->getNameTree());
			note->AddPage(wheeledBaseCtrl,textOutside);
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
		if(checkPanelExist(itemData))
		{
			wxString textOutside;
			textOutside<< itemData->getSimu()->getChild()->GetTitle()<< wxT(" Joints");
			RobotSimPanel* robotSimCtrl;
			robotSimCtrl = new RobotSimPanel(this,wxID_ANY,itemData);
			robotSimCtrl->getTitle()->SetLabel((wxString)itemData->getNameTree());
			note->AddPage(robotSimCtrl,textOutside);
			wxLogStatus(wxT("Robot Sim Panel"));
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
void MainWindow::OnColor(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	NodeTree *itemData = itemId.IsOk() ? (NodeTree *) tree->GetItemData(itemId):NULL;
	if(itemData->pointer.solidentity)
	{
		double _r,_g,_b;
		itemData->pointer.solidentity->getColor(_r,_g,_b);
		wxColour c = wxColour(_r*255,_g*255,_b*255);
		wxColour color = wxGetColourFromUser(0,c);
		if(color.IsOk())
		{
			double r = color.Red();
			double g = color.Green();
			double b = color.Blue();
			itemData->pointer.solidentity->setColor(r/255,g/255,b/255);
			for(int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();
		}
		for(int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();		
	}
}
void MainWindow::OnNewWorld(wxCommandEvent& WXUNUSED(event))
{
	World *w = new World;
	simuWorld = new SimulatedWorld(w);
	listWorlds.push_back(simuWorld);
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
}
void MainWindow::OnLoadMesh(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId itemId = tree->GetSelection();
	
	for(int i= 0; i<listWorlds.size(); i++)
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
				tree->AddNode(mesh, listWorlds[i]->getTreeItem());
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
	for(int i=0; i<listWorlds.size(); i++)
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
					tree->AddNode(p_obj, listWorlds[i]->getTreeItem());
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
	
	for(int i= 0; i<listWorlds.size(); i++)
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
void MainWindow::OnShowCanvas()
{
	wxTreeItemId itemId = tree->GetSelection();
	for(int i= 0; i<listWorlds.size(); i++)
	{
		if(listWorlds[i]->getTreeItem()==itemId)
		{
			listWorlds[i]->getChild()->Restore();
			listWorlds[i]->getChild()->Raise();
			listWorlds[i]->getChild()->Update();
			listWorlds[i]->getChild()->SetFocus();
		}
		else listWorlds[i]->getChild()->Lower();
	}
}
void MainWindow::showTree(bool sh)
{
	treeVisible=sh;
	int w, h;
	GetClientSize(&w,&h);
	if(treeVisible)
	{
		note->Show(true);
		note->SetSize(0,0,230,h);
		GetClientWindow()->SetSize(230,0,w-230,h);
		Refresh(false);
		wxLogStatus(wxT("Visible tree"));
	}
	else
	{
		note->Show(false);
		GetClientWindow()->SetSize(0,0,w,h);					
		Refresh(false);
		wxLogStatus(wxT("Unvisible tree"));
	}
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
		for(int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();	
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
		for(int i= 0; i<listWorlds.size(); i++)listWorlds[i]->getChild()->UpdateWorld();
	}	
}







