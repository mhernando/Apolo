#include "faceControlButtons.h"

BEGIN_EVENT_TABLE(FaceControlButtons, wxPanel)
	EVT_SLIDER(ID_ZOOMDESIGN,FaceControlButtons::ChangeZoom)
	EVT_BUTTON(ID_MOVERIGHT, FaceControlButtons::Move)
	EVT_BUTTON(ID_MOVEUP, FaceControlButtons::Move)
	EVT_BUTTON(ID_MOVEDOWN, FaceControlButtons::Move)
	EVT_BUTTON(ID_MOVELEFT, FaceControlButtons::Move)
END_EVENT_TABLE()

FaceControlButtons::FaceControlButtons(wxWindow *parent,wxWindowID id,const wxPoint& pos,const wxSize& size,FaceWidget* face):
														wxPanel(parent, wxID_ANY, pos, size)
{
	winId=id;
	base=face;
	CreatePanel();



}


void FaceControlButtons::CreatePanel()
{

	wxBoxSizer *sbox=new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer *Navigate=new wxStaticBoxSizer(wxVERTICAL,this);
	wxStaticBoxSizer *NavigateH=new wxStaticBoxSizer(wxHORIZONTAL,this);
	wxStaticBoxSizer *Size=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Canvas2D"));//Creacion de un nuevo wxStaticSizer

	Zoom=new wxSlider(this,ID_ZOOMDESIGN,100,10,200,wxDefaultPosition,wxDefaultSize,wxSL_VERTICAL|wxSL_LABELS|wxSL_AUTOTICKS,wxDefaultValidator,wxT("ZOOM"));
	Zoom->SetTickFreq(1);
	
	
	Up=new wxButton(this,ID_MOVEUP,wxT("Up"),wxDefaultPosition,wxSize(25,25));
	Right=new wxButton(this,ID_MOVERIGHT,wxT("Right"),wxDefaultPosition,wxSize(33,30));
	Left=new wxButton(this,ID_MOVELEFT,wxT("Left"),wxDefaultPosition,wxSize(33,30));
	Down=new wxButton(this,ID_MOVEDOWN,wxT("Down"),wxDefaultPosition,wxSize(25,25));

	Size->Add(Zoom,0,wxEXPAND);
	Navigate->Add(Up,0,wxEXPAND);
	NavigateH->Add(Left,0,wxEXPAND);
	NavigateH->Add(Right,0,wxEXPAND);
	Navigate->Add(NavigateH,0,wxEXPAND);
	Navigate->Add(Down,0,wxEXPAND);

	Size->Add(Navigate,0,wxEXPAND,20);

	sbox->Add(Size,1,wxEXPAND |wxALL);

	SetSizer(sbox);

}

FaceControlButtons::~FaceControlButtons(void)
{




}


void FaceControlButtons::ChangeZoom(wxCommandEvent& event)
{
	int id=event.GetId();

	if (id==ID_ZOOMDESIGN)
	{
		int scalado=Zoom->GetValue();
		base->design1->ZoomChange(scalado);
		RefreshCanvas();
	}
}



void FaceControlButtons::Move(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_MOVERIGHT)
	{
		base->design1->ChangePosition(false,false,true,false);
		RefreshCanvas();
	}
	if (id==ID_MOVEUP)
	{
		base->design1->ChangePosition(true,false,false,false);
		RefreshCanvas();
	}
	if (id==ID_MOVEDOWN)
	{
		base->design1->ChangePosition(false,true,false,false);
		RefreshCanvas();
	}
	if (id==ID_MOVELEFT)
	{
		base->design1->ChangePosition(false,false,false,true);
		RefreshCanvas();
	}

}


void FaceControlButtons::RefreshCanvas()
{	
	base->RefreshCanvas();
}
