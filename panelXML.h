#ifndef __APOLO__PANEL_XML__H
#define __APOLO__PANEL_XML__H

#include "mrcore.h"
#include "tree.h"
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "bitmaps/iconsearch.xpm"
#include "bitmaps/update.xpm"
#include "bitmaps/selworldxml.xpm"
#include "bitmaps/createworld.xpm"
#include "bitmaps/Objects.xpm"
#include "bitmaps/checkIcon.xpm"
#include "bitmaps/itemSelected.xpm"
#include "bitmaps/AddXMLEntity.xpm"
#include "bitmaps/xmlwindow.xpm"
#include "bitmaps/SaveXMLEdit.xpm"
#include <wx/laywin.h>


using namespace std;


class PanelXML : public wxFrame
{
public:
	PanelXML(wxWindow *parent,wxWindowID id,const wxString& title,Tree* tree);
	void CreatePanel();
	void InitToolbars();
	void Initialize();
	void ParseXML(wxCommandEvent& event);
	void SaveXML(wxCommandEvent& WXUNUSED(event));
	void UpdateXML(wxCommandEvent& event);
	void getItemXML();
	void UpdateSelectedWorld(wxCommandEvent& event);
	void UpdateList();
	void UpdateWorldsList();
	void ShowPanel();
	void ManageState();
	void updateEntity(wxString name);
	void CreateWorld(wxCommandEvent& event);
	wxArrayString getChoices(){return Choices;}
	void SelectItem(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void CreateNewEntity(wxCommandEvent& event);
	void SelectItemInList(wxListEvent& event);
	bool CheckDeclaredElement(string ele);
	bool CheckDeclaredAttribute(string attr);
	void CreateTree();
	void TreatXMLText(char * XMLText);
	void UpdateTreeView();
	void SetInformationFromTree(NodeTree* nod,TypeNode type);
	void GetElementVariables();
	void UpdateText(wxCommandEvent& event);
	Object* CreateEntity(XMLElement * elem);

	

private:
	wxPanel *panel;
	wxTextCtrl* textCtrl;
	wxTextCtrl*	EntityName;
	wxListCtrl* ItemsList;
	wxArrayString worldNames;
	wxArrayString Choices;
	wxComboBox* selectW;
	wxToolBar* Toolbar;
	wxBitmapButton* staticonworld;
	wxArrayString Items;
	wxBoxSizer *edit;
	int state;    //Create or modify
	int selectedItem;
	int selectedWorld;
	wxTreeItemId m_root;
	Tree* tree;
	Tree* treevisible;
	NodeTree* item;
	vector<char *> variables;
	vector<char *> values;


	DECLARE_EVENT_TABLE();
};

#endif // __APOLO__PANEL_XML__H