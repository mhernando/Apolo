#include "faceDesign.h"

DEFINE_EVENT_TYPE(wxEVT_FACEVERTEX_ADDED)
DEFINE_EVENT_TYPE(wxEVT_ALIGN_DONE)
DEFINE_EVENT_TYPE(wxEVT_CHECK_FACEVERTEX)
DEFINE_EVENT_TYPE(wxEVT_FACEVERTEX_MOVED)
DEFINE_EVENT_TYPE(wxEVT_CHECK_POINT_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_SET_NEW_POINT)
DEFINE_EVENT_TYPE(wxEVT_STARTED_MOVEMENT)
DEFINE_EVENT_TYPE(wxEVT_MOVEMENT_DONE)


BEGIN_EVENT_TABLE(FaceDesign, wxGLCanvas)
	EVT_PAINT(FaceDesign::Paint)
	EVT_LEFT_DOWN(FaceDesign::SelectedPoint)
	EVT_RIGHT_DOWN(FaceDesign::startToMove)
	EVT_RIGHT_UP(FaceDesign::finishMoving)
	EVT_MIDDLE_DOWN(FaceDesign::CheckPoint)
	EVT_MIDDLE_UP(FaceDesign::SetNewMovedPoint)
	EVT_COMMAND(wxID_ANY,wxEVT_FACEVERTEX_ADDED,FaceDesign::AlignPoints)
	EVT_COMMAND(wxID_ANY,wxEVT_CHECK_FACEVERTEX,FaceDesign::AlignPoints) //Cuando escogemos un punto a arrastrar también debemos calibrarlo para compararlo adecuadamente
	EVT_COMMAND(wxID_ANY,wxEVT_FACEVERTEX_MOVED,FaceDesign::AlignPoints)//Cuando soltamos el punto movido también debe calibrarse
	EVT_COMMAND(wxID_ANY,wxEVT_STARTED_MOVEMENT,FaceDesign::AlignPoints)//El punto a partir del que movemos la ventana se calibra
	EVT_COMMAND(wxID_ANY,wxEVT_MOVEMENT_DONE,FaceDesign::AlignPoints) //El punto final al que movemos la ventana también se calibra
	END_EVENT_TABLE()

FaceDesign::FaceDesign(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size)
:wxGLCanvas(parent,id,pos,size)
{
	design.SetViewPoint(-5,-5,5,5);	
	window=parent;
	previouszoom=100;
	startN=false;
	finishN=false;
	align=false;
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
		double aux=previouszoom-zoom;
			x2Di=x2Di-0.05*aux;
			x2Df=x2Df+0.05*aux;
			y2Di=y2Di-0.05*aux;
			y2Df=y2Df+0.05*aux;	
	}
		if (zoom>previouszoom)  //estamos aumentando zoom
		{
			double aux=zoom-previouszoom;
			x2Di=x2Di+0.05*aux;
			x2Df=x2Df-0.05*aux;
			y2Di=y2Di+0.05*aux;
			y2Df=y2Df-0.05*aux;	
		}
		design.SetViewPoint(x2Di,y2Di,x2Df,y2Df);
		previouszoom=zoom;	
}


void FaceDesign::SelectedPoint(wxMouseEvent& event)
{
	
	Point = event.GetPosition();
	if(event.LeftDown())
	{
		pointTomove=false;
		setPointMoved=false;
		wxCommandEvent DesignEvent( wxEVT_FACEVERTEX_ADDED,GetId() );
		DesignEvent.SetEventObject( window);
		GetEventHandler()->ProcessEvent(DesignEvent);
	}
	SetFocus();
	event.Skip();
}



void FaceDesign::CheckPoint(wxMouseEvent& event) 
//Función que se encargará de crear el evento cuando pulsamos el botón central del ratón y que conducirá al PointList para comprobar el punto
{
	Point = event.GetPosition();
	if(event.MiddleDown())
	{
		pointTomove=true;
		setPointMoved=false;
		wxCommandEvent DesignEvent( wxEVT_CHECK_FACEVERTEX,GetId() );
		DesignEvent.SetEventObject( window);
		GetEventHandler()->ProcessEvent(DesignEvent);
	}
	SetFocus();
	event.Skip();
}



void FaceDesign::SetNewMovedPoint(wxMouseEvent& event) 
//Función que se encargará de crear el evento cuando pulsamos el botón central del ratón y que conducirá al PointList para comprobar el punto
{
	Point = event.GetPosition();
	if(event.MiddleUp())
	{
		pointTomove=false;
		setPointMoved=true;
		wxCommandEvent DesignEvent( wxEVT_FACEVERTEX_MOVED,GetId() );
		DesignEvent.SetEventObject( window);
		GetEventHandler()->ProcessEvent(DesignEvent);
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
	if((pointTomove==true)&&(setPointMoved==false))  //Si estamos arrastrando el vértice 
	{
		wxCommandEvent CheckingEvent(wxEVT_CHECK_POINT_SELECTED,GetId() );
		CheckingEvent.SetEventObject(window);
		GetEventHandler()->ProcessEvent(CheckingEvent);
	}
	else if((setPointMoved==true)&&(pointTomove==false)) //Si estamos fijando la nueva posición del vértice arrastrado
	{
		wxCommandEvent ChangingEvent(wxEVT_SET_NEW_POINT,GetId() );
		ChangingEvent.SetEventObject(window);
		GetEventHandler()->ProcessEvent(ChangingEvent);
	}
	else if((startN==true)||(finishN==true))
	{
		movementControl();
	}
	else //Estamos dibujando simplemente un vértice
	{
		wxCommandEvent AlignEvent(wxEVT_ALIGN_DONE,GetId() );
		AlignEvent.SetEventObject(window);
		GetEventHandler()->ProcessEvent(AlignEvent);
	}
}




void FaceDesign::Paint(wxPaintEvent& event)
{
	wxPaintDC(this);
	SetCurrent();
	design.Draw();
	SwapBuffers();
}


void FaceDesign::startToMove(wxMouseEvent& event) //Clickeamos el punto que se toma como referencia para desplazar la ventana
{
	if(event.RightDown())
	{
		Point = event.GetPosition();
		startN=true;
		finishN=false;
		wxCommandEvent NavigateFirstEvent(wxEVT_STARTED_MOVEMENT,GetId() );
		NavigateFirstEvent.SetEventObject(window);
		GetEventHandler()->ProcessEvent(NavigateFirstEvent);
	}
	SetFocus();
	event.Skip();
}



void FaceDesign::finishMoving(wxMouseEvent& event)  //Cuando se deja de clickear se está estableciendo la nueva situación del punto
{
	if(event.RightUp())
	{
		Point = event.GetPosition();
		startN=false;
		finishN=true;
		wxCommandEvent NavigateFinalEvent(wxEVT_MOVEMENT_DONE,GetId() );
		NavigateFinalEvent.SetEventObject(window);
		GetEventHandler()->ProcessEvent(NavigateFinalEvent);
	}
	SetFocus();
	event.Skip();
}




void FaceDesign::movementControl()
{
	if (startN==true)
	{
		PointAux.x=puntox; //se guarda la componente X del punto calibrado en PointAux
		PointAux.y=puntoy; //se guarda la componente Y del punto calibrado en PointAux
	}

	if (finishN==true)
	{
		double difx=PointAux.x-puntox;  //se calcula el desplazamiento en la componenteX
		double dify=PointAux.y-puntoy;	//se calcula el desplazamiento en la componenteY
		x2Di=x2Di+difx;
		y2Di=y2Di+dify;
		x2Df=x2Df+difx;
		y2Df=y2Df+dify;
		design.SetViewPoint(x2Di,y2Di,x2Df,y2Df);
		startN=false;
		finishN=false;
		Refresh();
	}
		
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




