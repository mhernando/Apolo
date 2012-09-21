#include "simulatedWorld.h"



MainWindow * SimulatedWorld::mainWin = 0;
Tree * SimulatedWorld::tree = 0;

SimulatedWorld::SimulatedWorld(World *world)
{
	static int numWorld = 0;
	childView = new ChildView(mainWin, wxT("WORLD"), world);
	wxString defName=wxT("World");
	defName.Printf(wxT("%s %d"),defName, ++numWorld);
	name=defName.ToStdString();

	
	childView ->SetTitle(name);
	childView ->SetIcon(wxIcon(worldSelect_xpm));
	childView ->UpdateWorld();
	childView ->RefreshChild();
	mainNode = tree->GenerateSubTree(world,this);

	m_world = world;

}


void SimulatedWorld::AddObject(wxWindowID  	id)
{	

	NodeTree *obj;
	NodeTree *itemData = tree->GetSelection().IsOk() ? (NodeTree *) tree->GetItemData(tree->GetSelection()):NULL;


// Object Selected////

	if(id==ID_ADDSPHERE)
		obj=new NodeTree(new SpherePart,this);
	else if(id==ID_ADDCYL)
		obj=new NodeTree(new CylindricalPart,this);
	else if(id==ID_ADDPRI)
		obj=new NodeTree(new PrismaticPart,this);
    else if(id==ID_ADDIRRPRI)
	{	obj=new NodeTree(new PrismaticPart,this);obj->setTipo(N_IrregularPrismaticPart);}
	else if(id==ID_ADDFACESET)
		obj=new NodeTree(new FaceSetPart,this);
	else if(id==ID_ADDNEO)
		obj=new NodeTree(new  Pioneer3ATSim,this);
	else if(id==ID_ADDSCARA)
		obj=new NodeTree(new AdeptOneSim,this);
	else if(id==ID_ADDPUMA)
		obj=new NodeTree(new  Puma560Sim,this);
	else if(id==ID_ADDASEA)
		obj=new NodeTree(new AseaIRB2000Sim,this);
	else if(id==ID_ADDCUSTOM)
		obj=new NodeTree(new ComposedEntity,this);
	else if(id==ID_WHEEL)
		obj=new NodeTree(new WheeledBaseSim,this);
	else if(id==ID_LMS200)
		obj=new NodeTree(new LMS200Sim,this);
	else if(id==ID_PATROL)
		obj=new NodeTree(new PatrolbotSim,this);
	else if(id==ID_LMS100)
		obj=new NodeTree(new LMS100Sim,this);
	else if(id==ID_POWERCUBE)
		obj=new NodeTree(new PowerCube70Sim,this);
	else if(id==ID_CAMERA)
		obj=new NodeTree(new CameraSim,this);
	else if(id==ID_KINECT)
		obj=new NodeTree(new KinectSim,this);
	else if(id==ID_MOBILEROBOT)
		obj=new NodeTree(new MobileRobot(""),this);
	else if(id==ID_QUADROTOR)
		obj=new NodeTree(new QuadrotorSim,this);
	else if(id==ID_PERSON)
		obj=new NodeTree(new PersonSim,this);
	else if(id==ID_LASER)
		obj=new NodeTree(new LaserSensorSim,this);
	else if(id==ID_LASER3D)
		obj=new NodeTree(new LaserSensor3DSim,this);
	else if(id==ID_NEMOLASER)
		obj=new NodeTree(new NemoLaserSensor3DSim,this);
		
	else 
		obj=new NodeTree(new PositionableEntity,this);

	// Object addition or world addition//
	
	if(tree->GetSelection()!=mainNode)
	{
		newNode=tree->AddNode(obj->pointer.positionableentity,tree->GetSelection(),this);
		itemData->pointer.composedentity->addObject(obj->pointer.positionableentity);
	}
	else
	{
		(*m_world)+=obj->pointer.positionableentity;
		newNode=tree->AddNode(obj->pointer.positionableentity,tree->GetSelection(),this);
	}
	
	tree->Expand(tree->GetSelection());




	// Initial Properties //
	InitialProperties *ini= new InitialProperties(mainWin,obj,wxT("Properties"),id); 
	ini->ShowModal();
	


	if(ini->GetButtom()==false)
	{
		delete obj->pointer.positionableentity;
		delete obj;
		tree->Delete(newNode);
	}
	childView->UpdateWorld();

	
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
	childView ->Destroy();
	tree->Delete(mainNode);
	delete m_world;

}