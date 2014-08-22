#ifndef __APOLO__FACEWINDOW__H
#define __APOLO__FACEWINDOW__H

#include "mainWindow.h"
#include "mrcore.h"
#include "nodeTree.h"
#include "simulatedWorld.h"
#include "canvas.h"
#include "genericSlider.h"
#include <wx/grid.h>
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/splitter.h>




class FaceWidget;
class PointsList;
class MainWindow;


class FaceWindow : public wxPanel
{
public:
	FaceWindow(wxWindow *parent,NodeTree *obj, const wxString& title, const wxPoint& pos,const wxSize& size);
	void CreatePanel();
	void AddFace();
	void FaceOrientation(wxCommandEvent& event);
	void FaceButton(wxCommandEvent& event);
	void ColorChanged(wxCommandEvent& event);
	void CopyDesign(wxCommandEvent& event);
	void PasteDesign(wxCommandEvent& event);
	FaceWidget* getcanvas(){return canvas;}
	void setType(bool modification){modi=modification;}
	void setIndex(int num){index=num;}	
	
private:
	bool modi;
	int index;
	bool worldView;
	double red,green,blue;
	Face editedFace;
	FaceWidget *canvas;
	GenericSlider *roll,*pitch,*x_pos,*y_pos,*plane_dis,*transparency;
	wxButton *cView,*Accept;
	NodeTree *node;
	FaceSetPart *face;
	MainWindow* mainWin;


	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__FACEWINDOW__H