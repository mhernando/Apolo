#ifndef __APOLO__SIMULATED_WORLD__H
#define __APOLO__SIMULATED_WORLD__H

#include "mrcore.h"
#include "faceWindow.h"
#include "initialProperties.h"
#include "objectSelection.h"
#include "childView.h"
#include "tree.h"
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/msw/msgdlg.h>


class MainWindow;

class SimulatedWorld
{
public:
	SimulatedWorld(World *world);
	~SimulatedWorld();
	
	ObjectSelection *sel;
	wxTreeItemId getTreeItem(){return mainNode;}
	string getName(){return name;}
	void setName(string _name){ name=_name; childView->SetTitle(_name);}
	void setObjConnected(bool con){objConnected=con;}
	bool IsObjectConnected(){return objConnected;}
	void AddObject(wxWindowID  	id);
	void DeleteObject(wxTreeItemId itemId);
	World *getWorld(){return m_world;}
	ChildView* getChild(){return childView;}

	void SetCopiedDesign(vector<Vector2D> Copy);
	vector<Vector2D> GetCopiedDesign(){return CopiedDesign;}
	void CleanClipboard(){CopiedDesign.clear();};


	//Link methods
	void SetEntityToLink(PositionableEntity* pos){EntityToLink=pos;}
	PositionableEntity* GetEntityToLink(){return EntityToLink;}
	//void SetIdToLink(wxTreeItemId id){IdToLink=id;}
	//wxTreeItemId GetIdToLink(){return IdToLink;}
	void InsertId(wxTreeItemId id){ItemsId.push_back(id);}
	void InsertItem(PositionableEntity* item){Items.push_back(item);}
	vector<wxTreeItemId> getItemsId(){return ItemsId;}
	vector<PositionableEntity*> getItems(){return Items;}
	bool checkAddedItem(PositionableEntity* pos);
	void InitializeItemsVectors();
	bool checkLink(PositionableEntity* linker,PositionableEntity* linked);
	
	



	Tree* getTree(){return tree;}
	static MainWindow *mainWin;
	static Tree *tree;


	


	
private:

	bool objConnected;
	wxTreeItemId mainNode;
	string name;
	ChildView* childView;
	World* m_world;
	vector<Vector2D> CopiedDesign;
	PositionableEntity* EntityToLink;
	wxTreeItemId IdToLink;
	vector<wxTreeItemId> ItemsId;
	vector<PositionableEntity*> Items;
	wxMessageDialog* CompOption;
	





};
#endif //__APOLO__SIMULATED_WORLD__H