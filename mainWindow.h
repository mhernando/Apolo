#ifndef __APOLO__MAINWINDOW__H
#define __APOLO__MAINWINDOW__H


#include "mrcore.h"
#include "robotConnection.h"
#include "definitions.h"
#include "simulatedWorld.h"
#include "changeLocationCtrl.h"
#include "wheeledBasePanel.h"
#include "designProperties.h"
#include "childView.h"
#include "tree.h"
#include "robotSimPanel.h"
#include "converter.h"
#include "apoloPanel.h"
#include "objectSelection.h"
#include "apoloPort.h"
#include "manageWindows.h"
#include "robotSimGoTo.h"
#include "globalView.h"
#include "panelXML.h"
#include "faceSelection.h"
#include "initialProperties.h"


#include "bitmaps/new.xpm"
#include "bitmaps/apolo.xpm"
#include "bitmaps/loadWorld.xpm"
#include "bitmaps/loadObject.xpm"
#include "bitmaps/saveWorld.xpm"
#include "bitmaps/saveObject.xpm"
#include "bitmaps/loadWorldXML.xpm"
#include "bitmaps/loadObjectXML.xpm"
#include "bitmaps/saveWorldXML.xpm"
#include "bitmaps/saveObjectXML.xpm"
#include "bitmaps/xml.xpm"
#include "bitmaps/Links.xpm"
#include "bitmaps/treeStructure.xpm"
#include "bitmaps/NewWorld.xpm"
#include "bitmaps/mainLWorld.xpm"
#include "bitmaps/mainLxml.xpm"
#include "bitmaps/axis.xpm"




#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/splitter.h>
#include <wx/event.h>
#include <wx/notebook.h>
#include <wx/wfstream.h>
#include <vector>
#include <wx/colordlg.h> 
#include <wx/aui/auibook.h>
#include <wx/laywin.h>



using namespace mr;
using namespace std;

class ApoloPort;
class ManageWindows;
class RobotSimGoTo;
class FaceSelection;
class InitialProperties;
class MainWindow : public wxMDIParentFrame
{
public:
	MainWindow(wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos,const wxSize& size, const long style);
	
	void PropertiesDisplay(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnCloseNotebook(wxAuiNotebookEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnVisibleTree(wxCommandEvent& event);
	void OnNameItemTree(wxCommandEvent& event);
	void OnConverter(wxCommandEvent& event);
	void OnChangeForm(wxCommandEvent& event);
	void OnLinkTo(wxCommandEvent& event);
	void SetState(int num){state=num;};
	int GetState(){return state;}
	void OnChangeLocationCtrl(wxCommandEvent& event);
	void OnWheeledBasePanelCtrl(wxCommandEvent& event);
	void OnRobotSimPanelCtrl(wxCommandEvent& event);
	void OnRobotSimGoTo(wxCommandEvent& event);
	void OnDrawBox(wxCommandEvent& event);
	void OnDrawReference(wxCommandEvent& event);
	void OnColor(wxCommandEvent& event);
	void OnDesign(wxCommandEvent& event);
	void AddObject(wxCommandEvent& event);
	void OnNewWorld(wxCommandEvent& event);
	void OnLoadWorld(wxCommandEvent& event);
	void OnLoadMesh(wxCommandEvent& event);
	void OnLoadObject(wxCommandEvent& event);
	void OnSaveWorld(wxCommandEvent& event);
	void OnSaveObject(wxCommandEvent& event);
	void OnDeleteWorld(wxCommandEvent& event);
	void OnConnection(wxCommandEvent& event);
	void OnLaserStyle(wxCommandEvent& event);
	void OnSimpleJointMove(wxCommandEvent& event);
	void UpdateUILoadObject(wxUpdateUIEvent& event);
	void UpdateUISaveObject(wxUpdateUIEvent& event);
	void UpdateUISaveWorld(wxUpdateUIEvent& event);
	void OnLoadWorldXML(wxCommandEvent& event);
	//void OnLoadMesh(wxCommandEvent& event);
	void OnLoadObjectXML(wxCommandEvent& event);
	void OnSaveWorldXML(wxCommandEvent& event);
	void OnSaveObjectXML(wxCommandEvent& event);
	void SaveRobotAsRobotSimXml(wxCommandEvent& event);
	void UpdateUILoadObjectXML(wxUpdateUIEvent& event);
	void UpdateUISaveObjectXML(wxUpdateUIEvent& event);
	void UpdateUISaveWorldXML(wxUpdateUIEvent& event);
	void OnReplaceMenuBar();
	void CreateMenuBar();
	void InitToolBar(wxToolBar* tool);
	void ShowSelection(wxCommandEvent& event);
	void Search(wxTreeItemId item,bool toogle);
	void ShowReferenceComposed(wxCommandEvent& event);
	void HandleChildViews(wxCommandEvent &event);
	void ShowBox(bool box);
	void ShowReference(bool refer);
	void DeleteObject(wxCommandEvent& WXUNUSED(event));
	bool getToogleReference(){return rToogle;}
	bool checkPanelExist(NodeTree* node);
	bool getTreeVisible(){return treeVisible;}
	bool getBoxVisible(){return drawBox;}
	void OnVisibleConnectionLog(wxCommandEvent& event);
	bool getReferenceVisible(){return referVisible;}
	RobotConnection *GetConnection(){return connection;}
	SimulatedWorld *GetSimulated(){return simuWorld;}
	wxTreeItemId getRoot(){return m_root;}
	void OnSashDrag(wxSashEvent& event);
	void CheckProperties();
	bool getSliderValue(){return slider;}
	bool getPopMenuValue(){return popmenu;}
	bool getDesignValue(){return design_slider;}
	wxMenu* getMenuAbout(){return menuAbout;}
	wxMenu* getMenuSettings(){return menuSettings;}
	vector <SimulatedWorld *> listWorlds;
	void CopyPasteDesign(wxCommandEvent& event);
	void CloseEditConsole(wxCommandEvent& event);
	bool GetTreeStructureState(){ return treeStruc;}
	bool GetShowLinksState(){return showLinks;}
	void CreatePanelXML();
	void showXMLEditor(wxCommandEvent& event);
	void ViewAll(wxCommandEvent& event);


	
private:
	bool treeVisible;
	bool referVisible;
	bool drawBox;
	bool rToogle;
	bool slider,popmenu,design_slider;
	bool objectlinked;
	bool xmlEditorVisible;
	bool editionVisible;
	ApoloPort *port;
	SimulatedWorld* simuWorld;
	RobotConnection *connection;
	Tree* tree;
	globalView* view;
	PanelXML* edit;
	FaceSelection* vsele;
	InitialProperties* ini;
	wxTreeItemId m_root;
	wxToolBar* toolbar;
	wxAuiNotebook* note;
	wxMenu *menuFile,*menuAbout,*menuSettings,*menuFile2,*menuView,*menuSimulator;
	wxMenu* filesubmenu3;
	wxMenuBar* menubar;
	wxToolBar* treeToolbar;
	wxPanel* m_panel;
	wxSashLayoutWindow *s;
	int w, h;
	int typ;
	wxMenu *ipro,*osel,*dwid;
	ManageWindows*	managewindow;
	wxColour colour;
	int state;  //Variable estado para controlar el linkado
	bool treeStruc,showLinks;

	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__MAINWINDOW__H