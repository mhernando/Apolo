#ifndef __APOLO__SIMULATED_WORLD__H
#define __APOLO__SIMULATED_WORLD__H

#include "mrcore.h"
#include "initialProperties.h"
#include "objectSelection.h"
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
	ObjectSelection *sel;
	wxTreeItemId getTreeItem(){return mainNode;}
	void AddObject(wxWindowID  	id);
	void DeleteObject(wxTreeItemId itemId);
	World *getWorld(){return m_world;}
	wxTreeItemId getNewNode(){return newNode;};
	ChildView* getChild(){return childView;}


	
	static MainWindow *mainWin;
	static Tree *tree;
	
private:
	int cont;
	wxTreeItemId mainNode;
	wxTreeItemId newNode;
	ChildView* childView;
	World* m_world;
	
};
#endif //__APOLO__SIMULATED_WORLD__H