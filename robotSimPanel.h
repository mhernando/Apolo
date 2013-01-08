#ifndef __APOLO__ROBOT_SIM_PANEL__H
#define __APOLO__ROBOT_SIM_PANEL__H

#include "apoloPanel.h"
#include "nodeTree.h"
#include "genericSlider.h"
#include "manageWindows.h"
#include "tree.h"
#include <vector>
#include <wx/wx.h>

class ManageWindows;
class RobotSimPanel : public wxFrame
{
public:
	RobotSimPanel(wxWindow* parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData,NodeTree * parentData=NULL,bool simplejoint=false);
	void OnValueChanged(wxCommandEvent& event);
	wxStaticText* getTitle(){return title;}
	void OnClose(wxCloseEvent& event);
	NodeTree*  getItemNode () {return itemnode;}
	vector <GenericSlider *> listJoints;
	NodeTree* getItemParentData() {return itemParentData;}
	void setManageWindow (ManageWindows* mg);

	void Delete();

private:
	wxStaticText* title;
	int numJoints;
	double val;
	double min,max;
	bool simplejoint;
	wxString nameJoint;
	GenericSlider* joint;
	NodeTree* itemnode;
	ManageWindows* managewindow;
	NodeTree * itemParentData;




	DECLARE_EVENT_TABLE();
};


#endif //__APOLO__ROBOT_SIM_PANEL__H
