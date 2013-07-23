#include "prismWindow.h"
#include "wx/bmpbuttn.h"


BEGIN_EVENT_TABLE(PrismWindow, wxPanel)
	EVT_BUTTON(ID_SHOWTHREED, PrismWindow::OnButton)
	EVT_BUTTON(ID_ADDOWNFACE, PrismWindow::OnButton)
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
		wxStaticBoxSizer *canvas=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Visualization"));
		base=new FaceWidget(this,node->getSimu(),wxDefaultPosition,wxDefaultSize,true,true);
		base->GetCanvas3d()->AddObject(node->pointer.prismaticpart);
		cView=new wxButton(this,ID_SHOWTHREED,wxT("Create/Modify Prism"),wxDefaultPosition,wxSize(90,55));
		canvas->Add(base,5,wxEXPAND);
		canvas->Add(cView,0,wxALIGN_CENTRE|wxEXPAND);
		SetSizer(canvas);
}


void PrismWindow::OnButton(wxCommandEvent& event)
{
	
	int id=event.GetId();
	if(id==ID_SHOWTHREED)
	{
		base->GetView()->Show(true);
		base->GetView()->MakeModal(true);
		
	}
	if(id==ID_ADDOWNFACE)
	{
		node->pointer.prismaticpart->setPolygonalBase(*(base->GetView()->GetFace()));
		base->GetCanvas3d()->AddObject(node->pointer.prismaticpart);
		base->GetView()->Show(false);
		base->GetView()->MakeModal(false);
	}
}

/*
void PrismWindow::DrawBase(wxCommandEvent& event)
{
	node->pointer.prismaticpart->setPolygonalBase(*(base->GetView()->GetFace()));
	base->GetCanvas3d()->AddObject(node->pointer.prismaticpart);
}
*/


void PrismWindow::RefreshCanvas()
{	
	base->RefreshCanvas();
}


PrismWindow::~PrismWindow(void)
{


}


