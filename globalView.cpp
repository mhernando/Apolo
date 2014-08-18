#include "globalView.h"
#include "definitions.h"
#include "simulatedWorld.h"
#include "wx/bmpbuttn.h"


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
	EVT_COMBOBOX(ID_SETSIZE,globalView::SetCanvasSize)
END_EVENT_TABLE()

globalView::globalView(wxWindow *parent,wxWindowID id,const wxString& title)
:wxFrame(parent, wxID_ANY,title,wxDefaultPosition, wxSize(725,685),wxSTAY_ON_TOP | wxCAPTION | wxCLOSE_BOX ) 
{
	Panel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(725,685));
	CreatePanel();
	CreateFace();
	initializing=false;
	newSize=0;
}

void globalView::CreatePanel()
{
	wxColour colour(193,221,224);
	Panel->SetBackgroundColour(colour);
	wxBitmap bitmaps[6];
	bitmaps[0] = wxBitmap (zoom_xpm);
	bitmaps[1] = wxBitmap (align_xpm);
	bitmaps[2]= wxBitmap(SceneSize_xpm);
	bitmaps[3]= wxBitmap(DesignSize_xpm);
	bitmaps[4]= wxBitmap(CancelIcon_xpm);
	bitmaps[5]= wxBitmap(AcceptIcon_xpm);


	wxBoxSizer* global=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* frame=new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* Zsizer=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* Zcomp1Sizer=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* Zcomp2Sizer=new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* AlignCompSizer=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* AlignSizer=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* SelectSizeSizerV=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* SelectSizeSizer=new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* GridSizerV=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* GridSizerH=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* Sistem=new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer *DesignPanel=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *optionsBox=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BoxH=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *BoxV=new wxBoxSizer(wxVERTICAL);
	InitToolbar();
	
	
	wxStaticText* SizeText=new wxStaticText(Panel,wxID_ANY,wxT("Canvas size"),wxDefaultPosition,wxSize(35,15));
	wxStaticBitmap* SizeIcon=new wxStaticBitmap(Panel,wxID_ANY,bitmaps[3],wxDefaultPosition,wxSize(30,30));
	selectSize=new wxComboBox();
	selectSize->Create(Panel,ID_SETSIZE,wxEmptyString,wxDefaultPosition,wxSize(80,20),Sizes,wxCB_DROPDOWN);
	InsertSizes();
	selectSize->SetSelection(0);

	wxStaticBitmap* zoomIcon=new wxStaticBitmap(Panel,wxID_ANY,bitmaps[0],wxDefaultPosition,wxSize(32,32));
	wxStaticText* Zoom=new wxStaticText(Panel,wxID_ANY,wxT("Zoom"),wxDefaultPosition,wxSize(30,20));
	slidZoom=new wxSlider();
	slidZoom->Create(Panel,ID_ZOOMDESIGN,1,1,10,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL | wxSL_BOTTOM);
	ZoomValue=new wxTextCtrl(Panel,ID_ZOOMVALUE,wxEmptyString,wxDefaultPosition,wxSize(35,15),wxTE_CENTER);
	wxString zoomstr;
	int scalado=slidZoom->GetValue();
	zoomstr<<scalado;
	ZoomValue->AppendText(zoomstr);

	wxStaticText* AlignText=new wxStaticText(Panel,wxID_ANY,wxT("Align"),wxDefaultPosition,wxSize(35,15));
	wxStaticBitmap* alignIcon=new wxStaticBitmap(Panel,wxID_ANY,bitmaps[1],wxDefaultPosition,wxSize(32,32));
	wxRadioButton* alignOn=new wxRadioButton(Panel,ID_ALIGNON,wxT("&ON"),wxDefaultPosition,wxDefaultSize,wxRB_GROUP);
	wxRadioButton* alignOFF=new wxRadioButton(Panel,ID_ALIGNOFF,wxT("&OFF"));
	alignOn->SetValue(false);
	alignOFF->SetValue(true);
	
	wxStaticText* GridSize=new wxStaticText(Panel,wxID_ANY,wxT("Grid Size"),wxDefaultPosition,wxSize(45,15));
	radioGrid=new wxSpinCtrl(Panel,ID_GRIDSIZE,wxT("20"),wxDefaultPosition,wxSize(60,35),wxSP_ARROW_KEYS,1,50,20);
	wxStaticBitmap* gridIcon=new wxStaticBitmap(Panel,wxID_ANY,bitmaps[2],wxDefaultPosition,wxSize(32,32));

	points=new PointsList(Panel);
	points->AssociateDesign2D(this);
	Screen2D=new DesignMine(Panel,wxID_ANY,wxDefaultPosition,wxSize(550,550));
	Screen2D->SetCanvasSize(-5,5,-5,5);
	Vertical=new wxSlider(Panel,ID_VERTICALMOVE,50,0,100,wxDefaultPosition,wxSize(30,535),wxSL_VERTICAL|wxSL_RIGHT,wxDefaultValidator,wxT("VERTICAL POSITION"));
	Vertical->SetTickFreq(1);
	Horizontal=new wxSlider(Panel,ID_HORIZONTALMOVE,50,0,100,wxDefaultPosition,wxSize(550,30),wxSL_HORIZONTAL|wxSL_BOTTOM ,wxDefaultValidator,wxT("HORIZONTAL POSITION"));
	Horizontal->SetTickFreq(1);
	Cancel=new wxBitmapButton(Panel,ID_CANCELDESIGN,bitmaps[4],wxDefaultPosition);
	Accept=new wxBitmapButton(Panel,ID_ADDOWNFACE,bitmaps[5],wxDefaultPosition);
	
	Zcomp2Sizer->Add(Zoom,0,wxEXPAND);
	Zcomp2Sizer->Add(zoomIcon,0,wxEXPAND);
	Zcomp1Sizer->Add(slidZoom,0,wxEXPAND);
	Zcomp1Sizer->Add(ZoomValue,5,wxSHRINK);
	Zsizer->Add(Zcomp2Sizer,0,wxEXPAND);
	Zsizer->Add(Zcomp1Sizer,0,wxEXPAND);
	
	SelectSizeSizerV->Add(SizeText,0,wxEXPAND);
	SelectSizeSizer->Add(SizeIcon,0,wxEXPAND);
	SelectSizeSizer->Add(selectSize,0,wxEXPAND);
	SelectSizeSizerV->Add(SelectSizeSizer,0,wxEXPAND);

	AlignCompSizer->Add(alignIcon,0,wxEXPAND);
	AlignCompSizer->Add(alignOn,0,wxEXPAND);
	AlignCompSizer->Add(alignOFF,0,wxEXPAND);
	AlignSizer->Add(AlignText,0,wxEXPAND);
	AlignSizer->Add(AlignCompSizer,0,wxEXPAND);

	GridSizerV->Add(GridSize,0,wxEXPAND);
	GridSizerH->Add(gridIcon,0,wxEXPAND);
	GridSizerH->Add(radioGrid,0,wxEXPAND);
	GridSizerV->Add(GridSizerH,0,wxEXPAND);

	optionsBox->Add(Zsizer,0,wxEXPAND);
	optionsBox->AddSpacer(20);
	optionsBox->Add(SelectSizeSizerV,0,wxEXPAND);
	optionsBox->AddSpacer(20);
	optionsBox->Add(AlignSizer,0,wxEXPAND);
	optionsBox->AddSpacer(20);
	optionsBox->Add(GridSizerV,0,wxEXPAND);
	optionsBox->AddSpacer(20);
	
	
	BoxH->Add(Vertical,0,wxSHRINK|wxALIGN_BOTTOM);
	BoxH->Add(Screen2D,0,wxGROW);
	BoxV->Add(BoxH,0,wxGROW);
	BoxV->Add(Horizontal,0,wxSHRINK|wxALIGN_RIGHT);

	DesignPanel->Add(optionsBox,0,wxEXPAND);
	DesignPanel->Add(Toolbar,0,wxEXPAND);
	DesignPanel->Add(BoxV,0,wxEXPAND);
	
	Sistem->Add(Accept,0,wxALIGN_RIGHT);
	Sistem->AddSpacer(30);
	Sistem->Add(Cancel,0,wxALIGN_RIGHT);
	global->Add(DesignPanel,0,wxEXPAND);
	frame->Add(points,15,wxEXPAND);
	frame->Add(Sistem,0,wxALIGN_BOTTOM|wxALIGN_RIGHT);
	global->Add(frame,0,wxEXPAND);
	global->SetSizeHints(Panel);
	SetSizer(global);
	ManageSliders();
		
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
	Toolbar->Create(Panel,wxID_ANY,wxDefaultPosition,wxSize(210,25),wxBORDER_NONE|wxTB_HORIZONTAL);
	Toolbar->AddTool(ID_HALFWAYPOINT,bitmaps[0], wxT("Insert point"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_SELECTPOINTS,bitmaps[1], wxT("Move Figure"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_ERASEPOINTS,bitmaps[2], wxT("Erase Figure"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_COPYDESIGN,bitmaps[3], wxT("Copy Figure"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_PASTEDESIGN,bitmaps[4], wxT("Paste Figure"));
	Toolbar->AddSeparator();
	Toolbar->AddTool(ID_SHOWGRID,bitmaps[5], wxT("Show/Hide grid"));
	Toolbar->SetBackgroundColour(*wxWHITE);
	Toolbar->Realize();
}

void globalView::InsertSizes()
{
	const wxString aux1(wxT("10x10"));
	const wxString aux2(wxT("20x20"));
	const wxString aux3(wxT("5x5"));
	const wxString aux4(wxT("50x50"));
	const wxString aux5(wxT("..."));
	Sizes.Add(aux1);
	selectSize->Append(aux1);
	Sizes.Add(aux2);
	selectSize->Append(aux2);
	Sizes.Add(aux3);
	selectSize->Append(aux3);
	Sizes.Add(aux4);
	selectSize->Append(aux4);
	Sizes.Add(aux5);
	selectSize->Append(aux5);
}

void globalView::CreateFace()
{
	face=new Face();
}

void globalView::OnChangeZoom(wxCommandEvent& event)
{
	if(initializing==false)
	{
		int id=event.GetId();
		if (id==ID_ZOOMDESIGN)
		{
			initializing=true;
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
			if((value>=1)&&(value<=10))
			{
				Screen2D->SetZoom(value);
				Screen2D->ChangeZoom(value);	
				slidZoom->SetValue(value);
			}
			else wxMessageBox(wxT("Please introduce a value between 1 and 10"));
		}
	}
	initializing=false;
	if(slidZoom->GetValue()==1)
	{
		int selection=selectSize->GetCurrentSelection();
		if(0<=selection<=3)
		{
			Screen2D->InitializeCanvas(selection);
			Horizontal->SetValue(50);
			Vertical->SetValue(50);
		}
		else
		{
			Screen2D->InitializeCanvas(newSize,true);
			Horizontal->SetValue(50);
			Vertical->SetValue(50);
		}
	}
	ManageSliders();
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

	if(initializing==false)
	{
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

void globalView::LoadFace(Face loaded)
{
	for(int i=0;i<loaded.getNumVertex();i++)
	{
		points->SetPoints(loaded.getAbsoluteVertex(i).x,loaded.getAbsoluteVertex(i).y);
	}
}

void globalView::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
}

void globalView::DeleteFace()
{
	for(int i=face->getNumVertex()-1;i>=0;i--)
	{
		face->deleteVertex(i);
	}
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
	this->MakeModal(false);
}

void globalView::SetCanvasSize(wxCommandEvent &event)
{
	int id=event.GetId();
	if (id==ID_SETSIZE)
	{
		initializing=true;
		Vertical->SetValue(50);
		Horizontal->SetValue(50);
		int selection=selectSize->GetCurrentSelection();
		slidZoom->SetValue(1);
		ZoomValue->Clear();
		ZoomValue->AppendText(wxT("1"));

		if (selection==0) 
			Screen2D->InitializeCanvas(0);

		if (selection==1)
			Screen2D->InitializeCanvas(1);

		if (selection==2)
			Screen2D->InitializeCanvas(2);

		if (selection==3)
			Screen2D->InitializeCanvas(3);

		if (selection==4)
		{
			wxString valueTyped;
			wxString sizestring;
			wxTextEntryDialog myDialog(this,wxT("Personal size"),wxT("Insert new size"));
			if ( myDialog.ShowModal() == wxID_OK )
			{
				if(Sizes.size()==6)
				{
					selectSize->Delete(5);
					Sizes.erase(Sizes.begin()+5);
				}
				valueTyped = myDialog.GetValue();
				newSize = wxAtoi(valueTyped);
				sizestring<<valueTyped;
				sizestring<<"x";
				sizestring<<valueTyped;
				Sizes.push_back(sizestring);
				selectSize->AppendString(sizestring);
				selectSize->SetSelection(5);
				if(newSize>0) Screen2D->InitializeCanvas(newSize/2,true);
			}
			
		}
	}
	ManageSliders();
	initializing=false;
}

void globalView::ManageSliders()
{
	wxColour colour(212,212,222);
	wxColour colour2(193,221,224);

	if(Screen2D->getCompleteViewV())
	{
		Vertical->Disable();
		Vertical->SetBackgroundColour(colour);	
	}
		
	if(!Screen2D->getCompleteViewV())
	{
		Vertical->Enable(true);
		Vertical->SetBackgroundColour(colour2);
	}

	if(!Screen2D->getCompleteViewH())
	{
		Horizontal->Enable(true);
		Horizontal->SetBackgroundColour(colour2);
	}

	if(Screen2D->getCompleteViewH()) 
	{
		Horizontal->Disable();
		Horizontal->SetBackgroundColour(colour);
	}
}