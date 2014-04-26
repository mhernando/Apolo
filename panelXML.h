#ifndef __APOLO__PANEL_XML__H
#define __APOLO__PANEL_XML__H


#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/wx.h>
#include "bitmaps/iconsearch.xpm"
#include "bitmaps/xmlTreeIcon.xpm"
#include "bitmaps/update.xpm"
#include "bitmaps/selworldxml.xpm"
#include "bitmaps/createworld.xpm"
#include "bitmaps/Objects.xpm"
#include "bitmaps/checkIcon.xpm"
#include "bitmaps/itemSelected.xpm"
#include "bitmaps/AddXMLEntity.xpm"
#include "bitmaps/xmlwindow.xpm"
#include "bitmaps/SaveXMLEdit.xpm"


DECLARE_EVENT_TYPE(wxEVT_XMLPANEL_CLOSED, -1)  
DECLARE_EVENT_TYPE(wxEVT_SELECTED_ITEM,-1)


class PanelXML : public wxFrame
{
public:
	PanelXML(wxWindow *parent,wxWindowID id,const wxString& title);
	void CreatePanel();
	void Controls(wxCommandEvent& event);
	void InsertText(wxString str);
	wxTextCtrl* getTextCtrl(){return textCtrl;}
	void InitToolbars();
	wxString getContent(){return textCtrl->GetValue();}
	void Clear(){ textCtrl->Clear();}
	void AddWorld(const wxString& w);
	wxString getWorld();
	void SetWorld(int numW){selectW->SetSelection(numW);}
	wxArrayString getChoices(){return Choices;}
	void CreateWorld(wxCommandEvent& event);
	int getState(){return state;}
	void setState(int num);
	void UpdateObjetcsList(wxArrayString ob);
	void OnClose(wxCloseEvent& event);
	void SelectItem(wxCommandEvent& event);
	int getSelection(){return selection;}
	void CreateNewEntity(wxCommandEvent& event);
	void updateEntity(wxString name);
	void ParseXML(wxCommandEvent& event);
	void SaveXML(wxCommandEvent& WXUNUSED(event));




private:
	wxPanel *panel;
	wxTextCtrl* textCtrl;
	wxTextCtrl*	EntityName;
	wxArrayString Choices;
	wxComboBox* selectW;
	wxListBox *Objectslist;
	wxToolBar* Toolbar;
	wxArrayString Items;
	wxArrayString Variables;
	int state;    //Create or modify
	int selection;
	DECLARE_EVENT_TABLE();
};

#endif // __APOLO__PANEL_XML__H