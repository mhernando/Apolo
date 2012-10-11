#include "connectionLog.h"
#include "simulatedWorld.h"


ConnectionLog::ConnectionLog(wxWindow *window,wxString name)
		:wxDialog(window,wxID_ANY,name ,wxPoint(0,0), wxSize(1210,250),wxCAPTION|wxRESIZE_BORDER|wxMINIMIZE_BOX|wxSYSTEM_MENU| wxDIALOG_NO_PARENT) 
									   
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
	grid->CreateGrid(0,7);
	grid->EnableEditing(false);
	grid->SetColLabelValue(col,wxT("  World  "));
	grid->SetColumnWidth(col,150);
	grid->SetColLabelValue(++col,wxT("  Server/Client  "));
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
	//grid->GetParent()->SendSizeEvent();
	this->SetMaxSize(wxSize(1210,2000));
	this->SetMinSize(wxSize(680,250));
	col=0;
	
	

	
	
	
	
	
}

void ConnectionLog::AddConnection(NodeTree *robot)
{	
	wxString s_c=wxEmptyString;
	log.push_back(robot);
	grid->AppendRows(1);
	for(int i=0;i<7;i++) grid->SetCellAlignment(wxALIGN_CENTER,row,i);
	grid->SetRowLabelValue(row,wxString(robot->getNameTree()));
	robot->getSimu()->setObjConnected(true);
	
	if(robot->typeConnection==1)	
		s_c=wxT("Server");
			
	else if(robot->typeConnection==2)	
		s_c=wxT("Client");
	
	
	
	grid->SetCellValue(row,col,wxString(robot->getSimu()->getName()));
	grid->SetCellValue(row,++col,s_c);
	StateConnection(robot,false);
	row++;

}
void ConnectionLog::DeleteConnection(NodeTree *robot)
{
	wxString label=wxEmptyString;
	for(unsigned int i=0;i<log.size();i++)
	{
		if(log[i]==robot)
		{
			grid->DeleteRows(i);	
			log.erase(log.begin()+i);
			row--;
		}
	}

	robot->getSimu()->setObjConnected(false);
	for(unsigned int i=0;i<log.size();i++) 
	{
		
		grid->SetRowLabelValue(i,wxString(log[i]->getNameTree()));
		if(wxString(grid->GetCellValue(i,0)).Cmp(wxString(robot->getSimu()->getName()))==0)
			robot->getSimu()->setObjConnected(true);
	}
	
	

}

bool ConnectionLog::IsObjectConnected()
{
	if(log.size()>0)
		return true;
	else
		return false;
}

void ConnectionLog::StateConnection(NodeTree *robot,bool connected, bool errorData)
{

	mutex.Lock();
	wxString client,address,host,port,state;
	wxColour colour=*wxCYAN;

	for(unsigned int i=0;i<log.size();i++)
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
					if(!errorData)
					{
						state=wxT("Receiving Data");
						colour=*wxGREEN;
					}
					else
					{
						state=wxT("Receiving Erroneus Data");
						wxColourDatabase col;
						colour=col.Find(wxT("ORANGE"));
						
					}
					
					host=robot->client.Host;
					
	
				}
				else
					state=wxT("Waiting Server");
			
				address=robot->client.Address;
				port<<robot->client.Port;
				client=wxEmptyString;
		
			}
			col=2;
			grid->SetCellValue(i,col,state);
			grid->SetCellBackgroundColour(i,col,colour);
			grid->SetCellValue(i,++col,client);
			grid->SetCellValue(i,++col,host);
			grid->SetCellValue(i,++col,address);
			grid->SetCellValue(i,++col,port);
			col=0;
		}
	}
	mutex.Unlock();
}
