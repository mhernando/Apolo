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


	void SetEntityToLink(PositionableEntity* pos){EntityToLink=pos;}
	PositionableEntity* GetEntityToLink(){return EntityToLink;}
	void SetIdToLink(wxTreeItemId id){IdToLink=id;}
	wxTreeItemId GetIdToLink(){return IdToLink;}
	vector<PositionableEntity*> GetLinkersPos(){return LinkersPos;}
	vector<wxTreeItemId> getIdLinkersId(){return LinkersId;} 
	vector<vector<PositionableEntity*>> getLinksPos(){return LinksPos;}
	vector<vector<wxTreeItemId>> getLinksId(){return LinksId;}
	wxTreeItemId getItemID(int n,int m){return LinksId[n][m];}
	void InsertLinkerEntity (PositionableEntity* obj,wxTreeItemId Item);
	void InsertLinkedEntity (PositionableEntity* Linker,PositionableEntity* Linked,wxTreeItemId ItemLinked);
	void EraseLinked(PositionableEntity* pos,wxTreeItemId item);
	int getLinkerPositionable(PositionableEntity *en);
	wxTreeItemId getPositionableId(PositionableEntity *pos);
	bool CheckItemLinked(PositionableEntity* pos);
	void UpdateLinks();
	wxTreeItemId getLoadedObjectId(PositionableEntity* pos);
	void InsertLoadedId(wxTreeItemId nod){LoadedIds.push_back(nod);}
	PositionableEntity* getLinker(PositionableEntity* linked);



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
	vector<PositionableEntity*> LinkersPos;  //Se almacenan los objetos a los que se linkan los demás
	vector<vector<wxTreeItemId>> LinksId;
	vector<vector<PositionableEntity*>> LinksPos;
	vector<wxTreeItemId> LinkersId;
	vector<wxTreeItemId> LoadedIds;
	PositionableEntity* EntityToLink;
	wxTreeItemId IdToLink;




};
#endif //__APOLO__SIMULATED_WORLD__H