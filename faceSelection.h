#ifndef __FACE__SELECTION__H
#define __FACE__SELECTION__H


#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include "canvas.h"
#include "mrcore.h"
#include <wx/button.h>
#include "globalView.h"
#include "math\face.h"
#include "world\facesetpart.h"


using namespace mr;


class FaceSelection : public wxFrame
{
public:
	FaceSelection(wxWindow *parent,wxWindowID id,const wxString& title,FaceSetPart* set);
	void CreatePanel();
	void AddFaceSetPart();
	void SelectFace(wxSpinEvent& event);
	void AddFace(int num);
	void EditFace(wxCommandEvent& event);
	Face getFaceSelected(){return face;};
	Face* getFacePSelected(){return dir->getFaceP_I(facesel);};
	int numSelected(){return facesel;}


private:
	wxPanel *panel;
	wxTextCtrl* textCtrl;
	Canvas *canvas3dG,*canvasFace;
	wxSpinButton* num;
	FaceSetPart faceset;
	FaceSetPart* dir;
	Face face;
	wxButton* ok,*cancel;
	globalView* ed;
	int facesetSize;
	int facesel;
	int oldselection;

	DECLARE_EVENT_TABLE();
	
	

};

#endif // __FACE__SELECTION__H