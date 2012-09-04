#include "faceWidget.h"

BEGIN_EVENT_TABLE(FaceWidget, wxPanel)

EVT_COMMAND(wxID_ANY,wxEVT_FACEVERTEX_ADDED,FaceWidget::GetPoint)
EVT_SPLITTER_SASH_POS_CHANGING(wxID_ANY, OnChangeSplitter)
//EVT_COMMAND(wxID_ANY,wxEVT_POINT_ADDED,FaceWidget::AddVertex)

END_EVENT_TABLE()



FaceWidget::FaceWidget(wxWindow *parent,SimulatedWorld *simu,const wxPoint& pos,const wxSize& size,bool horizontal)
: wxPanel(parent, wxID_ANY, pos, size)
{
	window=parent;
	h=horizontal;
	world=simu;
	align=true;
	tableAssociated=false;
	CreatePanel();
	CreateFace();
	
	

}
void FaceWidget::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxHORIZONTAL);
		canvas= new wxSplitterWindow(this, ID_DRAG, wxDefaultPosition, wxDefaultSize,wxSP_LIVE_UPDATE || wxSP_3D);
		canvas->SetMinimumPaneSize(50);
		canvas1=new Canvas(canvas, wxID_ANY, wxDefaultPosition, wxDefaultSize,false);
		canvas2=new Canvas(canvas, wxID_ANY, wxDefaultPosition, wxDefaultSize);
		canvas2->UpdateWorld(world->getWorld());
		if(h)	canvas->SplitVertically(canvas1,canvas2,0);
		else	canvas->SplitHorizontally(canvas1,canvas2,0);
		fbox->Add(canvas,1,wxEXPAND );
		SetSizer(fbox);	
		
}



void FaceWidget::CreateFace()
{
	face=new Face();
	faceCopy=new Face((*face));
	canvas1->ClearObjects();
	canvas1->AddObject(faceCopy);
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
void FaceWidget::GetPoint(wxCommandEvent& event)
{
	Vector2D scale;
	Vector2D scaleTrans;
	point=canvas1->getCursorPosition();
	scale=canvas1->getViewScale2D();
	wxSize c_size=canvas1->GetSize();

	point.x-=c_size.GetWidth()/2;
	point.y-=c_size.GetHeight()/2;
	scaleTrans.x=c_size.GetWidth()/scale.x;
	x=point.x/scaleTrans.x;
	scaleTrans.y-=c_size.GetHeight()/scale.y;
	y=point.y/scaleTrans.y;

	if(align)
	{
		if(x>=0)x=(int)(x+0.5);
		else x=(int)(x-0.5);
	
		if(y>=0)y=(int)(y+0.5);
		else y=(int)(y-0.5);
	}
	if(tableAssociated)
		points->SetPoints(x,y);
	else
		AddVertex();
	
}


void FaceWidget::AddVertex()
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
	
	world->getChild()->UpdateWorld();
	RefreshCanvas();

}


void FaceWidget::OnChangeSplitter(wxSplitterEvent &event)
{
	
	//canvas1->SetViewPoint(-10,-10,10,10);

	
}

void FaceWidget::RefreshCanvas()
{
	
	canvas1->Refresh();
	canvas2->Refresh();

}

