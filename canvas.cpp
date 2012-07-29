#include "canvas.h"

BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
	EVT_PAINT(Canvas::Paint)
	EVT_SIZE(Canvas::Resized)
	EVT_MOTION(Canvas::OnMouseM)
	EVT_RIGHT_DOWN(Canvas::OnMouse)
	EVT_RIGHT_UP(Canvas::OnMouse)
	EVT_LEFT_DOWN(Canvas::OnMouse)
	EVT_LEFT_UP(Canvas::OnMouse)
END_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size)
:wxGLCanvas(parent, id, pos, size,0)
{
	flag = false;
}
void Canvas::InitGL()
{
	SetCurrent();
	scene.init();
}
void Canvas::Paint(wxPaintEvent& event)
{
	wxPaintDC(this);
	SetCurrent();

	if(!flag)
	{
		InitGL();
		flag = true;
	}
	
	scene.setViewSize(0,0,GetSize().x,GetSize().y);	
	
	scene.Draw();
	SwapBuffers();
}

void Canvas::UpdateWorld(World* w)
{
	this->scene.clearObjects();
	this->scene.addObject(w);
	Refresh(false);
}
void Canvas::UpdateMeshpart(MeshPart* m)
{
	this->scene.clearObjects();
	this->scene.addObject(m);
	Refresh(false);

}
void Canvas::Resized(wxSizeEvent& event)
{
	Refresh(false);
	event.Skip();
}
void Canvas::OnMouseM(wxMouseEvent& event)
{
	wxPoint pt = event.GetPosition();//return the physical mouse position

	if(event.RightIsDown()||event.LeftIsDown())
	{
		scene.MouseMove(pt.x,pt.y); 
		Refresh(false);
	}
	event.Skip();
}

void Canvas::OnMouse(wxMouseEvent& event)
{
	wxPoint pt = event.GetPosition(); 
	scene.MouseButton(pt.x,pt.y,2,event.RightIsDown(),event.ShiftDown(),event.ControlDown()); 
	scene.MouseButton(pt.x,pt.y,0,event.LeftIsDown(),event.ShiftDown(),event.ControlDown());
	SetFocus();
	event.Skip();
}