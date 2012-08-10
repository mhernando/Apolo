#include "canvas.h"

DEFINE_EVENT_TYPE(wxEVT_FACEVERTEX_ADDED)

BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
	EVT_PAINT(Canvas::Paint)
	EVT_SIZE(Canvas::Resized)
	EVT_MOTION(Canvas::OnMouseM)
	EVT_RIGHT_DOWN(Canvas::OnMouse)
	EVT_RIGHT_UP(Canvas::OnMouse)
	EVT_LEFT_DOWN(Canvas::OnMouse)
	EVT_LEFT_UP(Canvas::OnMouse)
	EVT_CHAR(Canvas::OnKey)
END_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size,bool triD)
:wxGLCanvas(parent, id, pos, size,0)
{
	window=parent;
	dimension=triD;
	flag = false;
	scene2D.SetViewPoint(-5,-5,5,5);
}
void Canvas::InitGL()
{
	SetCurrent();
	if(dimension) scene.init();
	else scene2D.init();
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
	
	if(dimension)
	{
	scene.setViewSize(0,0,GetSize().x,GetSize().y);	
	scene.Draw();
	}
	else scene2D.Draw();
	SwapBuffers();
}

void Canvas::UpdateWorld(World* w)
{
	if(dimension)
	{
		this->scene.clearObjects();
		this->scene.addObject(w);
	}
	else
	{
		this->scene2D.clearObjects();
		this->scene2D.addObject(w);
	}
	Refresh(false);
}
void Canvas::UpdateMeshpart(MeshPart* m)
{
	if(dimension)
	{
	this->scene.clearObjects();
	this->scene.addObject(m);
	}
	else
	{
		this->scene2D.clearObjects();
		this->scene2D.addObject(m);
	}

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
		if(dimension)scene.MouseMove(pt.x,pt.y); 
		else scene2D.MouseMove(pt.x,pt.y); 
		Refresh(false);
	}
	event.Skip();
}

void Canvas::OnMouse(wxMouseEvent& event)
{
	pt = event.GetPosition();
	
	

	if(dimension)
	{
		scene.MouseButton(pt.x,pt.y,2,event.RightIsDown(),event.ShiftDown(),event.ControlDown()); 
		scene.MouseButton(pt.x,pt.y,0,event.LeftIsDown(),event.ShiftDown(),event.ControlDown());
	}
	else
	{
		//scene2D.MouseButton(pt.x,pt.y,2,event.RightIsDown(),event.ShiftDown(),event.ControlDown()); 
		//scene2D.MouseButton(pt.x,pt.y,0,event.LeftIsDown(),event.ShiftDown(),event.ControlDown());
		if(event.LeftDown())
		{
		wxCommandEvent CanvasEvent( wxEVT_FACEVERTEX_ADDED,GetId() );
		CanvasEvent.SetEventObject( window);
		GetEventHandler()->ProcessEvent(CanvasEvent);
		}
	}

	SetFocus();
	event.Skip();
}

void Canvas::OnKey(wxKeyEvent& event)
{
	if(dimension)
	{
		scene.KeyDown(event.GetUnicodeKey());
		scene.SpecialKeyDown(event.GetUnicodeKey());
	}
	else
	{
		scene2D.KeyDown(event.GetUnicodeKey());
		//scene2D.KeyDown(event.GetKeyCode());
	
	}
	SetFocus();
	Refresh();
}

