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

	vector<PositionableEntity*> GetLinked(){return Linked;}
	vector<PositionableEntity*> GetLinkers(){return Linker;}
	void EraseLinked(int ind){Linked.erase(Linked.begin()+ ind);};
	PositionableEntity* GetParentEntity(){return Linker[Linker.size()-1];}
	void InsertPositionableEntity (PositionableEntity* obj){Linker.push_back(obj);};
	void InsertLinkedEntity (PositionableEntity* obj){Linked.push_back(obj);};

	static MainWindow *mainWin;
	static Tree *tree;

	


	
private:

	bool objConnected;
	wxTreeItemId mainNode;
	string name;
	ChildView* childView;
	World* m_world;
	vector<Vector2D> CopiedDesign;
	vector<PositionableEntity*> Linker;  //Se almacenan los objetos a los que se linkan los demás
	vector<PositionableEntity*> Linked; //Se almacenan los objetos que han sido ya linkados
	

};
#endif //__APOLO__SIMULATED_WORLD__H