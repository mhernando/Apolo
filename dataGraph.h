#ifndef __APOLO__DATA_GRAPH__H
#define __APOLO__DATA_GRAPH__H


#include "NodeTree.h"
#include "mathplot.h"
#include <wx/wx.h>


enum TypeData
{
	POSITION,
	VELOCITY,
	ACCELERATION,
};

class DataGraph: public wxFrame
{
public:
	DataGraph(wxWindow *parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData, NodeTree *parentData);
	~DataGraph();


template<class T> 
void enterData(TypeData _type, T data);

private:
	TypeData _data;
	std::vector<double> pos;
	std::vector<double> vel;
	std::vector<double> acel;

};

#endif //__APOLO__DATA_GRAPH__H