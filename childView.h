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
#include "bitmaps/box.xpm"
using namespace std;

class MainWindow;
class SimulatedWorld;

class ChildView: public wxMDIChildFrame
{
public:
	
	ChildView(wxMDIParentFrame *parent, const wxString& title, SimulatedWorld * w);

	void InitToolBar(wxToolBar* toolbar);
		
	void InitToolBar2(wxToolBar* toolbar2);

	void UpdateWorld();
	void RefreshChild();
	void CreateSubWindows();
	
	void OnClose(wxCloseEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnSimulator(wxWindowID id);
	void SplitHorizontalFirst();
	void SplitHorizontalSecond();
	void SplitVerticalFirst();
	void SplitVerticalSecond();
	void UnSplitFirst();
	void UnSplitSecond();
	void UpdateUIHorizontalFirst(wxUpdateUIEvent& event);
	void UpdateIsActivated(wxUpdateUIEvent& event);
	void UpdateUIHorizontalSecond(wxUpdateUIEvent& event);
	void UpdateUIVerticalFirst(wxUpdateUIEvent& event);
	void UpdateUIVerticalSecond(wxUpdateUIEvent& event);
	void UpdateUIUnsplitFirst(wxUpdateUIEvent& event);
	void UpdateUIUnsplitSecond(wxUpdateUIEvent& event);
	void UpdateUIPlay(wxUpdateUIEvent& event);
	void UpdateUIStop(wxUpdateUIEvent& event);
	Canvas* getCanvas(){return canvas1;};
	void SetPlaySimu(bool play){playsimu=play;}
	bool getPlaysimu(){return playsimu;}
	void SetIsActivated(bool active){isActivated=active;}
	bool getIsActivated(){return isActivated;}
	SimulatedWorld* getSimuWorld(){return s_world;}
	void ChangeBackgroundColor();
	
private:
	Canvas* canvas1;
	Canvas* canvas2;
	Canvas* canvas3;
	SimulatedWorld* s_world;
	World* m_world;
	MainWindow *mainWin;
	wxSplitterWindow* m_splitter;
	wxSplitterWindow* m_splitterSub;
	bool playsimu;
	bool isActivated;
	wxTimer m_timer;
	wxToolBar* toolbar;
	wxToolBar* toolbar2;
	DECLARE_EVENT_TABLE()
	
};

#endif // __APOLO__CHILD_VIEW__H
