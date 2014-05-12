#ifndef __APOLO__TREE__H
#define __APOLO__TREE__H

#include "mrcore.h"
#include "definitions.h"
#include "childView.h"
#include "nodeTree.h"

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/event.h>
#include <wx/imaglist.h>
#include <vector>
#include "bitmaps/icon1.xpm" //file 
#include "bitmaps/icon2.xpm" //file select
#include "bitmaps/icon3.xpm" //folder
#include "bitmaps/icon4.xpm" //folder select
#include "bitmaps/universe.xpm"
#include "bitmaps/cylindrical.xpm"
#include "bitmaps/cylindricalSelect.xpm"
#include "bitmaps/joint.xpm"
#include "bitmaps/jointSelect.xpm"
#include "bitmaps/prismatic.xpm"
#include "bitmaps/prismaticSelect.xpm"
#include "bitmaps/sphere.xpm"
#include "bitmaps/sphereSelect.xpm"
#include "bitmaps/composed.xpm"
#include "bitmaps/composedSelect.xpm"
#include "bitmaps/powercube.xpm"
#include "bitmaps/powercubeSelect.xpm"
#include "bitmaps/positionable.xpm"
#include "bitmaps/positionableSelect.xpm"
#include "bitmaps/pioneer.xpm"
#include "bitmaps/pioneerSelect.xpm"
#include "bitmaps/sickLms.xpm"
#include "bitmaps/sickLmsSelect.xpm"
#include "bitmaps/world.xpm"
#include "bitmaps/worldSelect.xpm"
#include "bitmaps/scara.xpm"
#include "bitmaps/scaraSelect.xpm"
#include "bitmaps/universeSelect.xpm"
#include "bitmaps/nemolaser.xpm"
#include "bitmaps/nemolaserSelect.xpm"
#include "bitmaps/meshpart.xpm"
#include "bitmaps/meshpartSelect.xpm"
#include "bitmaps/solidEntity.xpm"
#include "bitmaps/solidEntitySelect.xpm"
#include "bitmaps/laser.xpm"
#include "bitmaps/laserSelect.xpm"
#include "bitmaps/faceSetPart.xpm"
#include "bitmaps/faceSetPartSelect.xpm"
#include "bitmaps/wheeledBase.xpm"
#include "bitmaps/wheeledBaseSelect.xpm"
#include "bitmaps/robotSim.xpm"
#include "bitmaps/robotSimSelect.xpm"
#include "bitmaps/asea.xpm"
#include "bitmaps/aseaSelect.xpm"
#include "bitmaps/link.xpm"
#include "bitmaps/link2.xpm"


class MainWindow;
class SimulatedWorld;
class Tree : public wxTreeCtrl
{
private:
	wxString text;
	wxTreeItemId root;
	wxTreeItemId m_parent;
	bool sel;
	bool showLinks;
	struct m_item {int id; wxString name; wxIcon icon;};
	wxTreeItemId PreviousitemId;
	vector<NodeTree*> Nod;
	DECLARE_EVENT_TABLE();	

public:
	wxTreeItemId GetWorld(wxTreeItemId i);
	Tree(wxWindow * parent, const wxWindowID id);
	wxTreeItemId GenerateSubTree(SimulatedWorld *simu);
	NodeTree* AddNode(PositionableEntity * pos, wxTreeItemId parent,SimulatedWorld *simu);
	void Parent(wxTreeItemId r);
	void UpdateTree(SimulatedWorld* N);
	void showTreeStructure(SimulatedWorld* sim,bool view);
	void Structure(SimulatedWorld* sim,int linker,PositionableEntity* pos,wxTreeItemId nod);
	void Restructure(SimulatedWorld* sim,wxTreeItemId main);
	Tree::m_item SimplyItems(int id,wxString name, wxIcon icon);
	void OnItemMenu(wxTreeEvent& event);
	wxTreeItemId GenerateSubTree(ComposedEntity* w,SimulatedWorld* simu);
	void OnShowCanvas(wxMouseEvent& event);
	void ShowSelection(wxTreeEvent& event);
	void SetShowSelection(bool show){sel=show;};
	void RestoreItemImage(wxTreeItemId ID);
	bool CheckLink(PositionableEntity* selected,PositionableEntity* n);
	void ControlMouse(wxMouseEvent& event);
	void setShowLinks(bool sh);
	bool getShowLinks(){return showLinks;};
	void EraseMarks();
	bool ValidateNodeToLink(TypeNode type);

	
	MainWindow* m_mainWin;

};


class TreeItemData : public wxTreeItemData
{
public: 
	TreeItemData(const wxString& dat) : m_dat(dat){}
private:
	wxString m_dat;

};

#endif // __APOLO__TREE__H
