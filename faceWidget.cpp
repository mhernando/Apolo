#include "faceWidget.h"

DEFINE_EVENT_TYPE(wxEVT_POINT_ADDED)

BEGIN_EVENT_TABLE(FaceWidget, wxPanel)
EVT_COMMAND(wxID_ANY,wxEVT_ALIGN_DONE,FaceWidget::GetPoint)
END_EVENT_TABLE()


FaceWidget::FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal,bool pre)
: wxPanel(parent, wxID_ANY, pos, size)
{
	face=NULL;
	faceCopy=NULL;
	noPreliminar3D=pre;
	h=horizontal;
	world=simu;
	align=true;
	tableAssociated=false;
	worldView=false;
	CreatePanel();
	CreateFace();
}



void FaceWidget::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxVERTICAL);
		canvas=new wxSplitterWindow(this, ID_DRAG, wxDefaultPosition, wxDefaultSize,wxSP_LIVE_UPDATE || wxSP_3D);
		canvas->SetMinimumPaneSize (80);
		design1=new FaceDesign(canvas, wxID_ANY, wxDefaultPosition, wxDefaultSize);
		canvas2=new Canvas(canvas, wxID_ANY, wxDefaultPosition, wxDefaultSize);
		if(h)	canvas->SplitVertically(design1,canvas2,0);
		else	canvas->SplitHorizontally(design1,canvas2,0);
		fbox->Add(canvas,5,wxEXPAND);
		SetSizer(fbox);	
		
}


void FaceWidget::CreateFace()
{
	delete face;
	delete faceCopy;
	face=new Face();
	faceCopy=new Face((*face));
	design1->ClearObjects();
	design1->AddObject(faceCopy);
	ChangeView(worldView);
	RefreshCanvas();
	if(tableAssociated)	points->RefreshGrid();

}

void FaceWidget::AssociatePointTable(PointsList *point)
{
	points=point;
	tableAssociated=true;
	points->AssociateFace(this);


	//window->Connect(wxEVT_POINT_ADDED,wxCommandEventHandler(AddVertex);
}




void FaceWidget::ChangeView(bool wView)
{
	worldView=wView;
	wxColourDatabase selCol;
	canvas2->ClearObjects();
	
	if(worldView)
	{
		canvas2->UpdateWorld(world->getWorld());
		canvas2->ChangeBackGroundColour(selCol.Find(wxT("DARK GREY")));
		canvas2->DrawGrid(false);
	}
	else
	{
		canvas2->ChangeBackGroundColour(*wxBLACK);
		canvas2->DrawGrid();
	}
	if(noPreliminar3D==false) canvas2->AddObject(face);
	canvas2->Refresh();

}


void FaceWidget::GetPoint(wxCommandEvent& event)
{
	/*if(align)
	{
		if(x>=0)x=(int)(x+0.5);
		else x=(int)(x-0.5);
	
		if(y>=0)y=(int)(y+0.5);
		else y=(int)(y-0.5);
	}
	*/
	if(tableAssociated)
		points->SetPoints(design1->getXCalibrated(),design1->getYCalibrated());

	else
		SetVertex();
	
}




void FaceWidget::SetVertex(bool addvertex,bool changevertex,bool deletevertex,int deleteRow)
{
	if (addvertex)
	{
		if(tableAssociated)
		{
			Vector2D vertex=points->getLastPointAdded();
			face->addVertex(vertex.x,vertex.y);
			faceCopy->addVertex(vertex.x,vertex.y);
		}
		else 
		{
			face->addVertex(x,y);
			faceCopy->addVertex(x,y);
		}
	}

	if (changevertex)
		{
			Vector2D vertex=points->getChangePointAdded();
			int dir=points->getAuxRow();
			face->changeVertex(dir,vertex.x,vertex.y);
			faceCopy->changeVertex(dir,vertex.x,vertex.y);
		}
	
	if (deletevertex)
		{
			int ind=deleteRow;
			face->deleteVertex (ind);
			faceCopy->deleteVertex (ind);
		}


	world->getChild()->UpdateWorld();
	RefreshCanvas(); 
	
	wxCommandEvent pointAddedEvent( wxEVT_POINT_ADDED,GetId() );
	pointAddedEvent.SetEventObject(GetParent());
	GetEventHandler()->ProcessEvent(pointAddedEvent);




}


void FaceWidget::RefreshCanvas()
{
	design1->Refresh();
	canvas2->Refresh();
}

