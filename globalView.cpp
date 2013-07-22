#include "globalView.h"
#include "definitions.h"
#include "simulatedWorld.h"
#include "wx/bmpbuttn.h"



BEGIN_EVENT_TABLE(globalView, wxFrame)
	EVT_PAINT(globalView::Paint)
	EVT_SLIDER(ID_ZOOMDESIGN,globalView::ChangeZoom)
	EVT_SPIN_UP(ID_GRIDSIZE,globalView::ChangeGridSize)
	EVT_SPIN_DOWN(ID_GRIDSIZE,globalView::ChangeGridSize)
	EVT_SPINCTRL(ID_GRIDSIZE,globalView::ChangeGridSize)
	EVT_SLIDER(ID_VERTICALMOVE,globalView::ChangePosition)
	EVT_SLIDER(ID_HORIZONTALMOVE,globalView::ChangePosition)
	EVT_RADIOBUTTON(ID_ALIGNON,globalView::FaceAlign)
	EVT_RADIOBUTTON(ID_ALIGNOFF,globalView::FaceAlign)
	EVT_COMMAND(wxID_ANY,wxEVT_NEWVERTEX_ADDED,globalView::AddedMousePoint)
	EVT_COMMAND(wxID_ANY,wxEVT_CHANGE_POINT,globalView::ChangePoint)
	EVT_COMMAND(wxID_ANY,wxEVT_DELETE_POINT,globalView::DeletePoint)
	EVT_COMMAND(wxID_ANY,wxEVT_MOVE_POLYGON,globalView::ChangePolygonPosition)
	EVT_COMMAND(wxID_ANY,wxEVT_INSERT_POINT,globalView::InsertPoint)
	EVT_BUTTON(ID_HALFWAYPOINT,globalView::ManageButtons)
	EVT_BUTTON(ID_SELECTPOINTS,globalView::ManageButtons)
	EVT_BUTTON(ID_ERASEPOINTS,globalView::ManageButtons)

END_EVENT_TABLE()

globalView::globalView(wxWindow *parent, wxWindowID id,const wxString& title)
:wxFrame(parent, wxID_ANY,title,wxDefaultPosition, wxSize(750,750),wxSTAY_ON_TOP | wxCAPTION ) 
{
	CreatePanel();
	Panel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(750,740));
	Panel->SetBackgroundColour(*wxLIGHT_GREY);
	CreateFace();
	

}

void globalView::CreatePanel()
{
	wxBitmap bitmaps[3];
	bitmaps[0] = wxBitmap (halfwaypoint_xpm);
	bitmaps[1] = wxBitmap (move_xpm);
	bitmaps[2] = wxBitmap (erase_xpm);

	wxBitmapButton *HalfPoint=new wxBitmapButton(this,ID_HALFWAYPOINT,bitmaps[0],wxDefaultPosition,wxSize(30,30));
	wxBitmapButton *MovePoints=new wxBitmapButton(this,ID_SELECTPOINTS,bitmaps[1],wxDefaultPosition,wxSize(30,30));
	wxBitmapButton *erase=new wxBitmapButton(this,ID_ERASEPOINTS,bitmaps[2],wxDefaultPosition,wxSize(30,30));
	points=new PointsList(this);
	points->AssociateDesign2D(this);
	Finish=new wxButton(this,ID_ADDOWNFACE,wxT("Load"),wxDefaultPosition,wxSize(60,30));
	wxStaticBoxSizer *Button=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Load Design"));
	Button->Add(Finish,5,wxEXPAND);
	wxBoxSizer *global=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BOX=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *fbox=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *ConfigurationsBox=new wxBoxSizer(wxHORIZONTAL);
	wxStaticBoxSizer *ControlButtons=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Draw"));
	wxStaticBoxSizer* sizerVertical=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Vertical"));
	wxStaticBoxSizer* sizerHorizontal=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Horizontal"));
	Vertical=new wxSlider(this,ID_VERTICALMOVE,50,0,100,wxDefaultPosition,wxSize(40,500),wxSL_VERTICAL|wxSL_RIGHT,wxDefaultValidator,wxT("VERTICAL POSITION"));
	Vertical->SetTickFreq(1);
	Horizontal=new wxSlider(this,ID_HORIZONTALMOVE,50,0,100,wxDefaultPosition,wxSize(500,40),wxSL_HORIZONTAL,wxDefaultValidator,wxT("HORIZONTAL POSITION"));
	Horizontal->SetTickFreq(1);
	fbox->Add(Vertical,0,wxFIXED_MINSIZE);
	Screen2D=new DesignMine(this,wxID_ANY,wxDefaultPosition,wxSize(550,550));
	fbox->Add(Screen2D,5,wxEXPAND);
	Zoom=new wxSlider(this,ID_ZOOMDESIGN,100,10,200,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL|wxSL_LABELS|wxSL_AUTOTICKS,wxDefaultValidator,wxT("ZOOM"));
	Zoom->SetTickFreq(1);
	ConfigurationsBox->Add(Zoom,5,wxEXPAND);
	wxRadioButton* alignOn=new wxRadioButton(this,ID_ALIGNON,wxT("&ON"),wxDefaultPosition,wxDefaultSize,wxRB_GROUP);
	wxRadioButton* alignOFF=new wxRadioButton(this,ID_ALIGNOFF,wxT("&OFF"));
	alignOn->SetValue(false);
	alignOFF->SetValue(true);
	wxStaticBoxSizer* sizerAlign=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Align"));
	sizerAlign->Add(alignOn,0,wxALIGN_CENTER_VERTICAL|wxALL);
	sizerAlign->Add(alignOFF,0,wxALIGN_CENTER_VERTICAL|wxALL);
	ConfigurationsBox->Add(sizerAlign,5,wxEXPAND);
	radioGrid=new wxSpinCtrl(this,ID_GRIDSIZE,wxT("20"),wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,1,50,20);
	wxBoxSizer* sizerGrid=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Grid Size (x10)"));
	sizerGrid->Add(radioGrid,0,wxEXPAND);
	ConfigurationsBox->Add(sizerGrid,0,wxFIXED_MINSIZE);
	ConfigurationsBox->Add(Button,5,wxFIXED_MINSIZE);
	ControlButtons->Add(HalfPoint,0,wxALIGN_CENTER);
	ControlButtons->Add(MovePoints,0,wxALIGN_CENTER);
	ControlButtons->Add(erase,0,wxALIGN_CENTER);
	BOX->Add(ConfigurationsBox,0,wxEXPAND);
	BOX->Add(ControlButtons,0,wxEXPAND);
	BOX->Add(fbox,0,wxEXPAND);
	BOX->Add(Horizontal,0,wxFIXED_MINSIZE|wxALIGN_RIGHT);
	global->Add(BOX,0,wxEXPAND);
	global->Add(points,10,wxEXPAND);
	SetSizer(global);
}

void globalView::CreateFace()
{
	face=new Face();
}

void globalView::ChangeZoom(wxCommandEvent& event)
{
	int id=event.GetId();

	if (id==ID_ZOOMDESIGN)
	{
		int scalado=Zoom->GetValue();
		Screen2D->SetZoom(scalado);
		Screen2D->ChangeZoom(scalado);
	}
}


void globalView::ChangeGridSize(wxSpinEvent& event)  //Función que controlará el tamaño de la cuadricula
{
		float gridval;
		gridval=radioGrid->GetValue();
		gridval=gridval/10;
		Screen2D->SetGridSize(gridval);
}


void globalView::FaceAlign(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_ALIGNON)
	{
		Screen2D->SetAlign(true);
	}
	if (id==ID_ALIGNOFF)
	{
		Screen2D->SetAlign(false);
	}
}


void globalView::ChangePosition(wxCommandEvent& event)
{
	int id=event.GetId();

	if (id==ID_VERTICALMOVE)
	{
		int Vmov=Vertical->GetValue();
		Screen2D->ChangePosition(true,false,Vmov);
	}

	if (id==ID_HORIZONTALMOVE)
	{
		int Hmov=Horizontal->GetValue();
		Screen2D->ChangePosition(false,true,Hmov);
	}
}


void globalView::AddedMousePoint(wxCommandEvent& event)
{
	points->SetPoints(Screen2D->GetPointX(),Screen2D->GetPointY());
}



void globalView::ChangePoint(wxCommandEvent& event)
{
	int resp=Screen2D->GetMarkedPoint();
	if (resp<0) return;
	if (resp>=Screen2D->NumPoints()) return;
	points->MovedPoint(resp,Screen2D->GetPointX(),Screen2D->GetPointY());
}


void globalView::DeletePoint(wxCommandEvent& event)
{
	int resp=Screen2D->GetMarkedPoint();
	if (resp<0) return;
	if (resp>(Screen2D->NumPoints())) return;
	points->DeletePointMarked(resp);
}


void globalView::InsertPoint(wxCommandEvent& event)
{
	if(Screen2D->GetPoint1Mark()==Screen2D->GetPoint2Mark()) return;

	if(Screen2D->GetPoint1Mark()<Screen2D->GetPoint2Mark())
	{
		if((Screen2D->GetPoint1Mark()==0)&&(Screen2D->GetPoint2Mark()==Screen2D->NumPoints()-2))  //Insertamos un punto entre el último y el primero
		{
			
			points->SetDesignAssociated(false);
			points->SetPoints(Screen2D->GetVector()[Screen2D->NumPoints()-1].x,Screen2D->GetVector()[Screen2D->NumPoints()-1].y);
			face->addVertex(Screen2D->GetVector()[Screen2D->NumPoints()-1].x,Screen2D->GetVector()[Screen2D->NumPoints()-1].y);
			return;
		}
		points->InsertedPoint(Screen2D->GetPoint1Mark(),Screen2D->GetPoint2Mark(),Screen2D->GetPointX(),Screen2D->GetPointY());
		face->addVertex(Screen2D->GetVector()[Screen2D->NumPoints()-1].x,Screen2D->GetVector()[Screen2D->NumPoints()-1].y);
		for(int i=Screen2D->NumPoints()-2;i>=Screen2D->GetPoint2Mark();i--)
		{
			face->changeVertex(i,Screen2D->GetVector()[i].x,Screen2D->GetVector()[i].y);
		}
		//face->changeVertex(Screen2D->GetPoint2Mark(),Screen2D->GetVector()[Screen2D->GetPoint2Mark()].x,Screen2D->GetVector()[Screen2D->GetPoint2Mark()].y);
	}


	if(Screen2D->GetPoint2Mark()<Screen2D->GetPoint1Mark())
	{
		if((Screen2D->GetPoint2Mark()==0)&&(Screen2D->GetPoint1Mark()==Screen2D->NumPoints()-2))  //Insertamos un punto entre el último y el primero
		{
			points->SetDesignAssociated(false);
			points->SetPoints(Screen2D->GetVector()[Screen2D->NumPoints()-1].x,Screen2D->GetVector()[Screen2D->NumPoints()-1].y);
			face->addVertex(Screen2D->GetVector()[Screen2D->NumPoints()-1].x,Screen2D->GetVector()[Screen2D->NumPoints()-1].y);
			return;
		}
		points->InsertedPoint(Screen2D->GetPoint1Mark(),Screen2D->GetPoint2Mark(),Screen2D->GetPointX(),Screen2D->GetPointY());
		face->addVertex(Screen2D->GetVector()[Screen2D->NumPoints()-1].x,Screen2D->GetVector()[Screen2D->NumPoints()-1].y);
		for(int i=Screen2D->NumPoints()-2;i>=Screen2D->GetPoint1Mark();i--)
		{
			face->changeVertex(i,Screen2D->GetVector()[i].x,Screen2D->GetVector()[i].y);
		}
		//face->changeVertex(Screen2D->GetPoint1Mark(),Screen2D->GetVector()[Screen2D->GetPoint1Mark()].x,Screen2D->GetVector()[Screen2D->GetPoint1Mark()].y);

	}

}



void globalView::ManagePoints(bool addPoint,bool changePoint,bool deletePoint,int deleteRow)
{
	if (addPoint)
	{
		if (Screen2D->GetTypeOfPoint()==false) //Si añadimos el punto desde el Grid
		{
			Vector2D vertex=points->getLastPointAdded();
			Screen2D->AddPoint(vertex.x,vertex.y);
			face->addVertex(vertex.x,vertex.y);
		}
		else
		{
			face->addVertex(Screen2D->GetPointX(),Screen2D->GetPointY());
		}
	}

	if (changePoint)
	{
		if (Screen2D->GetTypeOfPoint()==false)
		{
			Vector2D vertex=points->getChangePointAdded();
			int dir=points->getAuxRow();
			Screen2D->ChangePoint(dir,vertex.x,vertex.y);
			face->changeVertex(dir,vertex.x,vertex.y);
		}
		else
		{
		face->changeVertex(Screen2D->GetMarkedPoint(),Screen2D->GetPointX(),Screen2D->GetPointY());
		}
	}
	
	if (deletePoint)
	{
		if (Screen2D->GetTypeOfPoint()==false)
		{
			int ind=deleteRow;
			face->deleteVertex (ind);
			Screen2D->DeleteVertex(ind);
			face->deleteVertex (ind);
		}
		else 
		{
			face->deleteVertex (Screen2D->GetMarkedPoint());
		}
	}

}



void globalView::ChangePolygonPosition(wxCommandEvent& event)
{
	points->MovedPoints();
	for(int i=0;i<Screen2D->NumPoints();i++)
	{
		face->changeVertex(i,Screen2D->GetVector()[i].x,Screen2D->GetVector()[i].y);
	}
}



void globalView::ManageButtons(wxCommandEvent& event)
{
	int id=event.GetId();

	if (id==ID_ERASEPOINTS)
	{
		Screen2D->eraseDesign();
		for (int i=0;i<points->grid->GetNumberRows();i++)
		{
			points->DeletePoint();
		}
	}

	if (id==ID_SELECTPOINTS)
	{
		Screen2D->SetCondition(2);
	}

	if (id==ID_HALFWAYPOINT)
	{
		Screen2D->SetCondition(4);
	}
}



void globalView::Paint(wxPaintEvent& event)
{
	wxPaintDC(this);
}