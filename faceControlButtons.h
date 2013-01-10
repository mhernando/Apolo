#ifndef __APOLO__FACECONTROLBUTTONS__H
#define __APOLO__FACECONTROLBUTTONS__H



#include "faceWidget.h"

class FaceWidget;
class FaceControlButtons:public wxPanel
{
public:
	FaceControlButtons(wxWindow *parent,wxWindowID id,const wxPoint& pos,const wxSize& size,FaceWidget* face);
	void Move(wxCommandEvent& event);
	void ChangeZoom(wxCommandEvent& event);
	void RefreshCanvas();
	void CreatePanel();
	~FaceControlButtons(void);	
	
	
private:
	FaceWidget* base;   
	wxButton *Up,*Down,*Left,*Right;
	wxWindowID winId;
	wxSlider* Zoom; 

		DECLARE_EVENT_TABLE();
};



#endif // __APOLO__FACECONTROLBUTTONS__H