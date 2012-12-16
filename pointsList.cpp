#include "pointsList.h"


//DEFINE_EVENT_TYPE(wxEVT_POINT_ADDED)

BEGIN_EVENT_TABLE(PointsList, wxPanel)
	
		EVT_GRID_CMD_CELL_CHANGE(wxID_ANY,PointsList::OnChange)
		EVT_GRID_CELL_RIGHT_CLICK(PointsList::OnItemMenu)
		EVT_MENU(ID_CHANGEVERTEX, PointsList::OnMenuChangePoint)
		EVT_MENU(ID_DELETEVERTEX, PointsList::OnMenuDeletePoint)

		
		//EVT_GRID_CELL_RIGHT_CLICK(wxID_ANY,PointsList::OnPosition)
		//EVT_GRID_CELL_LEFT_CLICK//////////////////////////////////////////////////////	
		//EVT_CONTEXT_MENU
		//EVT_RIGHT_DOWN
		
END_EVENT_TABLE()


PointsList::PointsList(wxWindow *window,const wxString label ,const wxPoint& pos ,const wxSize& size)
		:wxPanel(window,wxID_ANY ,pos, size) 
									   
{
	facesAssociated=false;

	parent=window;
	col=0;
	row=0;
	auxrow=0;
	name=label;
	CreatePanel();
	

}


void PointsList::CreatePanel()
{

	 wxBoxSizer *pbox=new wxBoxSizer(wxVERTICAL);
	 wxStaticBoxSizer *cbox=new wxStaticBoxSizer(wxVERTICAL,this,name);

	grid=new wxGrid( this, -1, wxDefaultPosition,wxDefaultSize);
	grid->CreateGrid(1,2);
	grid->SetColLabelValue(0,wxT("X"));
	grid->SetColLabelValue(1,wxT("Y"));
	
	if(name==wxEmptyString)
	{
		pbox->Add(grid,0);
		SetSizer(pbox);
	}
	else
	{
		cbox->Add(grid,0);
		SetSizer(cbox);
	}

	
}

void PointsList::SetPoints(double x,double y)
{	
	wxString value=wxEmptyString;
	grid->SetCellValue(row,col,value); //row=fila col=columna
	grid->SetCellValue(row,col,value<<x);
	value.Clear();
	grid->SetCellValue(row,++col,value);
	grid->SetCellValue(row,col,value<<y);
	value.Clear();
	col=0;
	SetVertex(row); //Put vertex in the panel
	

}

void PointsList::OnChange(wxGridEvent& event)
{

	auxrow=event.GetRow();
	SetVertex(auxrow);//if we click in a row of the panel


}


void PointsList::SetVertex(int r)
{
	
		if(grid->GetCellValue(r,0)!=wxEmptyString && grid->GetCellValue(r,1)!=wxEmptyString) 
		{
			if(col==0) // We do this to overwrite te cell values if you select a point from the 2D map,
					   //otherwise, i doesn't work correctly
			{
			wxString value;
			value=grid->GetCellValue(r,0);
			value.ToDouble(&lastPoint.x);
			value.Clear();
			value=grid->GetCellValue(r,1);
			value.ToDouble(&lastPoint.y);
			value.Clear();
			grid->SetCellBackgroundColour(r,0,*wxGREEN);
			grid->SetCellBackgroundColour(r,1,*wxGREEN);
			
			
			grid->SetReadOnly(row,0);
			grid->SetReadOnly(row,1);
			
			grid->AppendRows(1);
			
			row++;

			if(facesAssociated) faces->SetVertex();
			GetParent()->SendSizeEvent();
			}
		


		}
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//***********************************************   NUEVO    ***********************************************
	
	void PointsList::OnMenuChangePoint(wxCommandEvent& event)
{	
	
		int id = event.GetId();

		if(id == ID_CHANGEVERTEX)
		{
		//ChangeVertex* changeVertex;
		changeVertex = new ChangeVertex(this, ID_CHANGEVERTEX, wxT("Change vertex"),grid->GetCellValue(auxrow,0),
			grid->GetCellValue(auxrow,1));
		
		if (changeVertex->ShowModal()==ID_ACCEPT)
				ChangePoint();
		delete changeVertex;
		}
	
	event.Skip();

}

void PointsList::OnMenuDeletePoint(wxCommandEvent& WXUNUSED(event))
{	
		wxString msg;
		wxString s= wxString() << (auxrow+1);
		msg.Printf(wxT("Are you sure delete Vertex ")+s+wxT(" (X=")+grid->GetCellValue(auxrow,0)+wxT(" , Y=")+grid->GetCellValue(auxrow,1)+wxT(")?"));
		if ( wxMessageBox(msg, wxT("Please confirm"), wxICON_QUESTION | wxYES_NO) != wxYES )
			return;
		DeletePoint();
		

}


void PointsList::ChangePoint()

{
	//int r=auxrow;
	

		if(grid->GetCellValue(auxrow,0)!=wxEmptyString && grid->GetCellValue(auxrow,1)!=wxEmptyString) 
		{
				
			if(col==0) 
					
			{									
					wxString value;
					changePoint=changeVertex->GetNewVertex();
					grid->SetCellValue(auxrow,col,value);
					grid->SetCellValue(auxrow,col,value<<changePoint.x);
					value.Clear();
					grid->SetCellValue(auxrow,++col,value);
					grid->SetCellValue(auxrow,col,value<<changePoint.y);
					value.Clear();
					col=0;
					if(facesAssociated) faces->SetVertex(false,true);
													
					GetParent()->SendSizeEvent();
					
			
			}
		}

	


}



void PointsList::DeletePoint ()
{
		int r=auxrow;

		if(grid->GetCellValue(r,0)!=wxEmptyString && grid->GetCellValue(r,1)!=wxEmptyString) 
		{

	
			grid->DeleteRows(r,1);
			row--;
			if(facesAssociated) faces->SetVertex(false,false,true,r);
			GetParent()->SendSizeEvent();
			

		}

}
void PointsList::OnItemMenu(wxGridEvent& event)
{
		auxrow=event.GetRow();
		wxString s= wxString() << (auxrow+1);
		wxMenu menuGrid(wxT("Menu Vertex "+ s));
		menuGrid.Append(ID_CHANGEVERTEX, wxT("Change Vertex"));
		menuGrid.Append(ID_DELETEVERTEX, wxT("Delete Vertex"));
		PopupMenu(&menuGrid, event.GetPosition());

}




//****************************************************   FIN NUEVO   *********************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
void PointsList::AssociateFace(FaceWidget *face)
{
	faces=face;
	facesAssociated=true;

}
void PointsList::RefreshGrid()
{
	grid->DeleteRows(0,grid->GetNumberRows());
	grid->AppendRows(1);
	grid->GetParent()->SendSizeEvent();
	row=0;
	col=0;

}
