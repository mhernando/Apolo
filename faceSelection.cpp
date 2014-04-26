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
	dir=set;
	facesetSize=set->getSize()-1;
	num->SetRange(0,facesetSize);
	facesel=0;
	oldselection=0;
	AddFaceSetPart();
}



void FaceSelection::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *buttonsBox=new wxBoxSizer(wxHORIZONTAL);
		ok=new wxButton(this,ID_SELECTEDFACE,wxT("Edit Face"),wxDefaultPosition,wxSize(30,30));
		cancel=new wxButton(this,ID_CANCELSELECTION,wxT("Cancel"),wxDefaultPosition,wxSize(30,30));
		num=new wxSpinButton(this,ID_FACENUM,wxDefaultPosition,wxDefaultSize,wxSP_HORIZONTAL);
		canvas3dG=new Canvas(this,wxID_ANY,wxDefaultPosition,wxSize(150,150));
		canvasFace=new Canvas(this,wxID_ANY,wxDefaultPosition,wxSize(150,150));
		canvasFace->ChangeBackGroundColour(*wxBLUE);
		wxStaticText *Selection;
		Selection=new wxStaticText(this, wxID_ANY, wxString("FACE") ,wxDefaultPosition,wxDefaultSize, 0);
		fbox->Add(canvas3dG,5,wxEXPAND);
		fbox->Add(Selection,5,wxEXPAND);
		fbox->Add(num,5,wxEXPAND);
		fbox->Add(canvasFace,5,wxEXPAND);
		buttonsBox->Add(ok,5,wxEXPAND);
		buttonsBox->Add(cancel,5,wxEXPAND);
		fbox->Add(buttonsBox,5,wxEXPAND);
		SetSizer(fbox);	
}


void FaceSelection::AddFaceSetPart()
{
		dir->getFaceP_I(facesel)->setColor(1,1,0,0);
		canvas3dG->AddObject(dir);
		canvas3dG->Update();
		canvas3dG->Refresh();
		canvasFace->AddObject(dir->getFaceP_I(0));
		canvasFace->Update();
		canvasFace->Refresh();
}


void FaceSelection::SelectFace(wxSpinEvent& event)
{
	oldselection=facesel;
	facesel=num->GetValue();
	if(facesel==oldselection) return;
	AddFace(facesel);
}


void FaceSelection::AddFace(int num)
{
	dir->getFaceP_I(num)->setColor(1,1,0,0);
	dir->getFaceP_I(oldselection)->setColor(1,1,1,0);
	canvas3dG->Update();
	canvas3dG->Refresh();
	canvasFace->ClearObjects();
	canvasFace->AddObject(dir->getFaceP_I(num));
	canvasFace->Update();
	canvasFace->Refresh();
}


