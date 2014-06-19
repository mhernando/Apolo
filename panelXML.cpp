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
#include <wx/txtstrm.h>
#include <wx/msgdlg.h>
#include <wx/stc/stc.h>
#include "mainWindow.h"
#include "simulatedWorld.h"

using namespace std;
using namespace mr;


BEGIN_EVENT_TABLE(PanelXML,wxFrame)
	EVT_CLOSE(PanelXML::OnClose)
	EVT_MENU(ID_CREATEWORLDXML,PanelXML::CreateWorld)
	EVT_MENU(ID_ADDNEWENTITY,PanelXML::CreateNewEntity)
	EVT_MENU(ID_VALIDATEXML,PanelXML::ParseXML)
	EVT_MENU(ID_SAVEXMLEDIT,PanelXML::SaveXML)
	EVT_COMBOBOX(ID_SELECTW,PanelXML::UpdateSelectedWorld)
	EVT_MENU(ID_UPDATEXML, PanelXML::UpdateXML)
	EVT_BUTTON(ID_SHOWWORLDXML, PanelXML::UpdateSelectedWorld)
	EVT_LIST_ITEM_SELECTED(ID_SELECTLISTEDITEM, PanelXML::SelectItemInList)
END_EVENT_TABLE()



PanelXML::PanelXML(wxWindow *parent,wxWindowID id,const wxString& title,Tree* tr)
: wxFrame(parent,wxID_ANY,title,wxDefaultPosition, wxSize(990,580),wxSTAY_ON_TOP | wxCAPTION | wxCLOSE_BOX)
{
	panel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(990,580));
	tree=tr;
	CreatePanel();
	state=0;
	SetIcon(wxIcon(xmlwindow_xpm));
	selectedWorld=0;
}



void PanelXML::CreatePanel()
{
	CreateTree();
	panel->SetBackgroundColour(wxColour(120,200,130));
	panel->SetBackgroundColour(*wxWHITE);
	wxBitmap icons[3];
	icons[0]=wxBitmap(selworldxml_xpm);
	icons[1]=wxBitmap(Objects_xpm);
	icons[2]=wxBitmap(itemSelected_xpm);
	wxFont font(9,wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxFont staticfont(18,wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxFont EntFont(10,wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxBoxSizer *TreeSizer=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *Select=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *Tools=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *ele=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BOX=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *L=new wxBoxSizer(wxVERTICAL);
	edit=new wxBoxSizer(wxHORIZONTAL);

	ItemsList=new wxListCtrl(panel, ID_SELECTLISTEDITEM, wxDefaultPosition, wxSize(260,430), wxLC_REPORT|wxLC_SINGLE_SEL);
	ItemsList->SetBackgroundColour(wxColour(160,250,180));
	wxListItem col0;
    col0.SetId(0);
    col0.SetText( _("Name") );
    col0.SetWidth(130);
	col0.SetBackgroundColour(wxColour(100,150,110));
    ItemsList->InsertColumn(0, col0);
    wxListItem col1;
    col1.SetId(1);
    col1.SetText( _("Class") );
	col1.SetWidth(110);
	col1.SetBackgroundColour(wxColour(100,150,110));
    ItemsList->InsertColumn(1, col1);
		
	staticonworld=new wxBitmapButton(panel,ID_SHOWWORLDXML,icons[0],wxDefaultPosition,wxSize(30,30));
	selectW=new wxComboBox();
	wxStaticBitmap* entityIcon=new wxStaticBitmap(panel,wxID_ANY,icons[2],wxDefaultPosition,wxSize(30,30));
	EntityName=new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(150,20),wxTE_READONLY);
	selectW->Create(panel,ID_SELECTW,wxEmptyString,wxDefaultPosition,wxSize(110,20),Choices,wxCB_DROPDOWN);
	InitToolbars();
	textCtrl=new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(490,520),wxTE_MULTILINE|wxTE_LEFT|wxHSCROLL|wxTE_RICH2);
	wxStaticBitmap* objectsIcon=new wxStaticBitmap(panel,wxID_ANY,icons[1],wxDefaultPosition,wxDefaultSize);
	
	EntityName->SetFont(EntFont);
	EntityName->SetForegroundColour(*wxBLACK);
	EntityName->SetBackgroundColour(*wxWHITE);
	
	textCtrl->SetBackgroundColour(*wxWHITE);
	textCtrl->SetForegroundColour(*wxBLACK);
	textCtrl->SetFont(font);

	
	Select->Add(staticonworld,0,wxEXPAND);
	Select->Add(selectW,0,wxEXPAND);
	Select->Add(entityIcon,0,wxEXPAND);
	Select->Add(EntityName,0,wxEXPAND);
	BOX->Add(Select,0,wxEXPAND);
	BOX->Add(Toolbar,0,wxEXPAND);
	BOX->Add(textCtrl,0,wxEXPAND);
	edit->Add(BOX,0,wxEXPAND);
	L->Add(ItemsList,0,wxEXPAND);
	TreeSizer->Add(objectsIcon,0,wxEXPAND);
	TreeSizer->Add(treevisible,5,wxEXPAND);
	edit->Add(TreeSizer,5,wxEXPAND);
	edit->Add(L,0,wxEXPAND);
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
	Toolbar->SetBackgroundColour(*wxWHITE);
	Toolbar->Create(panel,wxID_ANY,wxDefaultPosition,wxSize(210,40),wxBORDER_NONE|wxTB_HORIZONTAL);
	Toolbar->AddTool(ID_UPDATEXML,icons[0], wxT("Update World"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_VALIDATEXML,icons[2], wxT("Validate"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_CREATEWORLDXML,icons[1], wxT("Create World"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_ADDNEWENTITY,icons[3], wxT("Add Entity"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_SAVEXMLEDIT,icons[4],wxT("Save XML"));
	Toolbar->Realize();
}


void PanelXML::Initialize()
{
	UpdateWorldsList();
}

void PanelXML::ShowPanel()
{
	UpdateTreeView();
	tree->Expand(tree->GetRootItem());
	textCtrl->Clear();
	if(tree->m_mainWin->listWorlds.size()>0)
	{
		state=0;
		ManageState();
		char* textXml=tree->m_mainWin->listWorlds[0]->getWorld()->CreateXMLText();
		TreatXMLText(textXml);
		UpdateObjetcsList();
		UpdateWorldsList();
	}
	this->Show(true);
	this->MakeModal(true);
	selectW->SetSelection(0);
}


void PanelXML::UpdateXML(wxCommandEvent &event)
{
	if(state==0) //Modify an existing world
	{	
		ManageState();
		wxString cont=textCtrl->GetValue();
		wxStringInputStream Stream(cont);
		wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
		if(m_pXmlDocument->Load(Stream))
		{
			wxCharBuffer buffer=cont.ToUTF8();
			char *xml=buffer.data();
			tree->m_mainWin->listWorlds[selectedWorld]->getWorld()->loadFromXMLText(xml);
			tree->UpdateTree(tree->m_mainWin->listWorlds[selectedWorld]);
			textCtrl->Clear();
			this->Show(false);
			this->MakeModal(false);
		}
	}

	if (state==1)  //Creating a new world
	{
		wxString cont=textCtrl->GetValue();
		wxStringInputStream Stream(cont);
		wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
		if(m_pXmlDocument->Load(Stream))
		{
			World* w=new World();
			wxCharBuffer buffer=cont.ToUTF8();
			char *xml=buffer.data();
			w->loadFromXMLText(xml);
			SimulatedWorld* sim=new SimulatedWorld(w);
			tree->m_mainWin->listWorlds.push_back(sim);
			tree->Expand(tree->GetRootItem());
			tree->Expand(tree->GetLastChild(tree->GetRootItem()));
			state=0;
			textCtrl->Clear();
			this->Show(false);
			this->MakeModal(false);
		}
	}
	
	if(state==2)//Modify an existing Entity
	{
		wxString cont=textCtrl->GetValue();
		wxStringInputStream Stream(cont);
		wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
		XMLElement *parent=new XMLElement();
		if(m_pXmlDocument->Load(Stream))
		{
			wxCharBuffer buffer=cont.ToUTF8();
			char *xml=buffer.data();
			(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem]->loadFromXMLText(xml);
			tree->UpdateTree(tree->m_mainWin->listWorlds[selectedWorld]);
			state=0;
			textCtrl->Clear();
			this->Show(false);
			this->MakeModal(false);
		}
	}
	
	if(state==3) //Creating a new Entity
	{
		wxString cont=textCtrl->GetValue();
		wxStringInputStream Stream(cont);
		wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
		if(m_pXmlDocument->Load(Stream))
		{
			char classname[50];
			XML x;
			wxCharBuffer buffer=cont.ToUTF8();
			char *xml=buffer.data();
			XMLElement* elem=x.Paste(xml);
			Object*	obj;
			PositionableEntity* p_obj = dynamic_cast<PositionableEntity *>(obj);
			(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())+=p_obj;
			tree->m_mainWin->listWorlds[selectedWorld]->getChild()->UpdateWorld();
			tree->AddNode(p_obj, tree->m_mainWin->listWorlds[selectedWorld]->getTreeItem(),tree->m_mainWin->listWorlds[selectedWorld]);
			state=0;
			textCtrl->Clear();
			this->Show(false);
			this->MakeModal(false);
		}
	}
	if(state==4) //Modify existing Entity from tree
	{
		wxString cont=textCtrl->GetValue();
		wxStringInputStream Stream(cont);
		wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
		if(m_pXmlDocument->Load(Stream))
		{
			wxCharBuffer buffer=cont.ToUTF8();
			char *xml=buffer.data();
			item->pointer.positionableentity->loadFromXMLText(xml);
			tree->UpdateTree(tree->m_mainWin->listWorlds[selectedWorld]);
			state=0;
			textCtrl->Clear();
			this->Show(false);
			this->MakeModal(false);
		}
	}

	if(state==5) //Modify existing world from tree
	{
		wxString cont=textCtrl->GetValue();
		wxStringInputStream Stream(cont);
		wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
		if(m_pXmlDocument->Load(Stream))
		{
			wxCharBuffer buffer=cont.ToUTF8();
			char *xml=buffer.data();
			item->pointer.world->loadFromXMLText(xml);
			tree->UpdateTree(tree->m_mainWin->listWorlds[selectedWorld]);
			state=0;
			textCtrl->Clear();
			this->Show(false);
			this->MakeModal(false);
		}
	}
}


void PanelXML::getItemXML()
{
		char* textXml=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem]->CreateXMLText();
		TreatXMLText(textXml);
		wxString name=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem]->getName();
		updateEntity(name);
		XMLElement* ele=new XMLElement();
		(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem]->writeToXML(ele);
}


void PanelXML::UpdateSelectedWorld(wxCommandEvent &event)
{
	state=0;
	ManageState();
	if(tree->m_mainWin->listWorlds.size()>0)
	{
		selectedWorld=selectW->GetSelection();
		textCtrl->Clear();
		char* textXml=tree->m_mainWin->listWorlds[selectedWorld]->getWorld()->CreateXMLText();
		TreatXMLText(textXml);
		UpdateObjetcsList();
	}
}


void PanelXML::CreateWorld(wxCommandEvent& event)
{
	state=1;
	textCtrl->Clear();
	selectW->Disable();
}


void PanelXML::CreateNewEntity(wxCommandEvent& event)
{
	state=3;
	EntityName->Clear();
	EntityName->SetBackgroundColour(*wxWHITE);
	textCtrl->Clear();
	selectW->Disable();
}


void PanelXML::UpdateWorldsList()
{
	selectW->Clear();
	worldNames.Clear();
	Choices.Clear();
	for(int i=0;i<tree->m_mainWin->listWorlds.size();i++)
	{	
		worldNames.push_back(tree->m_mainWin->listWorlds[i]->getName());
		selectW->Append(worldNames[i]);
		Choices.Add(worldNames[i]);
	}
}


void PanelXML::UpdateObjetcsList()
{
	ItemsList->DeleteAllItems();
	if((state==5)||(state==0))
	{
		for(int i=0;i<tree->m_mainWin->listWorlds[selectedWorld]->getWorld()->getNumObjects();i++)
		{
			long itemIndex;
			wxString name=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[i]->getName();
			wxString Classname=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[i]->getClassName();
			itemIndex=ItemsList->InsertItem(0, name); //want this for col. 1
			ItemsList->SetItem(itemIndex, 1, Classname); //want this for col. 2
		}
	}
	else if(state==4)
	{
		long itemIndex;
		wxString name=item->pointer.positionableentity->getName();
		wxString Classname=item->pointer.positionableentity->getClassName();
		itemIndex=ItemsList->InsertItem(0, name); //want this for col. 1
		ItemsList->SetItem(itemIndex, 1, Classname); //want this for col. 2
	}

	
}


void PanelXML::ParseXML(wxCommandEvent& event)
{
	wxString cont=textCtrl->GetValue();
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
				wxString cont=textCtrl->GetValue();
				wxStringInputStream Stream(cont);
				wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
				m_pXmlDocument->Load(Stream);
				m_pXmlDocument->Save(filec);
				wxLogMessage(wxT("Successfully save in %s"), saveFile.GetPath().c_str());
			}
}



void PanelXML::ManageState()
{
	if(state<0) return;
	if(state>3) return;
	if((state==0)||(state==5))
	{
		selectW->Enable();
		EntityName->Clear();
		EntityName->SetBackgroundColour(*wxWHITE);
	}
}


void PanelXML::OnClose(wxCloseEvent& event)
{
	this->Show(false);
	this->MakeModal(false);
	tree->m_mainWin->SetState(0);
}


void PanelXML::updateEntity(wxString name)
{
	EntityName->Clear();
	EntityName->AppendText(name);
}


void PanelXML::SelectItemInList(wxListEvent& event)
{
	int Itemsnum;
	Itemsnum=ItemsList->GetItemCount();
	int id=event.GetId();
	if (id==ID_SELECTLISTEDITEM)
	{
		long itemIndex=-1;
		itemIndex = ItemsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
		if (itemIndex == -1) return;
		state=2; 
		selectedItem=Itemsnum-itemIndex-1;
		EntityName->SetBackgroundColour(wxColour(140,255,180));
		getItemXML();
	}
}


void PanelXML::TreatXMLText(char *XMLText)
{
	textCtrl->Clear();
	wxTextAttr styleN(*wxBLACK);
	wxTextAttr styleE(*wxBLUE);
	wxTextAttr styleA(*wxRED);
	styleN.SetTextColour(*wxBLACK);
	styleE.SetTextColour(*wxBLUE);
	styleA.SetTextColour(*wxRED);
	string str(XMLText);
	string buffer;
	int i=0;
	while(i<str.size()-2)
	{
		if(str[i]==string("<"))
		{	
			textCtrl->SetDefaultStyle(styleE);
			textCtrl->WriteText(str[i]);
			i++;
			int j=i;
			while(str[i]!=string(">"))
			{
				bool check=true;
				if((str[i]==char(10))||(str[i]==char(13))||(str[i]==char(32))||(str[i]==string("=")))
					check=false;
				if (check) buffer+=str[i];
				if((str[i]==char(32))||(str[i]==string("="))||(str[i+1]==string(">")))
				{
					if(CheckDeclaredElement(buffer)) 
					{
						string aux;
						textCtrl->SetDefaultStyle(styleE);
						for(int k=j;k<=i;k++)
						{
							aux+=str[k];
						}
						wxString sData(aux);
						textCtrl->WriteText(sData);
						buffer.erase();
						j=i;
					}
					else if(CheckDeclaredAttribute(buffer)) 
					{
						string aux;
						textCtrl->SetDefaultStyle(styleA);
						for(int k=j;k<=i-1;k++)
						{
							aux+=(str[k]);
						}
						wxString sData(aux);
						textCtrl->WriteText(sData);
						buffer.erase();
						j=i;
					}
					else
					{
						string aux;
						textCtrl->SetDefaultStyle(styleN);
						for(int k=j;k<=i;k++)
						{
							aux+=(str[k]);
						}
						wxString sData(aux);
						textCtrl->WriteText(sData);
						buffer.erase();
						j=i;
					}
				}
				i++;
			}
		}
		if(str[i]==string(">"))
		{	
			textCtrl->SetDefaultStyle(styleE);
			textCtrl->WriteText(str[i]);
			i=i+1;
			if((i+2)<str.size())
			{
				string aux;
				while(str[i]!=string("<"))
				{
					aux+=(str[i]);
						i++;
				}
				textCtrl->SetDefaultStyle(styleN);
				wxString sData(aux);
				textCtrl->WriteText(sData);
			}
		}
	}
}



bool PanelXML::CheckDeclaredElement(string ele)
{
	vector <string> elements;
	string world("World");
	string prismatic("PrismaticPart");
	string faceset("FaceSetPart");
	string vertex("vertex");
	string face("face");
	string spherepart("SpherePart"); 
	string colour("colour");
	string worldE("/World");
	string prismaticE("/PrismaticPart");
	string facesetE("/FaceSetPart");
	string vertexE("/vertex");
	string faceE("/face");
	string spherepartE("/SpherePart"); 
	string colourE("/colour");
	string position("position");
	string positionE("/position");
	string orientation("orientation");
	string orientationE("/orientation");
	string SimpleJoint("SimpleJoint");
	string SimpleJointE("/SimpleJoint");
	elements.push_back(faceset);
	elements.push_back(prismatic);
	elements.push_back(world);
	elements.push_back(vertex);
	elements.push_back(face);
	elements.push_back(spherepart);
	elements.push_back(colour);
	elements.push_back(facesetE);
	elements.push_back(prismaticE);
	elements.push_back(worldE);
	elements.push_back(vertexE);
	elements.push_back(faceE);
	elements.push_back(spherepartE);
	elements.push_back(colourE);
	elements.push_back(position);
	elements.push_back(positionE);
	elements.push_back(orientation);
	elements.push_back(orientationE);
	elements.push_back(SimpleJoint);
	elements.push_back(SimpleJointE);

	for(int i=0;i<elements.size();i++)
	{
		if (!ele.compare(elements[i]))
			return true;
	}
}

bool PanelXML::CheckDeclaredAttribute(string attr)
{
	vector <string> attributes;
	string name("name");
	string height("height");
	string id("id");
	string red("r");
	string blue("b");
	string green("g");
	string radius("radius");
	string axis("axis");
	string factor("factor");
	string prisamtic("prismatic");
	string value("value");
	string vmax("vmax");
	string vmin("vmin");
	string offset("offset");
	attributes.push_back(name);
	attributes.push_back(height);
	attributes.push_back(id);
	attributes.push_back(red);
	attributes.push_back(blue);
	attributes.push_back(green);
	attributes.push_back(radius);
	attributes.push_back(axis);
	attributes.push_back(factor);
	attributes.push_back(prisamtic);
	attributes.push_back(value);
	attributes.push_back(vmax);
	attributes.push_back(vmin);
	attributes.push_back(offset);

	for(int i=0;i<attributes.size();i++)
	{
		if (!attr.compare(attributes[i]))
			return true;
	}
}


void PanelXML::CreateTree()
{
	treevisible=new Tree(panel,wxID_ANY);
	m_root = treevisible->AddRoot(wxT("Universe"), 0, 47, new TreeItemData(wxT("Root item")));
	treevisible->Parent(m_root);
	treevisible->XMLPanel=this;
	treevisible->m_mainWin=NULL;
}


void PanelXML::UpdateTreeView()
{
	treevisible->SetWorldsNum(0);
	treevisible->DeleteChildren(m_root);
	if(tree->m_mainWin->listWorlds.size()>0)
	{
		for(int i=0;i<tree->m_mainWin->listWorlds.size();i++)
			treevisible->GenerateSubTree(tree->m_mainWin->listWorlds[i]);
	}
	treevisible->Expand(treevisible->GetRootItem());
	treevisible->Expand(treevisible->GetLastChild(treevisible->GetRootItem()));
}


void PanelXML::SetInformationFromTree(NodeTree* nod,TypeNode type)
{
	char* XmlText;
	item=nod;
	if(type==N_World)
	{
		state=5;
		EntityName->Clear();
		XmlText=nod->pointer.world->CreateXMLText();
		string WorldName=nod->getSimu()->getName();
		for(int i=0;i<worldNames.size();i++)
		{
			if (WorldName==worldNames[i])
			{
				selectedWorld=i;
				selectW->SetStringSelection(WorldName);
			}
		}
	}

	else
	{
		state=4;
		XmlText=nod->pointer.positionableentity->CreateXMLText();
		string name=nod->pointer.positionableentity->getName();
		updateEntity(name);
		string WorldName=nod->getSimu()->getName();
		for(int i=0;i<worldNames.size();i++)
		{
			if (WorldName==worldNames[i])
			{
				selectedWorld=i;
				selectW->SetStringSelection(WorldName);
				
			}
		}
	}
	UpdateObjetcsList();
	TreatXMLText(XmlText);
}




