#ifndef __APOLO__MANAGEWINDOWS__H
#define __APOLO__MANAGEWINDOWS__H

#include "robotSimPanel.h"
#include "wheeledBasePanel.h"
#include "nodeTree.h"

class RobotSimPanel;
class WheeledBasePanel;

class ManageWindows
{


public:

	ManageWindows();
	~ManageWindows(void);
	void RefreshVectorPages(NodeTree* node);
	bool CheckWindowsExist (NodeTree* node);
	void addWindowRobotSim(RobotSimPanel* robot);
	void WindowRobotSimIsClosed (RobotSimPanel* rb);
	void addWindowWheeledBase(WheeledBasePanel* wh);
	void WindowWheeledBaseIsClosed (WheeledBasePanel* wh);
	vector<NodeTree*> getVectorWindows(){return vectorwindows;}
	vector<RobotSimPanel*> getVectorRobotSimPanel(){return robotsim;}
	vector<WheeledBasePanel*> getVectorWheeledBasePanel(){return wheelebase;}


private:

	vector<NodeTree*> vectorwindows;
	vector<RobotSimPanel*> robotsim;
	vector<WheeledBasePanel*> wheelebase;
	NodeTree* nodetree;

};


#endif // __APOLO__MANAGEWINDOWS__H