#ifndef __APOLO__MANAGEWINDOWS__H
#define __APOLO__MANAGEWINDOWS__H

#include "robotSimPanel.h"
#include "robotSimGoTo.h"
#include "wheeledBasePanel.h"
#include "nodeTree.h"


class RobotSimPanel;
class WheeledBasePanel;
class RobotSimGoTo;

class ManageWindows
{


public:

	ManageWindows();
	~ManageWindows(void);
	void RefreshVectorPages(NodeTree* node);
	bool CheckWindowsExist (NodeTree* node);

//manage robotsimpanel	
	void addWindowRobotSim(RobotSimPanel* robot);
	void WindowRobotSimIsClosed (RobotSimPanel* rb);
//manage wheeledbase		
	void addWindowWheeledBase(WheeledBasePanel* wh);
	void WindowWheeledBaseIsClosed (WheeledBasePanel* wh);

//manage robotsimgoto
	void addWindowRobotSimGoTo(RobotSimGoTo* robot);
	void WindowRobotSimGoToIsClosed (RobotSimGoTo* rb);

	vector<NodeTree*> getVectorWindows(){return vectorwindows;}
	vector<RobotSimPanel*> getVectorRobotSimPanel(){return robotsim;}
	vector<WheeledBasePanel*> getVectorWheeledBasePanel(){return wheelebase;}
	vector<RobotSimGoTo*> getVectorRobotSimGoTo(){return robotsimgoto;}

private:

	vector<NodeTree*> vectorwindows;
	vector<RobotSimPanel*> robotsim;
	vector<WheeledBasePanel*> wheelebase;
	vector<RobotSimGoTo*> robotsimgoto;
	NodeTree* nodetree;

};


#endif // __APOLO__MANAGEWINDOWS__H