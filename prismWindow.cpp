#include "prismWindow.h"
#include "wx/bmpbuttn.h"


BEGIN_EVENT_TABLE(PrismWindow, wxPanel)
	EVT_BUTTON(ID_SHOWTHREED, PrismWindow::OnButton)
	EVT_BUTTON(ID_ADDOWNFACE, PrismWindow::OnButton)
	EVT_BUTTON(ID_DEFAULT, PrismWindow::OnButton)
	EVT_BUTTON(ID_CANCELDESIGN, PrismWindow::OnButton)
	EVT_BUTTON(ID_COLOR, PrismWindow::ColorChanged)	
	EVT_BUTTON(ID_COPYDESIGN, PrismWindow::CopyDesign)
	EVT_BUTTON(ID_PASTEDESIGN, PrismWindow::PasteDesign)
	EVT_MENU(ID_COPYDESIGN, PrismWindow::CopyDesign)
	EVT_MENU(ID_PASTEDESIGN, PrismWindow::PasteDesign)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, PrismWindow::RefreshCanvas)
	END_EVENT_TABLE()

PrismWindow::PrismWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size)
	: wxPanel(parent, wxID_ANY, pos, size)
{
	node=obj;
	defName=node->getSimu()->tree->GetItemText(node->getSimu()->tree->GetLastChild(node->getSimu()->tree->GetSelection()));
	worldView=false;
	CreatePanel();
}


void PrismWindow::CreatePanel()
{
		wxStaticBoxSizer *canvas=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Visualization"));
		wxBoxSizer* box=new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* controls=new wxBoxSizer(wxVERTICAL);
		
		base=new FaceWidget(this,node->getSimu(),wxDefaultPosition,wxDefaultSize,true,true);
		pw=new PositionableWidget(this,node,wxT("Positionable Parameters"),wxDefaultPosition,wxDefaultSize,true,true,ID_ADDIRRPRI);
		dp=new DesignWidget(this,node,wxEmptyString,wxDefaultPosition , wxDefaultSize,true);
		wxButton *df = new wxButton(this,ID_DEFAULT,wxT("Create object with default parameters"),wxDefaultPosition,wxDefaultSize);

		controls->Add(df,0,wxEXPAND);
		controls->Add(pw,0,wxEXPAND);
		controls->Add(dp,0,wxEXPAND);
		box->Add(controls,0,wxEXPAND);


		node->pointer.prismaticpart->setPolygonalBase(*(base->GetView()->GetFace()));
		base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());

		cView=new wxButton(this,ID_SHOWTHREED,wxT("Create/Modify Prism"),wxDefaultPosition,wxSize(90,55));
		canvas->Add(base,5,wxEXPAND);
		canvas->Add(cView,0,wxALIGN_CENTRE|wxEXPAND);
		box->Add(canvas,0,wxEXPAND);
		SetSizer(box);
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

	if(id == ID_DEFAULT)
	{
		Transformation3D t;
		t.position=pw->getDefPosition();
		Vector3D defOrientation=pw->getDefOrientation();
		t.orientation.setRPY(defOrientation.x,defOrientation.y,defOrientation.z);
		node->pointer.positionableentity->setRelativeT3D(t);
		dp->SetSpecificValues(true);
		node->getSimu()->tree->SetItemText(node->getSimu()->tree->GetLastChild(node->getSimu()->tree->GetSelection()),defName);
		node->pointer.solidentity->setColor(pw->getDefRedColor(),pw->getDefGreenColor(),pw->getDefBlueColor());
		base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		base->RefreshCanvas();
	}
}


void PrismWindow::RefreshCanvas(wxCommandEvent &event)
{	
	base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
	base->RefreshCanvas();
}

PrismWindow::~PrismWindow(void)
{

}

void  PrismWindow::ColorChanged(wxCommandEvent& event)
{
		base->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		base->RefreshCanvas();
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
