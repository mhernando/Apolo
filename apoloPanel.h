#ifndef __APOLO__APOLO_PANEL__H
#define __APOLO__APOLO_PANEL__H

#include "nodeTree.h"
#include <wx/wx.h>

class ApoloPanel : public wxPanel
{ 
public:

	ApoloPanel(NodeTree* itemData,wxWindow *parent,wxWindowID id):itemNode(itemData),
		wxPanel(parent,id,wxDefaultPosition,wxDefaultSize){}
	
	NodeTree* getItemNode(){return itemNode;}
	
private:
	NodeTree* itemNode;
};
#endif //__APOLO__APOLO_PANEL__H