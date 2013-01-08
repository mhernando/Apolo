#include "wheeledBasePanel.h"
#include "mainWindow.h"

BEGIN_EVENT_TABLE(WheeledBasePanel,wxFrame)
	EVT_BUTTON(ID_UP,WheeledBasePanel::OnButton)
	EVT_BUTTON(ID_DOWN,WheeledBasePanel::OnButton)
	EVT_BUTTON(ID_LEFT,WheeledBasePanel::OnButton)
	EVT_BUTTON(ID_RIGHT,WheeledBasePanel::OnButton)
	EVT_BUTTON(ID_STOP,WheeledBasePanel::OnButton)
	EVT_CLOSE(WheeledBasePanel::OnClose)
END_EVENT_TABLE()

WheeledBasePanel::WheeledBasePanel(wxWindow *parent, wxWindowID id,const wxString& title_frame,NodeTree* itemData)
: wxFrame(parent, id,  title_frame,wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP) 
 
{
	itemnode = itemData;
	this->SetIcon(wxIcon(pioneer_xpm));

	wxBitmap bit[5];
	bit[0] = wxBitmap ( up_xpm);
	bit[1] = wxBitmap ( down_xpm);
	bit[2] = wxBitmap ( left_xpm);
	bit[3] = wxBitmap ( right_xpm);
	bit[4] = wxBitmap ( stop_xpm);

	wxPanel* panel=new wxPanel(this, wxID_ANY);
	title = new wxStaticText(panel,wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize(40,25));

	up = new wxBitmapButton(panel,ID_UP,bit[0],wxDefaultPosition,wxSize(48,48));//55
	down = new wxBitmapButton(panel,ID_DOWN,bit[1],wxDefaultPosition,wxSize(48,48));
	stop = new wxBitmapButton(panel,ID_STOP,bit[4],wxDefaultPosition,wxSize(48,48));
	left = new wxBitmapButton(panel,ID_LEFT,bit[2],wxDefaultPosition,wxSize(48,48));
	right = new wxBitmapButton(panel,ID_RIGHT,bit[3],wxDefaultPosition,wxSize(48,48));

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(left,0,wxALL,5);
	hbox->Add(stop,0,wxALL,5);
	hbox->Add(right,0,wxALL,5);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(title,0,wxLEFT|wxTOP,10);
	vbox->AddSpacer(15);
	vbox->Add(up,0,wxCENTRE|wxALL,1);
	vbox->Add(hbox,0,wxCENTRE|wxALL,1);
	vbox->Add(down,0,wxCENTRE|wxALL,1);
	
	panel->SetSizer(vbox);
	vbox->SetSizeHints(this);
}
void WheeledBasePanel::OnButton(wxCommandEvent& event)
{
	int id = event.GetId();
	double speed,rotspeed;
	itemnode->pointer.wheeledbasesim->getSpeed(speed,rotspeed);
	if(id == ID_UP)itemnode->pointer.wheeledbasesim->move(speed+0.1,rotspeed);
	if(id == ID_DOWN)itemnode->pointer.wheeledbasesim->move(speed-0.1,rotspeed);
	if(id == ID_STOP)itemnode->pointer.wheeledbasesim->move(0,0);
	if(id == ID_LEFT)itemnode->pointer.wheeledbasesim->move(speed,rotspeed+0.2);
	if(id == ID_RIGHT)itemnode->pointer.wheeledbasesim->move(speed,rotspeed-0.2);
	event.Skip();
}



void WheeledBasePanel::setManageWindow (ManageWindows* mg)
{
	managewindow=mg;
	mg->addWindowWheeledBase(this);

}

void WheeledBasePanel::Delete()
{

	managewindow->WindowWheeledBaseIsClosed(this);
	Destroy();	

}

void WheeledBasePanel::OnClose(wxCloseEvent& event)
{

	managewindow->WindowWheeledBaseIsClosed(this);
	Destroy();	

}