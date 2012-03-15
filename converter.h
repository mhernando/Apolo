#ifndef __APOLO__CONVERTER__H
#define __APOLO__CONVERTER__H

#include "mrcore.h"

#include <wx/wx.h> 
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/splitter.h>

#include "canvas.h"
#include "definitions.h"

#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/icono.xpm"

class Converter : public wxDialog
{
public:
	Converter(wxWindow *parent, wxWindowID id, const wxString& title);

	void OnQuit(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void resize(wxSizeEvent& event);
	void OnlyRead(MeshPart* mesh);
	void ClearAll();

private:
	wxWindowID winId;
	wxString fileName;
	wxString fileName2;
	MeshPart* tmesh;
	wxSize size;
	wxTextCtrl* tc;
	wxTextCtrl* tc2;
	wxPanel *pan;
	wxPanel *pan2;
	wxToolBar *toolbar;
	Canvas * m_canvas;
	
	DECLARE_EVENT_TABLE();
};

#endif // __APOLO__CONVERTER__H

