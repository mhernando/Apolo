#include "simulatedWorld.h"


MainWindow * SimulatedWorld::mainWin = 0;
Tree * SimulatedWorld::tree = 0;

SimulatedWorld::SimulatedWorld(World *world)
{
	static int numWorld = 0;
	objConnected=false;
	
	wxString defName=wxT("World");
	defName.Printf(wxT("%s %d"),defName, ++numWorld);
	name=defName.ToStdString();
	m_world = world;
	
	childView = new ChildView(mainWin, wxT("WORLD"),this);
	childView ->SetTitle(name);
	childView ->SetIcon(wxIcon(worldSelect_xpm));
	childView ->UpdateWorld();
	childView ->RefreshChild();
	mainNode = tree->GenerateSubTree(this);
	tree->UnselectAll();
	tree->SelectItem(mainNode);
}


void SimulatedWorld::AddObject(wxWindowID  	id)
{	
	InitialProperties *ini;
	PositionableEntity *obj=NULL;
	NodeTree *newNode=NULL;
	NodeTree *itemData = tree->GetSelection().IsOk() ? (NodeTree *) tree->GetItemData(tree->GetSelection()):NULL;


// Object Selected////

	if(id==ID_ADDSPHERE)
		obj=new SpherePart;
	else if(id==ID_ADDCYL)
		obj=new CylindricalPart;
	else if(id==ID_ADDPRI)
		obj=new PrismaticPart;
    else if(id==ID_ADDIRRPRI)
		obj=new PrismaticPart;
	else if(id==ID_ADDFACESET)
		obj=new FaceSetPart;
	else if(id==ID_ADDNEO)
		obj=new  Pioneer3ATSim;
	else if(id==ID_ADDSCARA)
		obj=new AdeptOneSim;
	else if(id==ID_ADDPUMA)
		obj=new  Puma560Sim;
	else if(id==ID_ADDASEA)
		obj=new AseaIRB2000Sim;
	else if(id==ID_ADDCUSTOM)
		obj=new ComposedEntity;
	else if(id==ID_WHEEL)
		obj=new WheeledBaseSim;
	else if(id==ID_LMS200)
		obj=new LMS200Sim;
	else if(id==ID_PATROL)
		obj=new PatrolbotSim;
	else if(id==ID_LMS100)
		obj=new LMS100Sim;
	else if(id==ID_POWERCUBE)
		obj=new PowerCube70Sim;
	else if(id==ID_CAMERA)
		obj=new CameraSim;
	else if(id==ID_KINECT)
		obj=new KinectSim;
	else if(id==ID_MOBILEROBOT)
		obj=new MobileRobot("");
	else if(id==ID_QUADROTOR)
		obj=new QuadrotorSim;
	else if(id==ID_PERSON)
		obj=new PersonSim;
	else if(id==ID_LASER)
		obj=new LaserSensorSim;
	else if(id==ID_LASER3D)
		obj=new LaserSensor3DSim;
	else if(id==ID_NEMOLASER)
		obj=new NemoLaserSensor3DSim;
	else if(id==ID_EUITIBOT)
		obj=new EUITIbotSim;
	else if(id==ID_ADDJOINT)
	{
		obj=AddJoint();
	}
	else 
		obj=new PositionableEntity;


	
	// Object addition or world addition//
	
	if(tree->GetSelection()!=mainNode)
	{
		newNode=tree->AddNode(obj,tree->GetSelection(),this);
		itemData->pointer.composedentity->addObject(obj);
	}
	else
	{
		(*m_world)+=obj;
		newNode=tree->AddNode(obj,tree->GetSelection(),this);
	}
	
	tree->Expand(tree->GetSelection());




	if(id==ID_ADDIRRPRI)
	{
		newNode->setTipo(N_IrregularPrismaticPart);
		newNode->setName(wxT("Irregular Prism"));
		tree->SetItemText(tree->GetLastChild(tree->GetSelection()),wxT("Irregular Prism"));
	}
	// Initial Properties //
	if(id==ID_ADDCUSTOM)
	{
		CompOption=new wxMessageDialog(NULL, wxT("Use the interface to build it?"),wxT("Composed Design"),wxYES_NO,wxDefaultPosition);
		if(CompOption->ShowModal()==wxID_YES)
		{
			ini= new InitialProperties(mainWin,newNode,wxT("Properties"),id); 
			ini->ShowModal();
		}
		else
		{
			childView->UpdateWorld();
			return;
		}
	}
	else if (id==ID_ADDJOINT)
	{
		return;
	}
	else
	{
		ini= new InitialProperties(mainWin,newNode,wxT("Properties"),id); 
		ini->ShowModal();
	}


	if(ini->GetButtom()==false)
	{
		delete obj;
		tree->Delete(tree->GetLastChild(tree->GetSelection()));
	}
	childView->UpdateWorld();
	delete ini;

}


void SimulatedWorld::DeleteObject(wxTreeItemId itemId)
{
	wxTreeItemId parentItem=tree->GetItemParent(itemId);
	wxTreeItemId objectItem=itemId;
	NodeTree *objectData = objectItem.IsOk() ? (NodeTree *) tree->GetItemData(objectItem):NULL;
	NodeTree *parentData = parentItem.IsOk() ? (NodeTree *) tree->GetItemData(parentItem):NULL;

	if(objectData->pointer.positionableentity->getLinkedTo()!=NULL)
		objectData->pointer.positionableentity->LinkTo(NULL);

	for(int i=0;i<Items.size();i++)
	{
		if(Items[i]->getLinkedTo()==objectData->pointer.positionableentity)
			Items[i]->LinkTo(NULL);
	}

	for(int i=0;i<Items.size();i++)
	{
		if(Items[i]==objectData->pointer.positionableentity)
		{
			Items.erase(Items.begin()+i);
			ItemsId.erase(ItemsId.begin()+i);
		}
	}

	if(parentItem!=mainNode)
	{
		parentData->pointer.composedentity->remove(objectData->pointer.positionableentity);
	}
	else 
		delete objectData->pointer.positionableentity;


	tree->Delete(objectItem);
	
	childView->UpdateWorld();
	
}


void SimulatedWorld::SetCopiedDesign(vector<Vector2D> Copy)
{
	if (Copy.size()>0) 
	{
		for(int i=0;i<Copy.size();i++)
		{
			CopiedDesign.push_back(Vector2D(Copy[i].x,Copy[i].y));
		}
	}
}


SimulatedWorld::~SimulatedWorld()
{
	tree->Delete(mainNode);
	childView->Destroy();	
	delete m_world;
}



bool SimulatedWorld::checkAddedItem(PositionableEntity* pos)
{
	if(Items.size()>0)
	{
		for(int i=0;i<Items.size();i++)
		{
			if(pos==Items[i]) return true;
		}
	}
	return false;
}


void SimulatedWorld::InitializeItemsVectors()
{
	ItemsId.clear();
	Items.clear();
}


bool SimulatedWorld::checkLink(PositionableEntity* linker,PositionableEntity* linked)
{
	bool aux1,aux2;
	aux1=false;
	aux2=false;
	for(int i=0;i<Items.size();i++)
	{
		if(linker==Items[i])aux1=true;
	}
	for(int i=0;i<Items.size();i++)
	{
		if(linked==Items[i])aux2=true;
	}
	if((aux1==true)&&(aux2==true))
	{
		if((linked->getOwner()==NULL)||(linker->getLinkedTo()==NULL)) return true; 
		else if(linker->getOwner()!=linked->getOwner()) return false;
		else if(linker->getLinkedTo()==linked) return false;
		else if(linker->getOwner()!=linker->getLinkedTo()->getOwner()) return true;
		else checkLink(linker->getLinkedTo(),linked);
	}
	else return false;
}



SimpleJoint* SimulatedWorld::AddJoint()
{
	SimpleJoint* createdJoint;
	wxArrayString options; wxString option1("Prismatic"); wxString option2("rotational");
	options.Add(option1); options.Add(option2);
	wxArrayString AxisOptions; wxString AxisOption1("X_AXIS"); wxString AxisOption2("Y_AXIS");wxString AxisOption3("Z_AXIS");
	AxisOptions.Add(AxisOption1);AxisOptions.Add(AxisOption2);AxisOptions.Add(AxisOption3);
	bool prismatic=false; int axis=0;double vmin=-PI; double vmax=PI;
	wxString Type;
	wxSingleChoiceDialog myDialog(NULL,wxString("Select joint type"),wxString("Options"),options);
	if (myDialog.ShowModal()==wxID_OK )
	{
		if(myDialog.GetStringSelection()==wxString("Prismatic")) prismatic=true;
			else prismatic=false;
	}
	wxTextEntryDialog myDialogVmax(NULL,wxString("Introduce Vmax"),wxGetTextFromUserPromptStr,wxEmptyString,wxTextEntryDialogStyle,wxDefaultPosition);
	if (myDialogVmax.ShowModal()==wxID_OK )
	{
		wxString value=myDialogVmax.GetValue();
		value.ToDouble(&vmax);
	}
	wxTextEntryDialog myDialogVmin(NULL,wxString("Introduce Vmin"),wxGetTextFromUserPromptStr,wxEmptyString,wxTextEntryDialogStyle,wxDefaultPosition);
	if (myDialogVmin.ShowModal()==wxID_OK )
	{
			wxString value=myDialogVmin.GetValue();
			value.ToDouble(&vmin);
			if(vmin>=vmax)
			{
				wxMessageDialog errorDialog(NULL, wxString("Wrong Value-SimpleJoint will be created with default values") ,wxMessageBoxCaptionStr,wxOK|wxCENTRE,wxDefaultPosition);
				errorDialog.ShowModal();
				vmin=-PI;
				vmax=PI;
			}
	}
	wxSingleChoiceDialog myDialog2(NULL,wxString("Select Axis"),wxString("Options"),AxisOptions);
	if (myDialog2.ShowModal()==wxID_OK )
	{
		if(myDialog2.GetStringSelection()==wxString("X_AXIS")) axis=1;
		if(myDialog2.GetStringSelection()==wxString("Y_AXIS")) axis=2;
		else axis=3;
	}

	if (axis==1) createdJoint=new SimpleJoint(vmax,vmin,true,0,X_AXIS,prismatic);
	else if(axis==2) createdJoint=new SimpleJoint(vmax,vmin,true,0,Y_AXIS,prismatic);
	else createdJoint=new SimpleJoint(vmax,vmin,true,0,Z_AXIS,prismatic);

	return createdJoint;
}










