#ifndef __APOLO__MAINWINDOW__H
#define __APOLO__MAINWINDOW__H


#include "mrcore.h"
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
#include "bitmaps/new.xpm"
#include "bitmaps/apolo.xpm"
#include "bitmaps/loadWorld.xpm"
#include "bitmaps/loadObject.xpm"
#include "bitmaps/saveWorld.xpm"
#include "bitmaps/saveObject.xpm"

using namespace std;

class MainWindow : public wxMDIParentFrame
{
public:
	static bool slider;
	static bool popmenu;
	static bool design_slider;
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
	void OnChangeLocationCtrl(wxCommandEvent& event);
	void OnWheeledBasePanelCtrl(wxCommandEvent& event);
	void OnRobotSimPanelCtrl(wxCommandEvent& event);
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
	void UpdateUILoadObject(wxUpdateUIEvent& event);
	void UpdateUISaveObject(wxUpdateUIEvent& event);
	void UpdateUISaveWorld(wxUpdateUIEvent& event);

	void InitToolBar(wxToolBar* tool);
	void OnShowCanvas();
	void ShowSelection(wxCommandEvent& event);
	void Search(wxTreeItemId item,bool toogle);
	void ShowReferenceComposed(wxCommandEvent& event);
	void ShowBox(bool box);
	void ShowReference(bool refer);
	void DeleteObject(wxCommandEvent& WXUNUSED(event));
	bool getToogleReference(){return rToogle;};

	bool checkPanelExist(NodeTree* node);
	bool getTreeVisible(){return treeVisible;}
	bool getBoxVisible(){return drawBox;}
	bool getReferenceVisible(){return referVisible;}
	SimulatedWorld *GetSimulated(){return simuWorld;}
	wxTreeItemId getRoot(){return m_root;}
	void OnSashDrag(wxSashEvent& event);
	void CheckProperties();
	vector <SimulatedWorld *> listWorlds;
	
private:

	bool treeVisible;
	bool referVisible;
	bool drawBox;
	bool rToogle;
	SimulatedWorld* simuWorld;
	Tree* tree;
	wxTreeItemId m_root;
	wxToolBar* toolbar;
	wxAuiNotebook* note;
	wxMenu* filemenu;
	wxMenu* filemenu2;
	wxMenu* filemenu3;
	wxMenu* filesubmenu3;
	wxMenuBar* menubar;
	wxPanel* m_panel;
	wxSashLayoutWindow *s;
	int w, h;
	DECLARE_EVENT_TABLE();

};
#endif // __APOLO__MAINWINDOW__H