#ifndef __APOLO__OBJECT_SELECTION__H
#define __APOLO__OBJECT_SELECTION__H  

#include "definitions.h"
#include "canvas.h"
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/wx.h>
#include <wx/splitter.h>


class ObjectSelection:public wxDialog
{

public:
	ObjectSelection(wxWindow *parent,const wxWindowID id, const wxPoint& pos,const wxSize& size);
    void ObjectSelected(wxListEvent &event);
	wxWindowID GetObject(){return ident;};
	void*  RotationalView(void* canvas1);
	void PreliminarView(wxListEvent &event);
	void OnClose(wxCloseEvent &event);
	void CreateList();
	
	
private:
	Canvas *preView;
	PrismaticPart p;
	Face face,face1,face2,face3;
	FaceSetPart set;
	Transformation3D trans;
	PositionableEntity *preliminar;
	wxListCtrl *ob_list,*comp_list;
	wxWindowID ident,type,object;
	wxWindow *window;
	DECLARE_EVENT_TABLE();
};



#endif
