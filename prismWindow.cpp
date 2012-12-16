#include "prismWindow.h"
#include "wx/bmpbuttn.h"

BEGIN_EVENT_TABLE(PrismWindow, wxPanel)
	EVT_BUTTON(ID_CHANGEVIEW, PrismWindow::OnButton)
	EVT_COMMAND(wxID_ANY,wxEVT_POINT_ADDED,PrismWindow::DrawBase)
	EVT_SLIDER(ID_ZOOMDESIGN,PrismWindow::ChangeZoom)
	EVT_BUTTON(ID_MOVERIGHT, PrismWindow::Move)
	EVT_BUTTON(ID_MOVEUP, PrismWindow::Move)
	EVT_BUTTON(ID_MOVEDOWN, PrismWindow::Move)
	EVT_BUTTON(ID_MOVELEFT, PrismWindow::Move)
	END_EVENT_TABLE()

PrismWindow::PrismWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size)
	: wxPanel(parent, wxID_ANY, pos, size)
{
	node=obj;
	worldView=false;
	CreatePanel();


}

void PrismWindow::CreatePanel()
{

		wxStaticBoxSizer *pri=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Base Design"));
		wxStaticBoxSizer *Size=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Canvas2D"));//Creacion de un nuevo wxStaticSizer
		wxStaticBoxSizer *Buttons=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Canvas3D"));//Creacion de un nuevo wxStaticSizer
		wxStaticBoxSizer *Controls=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Controls"));//Creacion de un nuevo wxStaticSizer
		wxStaticBoxSizer *Navigate=new wxStaticBoxSizer(wxVERTICAL,this);//Creacion de un nuevo wxStaticSizer
		wxStaticBoxSizer *NavigateH=new wxStaticBoxSizer(wxHORIZONTAL,this);//Creacion de un nuevo wxStaticSizer
		Zoom=new wxSlider(this,ID_ZOOMDESIGN,100,10,200,wxDefaultPosition,wxDefaultSize,wxSL_VERTICAL|wxSL_LABELS|wxSL_AUTOTICKS,wxDefaultValidator,wxT("ZOOM"));
		Zoom->SetTickFreq(1);
		base=new FaceWidget(this,node->getSimu(),wxDefaultPosition,wxDefaultSize,false,true);
		base->ChangeView(worldView);
		base->GetCanvas3D()->AddObject(node->pointer.prismaticpart);
		PointsList *points=new PointsList(this);
		cView=new wxButton(this,ID_CHANGEVIEW,wxT("World\nView3D"),wxDefaultPosition,wxSize(60,30));
		Up=new wxButton(this,ID_MOVEUP,wxT("Up"),wxDefaultPosition,wxSize(25,25));
		Right=new wxButton(this,ID_MOVERIGHT,wxT("Right"),wxDefaultPosition,wxSize(33,30));
		Left=new wxButton(this,ID_MOVELEFT,wxT("Left"),wxDefaultPosition,wxSize(33,30));
		Down=new wxButton(this,ID_MOVEDOWN,wxT("Down"),wxDefaultPosition,wxSize(25,25));
		base->AssociatePointTable(points);
		pri->Add(base,10,wxEXPAND);
		Buttons->Add(cView,0,wxEXPAND);
		Size->Add(Zoom,0,wxEXPAND);
		Navigate->Add(Up,0,wxEXPAND);
		NavigateH->Add(Left,0,wxEXPAND);
		NavigateH->Add(Right,0,wxEXPAND);
		Navigate->Add(NavigateH,0,wxEXPAND);
		Navigate->Add(Down,0,wxEXPAND);
		Size->Add(Navigate,0,wxEXPAND);
		Controls->Add(Size,0,wxEXPAND);
		Controls->Add(Buttons,0,wxEXPAND);
		pri->Add(Controls,0,wxEXPAND);
		pri->Add(points,0,wxEXPAND |wxALL, 5);
		SetSizer(pri);

}

void PrismWindow::OnButton(wxCommandEvent& event)
{
	
	int id=event.GetId();
	

	if(id==ID_CHANGEVIEW)
	{
		worldView=!worldView;
		base->ChangeView(worldView);
		if(worldView) cView->SetLabel(wxT("Prism\nView3D"));
		else	
		{
			cView->SetLabel(wxT("World\nView3D"));
			base->GetCanvas3D()->AddObject(node->pointer.prismaticpart);
		}
			
	}
}

void PrismWindow::DrawBase(wxCommandEvent& event)
{
		node->pointer.prismaticpart->setPolygonalBase(*(base->GetFace()));	
}

void PrismWindow::RefreshCanvas()
{	
	base->RefreshCanvas();
}


void PrismWindow::ChangeZoom(wxCommandEvent& event)
{
	int id=event.GetId();

	if (id==ID_ZOOMDESIGN)
	{
		int scalado=Zoom->GetValue();
		base->design1->ZoomChange(scalado);
		base->RefreshCanvas();
	}
}



void PrismWindow::Move(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_MOVERIGHT)
	{
		base->design1->ChangePosition(false,false,true,false);
		base->RefreshCanvas();
	}
	if (id==ID_MOVEUP)
	{
		base->design1->ChangePosition(true,false,false,false);
		base->RefreshCanvas();
	}
	if (id==ID_MOVEDOWN)
	{
		base->design1->ChangePosition(false,true,false,false);
		base->RefreshCanvas();
	}
	if (id==ID_MOVELEFT)
	{
		base->design1->ChangePosition(false,false,false,true);
		base->RefreshCanvas();
	}

}




PrismWindow::~PrismWindow(void)
{


}


