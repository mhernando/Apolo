#ifndef __APOLO__FACEDESIGN__H
#define __APOLO__FACEDESIGN__H


#include "mrcore.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>

DECLARE_EVENT_TYPE(wxEVT_FACEVERTEX_ADDED, -1)  //Evento de añadir vértice
DECLARE_EVENT_TYPE(wxEVT_ALIGN_DONE, -1)       //Evento de calibración


class FaceDesign:public wxGLCanvas
{
public:
FaceDesign(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size);
void InitFaceDesign();
void CalculateScale();  //Función que se encargará de tomar las medidas de la ventana
void ZoomChange(double zoom); //Función que se encargará de cambiar el ZOOM();
void ChangePosition(bool up,bool down,bool right,bool left);  //Función para cambiar la posición
void SelectedPoint(wxMouseEvent& event); //Obtener el punto cuando se hace click izquierdo con el raton
void ClearObjects();
void AddObject(GLObject * obj);
void UpdateWorld(World* w);
void AlignPoints(wxCommandEvent& event);  //Función que alineara los puntos con el grid para simplificar la funcion del facewidget
void Paint(wxPaintEvent& event);
Vector2D getScale() {return Scale;};
wxPoint getPosition() {return Point;};
double getXCalibrated() {return puntox;};
double getYCalibrated(){return puntoy;}



private:
	GLScene2D design;  //Variable de tipo GLScene que emplearemos para dibujar
	wxPoint Point; //Variable en la que guardaremos el punto seleccionado
	wxWindow* window;
	Vector2D Scale;   
	double x2Di,y2Di,x2Df,y2Df;
	double puntox,puntoy;
	double previouszoom;
	DECLARE_EVENT_TABLE()
};


#endif // __APOLO__FACEDESIGN__H