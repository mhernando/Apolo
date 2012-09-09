#ifndef __APOLO__CONNECTION_PARAMETERS__H
#define __APOLO__CONNECTION_PARAMETERS__H

#include "definitions.h"
#include <wx/wx.h>




class ConnectionParameters:public wxDialog

{
public:
	ConnectionParameters(wxWindow *parent,bool s_c,const wxString& title,wxPoint point=wxPoint(10,10),wxSize size=wxDefaultSize);
	void CreatePanel(bool sc);
	void OnButton(wxCommandEvent& event);
	bool IsAccepted(){return accept;}
	wxString GetAddress_Clients(){return address_clients;}
	wxString GetPort(){return port;}

private:
	bool accept;
	bool serv_client;
	wxString defPort,defAddress,defClients;
	wxString address_clients;
	wxString port;
	wxTextCtrl *clients_address_box;
	wxTextCtrl *port_box;
	
	
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__CONNECTION_PARAMETERS__H