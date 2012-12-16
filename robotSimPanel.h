#ifndef __APOLO__ROBOT_SIM_PANEL__H
#define __APOLO__ROBOT_SIM_PANEL__H

#include "apoloPanel.h"
#include "nodeTree.h"
#include "genericSlider.h"
#include <vector>
#include <wx/wx.h>


class RobotSimPanel : public wxFrame
{
public:
	RobotSimPanel(wxWindow* parent, wxWindowID id,const wxString& title_dialog, NodeTree* itemData,bool simplejoint=false);
	void OnValueChanged(wxCommandEvent& event);
	wxStaticText* getTitle(){return title;}

	vector <GenericSlider *> listJoints;

private:
	wxStaticText* title;
	int numJoints;
	double val;
	double min,max;
	bool simplejoint;
	wxString nameJoint;
	GenericSlider* joint;
	NodeTree* itemnode;

	DECLARE_EVENT_TABLE();
};


#endif //__APOLO__ROBOT_SIM_PANEL__H
