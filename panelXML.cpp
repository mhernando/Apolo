#include "panelXML.h"
#include "definitions.h"
#include <wx/xml/xml.h>
#include "wx/bmpbuttn.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/treelist.h>
#include <wx/statbmp.h>
#include <wx/textdlg.h>
#include <wx/sstream.h>
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/msgdlg.h>

using namespace std;


DEFINE_EVENT_TYPE(wxEVT_XMLPANEL_CLOSED)
DEFINE_EVENT_TYPE(wxEVT_SELECTED_ITEM)


BEGIN_EVENT_TABLE(PanelXML,wxFrame)
	EVT_CLOSE(PanelXML::OnClose)
	EVT_MENU(ID_CREATEWORLDXML,PanelXML::CreateWorld)
	EVT_LISTBOX_DCLICK(ID_SELECTEDENTITY,PanelXML::SelectItem)
	EVT_MENU(ID_ADDNEWENTITY,PanelXML::CreateNewEntity)
	EVT_MENU(ID_VALIDATEXML,PanelXML::ParseXML)
	EVT_MENU(ID_SAVEXMLEDIT,PanelXML::SaveXML)
END_EVENT_TABLE()



PanelXML::PanelXML(wxWindow *parent,wxWindowID id,const wxString& title)
: wxFrame(parent,wxID_ANY,title,wxDefaultPosition, wxSize(650,580),wxSTAY_ON_TOP | wxCAPTION | wxCLOSE_BOX)
{
	CreatePanel();
	panel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(650,580));
	state=0;
	SetIcon(wxIcon(xmlwindow_xpm));
}



void PanelXML::CreatePanel()
{
	wxBitmap icons[3];
	icons[0]=wxBitmap(selworldxml_xpm);
	icons[1]=wxBitmap(Objects_xpm);
	icons[2]=wxBitmap(itemSelected_xpm);
	wxFont font(9,wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxFont staticfont(18,wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxFont EntFont(10,wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxBoxSizer *Select=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *Tools=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *ele=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BOX=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *ItemList=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *edit=new wxBoxSizer(wxHORIZONTAL);
	wxBitmapButton* staticonworld=new wxBitmapButton(this,ID_SHOWWORLDXML,icons[0],wxDefaultPosition,wxSize(30,30));
	selectW=new wxComboBox();
	wxStaticBitmap* entityIcon=new wxStaticBitmap(this,wxID_ANY,icons[2],wxDefaultPosition,wxSize(30,30));
	EntityName=new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(150,20),wxTE_READONLY);
	selectW->Create(this,ID_SELECTW,wxEmptyString,wxDefaultPosition,wxSize(110,20),Choices,wxCB_DROPDOWN);
	InitToolbars();
	textCtrl=new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(490,520),wxTE_MULTILINE|wxTE_LEFT|wxHSCROLL);
	wxStaticBitmap* objectsIcon=new wxStaticBitmap(this,wxID_ANY,icons[1],wxDefaultPosition,wxSize(150,150));
	wxStaticText* list=new wxStaticText(this,wxID_ANY,wxT("Entities"),wxDefaultPosition,wxSize(40,30));
	EntityName->SetFont(EntFont);
	EntityName->SetForegroundColour(*wxBLUE);
	EntityName->SetBackgroundColour(*wxWHITE);
	textCtrl->SetBackgroundColour(*wxWHITE);
	textCtrl->SetForegroundColour(*wxBLACK);
	textCtrl->SetFont(font);
	list->SetFont(staticfont);
	list->SetForegroundColour(*wxBLUE);
	Objectslist=new wxListBox(this,ID_SELECTEDENTITY,wxDefaultPosition,wxSize(150,400),Items);
	Objectslist->SetFont(font);
	Select->Add(staticonworld,0,wxEXPAND);
	Select->Add(selectW,0,wxEXPAND);
	Select->Add(entityIcon,0,wxEXPAND);
	Select->Add(EntityName,0,wxEXPAND);
	BOX->Add(Select,0,wxEXPAND);
	BOX->Add(Toolbar,0,wxEXPAND);
	BOX->Add(textCtrl,0,wxEXPAND);
	edit->Add(BOX,0,wxEXPAND);
	ItemList->Add(objectsIcon,0,wxEXPAND);
	ItemList->Add(list,0,wxEXPAND);
	ItemList->Add(Objectslist,0,wxEXPAND);
	edit->Add(ItemList,0,wxEXPAND);
	SetSizer(edit);
}



void PanelXML::InitToolbars()
{
	wxBitmap icons[5];
	icons[0]=wxBitmap(update_xpm);
	icons[1]=wxBitmap(createworld_xpm);
	icons[2]=wxBitmap(checkIcon_xpm);
	icons[3]=wxBitmap(AddXMLEntity_xpm);
	icons[4]=wxBitmap(SaveXMLEdit_xpm);
	Toolbar=new wxToolBar();
	Toolbar->Create(this,wxID_ANY,wxDefaultPosition,wxSize(210,40),wxBORDER_NONE|wxTB_HORIZONTAL);
	Toolbar->AddTool(ID_UPDATEXML,icons[0], wxT("Update World"));
	Toolbar->AddTool(ID_VALIDATEXML,icons[2], wxT("Validate"));
	Toolbar->AddTool(ID_CREATEWORLDXML,icons[1], wxT("Create World"));
	Toolbar->AddTool(ID_ADDNEWENTITY,icons[3], wxT("Add Entity"));
	Toolbar->AddTool(ID_SAVEXMLEDIT,icons[4],wxT("Save XML"));
	Toolbar->Realize();
}


void PanelXML::AddWorld(const wxString& w)
{
	selectW->Append(w);
	Choices.Add(w);
}

void PanelXML::InsertText(wxString str)
{
	textCtrl->AppendText(str);
}

wxString PanelXML::getWorld()
{
	int aux=0;
	aux=selectW->GetSelection();
	return Choices[aux];
}

void PanelXML::CreateWorld(wxCommandEvent& event)
{
	state=1;
	textCtrl->Clear();
	selectW->Disable();
}

void PanelXML::setState(int num)
{
	if(num<0) return;
	if(num>3) return;
	if(num==0)
	{
		selectW->Enable();
		EntityName->Clear();
		EntityName->SetBackgroundColour(*wxWHITE);
	}
	state=num;
}


void PanelXML::UpdateObjetcsList(wxArrayString ob)
{
	Objectslist->Clear();
	for(int i=0;i<ob.size();i++)
	{
		Objectslist->AppendString(ob[i]);
		Items.Add(ob[i]);
	}
}


void PanelXML::OnClose(wxCloseEvent& event)
{
	this->Show(false);
	wxCommandEvent CloseEvent( wxEVT_XMLPANEL_CLOSED,GetId() );
	CloseEvent.SetEventObject(this);
	GetEventHandler()->ProcessEvent(CloseEvent);
}


void PanelXML::SelectItem(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_SELECTEDENTITY)
	{
		state=2; 
		selection=Objectslist->GetSelection();
		selectW->Disable();
		EntityName->SetBackgroundColour(*wxYELLOW);
		wxCommandEvent SelectEvent( wxEVT_SELECTED_ITEM,GetId() );
		SelectEvent.SetEventObject(this);
		GetEventHandler()->ProcessEvent(SelectEvent);
	}
}

void PanelXML::CreateNewEntity(wxCommandEvent& event)
{
	state=3;
	EntityName->Clear();
	EntityName->SetBackgroundColour(*wxWHITE);
	textCtrl->Clear();
	selectW->Disable();
}


void PanelXML::updateEntity(wxString name)
{
	EntityName->Clear();
	EntityName->AppendText(name);
}

void PanelXML::ParseXML(wxCommandEvent& event)
{
	wxString cont=getContent();
	wxStringInputStream Stream(cont);
	wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
	if(m_pXmlDocument->Load(Stream))
	{
		wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Parsing XML OK"), wxT("Parser XML"), wxOK | wxICON_INFORMATION);
		dial->ShowModal();
	}
}


void PanelXML::SaveXML(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog saveFile(this,wxT("Save World file XML "),wxEmptyString,wxEmptyString,
				wxT("XML files (*.xml)|*.xml"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
			if(saveFile.ShowModal() == wxID_OK)
			{
				wxString file = saveFile.GetPath();
				char filec[100];
				strcpy(filec, (const char*) file.mb_str(wxConvUTF8));
				wxString cont=getContent();
				wxStringInputStream Stream(cont);
				wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
				m_pXmlDocument->Load(Stream);
				m_pXmlDocument->Save(filec);
				wxLogMessage(wxT("Successfully save in %s"), saveFile.GetPath().c_str());
			}
}


