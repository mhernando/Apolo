#ifndef __APOLO__INITIAL_PROPERTIES__H
#define __APOLO__INITIAL_PROPERTIES__H  

#include "definitions.h"
#include "mrcore.h"
#include "mainWindow.h"
#include "nodeTree.h"
#include "simulatedWorld.h"
#include "tree.h"
#include "genericSlider.h"
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/colordlg.h>
#include "positionableWidget.h"
//

class InitialProperties:public wxDialog
{



public:
	InitialProperties(wxWindow *parent,wxWindowID id,PositionableEntity *obj, SimulatedWorld *s_world, const wxString& title);
	void OnValueChanged(wxCommandEvent& event);
	void OnButton(wxCommandEvent& event);
	int GetButtom(){return b_sel;};
	void OnClose(wxCloseEvent& event){b_sel=false; Destroy();};
	void ChangeColor(wxCommandEvent& event);
	void Update(wxWindowCreateEvent& event);
	void CreatePanel();
	
private:
	 bool b_sel;
	 wxWindowID winId;
	 wxStaticText *x_text;
	 wxTextCtrl *x_box;
	 wxStaticText *y_text;
	 wxTextCtrl *y_box;
	 wxStaticText *z_text;
	 wxTextCtrl *z_box;
	 wxStaticText *r_text;
	 wxTextCtrl *r_box;
	 wxStaticText *p_text;
	 wxTextCtrl *p_box;
	 wxStaticText *yw_text;
	 wxTextCtrl *yw_box;
	 wxStaticText *name_text;
	 wxTextCtrl *name_box;
	 wxStaticText *color_text;
	 wxBitmapButton *color_box;
	 
	 wxButton *accept;
	 wxButton *cancel;
	 wxButton *df;
	 GenericSlider *c_radio;
	 GenericSlider *c_height;
	 GenericSlider *c_vertex;
	 SimulatedWorld *world;
	 PositionableEntity *pos;
	 bool S_G;
	 Transformation3D t;
	 string text;
	 double x,y,z,r,p,yw;
	 PositionableWidget *pw;
	 double red,green,blue;
	 double defRadio,defHeight,defVertex;
	 
	
	union Object{
	SimpleJoint* simplejoint;
	Joint* joint;
	CylindricalPart* cylindricalpart;
	PrismaticPart* prismaticpart;
	LMS200Sim* lms200sim;
	LMS100Sim* lms100sim;
	FaceSetPart* facesetpart;
	PatrolbotSim* patrolbotsim;
	Pioneer3ATSim* pioneer3atsim;
	PowerCube70Sim* powercube70sim;
	NemoLaserSensor3DSim* nemolasersensor3dsim;
	LaserSensor3DSim* lasersensor3dsim;
	LaserSensorSim* lasersensorsim;
	MeshPart* meshpart;
	AdeptOneSim* adeptone;
	SpherePart * spherepart;
	SolidEntity * solidentity;
	};
	Object object;
	void GetSetSpecificValues(wxWindowID id, Object &tobject);

	DECLARE_EVENT_TABLE();
};




















#endif