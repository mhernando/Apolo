#include "converter.h"

BEGIN_EVENT_TABLE(Converter, wxDialog)
	EVT_SIZE(Converter::resize)
	EVT_MENU(wxID_EXIT, Converter::OnQuit)
	EVT_MENU(ID_OPEN, Converter::OnOpen)
	EVT_MENU(ID_SAVE, Converter::OnSaveAs)
END_EVENT_TABLE()

Converter::Converter(wxWindow *parent, wxWindowID id, const wxString& title)
: wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX)
{
	winId = id;
	Centre();
	wxBitmap bitmaps[2];
	bitmaps[0] = wxBitmap( open_xpm );
	bitmaps[1] = wxBitmap( save_xpm );

	SetIcon(wxIcon(icono_xpm));

	if(id == ID_CONVERMESH)
	{
		pan = new wxPanel(this);

		toolbar = new wxToolBar(pan, wxID_ANY);
		toolbar->AddTool(ID_SAVE, bitmaps[1], wxT("Save .stl"));
		toolbar->Realize();

		tc2 = new wxTextCtrl(pan, wxID_ANY, wxT("***CPP CODE***\n"), wxDefaultPosition,wxSize(400,300), wxTE_MULTILINE);

		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		vbox->Add(toolbar,0,wxEXPAND|wxALL,5);
		vbox->Add(tc2,1,wxEXPAND|wxALL,5);
		pan->SetSizer(vbox);
		vbox->SetSizeHints(this);
	}
	
	if(id == ID_CONVER)//wxSize(950, 500)
	{
		tmesh = 0;
		
		pan = new wxPanel(this);
		pan2 = new wxPanel(pan);
		wxSplitterWindow *sp = new wxSplitterWindow(pan2,-1, wxDefaultPosition,wxSize(400,400));

		toolbar = new wxToolBar(pan, wxID_ANY);
		toolbar->AddTool(ID_OPEN, bitmaps[0], wxT("New .stl"));
		toolbar->AddTool(ID_SAVE, bitmaps[1], wxT("Save .stl"));
		toolbar->Realize();

		tc = new wxTextCtrl(sp, wxID_ANY, wxT("     *** STL CODE ***\n"), wxDefaultPosition, wxSize(100,350), wxTE_MULTILINE);
		tc2 = new wxTextCtrl(sp, wxID_ANY, wxT("    *** MRCORE.CPP CODE ***\n"), wxDefaultPosition,wxSize(100,350), wxTE_MULTILINE);

		m_canvas = new Canvas(pan2, wxID_ANY,wxDefaultPosition, wxSize(400,350));
		m_canvas->SetFocus();

		sp->SetMinimumPaneSize(30);
		sp->SplitVertically(tc,tc2);

		wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
		hbox->Add(sp,2, wxEXPAND|wxALL,5);
		hbox->Add(m_canvas,2,wxEXPAND|wxALL,5);
		pan2->SetSizer(hbox);

		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		vbox->Add(toolbar,0,wxEXPAND|wxALL,5);
		vbox->Add(pan2,1,wxEXPAND|wxALL,5);
		pan->SetSizer(vbox);
		vbox->SetSizeHints(this);
	}
}
void Converter::resize(wxSizeEvent& event)
{
	if(tc || tc2 || m_canvas)size = GetClientSize();
	event.Skip();
}
void Converter::OnQuit(wxCommandEvent& WXUNUSED(event))
{	
    wxString msg;
    msg.Printf(wxT("Are you sure close window?"));
    if ( wxMessageBox(msg, wxT("Please confirm"), wxICON_QUESTION | wxYES_NO) != wxYES )return;
	Close(true);
}
void Converter::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog openFileDialog(this,wxT("Testing open file dialog"), wxEmptyString, wxEmptyString,
            wxT("STL files (*.stl)|*.stl"),wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK)
	{
		fileName = openFileDialog.GetPath();
		tc->LoadFile(fileName);
		char file[100];
		strcpy(file,(const char*)fileName.mb_str(wxConvUTF8));
		
		if(tmesh!=0)ClearAll();
		if(tmesh==0)
		{
			tmesh = new MeshPart;

			if(tmesh->loadAsciiSTL(file))m_canvas->UpdateMeshpart(tmesh);
			else if(tmesh->loadBinarySTL(file))m_canvas->UpdateMeshpart(tmesh);
			else wxLogError(wxT("Cannot load file %s."),openFileDialog.GetPath().c_str());
			tmesh->saveAsMRcoreCppCode("xxx.cpp");
			tc2->LoadFile(wxT("xxx.cpp"));
		}
		else wxLogError(wxT("Cannot show file %s."),openFileDialog.GetPath().c_str());	
	}
	wxRemoveFile(wxT("xxx.cpp"));

    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk())
    {
       wxLogError(wxT("Cannot open file %s."), openFileDialog.GetPath().c_str());
       return;
	}
 }
void Converter::OnlyRead(MeshPart* mesh)
{
	tmesh = mesh;
	mesh->saveAsMRcoreCppCode("xxx.cpp");
	tc2->LoadFile(wxT("xxx.cpp"));
	wxRemoveFile(wxT("xxx.cpp"));
}
void Converter::OnSaveAs(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog saveFileDialog(this,wxT("Testing save file dialog"), wxEmptyString, wxEmptyString,
            wxT("CPP files (*.cpp)|*.cpp"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
        
	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		fileName2 = saveFileDialog.GetPath();
		char file2[100];
		strcpy(file2, (const char*) fileName2.mb_str(wxConvUTF8));
		if(winId == ID_CONVER)tc->SaveFile(fileName2);
		tmesh->saveAsMRcoreCppCode(file2);

		wxLogMessage(wxT("successfully saved in %s"),saveFileDialog.GetPath().c_str());
		if(winId == ID_CONVER) 
		{
			tc->Clear();
			ClearAll();
			tc2->Clear();
		}
	}
}
void Converter::ClearAll()
{
	m_canvas->ClearObjects();
	m_canvas->Refresh(false);
	delete tmesh;
	tmesh = 0;
}
