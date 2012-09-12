#include "connectionLog.h"


ConnectionLog::ConnectionLog(wxWindow *window,wxString name)
		:wxDialog(window,wxID_ANY,name ,wxPoint(0,0), wxSize(1060,250),wxCAPTION|wxRESIZE_BORDER|wxMINIMIZE_BOX|wxSYSTEM_MENU| wxDIALOG_NO_PARENT) 
									   
{
	parent=window;
	tittle=name;
	col=0;
	row=0;
	CreatePanel();

}


void ConnectionLog::CreatePanel()
{
	grid=new wxGrid(this,-1, wxDefaultPosition,wxDefaultSize);
	grid->CreateGrid(0,6);
	grid->EnableEditing(false);
	grid->SetColLabelValue(col,wxT("  Server/Client  "));
	grid->SetColumnWidth(col,150);
	grid->SetColLabelValue(++col,wxT("      State      "));
	grid->SetColumnWidth(col,150);
	grid->SetColLabelValue(++col,wxT("Number of clients"));
	grid->SetColumnWidth(col,150);
	grid->SetColLabelValue(++col,wxT("       Host      "));
	grid->SetColumnWidth(col,150);
	grid->SetColLabelValue(++col,wxT("      Adress      "));
	grid->SetColumnWidth(col,150);
	grid->SetColLabelValue(++col,wxT("       Port       "));
	grid->SetColumnWidth(col,150);
	grid->SetRowLabelSize(150);
	grid->GetParent()->SendSizeEvent();
	this->SetMaxSize(wxSize(1060,2000));
	this->SetMinSize(wxSize(530,250));
	col=0;
	
	

	
	
	
	
	
}

void ConnectionLog::AddConnection(NodeTree *robot)
{	
	wxString s_c=wxEmptyString;
	
	log.push_back(robot);
	grid->AppendRows(1);
	for(int i=0;i<6;i++) grid->SetCellAlignment(wxALIGN_CENTER,row,i);
	
	grid->SetRowLabelValue(row,robot->getNameTree());
	
	if(robot->typeConnection==1)	
		s_c=wxT("Server");
			
	else if(robot->typeConnection==2)	
		s_c=wxT("Client");
	
	
	grid->SetCellValue(row,col,s_c);
	StateConnection(robot,false);
	row++;

}
void ConnectionLog::DeleteConnection(NodeTree *robot)
{
	wxString label=wxEmptyString;
	for(int i=0;i<log.size();i++)
	{
		if(log[i]==robot)
		{
			grid->DeleteRows(i);
			row--;
			log.erase(log.begin()+i);
		}
	}

	for(int i=0;i<log.size();i++) 	grid->SetRowLabelValue(i,log[i]->getNameTree());

}

bool ConnectionLog::IsObjectConnected()
{
	if(log.size()>0)
		return true;
	else
		return false;
}

void ConnectionLog::StateConnection(NodeTree *robot,bool connected)
{

	wxString client,address,host,port,state;
	wxColour colour=*wxCYAN;

	for(int i=0;i<log.size();i++)
	{	
		if(log[i]==robot)
		{
			if(robot->typeConnection==1)
			{

				if(connected)
				{
					state=wxT("Sending Data");
					colour=*wxRED;
					
				}
				else	state=wxT("Waiting Clients");
				
				port<<robot->server.Port;
				address=robot->server.Address;
				client<<robot->server.Clients;
			}

			

			else if(robot->typeConnection==2)
			{
				if(connected)
				{
					state=wxT("Receiving Data");
					colour=*wxGREEN;
					
					host=robot->client.Host;
					
	
				}
				else
					state=wxT("Waiting Server");
			
				address=robot->client.Address;
				port<<robot->client.Port;
				client=wxEmptyString;
		
			}
			
			grid->SetCellValue(i,++col,state);
			grid->SetCellBackgroundColour(i,col,colour);
			grid->SetCellValue(i,++col,client);
			grid->SetCellValue(i,++col,host);
			grid->SetCellValue(i,++col,address);
			grid->SetCellValue(i,++col,port);
			col=0;
		}
	}
	
}