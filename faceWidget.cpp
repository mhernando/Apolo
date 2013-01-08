#include "faceWidget.h"

DEFINE_EVENT_TYPE(wxEVT_POINT_ADDED)


BEGIN_EVENT_TABLE(FaceWidget, wxPanel)
EVT_COMMAND(wxID_ANY,wxEVT_ALIGN_DONE,FaceWidget::GetPoint)
EVT_COMMAND(wxID_ANY,wxEVT_CHECK_POINT_SELECTED,FaceWidget::CheckPointToMove)
EVT_COMMAND(wxID_ANY,wxEVT_SET_NEW_POINT,FaceWidget::ChangePoint)


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
		design1=new FaceDesign(canvas, wxID_ANY, wxDefaultPosition, wxSize(200,200));
		canvas2=new Canvas(canvas, wxID_ANY, wxDefaultPosition, wxSize(200,200));
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
	//Si la alineación está activada
	if(design1->GetAlign()==true)
	{
		if(tableAssociated)
		{
		
		//Ajuste X a la rejilla del design
			x=design1->getXCalibrated();
			int x1=(int)x;
			
			if (x1>0)
			{
				if ((x1%2)!=0) x=x+1;
			}

			if (x1<0)
			{
				if ((x1%2)!=0) x=x-1;
			}
			x=(int)x;

			
			//Ajuste Y a la rejilla del design 
			y=design1->getYCalibrated();
			int y1=(int)y;
			
			if (y1>0)
			{
				if ((y1%2)!=0) y=y+1;
			}

			if (y1<0)
			{
				if ((y1%2)!=0) y=y-1;
			}
			y=(int)y;


			points->SetPoints(x,y);
		}
		else SetVertex();
	}
	

	if(design1->GetAlign()==false)
	{
		if(tableAssociated)
		{
			points->SetPoints(design1->getXCalibrated(),design1->getYCalibrated());
		}
		else
			SetVertex();
	}
}






void FaceWidget::ChangePoint(wxCommandEvent& event)   //Cambiará el punto arrastrado por otro en la nueva posición
{
	if(tableAssociated)
	{
		//Si la alineación está activada
		if(design1->GetAlign()==true)
		{
			//Ajuste X a la rejilla del design
			x=design1->getXCalibrated();
			int x1=(int)x;
			
			if (x1>0)
			{
				if ((x1%2)!=0) x=x+1;
			}

			if (x1<0)
			{
				if ((x1%2)!=0) x=x-1;
			}
			x=(int)x;

			
			//Ajuste Y a la rejilla del design 
			y=design1->getYCalibrated();
			int y1=(int)y;
			
			if (y1>0)
			{
				if ((y1%2)!=0) y=y+1;
			}

			if (y1<0)
			{
				if ((y1%2)!=0) y=y-1;
			}
			y=(int)y;
		
			if (resp>-1) points->MovedPoint(resp,x,y);
		}
		else
		{
			if (resp>-1) points->MovedPoint(resp,design1->getXCalibrated(),design1->getYCalibrated());
		}

		resp=-1; 
		design1->FinishedChange();
	}
}





void FaceWidget::SetVertex(bool addvertex,bool changevertex,bool deletevertex,bool movepoint, int deleteRow)
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
	if (movepoint)
	{

			Vector2D vertex=points->getMovedPointAdded();
			//int dir=points->getAuxRow();
			face->changeVertex(resp,vertex.x,vertex.y);
			faceCopy->changeVertex(resp,vertex.x,vertex.y);

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




void FaceWidget::CheckPointToMove(wxCommandEvent& event)
{
	resp=points->CheckPoint(design1->getXCalibrated(),design1->getYCalibrated());
}




