#ifndef __APOLO__CHILD_VIEW__H
#define __APOLO__CHILD_VIEW__H

#include "mrcore.h"
#include "canvas.h"
#include "definitions.h"

#include <wx/mdi.h>
#include <wx/menu.h>
#include <wx/splitter.h>
#include <wx/timer.h>

#include "bitmaps/V1.xpm"
#include "bitmaps/V2.xpm"
#include "bitmaps/V3.xpm"
#include "bitmaps/V4.xpm"
#include "bitmaps/V5.xpm"
#include "bitmaps/play.xpm"
#include "bitmaps/stop2.xpm"

class MainWindow;

class ChildView: public wxMDIChildFrame
{
public:
	ChildView(wxMDIParentFrame *parent, const wxString& title, World * w);

	void InitToolBar(wxToolBar* toolbar);
		
	void InitToolBar2(wxToolBar* toolbar2);




	void UpdateWorld();
	void RefreshChild();
	void CreateSubWindows();
	
	void OnClose(wxCloseEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnHideChild(wxCommandEvent& event);
	void OnSimulator(wxCommandEvent& event);
	void SplitHorizontalFirst(wxCommandEvent& event);
	void SplitHorizontalSecond(wxCommandEvent& event);
	void SplitVerticalFirst(wxCommandEvent& event);
	void SplitVerticalSecond(wxCommandEvent& event);
	void UnSplitFirst(wxCommandEvent& event);
	void UnSplitSecond(wxCommandEvent& event);

	void UpdateUIHorizontalFirst(wxUpdateUIEvent& event);
	void UpdateUIHorizontalSecond(wxUpdateUIEvent& event);
	void UpdateUIVerticalFirst(wxUpdateUIEvent& event);
	void UpdateUIVerticalSecond(wxUpdateUIEvent& event);
	void UpdateUIUnsplitFirst(wxUpdateUIEvent& event);
	void UpdateUIUnsplitSecond(wxUpdateUIEvent& event);
	void UpdateUIPlay(wxUpdateUIEvent& event);
	void UpdateUIStop(wxUpdateUIEvent& event);

	void SetPlaySimu(bool play){playsimu=play;}
	bool getPlaysimu(){return playsimu;}
	
private:
	Canvas* canvas1;
	Canvas* canvas2;
	Canvas* canvas3;
	World* m_world;
	MainWindow *mainWin;
	wxSplitterWindow* m_splitter;
	wxSplitterWindow* m_splitterSub;
	bool playsimu;
	wxTimer m_timer;
	wxToolBar* toolbar;
	/////
	wxToolBar* toolbar2;
	/////
	wxMenu * file_menu;
	wxMenu * file_menu2;
	wxMenu * file_menu3;
	wxMenu * file_menu4;
	wxMenuBar * menubar;
	DECLARE_EVENT_TABLE()

};

#endif // __APOLO__CHILD_VIEW__H
