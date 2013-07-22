#ifndef __APOLO__ROBOT_SIM_GOTO__H
#define __APOLO__ROBOT_SIM_GOTO__H

#include "bitmaps/goto.xpm"

#include "apoloPanel.h"
#include "nodeTree.h"
#include "manageWindows.h"
#include "tree.h"
#include <vector>
#include <wx/wx.h>

class ManageWindows;
class RobotSimGoTo : public wxFrame
{
public:
	RobotSimGoTo(wxWindow* parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData,NodeTree * parentData=NULL);

	void OnValueChanges();
	wxStaticText* getTitle(){return title;}
	NodeTree*  getItemNode () {return itemnode;}

	void setManageWindow (ManageWindows* mg);
	void OnClose(wxCloseEvent& event);
	void Delete();
	void OnButton(wxCommandEvent& event);

private:
	bool noDelete;
	wxStaticText* title;
	NodeTree* itemnode;
	ManageWindows* managewindow;
	wxTextCtrl* coorX,* coorY,* coorZ;
	Transformation3D target;
	wxStaticText* trajectSelected;


	DECLARE_EVENT_TABLE();
};


#endif //__APOLO__ROBOT_SIM_GOTO__H
