#ifndef __APOLO__FACEDESIGN__H
#define __APOLO__FACEDESIGN__H


#include "mrcore.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>

DECLARE_EVENT_TYPE(wxEVT_FACEVERTEX_ADDED, -1)  //Evento de a�adir v�rtice
DECLARE_EVENT_TYPE(wxEVT_ALIGN_DONE, -1)       //Evento de calibraci�n
DECLARE_EVENT_TYPE(wxEVT_CHECK_FACEVERTEX, -1)  //Evento para arrastrar punto 
DECLARE_EVENT_TYPE(wxEVT_FACEVERTEX_MOVED, -1)  //Evento punto desplazado
DECLARE_EVENT_TYPE(wxEVT_CHECK_POINT_SELECTED, -1)  //Evento de checkeo
DECLARE_EVENT_TYPE(wxEVT_SET_NEW_POINT, -1) //Establece el nuevo punto movido
DECLARE_EVENT_TYPE(wxEVT_STARTED_MOVEMENT, -1) //Selecci�n del punto donde vamos a empezar a desplazar la ventana
DECLARE_EVENT_TYPE(wxEVT_MOVEMENT_DONE, -1) //Calculado el desplazamiento de la ventana de dise�o de la face
 

class FaceDesign:public wxGLCanvas
{
public:
FaceDesign(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size);
void InitFaceDesign();
void CalculateScale();  //Funci�n que se encargar� de tomar las medidas de la ventana
void ZoomChange(double zoom); //Funci�n que se encargar� de cambiar el ZOOM();
void ChangePosition(bool up,bool down,bool right,bool left);  //Funci�n para cambiar la posici�n
void SelectedPoint(wxMouseEvent& event); //Obtener el punto cuando se hace click izquierdo con el raton
void ClearObjects();
void AddObject(GLObject * obj);
void UpdateWorld(World* w);
void AlignPoints(wxCommandEvent& event);  //Funci�n que alineara los puntos con el grid para simplificar la funcion del facewidget
void Paint(wxPaintEvent& event);
void CheckPoint(wxMouseEvent& event); //Se encargar� de comprobar si el punto seleccionado con midddle es uno dibujado y a�adido en la lista de puntos
void SetNewMovedPoint(wxMouseEvent& event); //Establece el nuevo punto movido cuando soltamos middle button
Vector2D getScale() {return Scale;};
wxPoint getPosition() {return Point;};
double getXCalibrated() {return puntox;};
double getYCalibrated(){return puntoy;}
void startToMove(wxMouseEvent& event);  //Clickeo para arrastrar la ventana de dise�o
void finishMoving(wxMouseEvent& event);  //Liberar el click del rat�n donde soltar el punto
void movementControl();
void FinishedChange() { pointTomove=false;setPointMoved=false;} 
void SetAlign(bool Align){ align=Align;};
bool GetAlign() {return align;};



private:
	GLScene2D design;  //Variable de tipo GLScene que emplearemos para dibujar
	wxPoint Point; //Variable en la que guardaremos el punto seleccionado
	wxPoint PointAux;
	wxWindow* window;
	Vector2D Scale;   
	double x2Di,y2Di,x2Df,y2Df;
	double puntox,puntoy;
	double previouszoom;
	bool pointTomove,setPointMoved,startN,finishN,align;
	DECLARE_EVENT_TABLE()
};


#endif // __APOLO__FACEDESIGN__H