#ifndef __APOLO__OBJECT_SELECTION__H
#define __APOLO__OBJECT_SELECTION__H  

#include "definitions.h"
#include <wx/listctrl.h>
#include <wx/wx.h>

class ObjectSelection:public wxDialog
{

	

public:
	 ObjectSelection(wxWindow *parent,const wxWindowID id, const wxPoint& pos,const wxSize& size);
    void ObjectSelected(wxListEvent &event);
	wxWindowID GetObject(){return ident;};
	void CreateList();
	
	
private:
	wxListCtrl *ob_list,*comp_list;
	wxWindowID ident,type;

	DECLARE_EVENT_TABLE();
};



#endif