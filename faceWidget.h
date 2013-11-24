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
class PointsList;
class DesignMine;

class FaceWidget : public wxPanel
{
public:
	FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal=true, bool pre=false);
	void CreatePanel();
	void RefreshCanvas();
	void GetPoint(wxCommandEvent& event);  //Añade el nuevo punto desde el diseño2d
	void ChangePoint(wxCommandEvent& event); //Función que se encargará de establecer el nuevo punto desde el diseño 2D
	void DeletePoint(wxCommandEvent& event); //Función para el borrado de puntos desde el diseño2D
	void SetVertex(bool addvertex=true,bool changevertex=false,bool deletevertex=false,bool movepoint=false,int deleteRow=0);
	void CheckPointToMove(wxCommandEvent& event);  //Función que se encarga de comprobar si el punto seleccionado hay que moverlo
	void OnChangeSplitter(wxSplitterEvent &event);
	void AssociatePointTable(PointsList *point);
	void CreateFace();
	void CreateVis2D();
	void AlignFunction();  //Añadida para gestionar todas las alineaciones
	Face* GetFace() {return face;}
	globalView* GetView(){return Vis2d;};
	SimulatedWorld *GetWorld(){return world;}  //Añadida por mí 
	void ChangeColourCell(wxCommandEvent& event);

	///////Añadido de la GlobalView
	void DrawFace(GLObject* obj);
	Canvas* GetCanvas3d(){return canvas3d;};
	//////////////////////////////


	
private:
	bool tableAssociated;
	bool h;
	bool worldView;
	bool noPreliminar3D;
	double x,y;
	globalView *Vis2d;
	Face *face,*faceCopy;
	PointsList *points;
	SimulatedWorld *world;
	wxSplitterWindow* canvas;
	wxWindow* window;
	int resp; //Variable en la que vamos a almacenar la fila del punto a cambiar
	Canvas *canvas3d;
	NodeTree *node;
	SimulatedWorld *simu;
	Face *facedia;


	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWIDGET__H