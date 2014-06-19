#include "faceWindow.h"

BEGIN_EVENT_TABLE(FaceWindow, wxPanel)

EVT_BUTTON(ID_OTHERFACE, FaceWindow::FaceButton)
EVT_BUTTON(ID_SHOWTHREED, FaceWindow::FaceButton)
EVT_BUTTON(ID_ADDOWNFACE, FaceWindow::FaceButton)
EVT_BUTTON(ID_COLOR, FaceWindow::ColorChanged)	
EVT_BUTTON(ID_COPYDESIGN, FaceWindow::CopyDesign)
EVT_BUTTON(ID_PASTEDESIGN, FaceWindow::PasteDesign)
EVT_MENU(ID_COPYDESIGN, FaceWindow::CopyDesign)
EVT_MENU(ID_PASTEDESIGN, FaceWindow::PasteDesign)
EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, FaceWindow::FaceOrientation)



END_EVENT_TABLE()



FaceWindow::FaceWindow(wxWindow *parent,NodeTree *obj,const wxString& title, const wxPoint& pos,const wxSize& size)
: wxPanel(parent, wxID_ANY, pos, size)
{
	mainWin=(MainWindow*)parent->GetParent();;
	node=obj;
	worldView=false;
	red=green=blue=1.0f;
	CreatePanel();

}

void FaceWindow::CreatePanel()
{
		wxBoxSizer *fbox=new wxBoxSizer(wxHORIZONTAL);//general box
		wxBoxSizer *rbox=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *sbox=new wxBoxSizer(wxVERTICAL);
		
		
		wxButton *af = new wxButton(this,ID_OTHERFACE,wxT("Add another face"),wxDefaultPosition,wxDefaultSize);
		cView = new wxButton(this,ID_SHOWTHREED,wxT("Create/Modify Face"),wxDefaultPosition,wxDefaultSize);

		canvas=new FaceWidget(this,node->getSimu(),wxDefaultPosition,wxSize(700,400));
		canvas->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		
		PositionableWidget *pw=new PositionableWidget(this,node,wxT("Face Set Orientation"),wxDefaultPosition,wxDefaultSize,mainWin->getSliderValue(),false);


		
		wxBoxSizer *pbox=new wxBoxSizer(wxHORIZONTAL);
		wxStaticBoxSizer *buttons=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Create/Modify Face"));

		wxStaticBoxSizer *obox=new wxStaticBoxSizer(wxVERTICAL,this,wxT("Face Properties"));
		roll = new GenericSlider(this,wxT("Face Roll"),wxDefaultPosition,wxDefaultSize,false);//true = vertical
		pitch = new GenericSlider(this,wxT("Face Pitch"),wxDefaultPosition,wxDefaultSize,false);
		x_pos = new GenericSlider(this,wxT("X position"),wxDefaultPosition,wxDefaultSize,false);
		y_pos = new GenericSlider(this,wxT("Y position)"),wxDefaultPosition,wxDefaultSize,false);
		plane_dis = new GenericSlider(this,wxT("Normal Distance (Z)"),wxDefaultPosition,wxDefaultSize,false);
		transparency = new GenericSlider(this,wxT("Face Transparency"),wxDefaultPosition,wxDefaultSize,false);
		
		
		
		wxBitmapButton *color_box = new wxBitmapButton(this,ID_COLOR,wxIcon(colour_xpm),wxDefaultPosition,wxSize(100,50),4,wxDefaultValidator,wxT("Change face color"));
		
		//sliders
		roll->setProperties(-180,180);
		roll->setValue(0);
		pitch->setProperties(-180,180);
		pitch->setValue(0);
		x_pos->setProperties(-10,10,false);
		x_pos->setValue(0);
		y_pos->setProperties(-10,10,false);
		y_pos->setValue(0);
		plane_dis->setProperties(-10,10,false);
		plane_dis->setValue(0);
		transparency->setProperties(0,1);
		transparency->setValue(1);

		//sliders in box
		obox->Add(roll,0,wxALL|wxEXPAND,5);
		obox->Add(pitch,0,wxALL|wxEXPAND,5);
		obox->Add(x_pos,0,wxALL|wxEXPAND,5);
		obox->Add(y_pos,0,wxALL|wxEXPAND,5);
		obox->Add(plane_dis,0,wxALL|wxEXPAND,5);
		obox->Add(transparency,0,wxEXPAND|wxALL,5);
		obox->Add(color_box,0,wxEXPAND|wxALL,5);
		
		buttons->Add(cView,0,wxEXPAND);
		pbox->Add(buttons,1,wxSHRINK);

	
		sbox->Add(canvas,0,wxEXPAND );//vertical		
		sbox->Add(pbox,0,wxEXPAND);
		sbox->Add(pw,0,wxEXPAND);

		//rbox->Add(obox,0,wxEXPAND|wxALL,5);
		rbox->Add(af,0,wxEXPAND|wxALL,5);
		fbox->Add(obox,0,wxEXPAND|wxALL,5);	//horizontal
		fbox->Add(sbox,0,wxEXPAND|wxALL,5);
		fbox->Add(rbox,0,wxEXPAND);
		
	
		SetSizer(fbox);
	
}



void FaceWindow::FaceOrientation(wxCommandEvent& event)
{	
	Transformation3D trans;
	trans.position.x=x_pos->getValue();
	trans.position.y=y_pos->getValue();
	trans.position.z=plane_dis->getValue();
	trans.orientation.setRPY(deg2rad(roll->getValue()),deg2rad(pitch->getValue()),0);
	canvas->GetView()->GetFace()->setOrigin(trans.position);
	canvas->GetView()->GetFace()->setBase(trans);
	canvas->GetCanvas3d()->AddObject(canvas->GetView()->GetFace());         
	canvas->GetCanvas3d()->Update();
	canvas->RefreshCanvas();
}

void FaceWindow::FaceButton(wxCommandEvent& event)
{
	int id=event.GetId();
	if(id==ID_SHOWTHREED)

	{
		canvas->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		canvas->GetView()->Show(true);
		canvas->GetView()->MakeModal(true);
	}

	if(id==ID_ADDOWNFACE)
	{
		//node->pointer.facesetpart->addFace(*(canvas->GetView()->GetFace())); //PARA VER LA CARA QUE ESTAMOS DIBUJANDO
		canvas->GetCanvas3d()->AddObject(canvas->GetView()->GetFace());         //PERO QUE AUN ESTÁ SIN AÑADIR AL FACESETPART
		canvas->GetCanvas3d()->Update();
		//canvas->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		canvas->GetCanvas3d()->Refresh();
		roll->setValue(0);
		pitch->setValue(0);
		plane_dis->setValue(0);	
		canvas->GetView()->Show(false);
		canvas->GetView()->MakeModal(false);
	}

	if(id==ID_OTHERFACE)
	{

		node->pointer.facesetpart->addFace(*(canvas->GetView()->GetFace()));
		canvas->GetCanvas3d()->ClearObjects();
		canvas->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
		roll->setValue(0);
		pitch->setValue(0);
		plane_dis->setValue(0);
		canvas->CreateFace();
		canvas->CreateVis2D();
	}

}



void FaceWindow::AddFace()
{
	node->pointer.facesetpart->addFace(*(canvas->GetView()->GetFace()));
	canvas->GetCanvas3d()->ClearObjects();
	canvas->GetCanvas3d()->UpdateWorld(node->getSimu()->getWorld());
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
			red = color.Red();
			green = color.Green();
			blue = color.Blue();
			canvas->GetView()->GetFace()->setColor(red/255,green/255,blue/255,transparency->getValue());
			canvas->RefreshCanvas();
		}		
}


void FaceWindow::CopyDesign(wxCommandEvent& event)
{
	int id=event.GetId();
	if(id==ID_COPYDESIGN)
	{
		node->getSimu()->CleanClipboard();
		if(canvas->GetView()->GetScreen2D()->GetVector().size()>0)
		{
			vector<Vector2D> CopiedDesign=canvas->GetView()->GetScreen2D()->GetVector();
			node->getSimu()->SetCopiedDesign(CopiedDesign);
		}
	}
}


void FaceWindow::PasteDesign(wxCommandEvent& event)
{
	int id=event.GetId();
	if(id==ID_PASTEDESIGN)
	{
		canvas->GetView()->PasteDesign(node->getSimu()->GetCopiedDesign());
	}
}
