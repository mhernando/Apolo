#ifndef __APOLO__FACEWIDGET__H
#define __APOLO__FACEWIDGET__H

#include "mainWindow.h"
#include <wx/wx.h>
#include <wx/splitter.h>
#include "simulatedWorld.h"
#include "pointsList.h"
#include "canvas.h"
#include "mrcore.h"
#include "faceDesign.h"
#include <math.h>
#include <conio.h>

DECLARE_EVENT_TYPE(wxEVT_POINT_ADDED, -1)

class PointsList;
class FaceWidget : public wxPanel
{
public:
	FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal=true, bool pre=false);
	void CreatePanel();
	void RefreshCanvas();
	void GetPoint(wxCommandEvent& event);
	void ChangePoint(wxCommandEvent& event); //Función que se encargará de establecer el nuevo punto
	void SetVertex(bool addvertex=true,bool changevertex=false,bool deletevertex=false,bool movepoint=false,int deleteRow=0);
	void CheckPointToMove(wxCommandEvent& event);  //Función que se encarga de comprobar si el punto seleccionado hay que moverlo
	void OnChangeSplitter(wxSplitterEvent &event);
	void SetAlign(bool al){align=al;}
	bool GetAlign(){return align;}
	Canvas *GetCanvas3D(){return canvas2;}
	void AssociatePointTable(PointsList *point);
	void ChangeView(bool world);
	void CreateFace();
	Face* GetFace() {return face;}
	Canvas *canvas2;
	FaceDesign* design1;

	
private:
	bool tableAssociated;
	bool align,h;
	bool worldView;
	bool noPreliminar3D;
	double x,y;
	Face *face,*faceCopy;
	PointsList *points;
	SimulatedWorld *world;
	//Poner las canvas aquí después de la corrección
	wxSplitterWindow* canvas;
	int resp; //Variable en la que vamos a almacenar la fila del punto a cambiar
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWIDGET__H