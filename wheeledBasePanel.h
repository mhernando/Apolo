#ifndef __APOLO__WHEELED_BASE_PANEL__H
#define __APOLO__WHEELED_BASE_PANEL__H

#include "apoloPanel.h"
#include "definitions.h"
#include "nodeTree.h"
#include "simulatedWorld.h"
#include "manageWindows.h"

#include <wx/wx.h>
#include "bitmaps/up.xpm"
#include "bitmaps/down.xpm"
#include "bitmaps/left.xpm"
#include "bitmaps/right.xpm"
#include "bitmaps/stop.xpm"

class MainWindow;
class ManageWindows;
class WheeledBasePanel : public wxFrame
{
public:
	WheeledBasePanel(wxWindow *parent,wxWindowID id,const wxString& title_frame,NodeTree* itemData);
	void OnButton(wxCommandEvent& event);
	wxStaticText* getTitle(){return title;}
	NodeTree*  getItemNode () {return itemnode;}
	void OnClose(wxCloseEvent& event);
	void setManageWindow (ManageWindows* mg);
	void Delete();

private:
	wxStaticText* title;
	MainWindow *mainWin;
	NodeTree * itemnode;
	wxPanel *panel;
	wxButton *up;
	wxButton *down;
	wxButton *stop;
	wxButton *left;
	wxButton *right;
	ManageWindows* managewindow;

	DECLARE_EVENT_TABLE();

};

#endif // __APOLO__WHEELED_BASE_PANEL__H