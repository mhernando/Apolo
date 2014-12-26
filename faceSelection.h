#ifndef __FACE__SELECTION__H
#define __FACE__SELECTION__H


#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include "canvas.h"
#include "mrcore.h"
#include <wx/button.h>
#include "globalView.h"



using namespace mr;

class globalView;
class FaceSelection : public wxFrame
{
public:
	FaceSelection(wxWindow *parent,wxWindowID id,const wxString& title,FaceSetPart* set);
	void CreatePanel();
	void AddFaceSetPart();
	void SelectFace(wxSpinEvent& event);
	void UpdateSelectedFace(int num);
	Face* getFaceSelected(){return face;};
	int numSelected(){return facesel;}


private:
	wxPanel *panel;
	wxTextCtrl* textCtrl;
	Canvas *canvas3dG;
	wxSpinButton* num;
	FaceSetPart* dir;
	Face* face;
	wxButton* ok,*cancel;
	globalView* ed;
	int facesetSize;
	int facesel;
	int oldselection;
	wxTextCtrl* NumFace;

	DECLARE_EVENT_TABLE();
	
	

};

#endif // __FACE__SELECTION__H