#include "simulatedWorld.h"
#include "mainWindow.h"


MainWindow * SimulatedWorld::mainWin = 0;
Tree * SimulatedWorld::tree = 0;

SimulatedWorld::SimulatedWorld(World *world)
{
	static int numWorld = 0;
	childView = new ChildView(mainWin, wxT("WORLD"), world);
	mainNode = tree->GenerateSubTree(world,this);
	

	m_world = world;
	
	
	wxString title;
	title.Printf(wxT("World %d"), ++numWorld);
	childView ->SetTitle(title);
	childView ->SetIcon(wxIcon(worldSelect_xpm));
	childView ->UpdateWorld();
	childView ->RefreshChild();

	
}

void SimulatedWorld::AddObject(wxWindowID  	id)
{	

	NodeTree *obj=new NodeTree(new PositionableEntity);
	NodeTree *itemData = tree->GetSelection().IsOk() ? (NodeTree *) tree->GetItemData(tree->GetSelection()):NULL;


// Object Selected////

	if(id==ID_ADDSPHERE)
		obj->pointer.positionableentity=new SpherePart;
	if(id==ID_ADDCYL)
		obj->pointer.positionableentity=new CylindricalPart;
	if(id==ID_ADDPRI)
		obj->pointer.positionableentity=new PrismaticPart;
	if(id==ID_ADDFACESET)
		obj->pointer.positionableentity=new FaceSetPart;
	if(id==ID_ADDNEO)
		obj->pointer.positionableentity=new  Pioneer3ATSim;
	if(id==ID_ADDSCARA)
		obj->pointer.positionableentity=new AdeptOneSim;
	if(id==ID_ADDPUMA)
		obj->pointer.positionableentity=new  Puma560Sim;
	if(id==ID_ADDASEA)
		obj->pointer.positionableentity=new AseaIRB2000Sim;
	if(id==ID_ADDCUSTOM)
		obj->pointer.positionableentity=new ComposedEntity;
	if(id==ID_WHEEL)
		obj->pointer.positionableentity=new WheeledBaseSim;
	if(id==ID_LMS200)
		obj->pointer.positionableentity=new LMS200Sim;
	if(id==ID_PATROL)
		obj->pointer.positionableentity=new PatrolbotSim;
	if(id==ID_LMS100)
		obj->pointer.positionableentity=new LMS100Sim;
	if(id==ID_POWERCUBE)
		obj->pointer.positionableentity=new PowerCube70Sim;
	if(id==ID_LASER)
		obj->pointer.positionableentity=new LaserSensorSim;
	if(id==ID_LASER3D)
		obj->pointer.positionableentity=new LaserSensor3DSim;
	if(id==ID_NEMOLASER)
		obj->pointer.positionableentity=new NemoLaserSensor3DSim;

	// Object addition or world addition//
	
	if(tree->GetSelection()!=mainNode)
	{
		newNode=tree->AddNode(obj->pointer.positionableentity,tree->GetSelection());
		itemData->pointer.composedentity->addObject(obj->pointer.positionableentity);
	}
	else
	{
		(*m_world)+=obj->pointer.positionableentity;
		newNode=tree->AddNode(obj->pointer.positionableentity,tree->GetSelection());
	}
	
	tree->Expand(tree->GetSelection());




	// Initial Properties //
	InitialProperties *ini= new InitialProperties(mainWin,obj,this,wxT("Properties")); 
	ini->ShowModal();


	if(ini->GetButtom()==false)
	{
		delete obj->pointer.positionableentity;
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
		parentData->pointer.composedentity->remove(objectData->pointer.positionableentity);//->erase(parentData->pointer.composedentity->getIndexOf(objectData->pointer.positionableentity));
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