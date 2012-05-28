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
	PositionableEntity *obj;
	
	if(id==ID_ADDSPHERE)
		obj=new SpherePart;
	if(id==ID_ADDCYL)
		obj=new CylindricalPart;
	if(id==ID_ADDPRI)
		obj=new PrismaticPart;
	if(id==ID_ADDFACE)
		obj=new FaceSetPart;
	
	(*m_world)+=obj;
	
	InitialProperties *ini= new InitialProperties(mainWin,id,obj,this,wxT("Properties")); 
	ini->ShowModal();

	
	if(ini->GetButtom()==1)
	{	
		listObjects.push_back(obj);
		objectNodes.push_back(tree->AddNode(obj,mainNode));
	}
		
	if(ini->GetButtom()==2)
		delete obj;
		
	
	
	tree->ExpandAll();
	
	childView->UpdateWorld();
	childView ->RefreshChild();
}

void SimulatedWorld::DeleteObject(wxTreeItemId itemId)
{
	
	for(int i=0 ;i<listObjects.size();i++)
	{
		if(objectNodes[i]==itemId)
		{
			delete listObjects[i];
			listObjects.erase(listObjects.begin()+i);
			tree->Delete(objectNodes[i]);
			objectNodes.erase(objectNodes.begin()+i);	
			
		}
	}
	childView->UpdateWorld();

}
	


	


SimulatedWorld::~SimulatedWorld()
{
	childView ->Destroy();
	tree->Delete(mainNode);
	delete m_world;

}