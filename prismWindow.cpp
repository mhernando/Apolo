#include "prismWindow.h"
#include "wx/bmpbuttn.h"


BEGIN_EVENT_TABLE(PrismWindow, wxPanel)
	EVT_BUTTON(ID_SHOWTHREED, PrismWindow::OnButton)
	EVT_BUTTON(ID_ADDOWNFACE, PrismWindow::OnButton)

	EVT_BUTTON(ID_CANCELDESIGN, PrismWindow::OnButton)
	EVT_BUTTON(ID_COLOR, PrismWindow::ColorChanged)	
	EVT_BUTTON(ID_COPYDESIGN, PrismWindow::CopyDesign)
	EVT_BUTTON(ID_PASTEDESIGN, PrismWindow::PasteDesign)
	EVT_MENU(ID_COPYDESIGN, PrismWindow::CopyDesign)
	EVT_MENU(ID_PASTEDESIGN, PrismWindow::PasteDesign)

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

		node->pointer.prismaticpart->setPolygonalBase(*(base->GetView()->GetFace()));
		base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());

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

		base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		base->GetView()->Show(true);
		base->GetView()->MakeModal(true);
	}

	if(id==ID_ADDOWNFACE)
	{
		node->pointer.prismaticpart->setPolygonalBase(*(base->GetView()->GetFace()));
		base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		base->GetCanvas3d()->AddObject(node->pointer.prismaticpart);
		
		base->GetView()->MakeModal(false);
		base->GetView()->Hide();

	}
	if(id==ID_CANCELDESIGN)
	{
		base->GetView()->MakeModal(false);
		base->GetView()->Hide();
	}
}
/*
void PrismWindow::DrawBase(wxCommandEvent& event)
{
	node->pointer.prismaticpart->setPolygonalBase(*(base->GetView()->GetFace()));
	base->GetCanvas3d()->AddObject(node->pointer.prismaticpart);

	if(id==ID_CANCELDESIGN)
	{
		base->GetView()->MakeModal(false);
		base->GetView()->Hide();
	}

}
*/




void PrismWindow::RefreshCanvas()
{	
	base->RefreshCanvas();
}


PrismWindow::~PrismWindow(void)

{


}


void  PrismWindow::ColorChanged(wxCommandEvent& event)
{
		base->RefreshCanvas();
		base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
}


void PrismWindow::CopyDesign(wxCommandEvent& event)
{
	int id=event.GetId();
	if(id==ID_COPYDESIGN)
	{
		node->getSimu()->CleanClipboard();
		if(base->GetView()->GetScreen2D()->GetVector().size()>0)
		{
			vector<Vector2D> CopiedDesign=base->GetView()->GetScreen2D()->GetVector();
			node->getSimu()->SetCopiedDesign(CopiedDesign);
		}
	}
}


void PrismWindow::PasteDesign(wxCommandEvent& event)
{
	int id=event.GetId();
	if(id==ID_PASTEDESIGN)
	{
		base->GetView()->PasteDesign(node->getSimu()->GetCopiedDesign());
	}
}
