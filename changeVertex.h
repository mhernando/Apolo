#ifndef __APOLO__CHANGE_VERTEX__H
#define __APOLO__CHANGE_VERTEX__H

#include "pointsList.h"
#include <wx/button.h>

class PointsList;
class ChangeVertex:public wxDialog
{
public:
	ChangeVertex(wxWindow *parent, wxWindowID id, const wxString& title,const wxString& old_x,const wxString& old_y);

	void OnButton(wxCommandEvent& event);
	void VertexChanged(wxString vert_x,wxString vert_y);
	Vector2D GetNewVertex(){return newVertex;}
	void OnValuesChange();//wxCommandEvent& event);


private:
	bool emptyvertex;
	wxPanel *panel;
	wxButton *accept,*cancel;
	wxStaticText* vert_old_x,*vert_old_y,*vert_new_x,*vert_new_y;
	wxTextCtrl* name_box1,*name_box2,*name_box3,*name_box4;
	Vector2D newVertex,oldVertex;
	PointsList* pointlist;
	DECLARE_EVENT_TABLE();
};


#endif // __APOLO__CHANGE_VERTEX__H