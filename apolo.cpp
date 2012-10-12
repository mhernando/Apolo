
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
	wxBitmap bitmap;

	
    if (bitmap.LoadFile(wxT("ApoloImage.bmp"), wxBITMAP_TYPE_BMP))
      wxSplashScreen* splash = new wxSplashScreen(bitmap,wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,4000, NULL, -1, 
												  wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxSTAY_ON_TOP);
 
    wxYield();

	mrcoreInit();
	apoloMainWin = new MainWindow((wxFrame *) NULL, wxID_ANY, wxT("APOLO"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE | wxHSCROLL | wxVSCROLL);
	apoloMainWin->Show(true);
	SetTopWindow(apoloMainWin);
	return true;
}
