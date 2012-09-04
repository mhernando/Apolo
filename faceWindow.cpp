#include "faceWindow.h"




BEGIN_EVENT_TABLE(FaceWindow, wxPanel)

EVT_BUTTON(ID_OTHERFACE, FaceWindow::FaceButton)
EVT_BUTTON(ID_COLOR, FaceWindow::ColorChanged)	
EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, FaceWindow::FaceOrientation)
//EVT_COMMAND(wxID_ANY,wxEVT_POINT_ADDED,FaceWindow::AddVertex)
EVT_RADIOBOX(wxID_ANY, FaceWindow::FaceAlign)


END_EVENT_TABLE()



FaceWindow::FaceWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size)
: wxPanel(parent, wxID_ANY, pos, size)
{
	
	world=obj->getSimu();
	node=obj;
	CreatePanel();

}

void FaceWindow::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer *rbox=new wxBoxSizer(wxVERTICAL);	
		
		
		points=new PointsList(this,wxT("Face Coordenates"));
		af = new wxButton(this,ID_OTHERFACE,wxT("Add another face"),wxDefaultPosition,wxDefaultSize);
		wxSplitterWindow *drawFace= new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxSP_LIVE_UPDATE || wxSP_3D);
		drawFace->SetMinimumPaneSize(200);
		
		canvas=new FaceWidget(drawFace,world,wxDefaultPosition,wxDefaultSize);
		canvas->AssociatePointTable(points);
		
		PositionableWidget *pw=new PositionableWidget(drawFace,node,wxT("Face Set Orientation"),wxDefaultPosition,wxDefaultSize,MainWindow::slider,false);
		drawFace->SplitHorizontally(canvas,pw,0);
		
		wxStaticBoxSizer *obox=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Face Properties"));
		roll = new GenericSlider(this,"Face Roll",wxDefaultPosition,wxDefaultSize,false);//true = vertical
		pitch = new GenericSlider(this,"Face Pitch",wxDefaultPosition,wxDefaultSize,false);
		plane_dis = new GenericSlider(this,"Normal Distance (Z)",wxDefaultPosition,wxDefaultSize,true);
		wxString string[2]={wxT("1"), wxT("0")};
		wxString string2[2]={wxT("On"), wxT("Off")};
		trans = new wxRadioBox(this,wxID_ANY,wxT("Face Transparency"),wxDefaultPosition,wxDefaultSize,2,string);
		align = new wxRadioBox(this,wxID_ANY,wxT("Align Face Drawing "),wxDefaultPosition,wxDefaultSize,2,string2);
		wxBitmapButton *color_box = new wxBitmapButton(this,ID_COLOR,wxIcon(colour_xpm),wxDefaultPosition,wxSize(100,50),4,wxDefaultValidator,wxT("Change face color"));
		roll->setProperties(-180,180);
		roll->setValue(0);
		pitch->setProperties(-180,180);
		pitch->setValue(0);
		plane_dis->setProperties(-10,10);
		plane_dis->setValue(0);

		
		obox->Add(roll,0,wxALL|wxEXPAND,5);
		obox->Add(pitch,0,wxALL|wxEXPAND,5);
		obox->Add(plane_dis,1,wxALL|wxEXPAND,5);
		obox->Add(trans,0,wxEXPAND|wxALL,5);
		obox->Add(align,0,wxEXPAND|wxALL,5);
		obox->Add(color_box,0,wxEXPAND|wxALL,5);
	

		
		fbox->Add(obox,0,wxEXPAND|wxALL,5);

		fbox->Add(drawFace,1,wxEXPAND );
		rbox->Add(points,1,wxEXPAND|wxALL,5);
		rbox->Add(af,0,wxEXPAND);
		fbox->Add(rbox,0,wxEXPAND);
		
	
		SetSizer(fbox);
	
	
}



void FaceWindow::FaceAlign(wxCommandEvent& event)
{
	if(align->GetSelection()==0)
		canvas->SetAlign(true);
	else
		canvas->SetAlign(false);

}



void FaceWindow::FaceOrientation(wxCommandEvent& WXUNUSED(event))
{	
	Transformation3D trans;
	trans.position.z=plane_dis->getValue();
	trans.orientation.setRPY(deg2rad(roll->getValue()),deg2rad(pitch->getValue()),0);
	canvas->GetFace()->setBase(trans);
	canvas->RefreshCanvas();

}

void FaceWindow::FaceButton(wxCommandEvent& WXUNUSED(event))
{
	node->pointer.facesetpart->addFace((*canvas->GetFace()));
	world->getChild()->UpdateWorld();
	roll->setValue(0);
	pitch->setValue(0);
	plane_dis->setValue(0);
	canvas->CreateFace();

	
}



void  FaceWindow::ColorChanged(wxCommandEvent& event)
{
	
		wxColor color=wxGetColourFromUser(this);
		if(color.IsOk())
		{
			double red,green,blue,alfa=1;
			red = color.Red();
			green = color.Green();
			blue = color.Blue();
			if(trans->GetSelection()==1)
				alfa=0;

			canvas->GetFace()->setColor(red/255,green/255,blue/255,alfa);
			world->getChild()->UpdateWorld();
		
		}		
}

