#include "faceWidget.h"


DEFINE_EVENT_TYPE(wxEVT_POINT_ADDED)


BEGIN_EVENT_TABLE(FaceWidget, wxPanel)
EVT_COMMAND(wxID_ANY,wxEVT_NEWVERTEX_ADDED,FaceWidget::GetPoint)
EVT_COMMAND(wxID_ANY,wxEVT_CHANGE_POINT,FaceWidget::ChangePoint)
EVT_COMMAND(wxID_ANY,wxEVT_DELETE_POINT,FaceWidget::DeletePoint)
//EVT_COMMAND(wxID_ANY,wxEVT_MOVE_POLYGON,FaceWidget::ChangePolygonPosition)
END_EVENT_TABLE()


FaceWidget::FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal,bool pre)
: wxPanel(parent, wxID_ANY, pos, size)
{
	face=NULL;
	faceCopy=NULL;
	noPreliminar3D=pre;
	h=horizontal;
	world=simu;
	tableAssociated=false;
	worldView=false;
	CreatePanel();
	CreateFace();
}


void FaceWidget::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxHORIZONTAL);
		canvas3d=new Canvas(this,wxID_ANY,wxDefaultPosition,wxSize(790,500));
		fbox->Add(canvas3d,5,wxEXPAND);
		SetSizer(fbox);	
		Vis2d=new globalView(this,wxID_ANY,wxT("Design2D"));
	
}



void FaceWidget::CreateFace()
{
	delete face;
	delete faceCopy;
	face=new Face();
	faceCopy=new Face((*face));
	if(tableAssociated)	points->RefreshGrid();
	canvas3d->DrawGrid();
	
}


void FaceWidget::AssociatePointTable(PointsList *point)
{	
	points=point;
	tableAssociated=true;
	points->AssociateFace(this);
}



void FaceWidget::GetPoint(wxCommandEvent& event)
{
	points->SetPoints(Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY());
}


void FaceWidget::ChangePoint(wxCommandEvent& event)   //Cambiará el punto arrastrado por otro en la nueva posición
{
	int resp=Vis2d->GetScreen2D()->GetMarkedPoint();
	if (resp<0) return;
	if (resp>=Vis2d->GetScreen2D()->NumPoints()) return;
	points->MovedPoint(resp,Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY());
}



void FaceWidget::DeletePoint(wxCommandEvent& event)
{
	int resp=Vis2d->GetScreen2D()->GetMarkedPoint();
	if (resp<0) return;
	if (resp>Vis2d->GetScreen2D()->NumPoints()) return;
	points->DeletePointMarked(resp);
}




void FaceWidget::SetVertex(bool addvertex,bool changevertex,bool deletevertex,bool movepoint, int deleteRow)
{
	if (addvertex)
	{
		if (Vis2d->GetScreen2D()->GetTypeOfPoint()==false)
		{
			Vector2D vertex=points->getLastPointAdded();
			face->addVertex(vertex.x,vertex.y);
			faceCopy->addVertex(vertex.x,vertex.y);
			Vis2d->GetScreen2D()->AddPoint(vertex.x,vertex.y);   //si añadimos un punto desde el panel no se añadirá de nuevo al vector de puntos
		}
		else
		{
			face->addVertex(Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY());
			faceCopy->addVertex(Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY());

		}
	}

	if (changevertex)
	{
		if (Vis2d->GetScreen2D()->GetTypeOfPoint()==false)
		{
			Vector2D vertex=points->getChangePointAdded();
			int dir=points->getAuxRow();
			face->changeVertex(dir,vertex.x,vertex.y);
			faceCopy->changeVertex(dir,vertex.x,vertex.y);
			Vis2d->GetScreen2D()->ChangePoint(dir,vertex.x,vertex.y);
		}
		else
		{
			face->changeVertex(Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY(),Vis2d->GetScreen2D()->GetMarkedPoint());
			faceCopy->changeVertex(Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY(),Vis2d->GetScreen2D()->GetMarkedPoint());
		}
	}
	
	if (deletevertex)
	{
		if (Vis2d->GetScreen2D()->GetTypeOfPoint()==false)
		{
			int ind=deleteRow;
			face->deleteVertex (ind);
			faceCopy->deleteVertex (ind);
			Vis2d->GetScreen2D()->DeleteVertex(ind);
		}
		else 
		{
			face->deleteVertex (Vis2d->GetScreen2D()->GetMarkedPoint());
			faceCopy->deleteVertex(Vis2d->GetScreen2D()->GetMarkedPoint());
		}
	}

	if (movepoint)
	{
		if (Vis2d->GetScreen2D()->GetTypeOfPoint()==false)
		{
			Vector2D vertex=points->getMovedPointAdded();
			int dir=points->getAuxRow();
			face->changeVertex(resp,vertex.x,vertex.y);
			faceCopy->changeVertex(resp,vertex.x,vertex.y);
			Vis2d->GetScreen2D()->ChangePoint(resp,vertex.x,vertex.y);
		}
		else
		{
			face->changeVertex(Vis2d->GetScreen2D()->GetMarkedPoint(),Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY());
			faceCopy->changeVertex(Vis2d->GetScreen2D()->GetMarkedPoint(),Vis2d->GetScreen2D()->GetPointX(),Vis2d->GetScreen2D()->GetPointY());
		}

	}
	
	/*wxCommandEvent DeletePointEvent( wxEVT_POINT_ADDED,GetId() );
	DeletePointEvent.SetEventObject( window);
	GetEventHandler()->ProcessEvent(DeletePointEvent);
	world->getChild()->UpdateWorld();
	RefreshCanvas(); 
	*/
}



/*
void FaceWidget::ChangePolygonPosition(wxCommandEvent& event)
{
	int resp=Vis2d->GetScreen2D()->GetMarkedPoint();
	if (resp<0) return;
	if (resp>=Vis2d->GetScreen2D()->NumPoints()) return;
	points->MovedPoint(resp,Vis2d->GetScreen2D()->GetVector()[resp].x,Vis2d->GetScreen2D()->GetVector()[resp].y);
}

*/


void FaceWidget::ChangeColourCell(wxCommandEvent& event)
{
	points->MarkRow(Vis2d->GetScreen2D()->GetMarkedPoint());
}


void FaceWidget::DrawFace(GLObject* obj)
{
	canvas3d->AddObject(obj);
}

void FaceWidget::RefreshCanvas()
{
	canvas3d->Refresh();
}