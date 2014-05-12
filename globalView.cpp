#include "globalView.h"
#include "definitions.h"
#include "simulatedWorld.h"
#include "wx/bmpbuttn.h"



DEFINE_EVENT_TYPE(wxEVT_EDIT_CLOSED)

BEGIN_EVENT_TABLE(globalView, wxFrame)
	EVT_PAINT(globalView::OnPaint)
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
	EVT_MENU(ID_HALFWAYPOINT,globalView::ManageButtons)
	EVT_MENU(ID_SELECTPOINTS,globalView::ManageButtons)
	EVT_MENU(ID_ERASEPOINTS,globalView::ManageButtons)
	EVT_MENU(ID_SHOWGRID,globalView::ManageButtons)
	EVT_SLIDER(ID_ZOOMDESIGN,globalView::OnChangeZoom)
	EVT_TEXT_ENTER(ID_ZOOMVALUE,globalView::OnChangeZoom)
	EVT_CLOSE(globalView::OnClose)
END_EVENT_TABLE()

globalView::globalView(wxWindow *parent,wxWindowID id,const wxString& title)
:wxFrame(parent, wxID_ANY,title,wxDefaultPosition, wxSize(730,710),wxSTAY_ON_TOP | wxCAPTION | wxCLOSE_BOX ) 
{
	CreatePanel();
	Panel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(730,710));
	CreateFace();
}


void globalView::CreatePanel()
{
	wxColour colour(193,221,224);
	wxBitmap bitmaps[2];
	bitmaps[0] = wxBitmap (zoom_xpm);
	bitmaps[1] = wxBitmap (align_xpm);

	wxBoxSizer* global=new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* Zsizer=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* Zcomp1Sizer=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* Zcomp2Sizer=new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* AlignCompSizer=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* AlignSizer=new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* GridSizer=new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *DesignPanel=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *optionsBox=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BoxH=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BoxV=new wxBoxSizer(wxVERTICAL);
	InitToolbar();
	
	wxStaticBitmap* zoomIcon=new wxStaticBitmap(this,wxID_ANY,bitmaps[0],wxDefaultPosition,wxSize(32,32));
	wxStaticText* Zoom=new wxStaticText(this,wxID_ANY,wxT("Zoom"),wxDefaultPosition,wxSize(30,20));
	slidZoom=new wxSlider();
	slidZoom->Create(this,ID_ZOOMDESIGN,75,5,150,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL | wxSL_BOTTOM);
	ZoomValue=new wxTextCtrl(this,ID_ZOOMVALUE,wxEmptyString,wxDefaultPosition,wxSize(35,15),wxTE_CENTER);
	
	wxStaticText* AlignText=new wxStaticText(this,wxID_ANY,wxT("Align"),wxDefaultPosition,wxSize(35,15));
	wxStaticBitmap* alignIcon=new wxStaticBitmap(this,wxID_ANY,bitmaps[1],wxDefaultPosition,wxSize(32,32));
	wxRadioButton* alignOn=new wxRadioButton(this,ID_ALIGNON,wxT("&ON"),wxDefaultPosition,wxDefaultSize,wxRB_GROUP);
	wxRadioButton* alignOFF=new wxRadioButton(this,ID_ALIGNOFF,wxT("&OFF"));
	alignOn->SetValue(false);
	alignOFF->SetValue(true);
	
	wxStaticText* GridSize=new wxStaticText(this,wxID_ANY,wxT("Grid Size"),wxDefaultPosition,wxSize(45,15));
	radioGrid=new wxSpinCtrl(this,ID_GRIDSIZE,wxT("20"),wxDefaultPosition,wxSize(60,35),wxSP_ARROW_KEYS,1,50,20);
	
	points=new PointsList(this);
	points->AssociateDesign2D(this);
	Screen2D=new DesignMine(this,wxID_ANY,wxDefaultPosition,wxSize(550,550));
	Vertical=new wxSlider(this,ID_VERTICALMOVE,50,0,100,wxDefaultPosition,wxSize(40,550),wxSL_VERTICAL|wxSL_RIGHT,wxDefaultValidator,wxT("VERTICAL POSITION"));
	Vertical->SetTickFreq(1);
	Horizontal=new wxSlider(this,ID_HORIZONTALMOVE,50,0,100,wxDefaultPosition,wxSize(550,40),wxSL_HORIZONTAL|wxSL_BOTTOM ,wxDefaultValidator,wxT("HORIZONTAL POSITION"));
	Horizontal->SetTickFreq(1);
	Cancel=new wxButton(this,ID_CANCELDESIGN,wxT("Cancel"),wxDefaultPosition,wxSize(60,30));
	Finish=new wxButton(this,ID_ADDOWNFACE,wxT("Accept"),wxDefaultPosition,wxSize(60,30));
	
	
	Zcomp2Sizer->Add(Zoom,0,wxEXPAND);
	Zcomp2Sizer->Add(zoomIcon,0,wxEXPAND);
	Zcomp1Sizer->Add(slidZoom,0,wxEXPAND);
	Zcomp1Sizer->Add(ZoomValue,5,wxSHRINK);
	Zsizer->Add(Zcomp2Sizer,0,wxEXPAND);
	Zsizer->Add(Zcomp1Sizer,0,wxEXPAND);

	AlignCompSizer->Add(alignIcon,0,wxEXPAND);
	AlignCompSizer->Add(alignOn,0,wxEXPAND);
	AlignCompSizer->Add(alignOFF,0,wxEXPAND);
	AlignSizer->Add(AlignText,0,wxEXPAND);
	AlignSizer->Add(AlignCompSizer,0,wxEXPAND);

	GridSizer->Add(GridSize,0,wxEXPAND);
	GridSizer->Add(radioGrid,0,wxEXPAND);
	
	optionsBox->Add(Zsizer,0,wxEXPAND);
	optionsBox->AddSpacer(30);
	optionsBox->Add(AlignSizer,0,wxEXPAND);
	optionsBox->AddSpacer(30);
	optionsBox->Add(GridSizer,0,wxEXPAND);
	optionsBox->AddSpacer(30);
	optionsBox->Add(Finish,0,wxEXPAND);
	optionsBox->Add(Cancel,0,wxEXPAND);
	
	BoxH->Add(Vertical,0,wxEXPAND);
	BoxH->Add(Screen2D,0,wxEXPAND);
	BoxV->Add(BoxH,0,wxEXPAND);
	BoxV->Add(Horizontal,0,wxEXPAND);

	DesignPanel->Add(optionsBox,0,wxEXPAND);
	DesignPanel->Add(Toolbar,5,wxEXPAND);
	DesignPanel->Add(BoxV,0,wxEXPAND);
	global->Add(DesignPanel,0,wxEXPAND);
	global->Add(points,0,wxEXPAND);
	SetSizer(global);
	this->SetBackgroundColour(colour);
		
}


void globalView::InitToolbar()
{
	wxBitmap bitmaps[6];
	bitmaps[0] = wxBitmap (halfwaypoint_xpm);
	bitmaps[1] = wxBitmap (move_xpm);
	bitmaps[2] = wxBitmap (erase_xpm);
	bitmaps[3]=	 wxBitmap(icon_copy_xpm);
	bitmaps[4]=	 wxBitmap(PasteIcon_xpm);
	bitmaps[5]=	 wxBitmap(grid_xpm);
	Toolbar=new wxToolBar();
	Toolbar->Create(this,wxID_ANY,wxDefaultPosition,wxSize(210,25),wxBORDER_NONE|wxTB_HORIZONTAL);
	Toolbar->AddTool(ID_HALFWAYPOINT,bitmaps[0], wxT("Insert point"));
	Toolbar->AddTool(ID_SELECTPOINTS,bitmaps[1], wxT("Move Figure"));
	Toolbar->AddTool(ID_ERASEPOINTS,bitmaps[2], wxT("Erase Figure"));
	Toolbar->AddTool(ID_COPYDESIGN,bitmaps[3], wxT("Copy Figure"));
	Toolbar->AddTool(ID_PASTEDESIGN,bitmaps[4], wxT("Paste Figure"));
	Toolbar->AddTool(ID_SHOWGRID,bitmaps[5], wxT("Show/Hide grid"));
	Toolbar->SetBackgroundColour(wxColour(161,176,187));
	Toolbar->Realize();
}

void globalView::CreateFace()
{
	face=new Face();
}


void globalView::OnChangeZoom(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_ZOOMDESIGN)
	{
		wxString zoomstr;
		int scalado=slidZoom->GetValue();
		zoomstr<<scalado;
		Screen2D->SetZoom(scalado);
		Screen2D->ChangeZoom(scalado);
		ZoomValue->Clear();
		ZoomValue->AppendText(zoomstr);
	}

	if(id==ID_ZOOMVALUE)
	{
		wxString zoomstr=ZoomValue->GetValue();
		int value=wxAtoi(zoomstr);
		if((value>5)&&(value<150))
		{
			Screen2D->SetZoom(value);
			Screen2D->ChangeZoom(value);	
			slidZoom->SetValue(value);
		}
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

		points->DeletePoints();
		DeleteFace();
	}



	if (id==ID_SELECTPOINTS)
	{
		Screen2D->SetCondition(2);
	}

	if (id==ID_HALFWAYPOINT)
	{
		Screen2D->SetCondition(4);
	}


	if (id==ID_SHOWGRID) 
	{
		if (Screen2D->GetGridState()==true)
		{
			Screen2D->SetGridState(false);
			Screen2D->DrawScene2D();
			return;
		}

		if (Screen2D->GetGridState()==false)
		{
			Screen2D->SetGridState(true);
			Screen2D->DrawScene2D();
			return;
		}
	}
}




void globalView::LoadFace(Face* loaded)
{
	for(int i=0;i<loaded->getNumVertex();i++)
	{
		points->SetPoints(loaded->getAbsoluteVertex(i).x,loaded->getAbsoluteVertex(i).y);
	}
}



void globalView::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
}


void globalView::DeleteFace()
{
	delete face;
	face=new Face();
}



void globalView::PasteDesign(vector<Vector2D> CopiedFace)
{
	if (CopiedFace.size()<=0) return;
	Screen2D->SetPaste(true);
	for(int i=0;i<CopiedFace.size();i++)
	{
		Screen2D->SetAuxPoints(CopiedFace[i].x,CopiedFace[i].y);
	}
	Screen2D->DrawScene2D();
}


void globalView::OnClose(wxCloseEvent& event)
{
	this->Show(false);
	wxCommandEvent CloseEditEvent( wxEVT_EDIT_CLOSED,GetId() );
	CloseEditEvent.SetEventObject(this);
	GetEventHandler()->ProcessEvent(CloseEditEvent);
}