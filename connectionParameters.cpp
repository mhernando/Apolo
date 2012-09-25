#include "connectionParameters.h"

BEGIN_EVENT_TABLE(ConnectionParameters,wxDialog)
	EVT_BUTTON(ID_ACCEPT, ConnectionParameters::OnButton)
	EVT_BUTTON(ID_CANCEL, ConnectionParameters::OnButton)
	EVT_BUTTON(ID_DEFAULT, ConnectionParameters::OnButton)
END_EVENT_TABLE()

ConnectionParameters::ConnectionParameters(wxWindow *parent,bool s_c,const wxString& title,wxPoint point,wxSize size)
:wxDialog(parent,wxID_ANY, title, point, size,wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxMAXIMIZE_BOX ) 
									   
{
	defPort=wxT("13000");
	defAddress=wxT("127.0.0.1");
	defClients=wxT("5");
	clients_address_box=NULL;
	accept=false;
	serv_client=s_c;
	CreatePanel(s_c);	
}


void ConnectionParameters::CreatePanel(bool sc)
{

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *bbox = new wxBoxSizer(wxHORIZONTAL);
	wxButton *b_default = new wxButton(this,ID_DEFAULT,wxT("Default Values"),wxDefaultPosition,wxDefaultSize);
	
	wxStaticText *port_text = new wxStaticText(this, wxID_ANY, wxT("Port :    "),wxDefaultPosition,wxDefaultSize);
	port_box = new wxTextCtrl(this,wxID_ANY,defPort,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
	
	wxStaticText *clients_address_text = new wxStaticText(this, wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize);
	
	if(sc)
	{
	clients_address_text->SetLabel(wxT("Max. Clients :    "));
	clients_address_box = new wxTextCtrl(this,wxID_ANY,defClients,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
	}
	else
	{
	clients_address_text->SetLabel(wxT("Address :      "));
	clients_address_box = new wxTextCtrl(this,wxID_ANY,defAddress,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
	}
	
	wxButton *b_accept = new wxButton(this,ID_ACCEPT,wxT("Accept"),wxDefaultPosition,wxDefaultSize);
	wxButton *b_cancel = new wxButton(this,ID_CANCEL,wxT("Cancel"),wxDefaultPosition,wxDefaultSize);
	
	
	
	vbox->Add(b_default,0,wxEXPAND|wxALL,10);
	vbox->Add(port_text,0,wxLEFT,10);
	vbox->Add(port_box,0,wxEXPAND |wxALL,10);
	vbox->Add(clients_address_text,0,wxLEFT,10);
	vbox->Add(clients_address_box,0,wxEXPAND | wxALIGN_LEFT|wxALL,10);
	bbox->Add(b_accept,1);
	bbox->Add(b_cancel,1);
	vbox->Add(bbox,1,wxEXPAND |wxALL,5);
	
	vbox->SetSizeHints(this);
	SetSizer(vbox);
		
}

void ConnectionParameters::OnButton(wxCommandEvent& event)
{
	int id=event.GetId();
	if(id==ID_ACCEPT)
	{
		address_clients=clients_address_box->GetValue();
		port=port_box->GetValue();
		accept=true;
		Close();
	}
	if(id==ID_CANCEL)	
		Close();	

	if(id==ID_DEFAULT)
	{
		port=defPort;
		if(serv_client)address_clients=defClients;
		else address_clients=defAddress;
		Close(false);
	}
}

