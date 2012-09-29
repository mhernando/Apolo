#include "childView.h"
#include "mainWindow.h"


BEGIN_EVENT_TABLE(ChildView, wxMDIChildFrame)
	EVT_MENU(ID_HIDE, ChildView::OnHideChild)
	EVT_CLOSE(ChildView::OnClose)
	EVT_TIMER(ID_TIMER, ChildView::OnTimer)
	EVT_MENU(ID_SPLITHF, ChildView::SplitHorizontalFirst)
	EVT_MENU(ID_SPLITHS, ChildView::SplitHorizontalSecond)
	EVT_MENU(ID_SPLITVF, ChildView::SplitVerticalFirst)
	EVT_MENU(ID_SPLITVS, ChildView::SplitVerticalSecond)
	EVT_MENU(ID_UNSPLITF, ChildView::UnSplitFirst)
	EVT_MENU(ID_UNSPLITS, ChildView::UnSplitSecond)
	EVT_MENU(ID_PLAY, ChildView::OnSimulator)
	EVT_MENU(ID_STOP2, ChildView::OnSimulator)
	EVT_UPDATE_UI(ID_SPLITHF, ChildView::UpdateUIHorizontalFirst)
	EVT_UPDATE_UI(ID_SPLITHS, ChildView::UpdateUIHorizontalSecond)
	EVT_UPDATE_UI(ID_SPLITVF, ChildView::UpdateUIVerticalFirst)
	EVT_UPDATE_UI(ID_SPLITVS, ChildView::UpdateUIVerticalSecond)
	EVT_UPDATE_UI(ID_UNSPLITF, ChildView::UpdateUIUnsplitFirst)
	EVT_UPDATE_UI(ID_UNSPLITS, ChildView::UpdateUIUnsplitSecond)
	EVT_UPDATE_UI(ID_PLAY, ChildView::UpdateUIPlay)
	EVT_UPDATE_UI(ID_STOP2, ChildView::UpdateUIStop)

END_EVENT_TABLE()


ChildView::ChildView(wxMDIParentFrame *parent, const wxString& title, World *w)
: wxMDIChildFrame(parent, wxID_ANY, title, wxPoint(50,50), wxSize(500,400), wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE),m_timer(this,ID_TIMER)
{
	mainWin = (MainWindow*)parent;
	m_splitter = (wxSplitterWindow *) NULL;
	toolbar = 0;
	m_world = w;
	playsimu=false;
	//call timer
	m_timer.Start(100);

	//make a toolbar
	CreateToolBar( wxNO_BORDER | wxTB_FLAT |wxTB_HORIZONTAL, wxID_ANY, wxT(""));
	toolbar = GetToolBar();
	if(toolbar)InitToolBar(toolbar);

	//make a other menu
	filemenu = new wxMenu;
	wxMenuItem *c_item1 = new wxMenuItem(filemenu, ID_NEW, wxT("New world"),wxT("Create a new world"));
	c_item1->SetBitmap(new_xpm);
	filemenu->Append(c_item1);
	wxMenuItem *c_item2 = new wxMenuItem(filemenu,ID_LOADWORLD, wxT("Load world"), wxT("Load a file of world"));
	c_item2->SetBitmap(loadWorld_xpm);
	filemenu->Append(c_item2);
	wxMenuItem *c_item3 = new wxMenuItem(filemenu,ID_LOADOBJ, wxT("Load Object"), wxT("Add object to current World select"));
	c_item3->SetBitmap(loadObject_xpm);
	filemenu->Append(c_item3);
	filemenu->AppendSeparator();
	filemenu->Append(ID_LOADMESH, wxT("Import .stl"), wxT("Import .stl file"));
	filemenu->AppendSeparator();
	filemenu->Append(ID_CONVER, wxT("Converter .stl"), wxT("Converter .stl file"));
	filemenu->AppendSeparator();
	wxMenuItem *c_item4 = new wxMenuItem(filemenu,ID_SAVEWORLD, wxT("Save World"),wxT("Save world select"));
	c_item4->SetBitmap(saveWorld_xpm);
	filemenu->Append(c_item4);
	wxMenuItem *c_item5 = new wxMenuItem(filemenu,ID_SAVEOBJ, wxT("Save Object"), wxT("Save object select"));
	c_item5->SetBitmap(saveObject_xpm);
	filemenu->Append(c_item5);
	filemenu->Append(ID_DELETE, wxT("Delete world"),wxT("Delete world select"));
	filemenu->AppendSeparator();
	filemenu->AppendCheckItem(ID_VIS_TREE,wxT("Unvisible Tree"));
	filemenu->AppendCheckItem(ID_VIS_CONNLOG, wxT("Unvisible ConnectionLog"));
	filemenu->AppendSeparator();
	filemenu->Append(wxID_EXIT, wxT("Close all"), wxT("Quit the program"));

	filemenu2 = new wxMenu;
	wxMenuItem *item1 = new wxMenuItem(filemenu2, ID_SPLITHF, wxT("Double horizontal"));
	item1->SetBitmap(V1_xpm);
	wxMenuItem *item2 = new wxMenuItem(filemenu2, ID_SPLITVF, wxT("Double vertical"));
	item2->SetBitmap(V2_xpm);
	wxMenuItem *item3 = new wxMenuItem(filemenu2, ID_SPLITHS, wxT("Triple horizontal"));
	item3->SetBitmap(V4_xpm);
	wxMenuItem *item4 = new wxMenuItem(filemenu2, ID_SPLITVS, wxT("Triple vertical"));
	item4->SetBitmap(V3_xpm);
	wxMenuItem *item5 = new wxMenuItem(filemenu2, ID_UNSPLITF, wxT("Simple"));
	item5->SetBitmap(V5_xpm);
	filemenu2->Append(item1);
	filemenu2->Append(item2);
	filemenu2->Append(item3);
	filemenu2->Append(item4);
	filemenu2->Append(item5);
	filemenu2->Append(ID_UNSPLITS, wxT("Double"));
	filemenu2->Append(ID_HIDE, wxT("Hide World"));

	filemenu3 = new wxMenu;
	wxMenuItem *Iplay = new wxMenuItem(filemenu3, ID_PLAY, wxT("Play Simulator"));
	Iplay->SetBitmap(play_xpm);
	wxMenuItem *Istop = new wxMenuItem(filemenu3, ID_STOP2, wxT("Stop Simulator"));
	Istop->SetBitmap(stop2_xpm);
	filemenu3->Append(Iplay);
	filemenu3->Append(Istop);
	ReplaceMenu();
	CreateSubWindows();
	

}
void ChildView::InitToolBar(wxToolBar* toolbar)
{
	int a=0, b=0;
	wxBitmap bitmaps[7];
	bitmaps[0] = wxBitmap ( V1_xpm);
	bitmaps[1] = wxBitmap ( V2_xpm);
	bitmaps[2] = wxBitmap ( V3_xpm);
	bitmaps[3] = wxBitmap ( V4_xpm);
	bitmaps[4] = wxBitmap ( V5_xpm);
	bitmaps[5] = wxBitmap ( play_xpm);
	bitmaps[6] = wxBitmap ( stop2_xpm);


	toolbar->AddTool(ID_UNSPLITF, bitmaps[4], wxT("view unsplit simple"));
	toolbar->AddTool(ID_SPLITHF, bitmaps[0], wxT("view horizontal simple"));
	toolbar->AddTool(ID_SPLITVF, bitmaps[1], wxT("view vertical simple"));
	toolbar->AddTool(ID_SPLITHS, bitmaps[3], wxT("view horizontal complex"));
	toolbar->AddTool(ID_SPLITVS, bitmaps[2], wxT("view vertical complex"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_PLAY, bitmaps[5], wxT("PLAY"));
	toolbar->AddTool(ID_STOP2, bitmaps[6], wxT("STOP"));
	toolbar->AddSeparator();
	toolbar->Realize();
	
}

void ChildView::ReplaceMenu()
{
	
	menubar=mainWin->GetMenuBar();
	
	menubar->Replace(1,filemenu2,wxT("Views"));
	menubar->Replace(2,filemenu3,wxT("Run"));
	if(menubar->GetMenuCount()==3)
	{
		menubar->Replace(0,filemenu,wxT("&File"));
		menubar->Append(mainWin->getMenuAbout(),wxT("About"));
		menubar->Append(mainWin->getMenuSettings(), wxT("Settings") );
	}
	
}

void ChildView::UpdateWorld()
{
	canvas1->UpdateWorld(m_world);
	canvas2->UpdateWorld(m_world);
	canvas3->UpdateWorld(m_world);

}
void ChildView::RefreshChild()
{
	canvas1->Refresh(false);
	canvas2->Refresh(false);
	canvas3->Refresh(false);	
}
void ChildView::CreateSubWindows()
{
	int width, height;
	GetClientSize(&width, &height);

	//make a splitter
	m_splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize/*wxSize(width,height)*/);
	m_splitterSub = new wxSplitterWindow(m_splitter, wxID_ANY, wxDefaultPosition, wxSize(100, 100));
	m_splitter->SetMinimumPaneSize(50);
	m_splitterSub->SetMinimumPaneSize(50);

	//make a canvas
	canvas1 = new Canvas(m_splitter,wxID_ANY, wxDefaultPosition, wxSize(100,100));
	canvas2 = new Canvas(m_splitterSub,wxID_ANY, wxDefaultPosition, wxSize(100,100));
	canvas3 = new Canvas(m_splitterSub,wxID_ANY, wxDefaultPosition, wxSize(100,100));

	m_splitter->Initialize(canvas1);
	m_splitterSub->Show(false);
	wxLogStatus(wxT("created a new window"));
	Refresh(false);
}
void ChildView::OnClose(wxCloseEvent& event)
{
	this->Show(false);
}
void ChildView::OnTimer(wxTimerEvent &event)
{
	if(playsimu)
	{
		m_world->simulate(0.1);
		RefreshChild();
		wxLogStatus(wxT("Simulate World"));
	}
	else return;
}
void ChildView::OnHideChild(wxCommandEvent& WXUNUSED(event))
{
	this->Show(false);
}
void ChildView::OnSimulator(wxCommandEvent &event)
{
	int id = event.GetId();
	if(id == ID_PLAY)SetPlaySimu(true);
	if(id == ID_STOP2)SetPlaySimu(false);
}
void ChildView::SplitHorizontalFirst(wxCommandEvent& WXUNUSED(event))
{
	m_splitterSub->Show(false);

	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	m_splitter->SplitHorizontally(canvas1, m_splitterSub);
	m_splitterSub->SplitVertically(canvas2, canvas3);
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();

}
void ChildView::SplitHorizontalSecond(wxCommandEvent& WXUNUSED(event))
{
	
	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();
	m_splitter->Show(true);
	m_splitterSub->Show(true);
	m_splitter->SplitHorizontally(canvas1, m_splitterSub);
	m_splitterSub->SplitVertically(canvas2, canvas3);
	
}
void ChildView::SplitVerticalFirst(wxCommandEvent& WXUNUSED(event))
{
	m_splitterSub->Show(false);

	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	m_splitter->SplitVertically(canvas1, m_splitterSub);
	m_splitterSub->SplitHorizontally(canvas2, canvas3);
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();

}
void ChildView::SplitVerticalSecond(wxCommandEvent& WXUNUSED(event))
{
	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();
	m_splitterSub->Show(true);
	m_splitter->SplitVertically(canvas1, m_splitterSub);
	m_splitterSub->SplitHorizontally(canvas2, canvas3);
}
void ChildView::UnSplitFirst(wxCommandEvent& WXUNUSED(event))
{
	m_splitterSub->Show(false);
	if( m_splitter->IsSplit()) m_splitter->Unsplit();
}
void ChildView::UnSplitSecond(wxCommandEvent& WXUNUSED(event))
{
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();
}
void ChildView::UpdateUIHorizontalFirst(wxUpdateUIEvent& event)
{
	event.Enable(( (!m_splitter->IsSplit()) || (m_splitterSub->IsSplit()) || (m_splitter->GetSplitMode() != wxSPLIT_HORIZONTAL)));
}
void ChildView::UpdateUIHorizontalSecond(wxUpdateUIEvent& event)
{
	event.Enable(( (!m_splitterSub->IsSplit()) || (m_splitterSub->GetSplitMode() != wxSPLIT_VERTICAL)));
}
void ChildView::UpdateUIVerticalFirst(wxUpdateUIEvent& event)
{
	event.Enable(( (!m_splitter->IsSplit()) || (m_splitterSub->IsSplit()) || (m_splitter->GetSplitMode() != wxSPLIT_VERTICAL)));
}
void ChildView::UpdateUIVerticalSecond(wxUpdateUIEvent& event)
{
	event.Enable(( (!m_splitterSub->IsSplit()) || (m_splitterSub->GetSplitMode() != wxSPLIT_HORIZONTAL)));
}
void ChildView::UpdateUIUnsplitFirst(wxUpdateUIEvent& event)
{
	event.Enable((m_splitter->IsSplit()) || (m_splitterSub->Unsplit()));
}
void ChildView::UpdateUIUnsplitSecond(wxUpdateUIEvent& event)
{
	event.Enable((m_splitterSub->IsSplit()));
}
void ChildView::UpdateUIPlay(wxUpdateUIEvent& event)
{
	event.Enable(getPlaysimu()== false);
}
void ChildView::UpdateUIStop(wxUpdateUIEvent& event)
{
	event.Enable(getPlaysimu()== true);
}




