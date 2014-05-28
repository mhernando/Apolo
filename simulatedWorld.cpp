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
	UpdateLinks();

}


void SimulatedWorld::AddObject(wxWindowID  	id)
{	

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
	InitialProperties *ini= new InitialProperties(mainWin,newNode,wxT("Properties"),id); 
	ini->ShowModal();
	


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
	
	if(parentItem!=mainNode)
		parentData->pointer.composedentity->erase(parentData->pointer.composedentity->getIndexOf(objectData->pointer.positionableentity));
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



//////////////////////////////////////////////////////////////////
/////////////Nuevas funciones de linkado/////////////


void SimulatedWorld::InsertLinkerEntity (PositionableEntity* obj,wxTreeItemId Item)
{
	if(LinkersPos.size()>0)
	{
		for(int i=0;i<LinkersPos.size();i++)
		{
			if(obj==LinkersPos[i]) 
			{	
				return;
			}
		}
	}
	LinkersPos.push_back(obj);
	vector<PositionableEntity*> PosAux;
	LinksPos.push_back(PosAux);
	LinkersId.push_back(Item);
	vector<wxTreeItemId> aux;
	LinksId.push_back(aux);
	return;
}


void SimulatedWorld::InsertLinkedEntity(PositionableEntity* linker,PositionableEntity* linked,wxTreeItemId ItemLinked)
{
	for(int i=0;i<LinkersPos.size();i++)
	{
		if (linker==LinkersPos[i])
		{
			LinksPos[i].push_back(linked);
			LinksId[i].push_back(ItemLinked);
		}
	}
}


int SimulatedWorld::getLinkerPositionable(PositionableEntity* el)
{
	if(LinkersPos.size()>0)
	{
		for(int i=0;i<LinkersPos.size();i++)
		{
			if(el==LinkersPos[i]) return i;
		}
	}
	return -1;
}



void SimulatedWorld::EraseLinked(PositionableEntity* pos,wxTreeItemId item)
{
	int aux;
	for(int i=0;i<LinksPos.size();i++)
	{
		for(int j=0;j<LinksPos[i].size();j++)
		{
			if(LinksPos[i][j]==pos)
			{
				aux=i;
				LinksPos[i].erase(LinksPos[i].begin()+j);
				LinksId[i].erase(LinksId[i].begin()+j);
			}
		}
	}
	
	if(LinksPos[aux].size()==0)
	{
		LinkersPos.erase(LinkersPos.begin()+aux);
		LinkersId.erase(LinkersId.begin()+aux);
		LinksPos.erase(LinksPos.begin()+aux);
		LinksId.erase(LinksId.begin()+aux);
	}
	
}


wxTreeItemId SimulatedWorld::getPositionableId(PositionableEntity* pos)
{
	int aux=-1;
	if (LinkersPos.size()>0)
	{
		for(int i=0;i<LinkersPos.size();i++)
		{
			if(LinkersPos[i]==pos) return LinkersId[i];
		}
	}
	//////El elemento no era un linker
	
	if(LinksPos.size()>0)
	{
		for(int i=0;i<LinksPos.size();i++)
		{
			for(int j=0;j<LinksPos[i].size();j++)
			{
				if(pos==LinksPos[i][j]) return LinksId[i][j];
			}
		}
	}
}



bool SimulatedWorld::CheckItemLinked(PositionableEntity* pos)
{
	for(int i=0;i<LinksPos.size();i++)
	{
		for(int j=0;j<LinksPos[i].size();j++)
		{
			if(pos==LinksPos[i][j]) return true;
		}
	}
	return false;
}



//Función encargada de obtener el id del nodo en el arbol correspondiente a ese entity 
wxTreeItemId SimulatedWorld::getLoadedObjectId(PositionableEntity* pos)
{
	for(int i=0;i<m_world->getNumObjects();i++)
	{
		if(pos==(*m_world)[i]) return LoadedIds[i];
	}
}



void SimulatedWorld::UpdateLinks()
{
	bool added;
	for(int x=0;x<m_world->getNumObjects();x++)
	{
		added=false;
		PositionableEntity* pos=(*m_world)[x];
		if(pos->getLinkedTo()!=NULL)
		{
			if(LinkersPos.size()>0)
			{
				for(int i=0;i<LinkersPos.size();i++)
				{
					if(pos->getLinkedTo()==LinkersPos[i])
					{
						LinksPos[i].push_back(pos);	
						LinksId[i].push_back(LoadedIds[x]);
						added=true;
					}
				}
				if (added==false)
				{
					LinkersPos.push_back(pos->getLinkedTo());
					LinkersId.push_back(getLoadedObjectId(pos->getLinkedTo()));
					vector<PositionableEntity*> PosAux;
					LinksPos.push_back(PosAux);
					LinksPos[LinksPos.size()-1].push_back(pos);
					vector<wxTreeItemId> IdAux;
					LinksId.push_back(IdAux);
					LinksId[LinksId.size()-1].push_back(LoadedIds[x]);
				}
			}
			else
			{
				string name=pos->getLinkedTo()->getClassName();
				if (name=="Tcp")
				{
					EntitySet* Lnk;
					Lnk=pos->getLinkedTo()->getOwner();
					PositionableEntity* owner = dynamic_cast<PositionableEntity *>(Lnk);
					LinkersPos.push_back(owner);
					LinkersId.push_back(getLoadedObjectId(owner));
					vector<PositionableEntity*> PosAux;
					LinksPos.push_back(PosAux);
					LinksPos[LinksPos.size()-1].push_back(pos);
					vector<wxTreeItemId> idlinked;
					LinksId.push_back(idlinked);
					LinksId[LinksId.size()-1].push_back(LoadedIds[x]);
				}
				else
				{
					LinkersPos.push_back(pos->getLinkedTo());
					LinkersId.push_back(getLoadedObjectId(pos->getLinkedTo()));
					vector<PositionableEntity*> PosAux;
					LinksPos.push_back(PosAux);
					LinksPos[LinksPos.size()-1].push_back(pos);
					vector<wxTreeItemId> idlinked;
					LinksId.push_back(idlinked);
					LinksId[LinksId.size()-1].push_back(LoadedIds[x]);
				}
			}
		}
	}
}


PositionableEntity* SimulatedWorld::getLinker(PositionableEntity* linked)
{
	for(int i=0;i<LinksPos.size();i++)
	{
		for(int j=0;j<LinksPos[i].size();j++)
		{
			if(LinksPos[i][j]==linked)
			return LinkersPos[i];
		}
	}
}











