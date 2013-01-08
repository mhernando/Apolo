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

SimulatedWorld::~SimulatedWorld()
{
	tree->Delete(mainNode);
	childView->Destroy();	
	delete m_world;

}