#include "prismWindow.h"

BEGIN_EVENT_TABLE(PrismWindow, wxPanel)
	EVT_BUTTON(ID_CHANGEVIEW, PrismWindow::OnButton)
	EVT_COMMAND(wxID_ANY,wxEVT_POINT_ADDED,PrismWindow::DrawBase)
	
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
		
		base=new FaceWidget(this,node->getSimu(),wxDefaultPosition,wxDefaultSize,false,true);
		base->ChangeView(worldView);
		base->GetCanvas3D()->AddObject(node->pointer.prismaticpart);
		PointsList *points=new PointsList(this);
		cView=new wxButton(this,ID_CHANGEVIEW,wxT("World\nView3D"),wxDefaultPosition,wxSize(60,25));
		base->AssociatePointTable(points);
	
		
		pri->Add(base,1,wxEXPAND);
		pri->Add(points,0,wxEXPAND |wxALL, 5);
		pri->Add(cView,0,wxEXPAND |wxALL, 5);
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

PrismWindow::~PrismWindow(void)
{


}
