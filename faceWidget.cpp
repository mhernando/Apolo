#include "faceWidget.h"


DEFINE_EVENT_TYPE(wxEVT_POINT_ADDED)


BEGIN_EVENT_TABLE(FaceWidget, wxPanel)


END_EVENT_TABLE()


FaceWidget::FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal,bool pre)
: wxPanel(parent, wxID_ANY, pos, size)
{
	noPreliminar3D=pre;
	world=simu;
	worldView=false;
	CreatePanel();
}


void FaceWidget::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxHORIZONTAL);
		canvas3d=new Canvas(this,wxID_ANY,wxDefaultPosition,wxSize(790,500));
		canvas3d->DrawGrid();
		fbox->Add(canvas3d,5,wxEXPAND);
		SetSizer(fbox);	
		Vis2d=new globalView(this,wxID_ANY,wxT("Design2D"));
}



void FaceWidget::DrawFace(GLObject* obj)
{
	canvas3d->AddObject(obj);
}

void FaceWidget::RefreshCanvas()
{
	canvas3d->Refresh();
	canvas3d->Update();
}


void FaceWidget::CreateVis2D()
{
	Vis2d->Destroy();
	Vis2d=new globalView(this,wxID_ANY,wxT("Design2D"));
}




