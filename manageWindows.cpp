#include "manageWindows.h"



ManageWindows::ManageWindows(void)
{

}

ManageWindows::~ManageWindows(void)
{
}


//Gestor de las ventanas de menus del robotsimpanel
void ManageWindows::addWindowRobotSim(RobotSimPanel* robot)
{
	robotsim.push_back(robot);

}

void ManageWindows::WindowRobotSimIsClosed (RobotSimPanel* rb)
{

	for (int i=0; i<(int)robotsim.size();i++)
	{
		if (robotsim[i]==rb)
		{
			RefreshVectorPages(robotsim[i]->getItemNode());
			robotsim.erase(robotsim.begin()+i);
			return;
		}
	}

}

void ManageWindows::addWindowRobotSimGoTo(RobotSimGoTo* robot)
{
	robotsimgoto.push_back(robot);

}
void ManageWindows::WindowRobotSimGoToIsClosed (RobotSimGoTo* rb)
{

	for (int i=0; i<(int)robotsimgoto.size();i++)
	{
		if (robotsimgoto[i]==rb)
		{
			RefreshVectorPages(robotsimgoto[i]->getItemNode());
			robotsimgoto.erase(robotsimgoto.begin()+i);
			return;
		}
	}

}

//Gestor de las ventanas de control del wheeledbase
void ManageWindows::addWindowWheeledBase(WheeledBasePanel* wh)
{
	
	wheelebase.push_back(wh);

}

void ManageWindows::WindowWheeledBaseIsClosed (WheeledBasePanel* wh)
{
	for (int i=0; i<(int)wheelebase.size();i++)
	{
		if (wheelebase[i]==wh)
		{
			RefreshVectorPages(wheelebase[i]->getItemNode());
			wheelebase.erase(wheelebase.begin()+i);
			return;
		}
	}
}


// Gestor general para evitar la duplicacion de ventanas, además de su correspondiente eliminacion
bool ManageWindows::CheckWindowsExist (NodeTree* node)
{
	if (vectorwindows.empty())
	{
		vectorwindows.push_back(node);
		return true;
	}

	for (int i=0; i<(int)vectorwindows.size();i++)
	{		

		if (vectorwindows[i]==node)		
				return false;
	}
		
	vectorwindows.push_back(node);
	return true;
}

void ManageWindows::RefreshVectorPages (NodeTree* node)
{
	
	for (int i=0; i<(int)vectorwindows.size();i++)

		if (vectorwindows[i]==node)
		{
			vectorwindows.erase(vectorwindows.begin()+i);
			return;
		}
}


