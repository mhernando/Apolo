#ifndef __APOLO__CANVAS__H
#define __APOLO__CANVAS__H

#include "mrcore.h"
#include <wx/glcanvas.h>
#include <wx/wx.h>

DECLARE_EVENT_TYPE(wxEVT_FACEVERTEX_ADDED, -1)
class Canvas : public wxGLCanvas
{
public:
	Canvas(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size,bool triD=true);
	GLScene scene;
	GLScene2D scene2D;
	bool dimension;
	void InitGL();
	void Paint(wxPaintEvent& event);
	void Resized(wxSizeEvent& event);
	void OnMouseM(wxMouseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void UpdateWorld(World* w);
	void UpdateMeshpart(MeshPart* m);
	void OnKey(wxKeyEvent& event);
	wxPoint getCursorPosition(){return pt;};
	Vector2D getViewScale2D(){return scale2D;};
	void Scale2D();

	
private:
	Vector2D scale2D;
	double x2Di,y2Di,x2Df,y2Df;
	wxPoint pt;
	wxWindow *window;
	bool flag;
	DECLARE_EVENT_TABLE()
};

#endif // __APOLO__CANVAS__H

