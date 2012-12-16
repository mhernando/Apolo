#include "canvas.h"

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
	scene.SetViewPoint(20,0,10);
}

void Canvas::AddObject(GLObject * obj)
{
	if(dimension) scene.addObject(obj);
}

void Canvas::ChangeBackGroundColour(wxColour colour)
{
	if(dimension) 
	{	
		scene.BackgroundColor((float)colour.Red()/255.0f,(float)colour.Green()/255.0f,(float)colour.Blue()/255.0f);
		Refresh();
	}
	else return;
}

void Canvas::InitGL()
{
	SetCurrent();
	if(dimension) scene.init();
}

double Canvas::GetViewPoint(double &dist_or_2Dx,double &rot_or_2Dy,double &elv_or_2Dfx,double s2Dfy)
{
	if(dimension) 
	{
		scene.GetViewPoint(dist_or_2Dx,rot_or_2Dy,elv_or_2Dfx);
		return 0;
	}
}


void Canvas::SetViewPoint(double dist_or_2Dx,double rot_or_2Dy,double elv_or_2Dfx,double s2Dfy)
{
	if(dimension) scene.SetViewPoint(dist_or_2Dx,rot_or_2Dy,elv_or_2Dfx);	
}

void Canvas::ClearObjects()
{
	if(dimension)
		scene.clearObjects();
}

void Canvas::SetViewCenter(double x, double y, double z)
{
	if(dimension)
		scene.SetViewCenter(x,y,z);

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

	SwapBuffers();
}

void Canvas::UpdateWorld(World* w)
{
	if(dimension)
	{
		this->scene.clearObjects();
		this->scene.addObject(w);
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
	/*else
	{
		
		scene2D.KeyDown(event.GetUnicodeKey());
		if(event.GetUnicodeKey()=='+')
			Scale2D();
		if(event.GetUnicodeKey()=='-')
			Scale2D();
	
	}
	*/
	SetFocus();
	Refresh();
}




void Canvas::SetShowFrame(bool show)
{
	if(dimension)
		scene.setShowFrame(show);
	
}

void Canvas::DrawGrid(bool draw)
{
	if(dimension)
		scene.setShowGrid(draw);
	//else
		//scene2D.setShowGrid(draw); /// implementar setShowGrid en MRCore en glscene2D

}