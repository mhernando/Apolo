#ifndef __APOLO__CANVAS__H
#define __APOLO__CANVAS__H

#include "mrcore.h"
#include <wx/glcanvas.h>
#include <wx/wx.h>

class Canvas : public wxGLCanvas
{
public:
	Canvas(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size);
	
	void InitGL();
	void Paint(wxPaintEvent& event);
	void Resized(wxSizeEvent& event);
	void OnMouseM(wxMouseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void UpdateWorld(World* w);
	void UpdateMeshpart(MeshPart* m);

	GLScene scene;
private:
	
	bool flag;
	DECLARE_EVENT_TABLE()
};

#endif // __APOLO__CANVAS__H

