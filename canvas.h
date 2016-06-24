#ifndef __APOLO__CANVAS__H
#define __APOLO__CANVAS__H

#include "mrcore.h"
#include <wx/glcanvas.h>
#include <wx/wx.h>


class Canvas : public wxGLCanvas
{
public:
	Canvas(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size,bool triD=true);
	void SetCurrent(){wxGLCanvas::SetCurrent(*m_context);}
	bool dimension;
	void InitGL();
	void AddObject(GLObject * obj);
	void SetViewPoint(double dist_or_2Dx,double rot_or_2Dy,double elv_or_2Dfx,double s2Dfy=0);
	void GetViewPoint(double &dist_or_2Dix,double &rot_or_2Diy,double &elv_or_2Dfx,double s2Dfy=0);
	void SetViewCenter(double x, double y, double z);
	void SetShowFrame(bool show);
	void ClearObjects();
	void Paint(wxPaintEvent& event);
	void DrawGrid(bool draw=true);
	void Resized(wxSizeEvent& event);
	void OnMouseM(wxMouseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void UpdateWorld(World* w);
	void UpdateMeshpart(MeshPart* m);
	void OnKey(wxKeyEvent& event);
	wxPoint getCursorPosition(){return pt;};
	void ChangeBackGroundColour(wxColour colour);
	
	
private:
	GLScene scene;
	double x2Di,y2Di,x2Df,y2Df;
	wxPoint pt;
	wxGLContext*	m_context;
	wxWindow *window;
	bool flag;
	DECLARE_EVENT_TABLE()
};

#endif // __APOLO__CANVAS__H

