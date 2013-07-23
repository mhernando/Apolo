
#include "designMine.h"
#include "definitions.h"
#include <wx/popupwin.h>
#include <wx/cursor.h>

using namespace std;



DEFINE_EVENT_TYPE(wxEVT_NEWVERTEX_ADDED)
DEFINE_EVENT_TYPE(wxEVT_CHANGE_POINT)
DEFINE_EVENT_TYPE(wxEVT_DELETE_POINT)
DEFINE_EVENT_TYPE(wxEVT_INSERT_POINT)
DEFINE_EVENT_TYPE(wxEVT_MOVE_POLYGON)
DEFINE_EVENT_TYPE(wxEVT_COLOUR_CELL)

BEGIN_EVENT_TABLE(DesignMine, wxGLCanvas)
	EVT_PAINT(DesignMine::Paint)
	EVT_LEFT_DOWN(DesignMine::New_Move_Point)
	EVT_LEFT_UP(DesignMine::FreePoint)
	EVT_RIGHT_DOWN(DesignMine::MenuRightButton)
	EVT_RIGHT_UP(DesignMine::MenuRightButton)
	EVT_MIDDLE_DOWN(DesignMine::Menu)
	EVT_MOTION(DesignMine::ManagePoints)
	EVT_MENU(ID_SHOWGRID,DesignMine::ManageOptions)
	EVT_MENU(ID_HIDEGRID,DesignMine::ManageOptions)
	EVT_MENU(ID_LINES,DesignMine::ManageOptions)
	EVT_MENU(ID_POLYGON,DesignMine::ManageOptions)
	EVT_MENU(ID_SELECTPOINTS,DesignMine::ManageOptions)
	EVT_MENU(ID_ERASEVERTEX,DesignMine::MenuPointOptions)
	EVT_MENU(ID_HALFWAYPOINT,DesignMine::ManageOptions)
END_EVENT_TABLE()

DesignMine::DesignMine(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size)
:wxGLCanvas(parent,id,pos,size)
{
	x2Di=-5;
	y2Di=-5;
	x2Df=5;
	y2Df=5;
	CalculateScale();
	gridSize=2;
	Align=false;
	previousZoom=100;
	zoom=100;
	MarkNum=-1;
	Grid=true;
	polygon=false;
	estado=0;
	MousePoint=false;
	previousH=50;
	previousV=50;
	clicks=0;
	wxSetCursor(wxCURSOR_CROSS);
}


void DesignMine::Paint(wxPaintEvent& event)
{
	wxPaintDC(this);
	DrawScene2D();
}


void DesignMine::Init()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer
	//Para definir el punto de vista
	glMatrixMode(GL_PROJECTION); // Proyection mode
	glLoadIdentity(); 
	glOrtho(x2Di , x2Df , y2Di ,y2Df ,0.0, 1.0); // Proyección ortográfica, dentro del cubo señalado
}


void DesignMine::Present()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer

	//Para definir el punto de vista
	glMatrixMode(GL_PROJECTION); // Proyection mode
	glLoadIdentity(); 
	glOrtho(x2Di , x2Df , y2Di , y2Df ,0.0, 1); // Proyección ortográfica, dentro del cubo señalado
	glMatrixMode(GL_MODELVIEW); // Model mode
	glLoadIdentity();							// Reset The Current Matrix
	glPushMatrix();
	glPopMatrix();
}


void DesignMine::DrawOrigin()
{
	glLineWidth (4);
	glDisable(GL_LINE_STIPPLE);
	glBegin (GL_LINES);
	glColor3f ( 0,1 ,0);
	glVertex2f (0,0); 
	glVertex2f (0,1);
	glEnd ( );
	glBegin (GL_LINES);
	glColor3f ( 1,0.5 ,0);
	glVertex2f(0,0);
	glVertex2f(1,0);
	glEnd ( );
	glPointSize(10.0);
	glBegin ( GL_POINTS );
	glColor3f ( 0, 0, 0 );
	glVertex2f (0,0); 
	glEnd ( );
}


void DesignMine::DrawGrid(float size)
{
	if (Grid==true)
	{
	glColor3f ( 0, 0.6, 0);
	glLineWidth(1);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(5,0xAAAA);
	glBegin(GL_LINES); 
	glNormal3f(0,0,0.8);
	for(int i=0;i<100;i++)
	{
		glVertex3f(i*size,-100*size,0);
		glVertex3f(i*size,100*size,0);			
		glVertex3f(-i*size,-100*size,0);
		glVertex3f(-i*size,100*size,0);			
		
		glVertex3f(-100*size,i*size,0);
		glVertex3f(100*size,i*size,0);				
		glVertex3f(-100*size,-i*size,0);
		glVertex3f(100*size,-i*size,0);				
	}
	glDisable(GL_LINE_STIPPLE);
	glEnd();
	}
	
}


void DesignMine::CalculateScale()
{
	Scale.x=x2Df-x2Di;
	Scale.y=y2Df-y2Di;
}



void DesignMine::ScalePoint()
{
	wxSize SizeDesign;
	Vector2D Transform;
	CalculateScale();
	SizeDesign=this->GetVirtualSize();
	MousePosition.x-=SizeDesign.GetWidth()/2;
	Transform.x=SizeDesign.GetWidth()/Scale.x;
	MousePosition.y-=SizeDesign.GetHeight()/2;
	Transform.y=SizeDesign.GetHeight()/Scale.y;
	x=MousePosition.x/Transform.x+((x2Di+x2Df)/2);
	y=-MousePosition.y/Transform.y+((y2Di+y2Df)/2);
}


void DesignMine::DrawPoints()
{
		SetCurrent();
		if (points.size()>0)
		{
			for (int i=0;i<marcas.size();i++)
			{

				if(marcas[i]==true)
				{
					glPointSize(14.0);
					glBegin ( GL_POINTS );
					glColor3f ( 1, 0.9, 0.2);
					glVertex2f (points[i].x,points[i].y); 
					glEnd ( );
				}
				else
				{
					glPointSize(10.0);
					glBegin ( GL_POINTS );
					glColor3f ( 1, 0, 0);
					glVertex2f (points[i].x,points[i].y); 
					glEnd ( );
				}
			}
		}
}



void DesignMine::DrawLines()
{
	glDisable(GL_LINE_STIPPLE);
	glLineWidth (2);
	if(points.size()>1)
	{
		if (polygon==false) 
		{
				glColor3f(0.0f, 0.0f, 1.0f);
				glBegin( GL_LINE_LOOP );
				for(int i=1;i<points.size();i++)
				{
						glVertex2f (points[i-1].x,points[i-1].y); 
						glVertex2f (points[i].x,points[i].y);
				}
				glEnd ( );
		}
		if (polygon==true)
		{	
				glColor3f(0.0f, 0.0f, 1.0f);
				glBegin(GL_POLYGON);
				glColor3f(0.0f, 0.0f, 1.0f);
				for(int i=1;i<points.size();i++)
				{
					glVertex2f (points[i-1].x,points[i-1].y); 
					glVertex2f (points[i].x,points[i].y);
				}
				glEnd ( );
		}
	}
}


void DesignMine::DrawScene2D()
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		SetCurrent();
		Init();
		Present();
		DrawGrid(gridSize);
		DrawOrigin();
		DrawLines();
		DrawPoints();
		SwapBuffers();
}


void DesignMine::Menu(wxMouseEvent& event)
{
		wxPoint pt = event.GetPosition();
		wxMenu menu,menu2;
		wxMenu *submenuGrid=new wxMenu;
		wxMenu *submenuFigure=new wxMenu;
		wxMenu *submenuSelect=new wxMenu;
		menu.Append(wxID_ANY,wxT("Options"));
		menu.AppendSeparator();
		menu.AppendSubMenu(submenuGrid,wxT("Grid"));
		submenuGrid->Append(ID_SHOWGRID,wxT("GridOn"));
		submenuGrid->Append(ID_HIDEGRID,wxT("GridOFF"));
		menu.AppendSubMenu(submenuFigure,wxT("Figure"));
		submenuFigure->Append(ID_LINES,wxT("Outline"));
		submenuFigure->Append(ID_HALFWAYPOINT,wxT("Half-WayPoint"));
		submenuFigure->Append(ID_POLYGON,wxT("Polygon"));
		menu.AppendSubMenu(submenuSelect,wxT("Select"));
		submenuSelect->Append(ID_SELECTPOINTS,wxT("Select all points"));
		PopupMenu(&menu,pt);
}


void DesignMine::ManageOptions(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_SHOWGRID) 
	{
		Grid=true;
		DrawScene2D();
	}	
	if (id==ID_HIDEGRID) 
	{
		Grid=false;
		DrawScene2D();
	}
	if (id==ID_LINES) 
	{
		polygon=false;
		DrawScene2D();
	}
	if (id==ID_POLYGON) 
	{
		polygon=true;
		DrawScene2D();
	}

	if (id==ID_SELECTPOINTS) 
	{
			estado=2;   //Seleccionamos todos los puntos 
			DrawScene2D();
			

	}
	if (id==ID_HALFWAYPOINT)
	{
			estado=4; //a la espera de la selección de los puntos
	}
	

}



void DesignMine::AlignFunction()
{
		float x1,x2,y2,y1;
		x1=((int)(x/gridSize))*gridSize;
		if (x1>0)
		{
			x2=x1+gridSize;
			if ((x-x1)<(x2-x)) x=x1;
			else x=x2;
		}

		if (x1<0) 
		{
			x2=x1-gridSize;
			if((x1-x)<(x-x2)) x=x1;
			else x=x2;
		}

		if(x1==0)
		{
			if (x>0)
			{
				x2=gridSize;
				if ((x-x1)<(x2-x)) x=x1;
				else x=x2;
			}
			if (x<0)
			{
				x2=-gridSize;
				if((x1-x)<(x-x2)) x=x1;
				else x=x2;
			}
		}


		y1=((int)(y/gridSize))*gridSize;
		if (y1>0)
		{
			y2=y1+gridSize;
			if ((y-y1)<(y2-y)) y=y1;
			else y=y2;
		}

		if (y1<0) 
		{
			y2=y1-gridSize;
			if((y1-y)<(y-y2)) y=y1;
			else y=y2;
		}

		if(y1==0)
		{
			if (y>0)
			{
				y2=gridSize;
				if ((y-y1)<(y2-y)) y=y1;
				else y=y2;
			}
			if (y<0)
			{
				y2=-gridSize;
				if((y1-y)<(y-y2)) y=y1;
				else y=y2;
			}
		}
}


void DesignMine::SetGridSize(float s)
{
	if (s<=0) return;
	gridSize=s;
	DrawScene2D();	
}


void DesignMine::SetView(double xi,double xf,double yi,double yf)
{
	x2Di=xi;
	x2Df=xf;
	y2Di=yi;
	y2Df=yf;
	DrawScene2D();
}


void DesignMine::ChangeZoom(double z)
{
	if (z<previousZoom)  //estamos disminuyendo zoom
	{
		double aux=previousZoom-z;
		x2Di=x2Di-0.05*aux;
		x2Df=x2Df+0.05*aux;
		y2Di=y2Di-0.05*aux;
		y2Df=y2Df+0.05*aux;	
	}
	if (z>previousZoom)  //estamos aumentando zoom
	{
		double aux=z-previousZoom;
		x2Di=x2Di+0.05*aux;
		x2Df=x2Df-0.05*aux;
		y2Di=y2Di+0.05*aux;
		y2Df=y2Df-0.05*aux;	
	}
		DrawScene2D();
		previousZoom=z;	
}



void DesignMine::ChangePosition(bool vertical,bool horizontal,double variation)
{
	if ((vertical==true)&&(horizontal==true)) return;
	if (vertical==true)
	{
		if (previousV<variation)  //Moviendo vertical hacia abajo
		{
			double aux=variation-previousV;
			y2Di=y2Di-0.1*aux;
			y2Df=y2Df-0.1*aux;
		}
		if (previousV>variation)  //Moviendo vertical hacia abajo
		{
			double aux=previousV-variation;
			y2Di=y2Di+0.1*aux;
			y2Df=y2Df+0.1*aux;
		}
		previousV=variation;
		
	}

	if (horizontal==true)
	{
		if (previousH<variation)  //Moviendo horizontal hacia derecha
		{
			double aux=variation-previousH;
			x2Di=x2Di+0.1*aux;
			x2Df=x2Df+0.1*aux;
		}
		if (previousH>variation)  //Moviendo horizontal hacia izquierda
		{
			double aux=previousH-variation;
			x2Di=x2Di-0.1*aux;
			x2Df=x2Df-0.1*aux;
		}
		previousH=variation;
	}


	DrawScene2D();
}




void DesignMine::ChangeView(bool up,bool down,bool right,bool left)
{

	if ((up==true)&&(down==true)) return;  //no puede moverse arriba y abajo a la vez
	if ((right==true)&&(left==true)) return; //no puede moverse a la izquierda y a la derecha a la vez
	
	if (right==true) 
	{
		x2Di=x2Di+0.5;
		x2Df=x2Df+0.5;
	}

	if (left==true)
	{
		x2Di=x2Di-0.5;
		x2Df=x2Df-0.5;
	}

	if (up==true)
	{
		y2Di=y2Di+0.5;
		y2Df=y2Df+0.5;
	}


	if (down==true)
	{
		y2Di=y2Di-0.5;
		y2Df=y2Df-0.5;
	}

	DrawScene2D();
}




void DesignMine::AddPoint(double a,double b)
{
	points.push_back(Vector2D(a,b));
	marcas.push_back(false);
	DrawScene2D();
}



void DesignMine::ChangePoint(int ind,double a,double b)
{
	if (ind<0)return;
	if (ind>=(int)points.size()) return;
	points[ind]=Vector2D(a,b);
	DrawScene2D();
}


void DesignMine::DeleteVertex(int position)
{
	if (position<0)return;
	if (position>=(int)points.size()) return;
	if (points.empty()) return;
	points.erase(points.begin()+position);
	marcas.erase(marcas.begin()+position);
	DrawScene2D();
}



void DesignMine::MoveScreen(wxMouseEvent& event)
{
		AuxPoint.x=x;
		AuxPoint.y=y;
		if(event.MiddleDown())
		{
		MousePosition=event.GetPosition();
		ScalePoint();
		double difx=AuxPoint.x-x;  
		double dify=AuxPoint.y-y;
		x2Di=x2Di+difx;
		y2Di=y2Di+dify;
		x2Df=x2Df+difx;
		y2Df=y2Df+dify;
		DrawScene2D();
		}
}


void DesignMine::New_Move_Point(wxMouseEvent& event)
{
	if(points.size()>=1)
	{
		for (int i=0;i<points.size();i++)
		{
			if ((marcas[i]==true)&&(event.LeftDown())&&(estado==0))
			{
					estado=1;  //Estamos arrastrando el punto
			}
			
			if ((estado==2)&&(event.LeftDown())) 
			{
				estado=0;
				for (int i=0;i<points.size();i++) marcas[i]=false;
			}
		}

		for (int i=0;i<points.size();i++)
		{
			if ((estado==4)&&(event.LeftDown()))
			{
				if (marcas[i]==true)
				{
					point1.x=points[i].x;
					point1.y=points[i].y;
					point1Mark=i;
					DrawScene2D();	
					estado=5;
				}
			}
		}


		for (int i=0;i<points.size();i++)
		{
			if ((estado==6)&&(event.LeftDown()))
			{
				if (marcas[i]==true)
				{
					point2.x=points[i].x;
					point2.y=points[i].y;
					point2Mark=i;
					marcas[point1Mark]=true;
					DrawScene2D();
					estado=7;
				}
			}
		}
		

		if(event.LeftDown()&&(estado==8)) //Se va a posicionar el nuevo punto
		{
			wxSetCursor(wxCURSOR_CROSS);
			MousePosition = event.GetPosition();
			ScalePoint();
			if(Align==true) AlignFunction();

			if(point1Mark==point2Mark) return;

			if (point1Mark<point2Mark)
			{
				if((point2Mark==points.size()-1)&&(point1Mark==0)) 
				{
					points.push_back(Vector2D(x,y));
					marcas.push_back(false);
					wxCommandEvent InsertEvent( wxEVT_INSERT_POINT,GetId() );
					InsertEvent.SetEventObject( window);
					GetEventHandler()->ProcessEvent(InsertEvent);
					estado=0;
					return;
				}
				points.push_back(Vector2D(points[points.size()-1].x,points[points.size()-1].y));
				marcas.push_back(false);
				for(int i=points.size()-2;i>point2Mark;i--)
				{
					points[i]=points[i-1];
				}
				ChangePoint(point2Mark,x,y);
			}


			if (point2Mark<point1Mark)
			{
				if((point1Mark==points.size()-1)&&(point2Mark==0)) 
				{
					points.push_back(Vector2D(x,y));
					marcas.push_back(false);
					wxCommandEvent InsertEvent( wxEVT_INSERT_POINT,GetId() );
					InsertEvent.SetEventObject( window);
					GetEventHandler()->ProcessEvent(InsertEvent);
					estado=0;
					return;
				}

				points.push_back(Vector2D(points[points.size()-1].x,points[points.size()-1].y));
				marcas.push_back(false);
				for(int i=points.size()-2;i>point1Mark;i--)
				{
					points[i]=points[i-1];
				}
			ChangePoint(point1Mark,x,y);
			}
			DrawScene2D();
			wxCommandEvent InsertEvent( wxEVT_INSERT_POINT,GetId() );
			InsertEvent.SetEventObject( window);
			GetEventHandler()->ProcessEvent(InsertEvent);
			estado=9;
		}
	}



	if(event.LeftDown()&&(estado==0))  //Se añade un nuevo punto
	{
		MousePosition = event.GetPosition();
		ScalePoint();
		if(Align==true) AlignFunction();
		MousePoint=true;
		wxCommandEvent DesignEvent( wxEVT_NEWVERTEX_ADDED,GetId() );
		DesignEvent.SetEventObject( window);
		GetEventHandler()->ProcessEvent(DesignEvent);
		points.push_back(Vector2D(x,y));
		marcas.push_back(true);
		DrawScene2D();
		MousePoint=false;
	}
	SetFocus();
	event.Skip();
}




void DesignMine::ManagePoints(wxMouseEvent& event)
{	
	float difx;
	float dify;
		MousePosition = event.GetPosition();
		ScalePoint();
		if (points.size()>0)
		{
			switch (estado)
			{
					case 0:   //si no tenemos pulsado ningún boton del ratón
						wxSetCursor(wxCURSOR_CROSS);
						for (int i=0;i<(int)points.size();i++)
						{
							difx=abs(x-points[i].x);
							dify=abs(y-points[i].y);
							if((abs(difx)<0.3/(zoom/20))&&(abs(dify)<0.3/(zoom/20)))
							{
								MarkNum=i;
								marcas[i]=true;
								
							}
							else
							{
								marcas[i]=false;
								MarkNum=-1;
							}				
						}

						DrawScene2D();
						break;


					case 1: 
						//si estamos arrastrando
						wxSetCursor(wxCURSOR_POINT_LEFT);
						for (int i=0;i<(int)points.size();i++)
						{
							float m,n,mx,my;
							if (marcas[i]==true)
							{
								MarkNum=1;
								for(int i=1;i<points.size();i++)
								{
									if((i!=MarkNum)&&(i-1!=MarkNum))
									{
									mx=points[i].x-points[i-1].x;
									my=points[i].y-points[i-1].y;
									if(my==0) m=0;
									if(mx==0) m=1;
									if((my!=0)&&(mx!=0)) m=my/mx;
									n=points[i].y-m*points[i].x;
									if(points[i].y==m*points[i].x+n) estado=0;
									}
								}
								MarkNum=i;
								ChangePoint(MarkNum,x,y);
								MousePoint=true;
								wxCommandEvent FreePointEvent( wxEVT_CHANGE_POINT,GetId() );
								FreePointEvent.SetEventObject( window);
								GetEventHandler()->ProcessEvent(FreePointEvent);
								MousePoint=false;
							}
						}
						DrawScene2D();
						break;

					case 2: 
						//si hemos seleccionado todos los puntos
						if (estado==2)	wxSetCursor(wxCURSOR_HAND);
						for(int i=0;i<(int)points.size();i++)
						{
							marcas[i]=true;
						}
						DrawScene2D();
						break;

					case 3: //se está moviendo el polígono entero
						if (estado==2)	wxSetCursor(wxCURSOR_HAND);
						for(int i=0;i<(int)points.size();i++)
						{
							points[i].x=points[i].x+(x-xLast);
							points[i].y=points[i].y+(y-yLast);
							MarkNum=i;
							
						}
							xLast=x;
							yLast=y;
							DrawScene2D();
							break;

					case 4:
					case 5:
					case 6:
					case 7:
						wxSetCursor(wxCURSOR_LEFT_BUTTON);
						for (int i=0;i<(int)points.size();i++)
						{
							difx=abs(x-points[i].x);
							dify=abs(y-points[i].y);
							if((abs(difx)<0.5/(zoom/15))&&(abs(dify)<0.5/(zoom/20)))
							{
								marcas[i]=true;
							}
							else
							{
								marcas[i]=false;

							}				
						}
					case 8:
						wxSetCursor(wxCURSOR_CROSS);
				}
		}
	SetFocus();
	event.Skip();

}


void DesignMine::FreePoint(wxMouseEvent& event)
{
	if((event.LeftUp())&&(estado==1))
	{
		MousePosition=event.GetPosition();
		ScalePoint();
		estado=0;
		DrawScene2D();
	}

	if((event.LeftUp())&&(estado==5)) estado=6;
	if((event.LeftUp())&&(estado==7)) estado=8;
	if((event.LeftUp())&&(estado==9)) estado=0;
	SetFocus();
	event.Skip();
}



void DesignMine::MenuRightButton(wxMouseEvent& event)
{
	MousePosition=event.GetPosition();
	ScalePoint();
	float PREX,PREY;
		if(points.size()>=1)
		{
			for (int i=0;i<points.size();i++)
			{
				if ((marcas[i]==true)&&(event.RightDown()&&(estado==0))) 
				{
					MarkNum=i;
					wxPoint pt = event.GetPosition();
					wxMenu menu;
					menu.Append(ID_ERASEVERTEX,wxT("Delete Vertex"));
					PopupMenu(&menu,pt);
				}
			}
			if ((event.RightDown()&&(estado==2))) 
			{
				MousePosition=event.GetPosition();
				ScalePoint();
				xLast=x;
				yLast=y;
				estado=3; //Se mueven todos los puntos
			}
			if ((event.RightUp()&&(estado==3))) 
			{
				MousePoint=true;
				wxCommandEvent MovePolygonEvent( wxEVT_MOVE_POLYGON,GetId() );
				MovePolygonEvent.SetEventObject( window);
				GetEventHandler()->ProcessEvent(MovePolygonEvent);
				MousePoint=false;
				estado=0; 
			}

		}
}


void DesignMine::MenuPointOptions(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_ERASEVERTEX) 
	{
		MousePoint=true;
		wxCommandEvent DeletePointEvent( wxEVT_DELETE_POINT,GetId() );
		DeletePointEvent.SetEventObject( window);
		GetEventHandler()->ProcessEvent(DeletePointEvent);
		MousePoint=false;
		DeleteVertex(MarkNum);
	}	 
}


void DesignMine::eraseDesign()
{
	for (int i=0;i<points.size();i++)
	{
		points.erase(points.begin()+i);
		marcas.erase(marcas.begin()+i);
	}
	
}

