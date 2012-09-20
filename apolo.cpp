
#include "mrcore.h"
#include "mainWindow.h"
#include <wx/wx.h>
#include <wx/splash.h>
#include <wx/menu.h>

class Apolo : public wxApp
{
public:
	virtual bool OnInit();
	MainWindow* apoloMainWin;
};

IMPLEMENT_APP(Apolo);


bool Apolo::OnInit()
{
	/*wxBitmap bitmap;
    if (bitmap.LoadFile(wxT("splash.bmp"), wxBITMAP_TYPE_BMP))
      wxSplashScreen* splash = new wxSplashScreen(bitmap,wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,500, NULL, -1, 
												  wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxSTAY_ON_TOP);
 
    wxYield();*/

	
	mrcoreInit();
	apoloMainWin = new MainWindow((wxFrame *) NULL, wxID_ANY, wxT("APOLO"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE | wxHSCROLL | wxVSCROLL);

	//make a menu
	wxMenu *filemenu = new wxMenu;
	wxMenuItem *item1 = new wxMenuItem(filemenu, ID_NEW, wxT("New world"),wxT("Create a new world"));
	item1->SetBitmap(new_xpm);
	filemenu->Append(item1);
	wxMenuItem *item2 = new wxMenuItem(filemenu,ID_LOADWORLD, wxT("Load world"), wxT("Load a file of world"));
	item2->SetBitmap(loadWorld_xpm);
	filemenu->Append(item2);
	filemenu->AppendSeparator();
	filemenu->AppendCheckItem(ID_VIS_TREE, wxT("Unvisible tree"));
	filemenu->AppendSeparator();
	filemenu->Append(wxID_EXIT, wxT("E&xit"), wxT("Quit the program"));
	
	wxMenu *filemenu2 = new wxMenu;
	filemenu2->Append(wxID_ABOUT, wxT("&About..."), wxT("Information program"));

	wxMenuBar *menubar = new wxMenuBar;
	menubar->Append(filemenu, wxT("&File"));
	menubar->Append(filemenu2, wxT("About"));

	apoloMainWin->SetMenuBar(menubar);
	apoloMainWin->Show(true);
	SetTopWindow(apoloMainWin);
	return true;
}
