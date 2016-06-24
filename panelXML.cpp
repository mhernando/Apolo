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
	EVT_TEXT_ENTER(ID_TEXTCHANGE,PanelXML::UpdateText)
END_EVENT_TABLE()



PanelXML::PanelXML(wxWindow *parent,wxWindowID id,const wxString& title,Tree* tr)
: wxFrame(parent,wxID_ANY,title,wxDefaultPosition, wxSize(990,680),wxSTAY_ON_TOP | wxCAPTION | wxCLOSE_BOX)
{
	panel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(990,680));
	tree=tr;
	CreatePanel();
	state=0;
	SetIcon(wxIcon(xmlwindow_xpm));
	selectedWorld=0;
}



void PanelXML::CreatePanel()
{
	CreateTree();
	panel->SetBackgroundColour(*wxWHITE);
	wxBitmap icons[3];
	icons[0]=wxBitmap(selworldxml_xpm);
	icons[1]=wxBitmap(Objects_xpm);
	icons[2]=wxBitmap(itemSelected_xpm);
	wxFont font(9,wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxFont EntFont(10,wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxBOLD,false);
	wxBoxSizer *TreeSizer=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *Select=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *Tools=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *ele=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BOX=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *L=new wxBoxSizer(wxVERTICAL);
	edit=new wxBoxSizer(wxHORIZONTAL);

	ItemsList=new wxListCtrl(panel, ID_SELECTLISTEDITEM, wxDefaultPosition, wxSize(240,680), wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES | wxLC_VRULES);
	wxFont fontList(11,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxBOLD);
	ItemsList->SetFont(font);
	wxListItem col0;
    col0.SetId(0);
    col0.SetText( _("Name") );
    col0.SetWidth(130);
    ItemsList->InsertColumn(0, col0);
    wxListItem col1;
    col1.SetId(1);
    col1.SetText( _("Value") );
	col1.SetWidth(110);
    ItemsList->InsertColumn(1, col1);
		
	staticonworld=new wxBitmapButton(panel,ID_SHOWWORLDXML,icons[0],wxDefaultPosition,wxSize(30,30));
	selectW=new wxComboBox();
	wxStaticBitmap* entityIcon=new wxStaticBitmap(panel,wxID_ANY,icons[2],wxDefaultPosition,wxSize(30,30));
	EntityName=new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(150,20),wxTE_READONLY);
	selectW->Create(panel,ID_SELECTW,wxEmptyString,wxDefaultPosition,wxSize(110,20),Choices,wxCB_DROPDOWN);
	InitToolbars();
	textCtrl=new wxTextCtrl(panel,ID_TEXTCHANGE,wxEmptyString,wxDefaultPosition,wxSize(510,620),wxTE_MULTILINE|wxHSCROLL|wxTE_RICH2|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
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
	Toolbar->AddTool(ID_UPDATEXML, "Update World", icons[0]);
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_VALIDATEXML,"Validate", icons[2]);
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_CREATEWORLDXML, "Create World", icons[1]);
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_ADDNEWENTITY, "Add Entity" , icons[3]);
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_SAVEXMLEDIT, "Save XML", icons[4]);
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
		UpdateList();
		UpdateWorldsList();
	}
	this->Show(true);
//	this->MakeModal(true);
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
//			this->MakeModal(false);
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
//			this->MakeModal(false);
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
//			this->MakeModal(false);
		}
	}
	
	if(state==3) //Creating a new Entity
	{
		wxString cont=textCtrl->GetValue();
		wxStringInputStream Stream(cont);
		wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
		if(m_pXmlDocument->Load(Stream))
		{
			XML x;
			wxCharBuffer buffer=cont.ToUTF8();
			char *xml=buffer.data();
			XMLElement* elem=x.Paste(xml);
			Object* obj=CreateEntity(elem);
			PositionableEntity* p_obj = dynamic_cast<PositionableEntity *>(obj);
			if(p_obj)
			{
				(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())+=p_obj;
				tree->m_mainWin->listWorlds[selectedWorld]->getChild()->UpdateWorld();
				tree->AddNode(p_obj, tree->m_mainWin->listWorlds[selectedWorld]->getTreeItem(),tree->m_mainWin->listWorlds[selectedWorld]);	
			}
			state=0;
			textCtrl->Clear();
			this->Show(false);
//			this->MakeModal(false);
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
//			this->MakeModal(false);
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
//			this->MakeModal(false);
		}
	}
}


Object* PanelXML::CreateEntity(XMLElement * elem)
{
	char className[50]={0};
	elem->GetElementName(className);
	Object*	pObj=Object::create(string(className));
	if(pObj)
	{
		pObj->readFromXML(elem);
		return pObj;
	}
	else 
		return 0;
}


void PanelXML::getItemXML()
{
		char* textXml=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem]->CreateXMLText();
		TreatXMLText(textXml);
		wxString name=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem]->getName();
		updateEntity(name);
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
		UpdateList();
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


void PanelXML::UpdateList()
{
	ItemsList->DeleteAllItems();
	if((state==5)||(state==0))
	{
		long itemIndex;
		int aux=0;
		for(int i=0;i<tree->m_mainWin->listWorlds[selectedWorld]->getWorld()->getNumObjects();i++)
		{
			wxString name=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[i]->getName();
			wxListItem nam;
			nam.SetId(0);
			nam.SetText(name);
			nam.SetWidth(130);
			if((aux % 2)==0) nam.SetBackgroundColour(wxColour(255,255,255));
			 else nam.SetBackgroundColour(wxColour(160,255,160));
			wxString Classname=(*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[i]->getClassName();
			itemIndex=ItemsList->InsertItem(nam); 
			ItemsList->SetItem(itemIndex, 1, Classname); 
			aux++;
		}
	}
	else if((state==4)||(state==2))
	{
		int aux=0;
		long itemIndex;
		for(int i=0;i<variables.size();i++)
		{
			wxString name=wxString(variables[i]);
			wxListItem nam;
			nam.SetId(0);
			nam.SetText(name);
			nam.SetWidth(130);
			if((aux % 2)==0) nam.SetBackgroundColour(wxColour(255,255,255));
			 else nam.SetBackgroundColour(wxColour(160,255,160));
			itemIndex=ItemsList->InsertItem(nam);
			wxString value=wxString(values[i]);
			ItemsList->SetItem(itemIndex,1,value);
			aux++;
		}
	}
}


void PanelXML::ParseXML(wxCommandEvent& event)
{
	wxString cont=textCtrl->GetValue();
	wxStringInputStream Stream(cont);
	wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
	if(m_pXmlDocument->Load(Stream))
	{
		wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Parsing XML Correct"), wxT("Parser XML"), wxOK | wxICON_INFORMATION);
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
				wxString cad;
				cad=wxT("<root>");
				cad+=cont;
				cad+=wxT("</root>");
				wxStringInputStream Stream(cad);
				wxXmlDocument* m_pXmlDocument = new wxXmlDocument();
				m_pXmlDocument->Load(Stream);
				m_pXmlDocument->Save(filec);
				wxLogMessage(wxT("Successfully save in %s"), saveFile.GetPath().c_str());
			}
}



void PanelXML::ManageState()
{
	if(state<0) return;
	if(state>5) return;
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
//	this->MakeModal(false);
	tree->m_mainWin->SetState(0);
}


void PanelXML::updateEntity(wxString name)
{
	EntityName->Clear();
	EntityName->AppendText(name);
	EntityName->SetBackgroundColour(wxColour(135,255,135));
}


void PanelXML::SelectItemInList(wxListEvent& event)
{
	int Itemsnum;
	Itemsnum=ItemsList->GetItemCount();
	int id=event.GetId();
	if((state==0)||(state==5))
	{
		if (id==ID_SELECTLISTEDITEM)
		{
			long itemIndex=-1;
			itemIndex = ItemsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (itemIndex == -1) return;
			state=2; 
			selectedItem=Itemsnum-itemIndex-1;
			getItemXML();
			GetElementVariables();
		}
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
				if((str[i]==char(10))||(str[i]==char(13))||(str[i]==char(32))||(str[i]==string("="))||(str[i]==char(9)))
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
						j=i+1;
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
						j=i+1;
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
	string world("World"); elements.push_back(world);
	string worldE("/World"); elements.push_back(worldE);
	string prismatic("PrismaticPart"); 	elements.push_back(prismatic);
	string prismaticE("/PrismaticPart"); elements.push_back(prismaticE);
	string faceset("FaceSetPart"); elements.push_back(faceset);
	string facesetE("/FaceSetPart"); elements.push_back(facesetE);
	string vertex("vertex"); elements.push_back(vertex);
	string vertexE("/vertex"); elements.push_back(vertexE);
	string face("face"); elements.push_back(face);
	string faceE("/face"); elements.push_back(faceE);
	string spherepart("SpherePart"); elements.push_back(spherepart);
	string spherepartE("/SpherePart"); elements.push_back(spherepartE);
	string colour("colour"); elements.push_back(colour);
	string colourE("/colour"); elements.push_back(colourE);
	string position("position"); elements.push_back(position);
	string positionE("/position"); elements.push_back(positionE);
	string orientation("orientation"); elements.push_back(orientation);
	string orientationE("/orientation"); elements.push_back(orientationE);
	string SimpleJoint("SimpleJoint"); elements.push_back(SimpleJoint);
	string SimpleJointE("/SimpleJoint"); elements.push_back(SimpleJointE);
	string CylindricalPart("CylindricalPart");elements.push_back(CylindricalPart);
	string CylindricalPartE("/CylindricalPart");elements.push_back(CylindricalPartE);
	string Asea("AseaIRB2000Sim");	elements.push_back(Asea);
	string AseaE("/AseaIRB2000Sim"); elements.push_back(AseaE);
	string Puma("Puma560Sim"); elements.push_back(Puma);
	string PumaE("/Puma560Sim"); elements.push_back(PumaE);
	string Composed("ComposedEntity"); elements.push_back(Composed);
	string ComposedE("/ComposedEntity"); elements.push_back(ComposedE);
	string Scara("AdeptOneSim"); elements.push_back(Scara);
	string ScaraE("/AdeptOneSim"); elements.push_back(ScaraE);
	string RobotSim("RobotSim"); elements.push_back(RobotSim);
	string RobotSimE("/RobotSim");elements.push_back(RobotSimE);
	string Tcp("Tcp");elements.push_back(Tcp);
	string TcpE("/Tcp");elements.push_back(TcpE);
	string Actuator("Actuator");elements.push_back(Actuator);
	string ActuatorE("/Actuator");elements.push_back(ActuatorE);


	for(int i=0;i<elements.size();i++)
	{
		if (!ele.compare(elements[i]))
			return true;
	}
}

bool PanelXML::CheckDeclaredAttribute(string attr)
{
	vector <string> attributes;
	string name("name"); attributes.push_back(name);
	string height("height"); attributes.push_back(height);
	string id("id"); attributes.push_back(id);
	string red("r"); attributes.push_back(red);
	string blue("b"); attributes.push_back(blue);
	string green("g"); attributes.push_back(green);
	string radius("radius"); attributes.push_back(radius);
	string axis("axis"); attributes.push_back(axis);
	string factor("factor"); attributes.push_back(factor);
	string prisamtic("prismatic"); attributes.push_back(prisamtic);
	string value("value"); attributes.push_back(value);
	string vmax("vmax"); attributes.push_back(vmax);
	string vmin("vmin");attributes.push_back(vmin);
	string offset("offset"); attributes.push_back(offset);
	string linkto("linkTo"); attributes.push_back(linkto);
	string intersectable("intersectable"); attributes.push_back(intersectable);
	string speed("speed"); attributes.push_back(speed);
	string maxSpeed("maxSpeed"); attributes.push_back(maxSpeed);
	string meridian("meridian"); attributes.push_back(meridian);
	string parallel("parallel"); attributes.push_back(parallel);
	string rotSpeed("rotSpeed"); attributes.push_back(rotSpeed);
	string startAngle("startAngle"); attributes.push_back(startAngle);
	string stepAngle("stepAngle"); attributes.push_back(stepAngle);
	string numSteps("numSteps"); attributes.push_back(numSteps);
	string maxRange("maxRange"); attributes.push_back(maxRange);
	string sigma("sigma"); attributes.push_back(sigma);




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
	if(tree->m_mainWin->listWorlds.size()>0)
	{
		treevisible->SetWorldsNum(0);
		treevisible->DeleteChildren(m_root);

		for(int i=0;i<tree->m_mainWin->listWorlds.size();i++)
			treevisible->GenerateSubTree(tree->m_mainWin->listWorlds[i]);

		treevisible->Expand(treevisible->GetRootItem());
		treevisible->Expand(treevisible->GetLastChild(treevisible->GetRootItem()));
	}
}


void PanelXML::SetInformationFromTree(NodeTree* nod,TypeNode type)
{
	char* XmlText;
	item=nod;
	if(type==N_World)
	{
		state=5;
		EntityName->Clear();
		ManageState();
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
		UpdateList();
	}

	else
	{
		state=4;
		string name;
		XmlText=nod->pointer.positionableentity->CreateXMLText();
		name=nod->pointer.positionableentity->getName();
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
		GetElementVariables();
	}
	TreatXMLText(XmlText);
}



void PanelXML::GetElementVariables()
{
	variables.clear();
	values.clear();
	XMLElement ** Children;
	XMLVariable** vectorVar;
	XMLElement* eleme=new XMLElement();
	if((state==4)&&(dynamic_cast<ComposedEntity *>(item->pointer.positionableentity)))  
	{
		ItemsList->DeleteAllItems();
		return;
	}
	else if ((state==2)&&(dynamic_cast<ComposedEntity *>((*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem])))
	{
		ItemsList->DeleteAllItems();
		return;
	}
	else if (state==4) item->pointer.positionableentity->writeToXML(eleme);
	else (*tree->m_mainWin->listWorlds[selectedWorld]->getWorld())[selectedItem]->writeToXML(eleme);
		
	vectorVar=eleme->GetVariables();
	for(int i=0;i<eleme->GetVariableNum();i++)
	{
		char* value=new char();
		char *variable=new char();
		vectorVar[i]->GetName(variable);
		variables.push_back(variable);
		vectorVar[i]->GetValue(value);
		values.push_back(value);
	}
	if(eleme->GetChildrenNum()!=0)
	{
		int k=0;
		Children=eleme->GetChildren();
		for(int i=0;i<eleme->GetChildrenNum();i++)
		{
			for(int j=0;j<Children[i]->GetVariableNum();j++)
			{
				vectorVar=Children[i]->GetVariables();
				char* value=new char();
				char *variable=new char();
				vectorVar[k]->GetName(variable);
				variables.push_back(variable);
				vectorVar[k]->GetValue(value);
				values.push_back(value);
				k++;
			}
		}
	}
	UpdateList();
}


void PanelXML::UpdateText(wxCommandEvent& event)
{
	long cursor=textCtrl->GetInsertionPoint();
	wxString cont=textCtrl->GetValue();
	wxStringInputStream Stream(cont);
	wxCharBuffer buffer=cont.ToUTF8();
	char *xml=buffer.data();
	TreatXMLText(xml);
	textCtrl->SetInsertionPoint(cursor);
}