#include "childView.h"
#include "mainWindow.h"


BEGIN_EVENT_TABLE(ChildView, wxMDIChildFrame)
	EVT_CLOSE(ChildView::OnClose)
	EVT_TIMER(ID_TIMER, ChildView::OnTimer)	
	EVT_UPDATE_UI(ID_SPLITHF, ChildView::UpdateUIHorizontalFirst)
	EVT_UPDATE_UI(ID_SPLITHS, ChildView::UpdateUIHorizontalSecond)
	EVT_UPDATE_UI(ID_SPLITVF, ChildView::UpdateUIVerticalFirst)
	EVT_UPDATE_UI(ID_SPLITVS, ChildView::UpdateUIVerticalSecond)
	EVT_UPDATE_UI(ID_UNSPLITF, ChildView::UpdateUIUnsplitFirst)
	EVT_UPDATE_UI(ID_UNSPLITS, ChildView::UpdateUIUnsplitSecond)
	EVT_UPDATE_UI(ID_PLAY, ChildView::UpdateUIPlay)
	EVT_UPDATE_UI(ID_STOP2, ChildView::UpdateUIStop)
	EVT_UPDATE_UI(wxID_ANY,ChildView::UpdateIsActivated)


END_EVENT_TABLE()


ChildView::ChildView(wxMDIParentFrame *parent, const wxString& title, SimulatedWorld *sw)
: wxMDIChildFrame(parent, wxID_ANY, title, wxPoint(50,50), wxSize(500,400), wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE),m_timer(this,ID_TIMER)
{
	
	mainWin = (MainWindow*)parent;
	m_splitter = (wxSplitterWindow *) NULL;
	toolbar = 0;
	s_world= sw;
	m_world = sw->getWorld();
	playsimu=false;
	isActivated=true;
	//call timer
	m_timer.Start(100);

	//make a toolbar
	CreateToolBar( wxNO_BORDER | wxTB_FLAT |wxTB_HORIZONTAL, wxID_ANY, wxT(""));
	toolbar = GetToolBar();
	if(toolbar)InitToolBar(toolbar);

	CreateSubWindows();
	
	

}
void ChildView::InitToolBar(wxToolBar* toolbar)
{
	int a=0, b=0;
	wxBitmap bitmaps[8];
	bitmaps[0] = wxBitmap ( V1_xpm);
	bitmaps[1] = wxBitmap ( V2_xpm);
	bitmaps[2] = wxBitmap ( V3_xpm);
	bitmaps[3] = wxBitmap ( V4_xpm);
	bitmaps[4] = wxBitmap ( V5_xpm);
	bitmaps[5] = wxBitmap ( play_xpm);
	bitmaps[6] = wxBitmap ( stop2_xpm);
	bitmaps[7] = wxBitmap ( colour_xpm);


	toolbar->AddTool(ID_UNSPLITF, bitmaps[4], wxT("view unsplit simple"));
	toolbar->AddTool(ID_SPLITHF, bitmaps[0], wxT("view horizontal simple"));
	toolbar->AddTool(ID_SPLITVF, bitmaps[1], wxT("view vertical simple"));
	toolbar->AddTool(ID_SPLITHS, bitmaps[3], wxT("view horizontal complex"));
	toolbar->AddTool(ID_SPLITVS, bitmaps[2], wxT("view vertical complex"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_PLAY, bitmaps[5], wxT("PLAY"));
	toolbar->AddTool(ID_STOP2, bitmaps[6], wxT("STOP"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_CANVASCOLOR, bitmaps[7], wxT("Change Background Colour"));
	toolbar->AddSeparator();
	toolbar->Realize();
	
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
void ChildView::ChangeBackgroundColor()
{
		wxColour colour = wxGetColourFromUser(0);
		if(colour.IsOk())
		{
			canvas1->ChangeBackGroundColour(colour);
			canvas2->ChangeBackGroundColour(colour);
			canvas3->ChangeBackGroundColour(colour);
		}
}

void ChildView::OnSimulator(wxWindowID id)
{
	if(id == ID_PLAY)SetPlaySimu(true);
	if(id == ID_STOP2)SetPlaySimu(false);
}
void ChildView::SplitHorizontalFirst()
{
	m_splitterSub->Show(false);

	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	m_splitter->SplitHorizontally(canvas1, m_splitterSub);
	m_splitterSub->SplitVertically(canvas2, canvas3);
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();

}
void ChildView::SplitHorizontalSecond()
{
	
	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();
	m_splitter->Show(true);
	m_splitterSub->Show(true);
	m_splitter->SplitHorizontally(canvas1, m_splitterSub);
	m_splitterSub->SplitVertically(canvas2, canvas3);
	
}
void ChildView::SplitVerticalFirst()
{
	m_splitterSub->Show(false);

	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	m_splitter->SplitVertically(canvas1, m_splitterSub);
	m_splitterSub->SplitHorizontally(canvas2, canvas3);
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();

}

void ChildView::SplitVerticalSecond()
{
	if( m_splitter->IsSplit()) m_splitter->Unsplit();
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();
	m_splitterSub->Show(true);
	m_splitter->SplitVertically(canvas1, m_splitterSub);
	m_splitterSub->SplitHorizontally(canvas2, canvas3);
}
void ChildView::UnSplitFirst()
{
	m_splitterSub->Show(false);
	if( m_splitter->IsSplit()) m_splitter->Unsplit();
}
void ChildView::UnSplitSecond()
{
	if( m_splitterSub->IsSplit()) m_splitterSub->Unsplit();
}
void ChildView::UpdateUIHorizontalFirst(wxUpdateUIEvent& event)
{
	event.Enable(( (!m_splitter->IsSplit()) || (m_splitterSub->IsSplit()) || (m_splitter->GetSplitMode() != wxSPLIT_HORIZONTAL)));
}
void ChildView::UpdateIsActivated(wxUpdateUIEvent& event)
{
	#ifdef _WIN32
	if(IsMouseInWindow())
		getSimuWorld()->tree->SelectItem(getSimuWorld()->getTreeItem());
	#endif
	
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




