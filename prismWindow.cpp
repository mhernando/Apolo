#include "prismWindow.h"
#include "wx/bmpbuttn.h"

BEGIN_EVENT_TABLE(PrismWindow, wxPanel)
	EVT_BUTTON(ID_CHANGEVIEW, PrismWindow::OnButton)
	EVT_COMMAND(wxID_ANY,wxEVT_POINT_ADDED,PrismWindow::DrawBase)
	EVT_RADIOBOX(wxID_ANY,PrismWindow::DesignAlign)
	END_EVENT_TABLE()

PrismWindow::PrismWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size)
	: wxPanel(parent, wxID_ANY, pos, size)
{
	node=obj;
	worldView=false;
	CreatePanel();
	radioButton1->SetSelection(1);


}

void PrismWindow::CreatePanel()
{
		wxBoxSizer *vbox=new wxBoxSizer(wxHORIZONTAL);//global box
		wxBoxSizer *sbox=new wxBoxSizer(wxHORIZONTAL);//specific box
		

		wxStaticBoxSizer *pri=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Base Design"));
		
		wxStaticBoxSizer *canvas=new wxStaticBoxSizer(wxHORIZONTAL,this,wxT("Visualization"));

		wxStaticBoxSizer *Buttons=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Canvas3D"));

		
		base=new FaceWidget(this,node->getSimu(),wxDefaultPosition,wxDefaultSize,true,true);
		base->ChangeView(worldView);
		base->GetCanvas3D()->AddObject(node->pointer.prismaticpart);
		
		PointsList *points=new PointsList(this);
		
		cView=new wxButton(this,ID_CHANGEVIEW,wxT("World\nView3D"),wxDefaultPosition,wxSize(60,30));
		
		base->AssociatePointTable(points);
		
		canvas->Add(base,10,wxEXPAND);
		Buttons->Add(cView,0,wxEXPAND);

		
		wxString string[2]={wxT("On"), wxT("Off")}; //Los posibles valores que va a tener el RadioBox
		radioButton1=new wxRadioBox(this,wxID_ANY,wxT("Align"),wxDefaultPosition,wxDefaultSize,2,string); //Botón para alinear los puntos 
		
		controlButtons=new FaceControlButtons(this,ID_ADDIRRPRI,wxDefaultPosition,wxDefaultSize,base);
		
		sbox->Add(controlButtons,1,wxEXPAND |wxALL);
		sbox->Add(Buttons,1,wxEXPAND |wxALL);

		pri->Add(canvas,0,wxEXPAND);
		pri->Add(sbox,0,wxEXPAND |wxALL);
		pri->Add(radioButton1,0,wxEXPAND,20);
		
		vbox->Add(pri,0,wxEXPAND);
		vbox->Add(points,0,wxEXPAND |wxALL, 5);
		SetSizer(vbox);
		vbox->SetSizeHints(this);

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


void PrismWindow::DesignAlign(wxCommandEvent& event)
{
	if (radioButton1->GetSelection()==0)
		base->design1->SetAlign(true);
	else base->design1->SetAlign(false);
}



PrismWindow::~PrismWindow(void)
{


}


