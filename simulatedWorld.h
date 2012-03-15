#ifndef __APOLO__SIMULATED_WORLD__H
#define __APOLO__SIMULATED_WORLD__H

#include "mrcore.h"
#include "childView.h"
#include "tree.h"
#include <wx/wx.h>
#include <wx/treectrl.h>

class MainWindow;

class SimulatedWorld
{
public:
	SimulatedWorld(World *world);
	~SimulatedWorld();

	wxTreeItemId getTreeItem(){return mainNode;}
	World *getWorld(){return m_world;}
	ChildView* getChild(){return childView;}
	
	static MainWindow *mainWin;
	static Tree *tree;
	
private:
	wxTreeItemId mainNode;
	ChildView* childView;
	World* m_world;
};
#endif //__APOLO__SIMULATED_WORLD__H