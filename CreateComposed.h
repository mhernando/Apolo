#ifndef __APOLO__CREATE_COMPOSED__H
#define __APOLO__CREATE_COMPOSED__H

#include "mrcore.h"
#include "wx/wx.h"
#include "definitions.h"
#include <wx/panel.h>
#include "canvas.h"
#include "globalView.h"
#include "genericSlider.h"
#include "bitmaps/cylinder.xpm"
#include "bitmaps/sphereCom.xpm"
#include "bitmaps/prismCom.xpm"
#include "bitmaps/coloursComp.xpm"
#include "bitmaps/AddComposedIcon.xpm"
#include "bitmaps/CreateJoint.xpm"
#include "bitmaps/AddAnotherItem.xpm"
#include "bitmaps/FinishComp.xpm"
#include "bitmaps/ComposedTCP.xpm"
#include "bitmaps/IrrPrismCom.xpm"



class globalView;

class CreateComposed:public wxPanel
{
public:
	CreateComposed(wxWindow *window,NodeTree* nod,wxWindowID id,wxPoint Pos,wxSize siz);
	void CreatePanel();
	void AddNewObject(wxCommandEvent& event);
	void ChangeDesignProperties(wxCommandEvent& event);
	void AddNewComposed(wxCommandEvent& event);
	void AddActualItem(wxCommandEvent& event);
	ComposedEntity* getCreation(){return creation;}
	void SelectComposed(wxCommandEvent& event);
	void ResetValues();
	bool getcheckAddition(){return checkAddition;}



private:
	ComposedEntity* creation;
	ComposedEntity* currentComp;
	vector<ComposedEntity *> IntegratedParts;
	vector<SimpleJoint*> Joints;
	vector<Actuator *> Actuators;
	wxToolBar* Toolbar;
	wxToolBar* MainToolbar;
	wxComboBox* selectComposedtoView;
	wxComboBox* CurrentComposed;
	Canvas *canvas3d;
	PositionableEntity* Temp;
	CylindricalPart *cylin;
	SpherePart* sphere;
	PrismaticPart* prism;
	SimpleJoint* joint;
	Tcp* TCP;
	Actuator* actuator;
	GenericSlider *PositionX,*PositionY,*PositionZ,*Roll,*Pitch,*Yaw,*Height,*Radio,*BaseVertex;
	int type,ActualComp;
	wxString Name;
	wxArrayString VComposedNames;
	wxArrayString ComposedIntegratedNames;
	globalView* CreateFig;
	bool checkAddition;
	RobotSim* robot;



	
	DECLARE_EVENT_TABLE();
	
};

#endif // __APOLO__CREATE_COMPOSED__H