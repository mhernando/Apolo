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
SimulatedWorld::~SimulatedWorld()
{
	childView ->Destroy();
	tree->Delete(mainNode);
	delete m_world;

}