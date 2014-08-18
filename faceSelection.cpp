#include "faceSelection.h"
#include "definitions.h"
#include <wx/sizer.h>
#include "mrcore.h"
#include <wx/spinctrl.h>



using namespace std;



BEGIN_EVENT_TABLE(FaceSelection,wxFrame)
	EVT_SPIN_UP(ID_FACENUM,FaceSelection::SelectFace)
	EVT_SPIN_DOWN(ID_FACENUM,FaceSelection::SelectFace)
	EVT_SPIN(ID_FACENUM,FaceSelection::SelectFace)
END_EVENT_TABLE()



FaceSelection::FaceSelection(wxWindow *parent,wxWindowID id,const wxString& title,FaceSetPart* set)
: wxFrame(parent,wxID_ANY,title,wxDefaultPosition, wxSize(400,400),wxSTAY_ON_TOP | wxCAPTION )
{
	CreatePanel();
	panel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(400,400));
	dir=new FaceSetPart(*set);
	facesetSize=(set->getSize())-1;
	num->SetRange(0,facesetSize);
	facesel=0;
	face=dir->getFaceI(0);
	oldselection=0;
	wxString numsele;
	numsele<<0;
	NumFace->AppendText(numsele);
	AddFaceSetPart();
}



void FaceSelection::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *NumBox=new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer *buttonsBox=new wxBoxSizer(wxHORIZONTAL);
		ok=new wxButton(this,ID_SELECTEDFACE,wxT("Edit Face"),wxDefaultPosition,wxSize(30,30));
		cancel=new wxButton(this,ID_CANCELSELECTION,wxT("Cancel"),wxDefaultPosition,wxSize(30,30));
		num=new wxSpinButton(this,ID_FACENUM,wxDefaultPosition,wxDefaultSize,wxSP_HORIZONTAL);
		canvas3dG=new Canvas(this,wxID_ANY,wxDefaultPosition,wxSize(300,300));
		wxStaticText *Selection;
		Selection=new wxStaticText(this, wxID_ANY, wxString("FACE") ,wxDefaultPosition,wxDefaultSize, 0);
		NumFace=new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(35,15),wxTE_CENTER);
		fbox->Add(canvas3dG,5,wxEXPAND);
		NumBox->Add(Selection,5,wxEXPAND);
		NumBox->Add(NumFace,5,wxEXPAND);
		fbox->Add(NumBox,5,wxEXPAND);
		fbox->Add(num,5,wxEXPAND);
		buttonsBox->Add(ok,5,wxEXPAND);
		buttonsBox->Add(cancel,5,wxEXPAND);
		fbox->Add(buttonsBox,5,wxEXPAND);
		SetSizer(fbox);	
}


void FaceSelection::AddFaceSetPart()
{
	for(int i=0;i<dir->getSize();i++)
	{
		dir->getAbsoluteFaceI(i)->setColor(1,1,1,0);
	}
		dir->getAbsoluteFaceI(facesel)->setColor(1,1,0,0);
		canvas3dG->AddObject(dir);
		canvas3dG->Update();
		canvas3dG->Refresh();
}


void FaceSelection::SelectFace(wxSpinEvent& event)
{
	oldselection=facesel;
	facesel=num->GetValue();
	NumFace->Clear();
	wxString numsele;
	int sel=facesel;
	numsele<<sel;
	NumFace->AppendText(numsele);
	if(facesel==oldselection) return;
	face=dir->getAbsoluteFaceI(facesel);
	UpdateSelectedFace(facesel);
}


void FaceSelection::UpdateSelectedFace(int num)
{
	dir->getAbsoluteFaceI(num)->setColor(1,1,0,0);
	dir->getAbsoluteFaceI(oldselection)->setColor(1,1,1,0);
	canvas3dG->Update();
	canvas3dG->Refresh();
}


