#include "faceDesign.h"

DEFINE_EVENT_TYPE(wxEVT_FACEVERTEX_ADDED)
DEFINE_EVENT_TYPE(wxEVT_ALIGN_DONE)


BEGIN_EVENT_TABLE(FaceDesign, wxGLCanvas)
	EVT_PAINT(FaceDesign::Paint)
	EVT_LEFT_DOWN(FaceDesign::SelectedPoint)
	EVT_COMMAND(wxID_ANY,wxEVT_FACEVERTEX_ADDED,FaceDesign::AlignPoints)
	END_EVENT_TABLE()

FaceDesign::FaceDesign(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size)
:wxGLCanvas(parent,id,pos,size)
{
	design.SetViewPoint(-5,-5,5,5);	
	window=parent;
	previouszoom=100;
}


void FaceDesign::InitFaceDesign()
{
	SetCurrent();
	design.init();	
}


void FaceDesign::CalculateScale()
{
		design.GetViewPoint(x2Di,y2Di,x2Df,y2Df);
		Scale.x=x2Df-x2Di;
		Scale.y=y2Df-y2Di;
}


void FaceDesign::ZoomChange(double zoom)
{
	design.GetViewPoint(x2Di,y2Di,x2Df,y2Df);
	if (zoom<previouszoom)  //estamos disminuyendo zoom
	{
			x2Di=x2Di-0.1;
			x2Df=x2Df+0.1;
			y2Di=y2Di-0.1;
			y2Df=y2Df+0.1;	
	}
		if (zoom>previouszoom)  //estamos aumentando zoom
		{
			x2Di=x2Di+0.1;
			x2Df=x2Df-0.1;
			y2Di=y2Di+0.1;
			y2Df=y2Df-0.1;	
		}
		design.SetViewPoint(x2Di,y2Di,x2Df,y2Df);
		previouszoom=zoom;	
}


void FaceDesign::SelectedPoint(wxMouseEvent& event)
{
	Point = event.GetPosition();
	if(event.LeftDown())
	{
		wxCommandEvent CanvasEvent( wxEVT_FACEVERTEX_ADDED,GetId() );
		CanvasEvent.SetEventObject( window);
		GetEventHandler()->ProcessEvent(CanvasEvent);
	}
	SetFocus();
	event.Skip();
}


void FaceDesign::ClearObjects()
{
	design.clearObjects();
}


void FaceDesign::AddObject(GLObject * obj)
{
	design.addObject(obj);
}


void FaceDesign::UpdateWorld(World* w)
{
	this->design.clearObjects();
	this->design.addObject(w);
	Refresh(false);
}



void FaceDesign::AlignPoints(wxCommandEvent& event)
{
	wxSize SizeDesign;
	Vector2D Transform;
	this->CalculateScale();
	SizeDesign=this->GetVirtualSize();
	Point.x-=SizeDesign.GetWidth()/2;
	Transform.x=SizeDesign.GetWidth()/Scale.x;
	Point.y-=SizeDesign.GetHeight()/2;
	Transform.y=SizeDesign.GetHeight()/Scale.y;
	puntox=Point.x/Transform.x+((x2Di+x2Df)/2);
	puntoy=-Point.y/Transform.y+((y2Di+y2Df)/2); //Cambiamos el signo para que se corresponda con la orientación de la ventana 
	//Crea el evento de alineación el cúal usará facewidget para añadir definitivamente el punto
	wxCommandEvent AlignEvent(wxEVT_ALIGN_DONE,GetId() );
	AlignEvent.SetEventObject(window);
	GetEventHandler()->ProcessEvent(AlignEvent);
}


void FaceDesign::Paint(wxPaintEvent& event)
{
	wxPaintDC(this);
	SetCurrent();
	design.Draw();
	SwapBuffers();
}



void FaceDesign::ChangePosition(bool up,bool down,bool right,bool left)
{
	if ((up==true)&&(down==true)) return;  //no puede moverse arriba y abajo a la vez
	if ((right==true)&&(left==true)) return; //no puede moverse a la izquierda y a la derecha a la vez
	
	if (right==true) 
	{
		design.GetViewPoint(x2Di,y2Di,x2Df,y2Df);
		x2Di=x2Di+0.5;
		x2Df=x2Df+0.5;
		design.SetViewPoint(x2Di,y2Di,x2Df,y2Df);
	}

	if (left==true)
	{
		design.GetViewPoint(x2Di,y2Di,x2Df,y2Df);
		x2Di=x2Di-0.5;
		x2Df=x2Df-0.5;
		design.SetViewPoint(x2Di,y2Di,x2Df,y2Df);
	}

	if (up==true)
	{
		design.GetViewPoint(x2Di,y2Di,x2Df,y2Df);
		y2Di=y2Di+0.5;
		y2Df=y2Df+0.5;
		design.SetViewPoint(x2Di,y2Di,x2Df,y2Df);
	}

	if (down==true)
	{
		design.GetViewPoint(x2Di,y2Di,x2Df,y2Df);
		y2Di=y2Di-0.5;
		y2Df=y2Df-0.5;
		design.SetViewPoint(x2Di,y2Di,x2Df,y2Df);
	}

}




