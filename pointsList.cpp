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
	designAssociated=true;
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
	grid->SetDefaultColSize(54,true);
	grid->SetDefaultCellBackgroundColour(*wxWHITE);
	grid->CreateGrid(1,2);
	grid->SetLabelBackgroundColour(*wxCYAN);
	grid->SetColLabelValue(0,wxT("X"));
	grid->SetColLabelValue(1,wxT("Y"));
	grid->SetColLabelSize(wxGRID_AUTOSIZE);
	grid->SetRowLabelSize(wxGRID_AUTOSIZE);

	
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

			//if(facesAssociated) faces->SetVertex();
			if(designAssociated) Screen2D->ManagePoints();
			GetParent()->SendSizeEvent();
			}
		
		}
	
}


	void PointsList::OnMenuChangePoint(wxCommandEvent& event)
{	
	
		int id = event.GetId();

		if(id == ID_CHANGEVERTEX)
		{
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
					//if(facesAssociated) faces->SetVertex(false,true);
					if(designAssociated) Screen2D->ManagePoints(false,true,false);								
					GetParent()->SendSizeEvent();
			}
		}
}


void PointsList::MovedPoint(int rowtochange,double x,double y)

{
		if(grid->GetCellValue(rowtochange,0)!=wxEmptyString && grid->GetCellValue(rowtochange,1)!=wxEmptyString) 
		{
			if(col==0) 	
			{									
					wxString value;
					movedPoint.x=x;
					movedPoint.y=y;
					grid->SetCellValue(rowtochange,col,value);
					grid->SetCellValue(rowtochange,col,value<<x);
					value.Clear();
					grid->SetCellValue(rowtochange,++col,value);
					grid->SetCellValue(rowtochange,col,value<<y);
					value.Clear();
					col=0;
					//if(facesAssociated) faces->SetVertex(false,false,false,true);
					if(designAssociated) Screen2D->ManagePoints(false,true,false);
					GetParent()->SendSizeEvent();
			}
		}
}




void PointsList::MovedPoints()
{
	for(int i=0;i<Screen2D->GetScreen2D()->NumPoints();i++)
	{
		if(grid->GetCellValue(i,0)!=wxEmptyString && grid->GetCellValue(i,1)!=wxEmptyString) 
		{
			if(col==0) 	
			{									
					wxString value;
					movedPoint.x=Screen2D->GetScreen2D()->GetVector()[i].x;
					movedPoint.y=Screen2D->GetScreen2D()->GetVector()[i].y;
					grid->SetCellValue(i,col,value);
					grid->SetCellValue(i,col,value<<Screen2D->GetScreen2D()->GetVector()[i].x);
					value.Clear();
					grid->SetCellValue(i,++col,value);
					grid->SetCellValue(i,col,value<<Screen2D->GetScreen2D()->GetVector()[i].y);
					value.Clear();
					col=0;
					GetParent()->SendSizeEvent();
			}
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
			//if(facesAssociated) faces->SetVertex(false,false,true,false,r);
			if(designAssociated) Screen2D->ManagePoints(false,false,true,r);
			GetParent()->SendSizeEvent();
		}
}


void PointsList::DeletePointMarked (int r)
{
		if(grid->GetCellValue(r,0)!=wxEmptyString && grid->GetCellValue(r,1)!=wxEmptyString) 
		{
			grid->DeleteRows(r,1);
			row--;
			//if(facesAssociated) faces->SetVertex(false,false,true,false,r);
			if(designAssociated) Screen2D->ManagePoints(false,false,true,r);
			GetParent()->SendSizeEvent();
		}
}



void PointsList::InsertedPoint(int first,int second,double x,double y)
{
	if(first==second) return;  //Los dos índices introducidos son iguales

	if (first<second)  //El primer punto seleccionado tiene un índice inferior al segundo
	{
		if((second==grid->GetNumberRows()-1)&&(first==0))   //Si insertamos un punto entre el primero y el último
		{
			designAssociated=false;
			SetPoints(Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].x,
				Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].y);
			designAssociated=true;
			return;
		}

		//Si el punto a insertar no se encuentra entre el primero y el último
		designAssociated=false;
		SetPoints(Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].x,
			Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].y);
		for(int i=Screen2D->GetScreen2D()->NumPoints()-2;i>second;i--)
		{
			MovedPoint(i,Screen2D->GetScreen2D()->GetVector()[i].x,Screen2D->GetScreen2D()->GetVector()[i].y);
		}
		MovedPoint(second,x,y);
		designAssociated=true;
	}


	if (second<first)  //El segundo punto seleccionado tiene un índice inferior al primero
	{
		if((first==grid->GetNumberRows()-1)&&(second==0)) 
		{
			designAssociated=false;
			SetPoints(Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].x,
				Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].y);
			designAssociated=true;
			return;
		}
		//Si el punto a insertar no se encuentra entre el primero y el último
		designAssociated=false;
		SetPoints(Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].x,
		Screen2D->GetScreen2D()->GetVector()[Screen2D->GetScreen2D()->NumPoints()-1].y);
		for(int i=Screen2D->GetScreen2D()->NumPoints()-2;i>first;i--)
		{
			MovedPoint(i,Screen2D->GetScreen2D()->GetVector()[i].x,Screen2D->GetScreen2D()->GetVector()[i].y);
		}
		MovedPoint(first,x,y);
		designAssociated=true;
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
	

void PointsList::AssociateFace(FaceWidget *face)
{
	faces=face;
	facesAssociated=true;
}

void PointsList::AssociateDesign2D(globalView *Design2D)
{
	Screen2D=Design2D;
	designAssociated=true;
}



void PointsList::RefreshGrid()
{
	grid->DeleteRows(0,grid->GetNumberRows());
	grid->AppendRows(1);
	grid->GetParent()->SendSizeEvent();
	row=0;
	col=0;
}


//Función que tiene que comprobar si el punto señalado está en la lista de puntos añadidos.Devuelve el número de fila donde esté el punto 
//con el que coincida
int PointsList::CheckPoint(double x,double y)
{
	int respuesta=-1;		
	double difx=0,dify=0;
	for(int i=0;i<grid->GetNumberRows();i++)
	{
		Vector2D punto;
		wxString valueCell;
		valueCell=grid->GetCellValue(i,0);
		valueCell.ToDouble(&punto.x);
		valueCell.Clear();
		valueCell=grid->GetCellValue(i,1);
		valueCell.ToDouble(&punto.y);
		valueCell.Clear();
		difx=x-punto.x;
		dify=y-punto.y;
		if ((abs(difx)<0.25)&&(abs(dify)<0.25)) respuesta=i;
	}
	return respuesta;
}



void PointsList::MarkRow(int r)
{
	if(r>grid->GetNumberRows())return;
	if (r<0) return;
	for(int i=0;i<=grid->GetNumberRows();i++)
	{
		if (i==r)
		{
			grid->EnableEditing(true); 
			grid->SetCellBackgroundColour(r,0,*wxYELLOW);
			grid->SetCellBackgroundColour(r,1,*wxYELLOW);
			grid->EnableEditing(false); 
		}
		else 
		{
			grid->EnableEditing(true); 
			grid->SetCellBackgroundColour(r,0,*wxGREEN);
			grid->SetCellBackgroundColour(r,1,*wxGREEN);
			grid->EnableEditing(false); 		
		}
	}
}



