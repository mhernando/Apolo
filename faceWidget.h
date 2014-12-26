#ifndef __APOLO__FACEWIDGET__H
#define __APOLO__FACEWIDGET__H

#include "mainWindow.h"
#include <wx/wx.h>
#include <wx/splitter.h>
#include "simulatedWorld.h"
#include "pointsList.h"
#include "canvas.h"
#include "mrcore.h"
#include <math.h>
#include <conio.h>
#include "globalView.h"
#include "designMine.h"
#include <vector>

DECLARE_EVENT_TYPE(wxEVT_POINT_ADDED, -1)


class globalView;
class DesignMine;

class FaceWidget : public wxPanel
{
public:
	FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal=true, bool pre=false);
	void CreatePanel();
	void RefreshCanvas();
	void CreateVis2D();
	void AlignFunction();  
	globalView* GetView(){return Vis2d;};
	SimulatedWorld *GetWorld(){return world;}  
	void DrawFace(GLObject* obj);
	Canvas* GetCanvas3d(){return canvas3d;};



	
private:

	bool worldView;
	bool noPreliminar3D;
	double x,y;
	globalView *Vis2d;
	SimulatedWorld *world;
	wxWindow* window;
	int resp; 
	Canvas *canvas3d;
	NodeTree *node;
	SimulatedWorld *simu;

	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWIDGET__H