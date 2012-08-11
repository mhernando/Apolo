#include "pointsList.h"


DEFINE_EVENT_TYPE(wxEVT_POINT_ADDED)

BEGIN_EVENT_TABLE(PointsList, wxPanel)

EVT_GRID_CMD_CELL_CHANGE(wxID_ANY,PointsList::OnChange)

	
END_EVENT_TABLE()

PointsList::PointsList(wxWindow *window,wxWindowID id,const wxString label ,const wxPoint& pos ,const wxSize& size)
		:wxPanel(window,id ,pos, size) 
									   
{
	parent=window;
	col=0;
	row=0;
	name=label;
	CreatePanel();
	

}


void PointsList::CreatePanel()
{

	wxStaticBoxSizer *pbox=new wxStaticBoxSizer(wxVERTICAL,this,name);

	grid=new wxGrid( this, -1, wxDefaultPosition,wxDefaultSize);
	grid->CreateGrid(1,2);
	grid->SetColLabelValue(0,wxT("X"));
	grid->SetColLabelValue(1,wxT("Y"));
	pbox->Add(grid,0);
	SetSizer(pbox);
	
}

void PointsList::SetPoints(double x,double y)
{	
	wxString value;

	grid->SetCellValue(row,col,(value<<x));
	value.Clear();

	grid->SetCellValue(row,++col,value<<y);
	value.Clear();
	SetVertex(row);
	
	col=0;

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
		wxString value;
		value=grid->GetCellValue(r,0);
		value.ToDouble(&lastPoint.x);
		value.Clear();
		value=grid->GetCellValue(r,1);
		value.ToDouble(&lastPoint.y);
		value.Clear();
		grid->SetCellBackgroundColour(r,0,*wxGREEN);
		grid->SetCellBackgroundColour(r,1,*wxGREEN);
		grid->AppendRows(1);
		row++;
		wxCommandEvent PointEvent( wxEVT_POINT_ADDED,GetId() );
		PointEvent.SetEventObject( parent);
		parent->GetEventHandler()->ProcessEvent(PointEvent);
		grid->GetParent()->SendSizeEvent();
		
	}
	
	
}

void PointsList::RefreshGrid()
{
	grid->DeleteRows(0,grid->GetNumberRows());
	grid->AppendRows(1);
	grid->GetParent()->SendSizeEvent();
	row=0;
	col=0;

}
