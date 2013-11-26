#include "changeVertex.h"
#include "definitions.h"



BEGIN_EVENT_TABLE(ChangeVertex, wxDialog)
	
	EVT_BUTTON(ID_ACCEPT, ChangeVertex::OnButton)
	EVT_BUTTON(ID_CANCEL, ChangeVertex::OnButton)
	//EVT_TEXT(wxID_ANY, ChangeVertex::OnValuesChange)

END_EVENT_TABLE()



ChangeVertex::ChangeVertex(wxWindow *parent, wxWindowID id, const wxString& title,const wxString& old_x,const wxString& old_y)
:wxDialog(parent, id, title, wxPoint(500,100), wxSize(525,300),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
{
	//changevert=new wxDialog(this,ID_CHANGEVERTEX, wxT("Change vertex"), wxPoint(500,100), wxSize(300,300),wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP);
	
	
	if (id==ID_CHANGEVERTEX)
	{

		wxString value;
		value=old_x;
		newVertex.x=value.ToDouble(&oldVertex.x);
		value.Clear();
		value=old_y;
		newVertex.y=value.ToDouble(&oldVertex.y);
		value.Clear();
		
		panel = new wxPanel(this, wxID_ANY);
		

		vert_old_x=new wxStaticText(panel,wxID_ANY, wxT("Old x :"),wxDefaultPosition,wxDefaultSize);
		name_box1 = new wxTextCtrl(panel,wxID_ANY,old_x,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
		name_box1->SetEditable(false);

		vert_old_y=new wxStaticText(panel,wxID_ANY, wxT("Old y :"),wxDefaultPosition,wxDefaultSize);
		name_box2 = new wxTextCtrl(panel,wxID_ANY,old_y,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
		name_box2->SetEditable(false);

		vert_new_x=new wxStaticText(panel,wxID_ANY, wxT("New x :"),wxDefaultPosition,wxDefaultSize);
		name_box3 = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
			
		vert_new_y=new wxStaticText(panel,wxID_ANY, wxT("New y :"),wxDefaultPosition,wxDefaultSize);
		name_box4 = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
		
		accept = new wxButton(panel,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxDefaultSize);
		cancel = new wxButton(panel,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxDefaultSize);

		wxBoxSizer *s_box=new wxBoxSizer(wxHORIZONTAL);
		s_box->Add(accept,0,wxALIGN_LEFT);
		s_box->AddSpacer(5);
		s_box->Add(cancel,0,wxALIGN_RIGHT);

		wxBoxSizer *vertoldbox = new wxBoxSizer(wxHORIZONTAL);
		vertoldbox->Add(vert_old_x,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
		vertoldbox->Add(name_box1,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
		vertoldbox->AddSpacer(5);
		vertoldbox->Add(vert_old_y,0,wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,5);
		vertoldbox->Add(name_box2,0,wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,5);

		wxBoxSizer *vertnewbox = new wxBoxSizer(wxHORIZONTAL);
		vertnewbox->Add(vert_new_x,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
		vertnewbox->Add(name_box3,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
		vertnewbox->AddSpacer(5);
		vertnewbox->Add(vert_new_y,0,wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,5);
		vertnewbox->Add(name_box4,0,wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,5);

		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		vbox->Add(vertoldbox,1,wxEXPAND|wxALL,5);
		vbox->Add(vertnewbox,1,wxEXPAND|wxALL,5);
		vbox->Add(s_box,wxSizerFlags(0).Align(wxALIGN_BOTTOM));
		
		panel->SetSizer(vbox);
		vbox->SetMinSize(300,300);
		vbox->SetSizeHints(this);

	}

}

void ChangeVertex::OnButton(wxCommandEvent& event)
{
	int id = event.GetId();


	if(id == ID_ACCEPT)
	{
		OnValuesChange();
		EndModal(ID_ACCEPT);
	}
	
	if(id == ID_CANCEL)
	{
		newVertex=oldVertex;
		EndModal(ID_CANCEL);
	}
	event.Skip();
}


void ChangeVertex::OnValuesChange()
{

	if (name_box3->GetValue()==wxEmptyString && name_box4->GetValue()!=wxEmptyString)//If New x is empty
		VertexChanged(name_box1->GetValue(),name_box4->GetValue());

	else if (name_box3->GetValue()!=wxEmptyString && name_box4->GetValue()==wxEmptyString)//If New y is empty
			VertexChanged(name_box3->GetValue(),name_box2->GetValue());

	else if (name_box3->GetValue()==wxEmptyString && name_box4->GetValue()==wxEmptyString)//if both are empty
			VertexChanged(name_box1->GetValue(),name_box2->GetValue());
		
	else 
			VertexChanged(name_box3->GetValue(),name_box4->GetValue());

}


void ChangeVertex::VertexChanged(wxString vert_x,wxString vert_y)
{
	wxString value;
	value=vert_x;
	value.ToDouble(&newVertex.x);
	value.Clear();
	value=vert_y;
	value.ToDouble(&newVertex.y);
	value.Clear();
}
