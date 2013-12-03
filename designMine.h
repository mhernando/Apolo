#ifndef __APOLO__DESIGNMINE__H
#define __APOLO__DESIGNMINE__H


#include "mrcore.h"
#include <wx/glcanvas.h>
#include <wx/wx.h>
#include <vector>
#include "math\vector2d.h"



DECLARE_EVENT_TYPE(wxEVT_NEWVERTEX_ADDED, -1)  //Evento de a�adir v�rtice
DECLARE_EVENT_TYPE(wxEVT_CHANGE_POINT, -1) //Evento de v�rtice cambiado
DECLARE_EVENT_TYPE(wxEVT_DELETE_POINT,-1) //Evento de borrado de v�rtice
DECLARE_EVENT_TYPE(wxEVT_MOVE_POLYGON,-1)  //Cambio de posici�n del pol�gono completo
DECLARE_EVENT_TYPE(wxEVT_COLOUR_CELL,-1) 
DECLARE_EVENT_TYPE(wxEVT_INSERT_POINT,-1)  //Insercci�n de un punto intermedio


class DesignMine: public wxGLCanvas
{
public:
	DesignMine(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size);
	void Init();
	void Present();
	void DrawScene2D();
	void Paint(wxPaintEvent& event);
	void DrawOrigin();
	void DrawGrid(float size);
	void New_Move_Point(wxMouseEvent& event);
	void ManagePoints(wxMouseEvent& event);
	void FreePoint(wxMouseEvent& event);
	void DrawPoints();
	void DrawLines();
	void DrawNextLine(wxMouseEvent& event);
	void CalculateScale();
	void ScalePoint();
	void AlignFunction();
	void Menu(wxMouseEvent& event);
	void MenuRightButton(wxMouseEvent& event);
	void MovePoint(wxMouseEvent& event);
	int NumPoints(){return points.size();};
	vector<Vector2D> GetVector(){return points;};

	vector<Vector2D> GetAuxVector(){return auxpoints;}; 
	void SetAuxPoints(double x,double y){auxpoints.push_back(Vector2D(x,y));};

	float GetLastPointX(){return xLast;};
	float GetLastPointY(){return yLast;};
	float GetPointX(){return x;}
	float GetPointY(){return y;}
	void AddPoint(double a,double b);
	void ChangePoint(int i,double a,double b);
	void DeleteVertex(int position);
	void MenuPointOptions(wxCommandEvent& event);
	void SetGridSize(float s);
	void SetZoom(double z){zoom=z;};
	void SetAlign(bool align){Align=align;}
	void SetView(double xi,double xf,double yi,double yf);
	void GetView(double&xi, double& yi,double& xf, double& yf){xi=x2Di;yi=y2Di;xf=x2Df;yf=y2Df;}
	void ChangeView(bool up,bool down,bool right,bool left);
	void ChangeZoom(double zoom);
	void ChangePosition(bool vertical,bool horizontal,double variation);
	void MoveScreen(wxMouseEvent& event);
	void ManageOptions(wxCommandEvent& event);
	int GetMarkedPoint(){return MarkNum;}
	bool GetTypeOfPoint(){return MousePoint;}  //Da a conocer si el punto introducido es por rat�n o exteriormente
	int GetPoint2Mark(){return point2Mark;};
	int GetPoint1Mark(){return point1Mark;};
	void SetCondition(int a){estado=a;};
	int GetCondition(){return estado;};
	void eraseDesign();

	bool Intersection(float Sx1,float Sx2,float Sy1,float Sy2,float Qa1,float Qa2,float Qb1,float Qb2);
	void SetPaste(bool val){paste=val;};
	void clearAuxPoints(){auxpoints.clear();}
	bool GetGridState(){return Grid;};
	void SetGridState(bool state){Grid=state;};
	



private:
	double x,y,x2Di,x2Df,y2Di,y2Df,gridSize,zoom,previousZoom,previousH,previousV;
	float xLast,yLast;
	int MarkNum,point1Mark,point2Mark,clicks;
	wxPoint MousePosition;
	wxPoint AuxPoint;
	double puntox,puntoy;
	wxWindow* window;
	Vector2D Scale;
	Face *Polyg;
	Vector2D point1,point2;
	vector<Vector2D> points;
	vector<Vector2D> auxpoints;
	vector<bool> marcas;

	bool Align,Grid,MousePoint,paste;

	int estado;
	DECLARE_EVENT_TABLE()
};

#endif // __APOLO__DESIGNMINE__H