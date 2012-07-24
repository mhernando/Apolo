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
	wxTreeItemId composedItem=tree->GetSelection();
	NodeTree *itemData = composedItem.IsOk() ? (NodeTree *) tree->GetItemData(composedItem):NULL;


// Object Selected////

	if(id==ID_ADDSPHERE)
		obj->pointer.positionableentity=new SpherePart;
	if(id==ID_ADDCYL)
		obj->pointer.positionableentity=new CylindricalPart;
	if(id==ID_ADDPRI)
		obj->pointer.positionableentity=new PrismaticPart;
	if(id==ID_ADDFACE)
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

	// Object addition or world addition//
	
	if(tree->GetSelection()!=mainNode)
		itemData->pointer.composedentity->addObject(obj->pointer.positionableentity);
	else
		(*m_world)+=obj->pointer.positionableentity;
	
	
	// Initial Properties //
	InitialProperties *ini= new InitialProperties(mainWin,obj,this,wxT("Properties")); 
	ini->ShowModal();


	if(ini->GetButtom())
	{
		if(tree->GetSelection()!=mainNode)
		{
			tree->AddNode(itemData->pointer.composedentity,tree->GetItemParent(tree->GetSelection()));
			tree->Delete(composedItem);
		}
	
		else
			tree->AddNode(obj->pointer.positionableentity,tree->GetSelection());
	}	
	else
		delete obj->pointer.positionableentity;
		
		
	tree->Expand(tree->GetSelection());
	
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