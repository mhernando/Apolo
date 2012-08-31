#include "pointsList.h"


//DEFINE_EVENT_TYPE(wxEVT_POINT_ADDED)

BEGIN_EVENT_TABLE(PointsList, wxPanel)

EVT_GRID_CMD_CELL_CHANGE(wxID_ANY,PointsList::OnChange)

	
END_EVENT_TABLE()

PointsList::PointsList(wxWindow *window,const wxString label ,const wxPoint& pos ,const wxSize& size)
		:wxPanel(window,wxID_ANY ,pos, size) 
									   
{
	facesAssociated=false;
	parent=window;
	col=0;
	row=0;
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
	grid->SetCellValue(row,col,value);
	grid->SetCellValue(row,col,value<<x);
	value.Clear();
	grid->SetCellValue(row,++col,value);
	grid->SetCellValue(row,col,value<<y);
	value.Clear();
	col=0;
	SetVertex(row);
	

}
void PointsList::OnChange(wxGridEvent& event)
{
	
	row=event.GetRow();
	SetVertex(row);

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
		if(facesAssociated) faces->AddVertex();
		/*
		wxCommandEvent PointEvent( wxEVT_POINT_ADDED,GetId() );
		PointEvent.SetEventObject(parent);
		parent->GetEventHandler()->ProcessEvent(PointEvent);
		
		*/
		GetParent()->SendSizeEvent();
		}
		
	}
	
	
}
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
