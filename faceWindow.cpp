#include "faceWindow.h"


BEGIN_EVENT_TABLE(FaceWindow, wxPanel)

EVT_COMMAND(wxID_ANY,wxEVT_FACEVERTEX_ADDED,FaceWindow::GetPoint)
EVT_COMMAND(wxID_ANY,wxEVT_POINT_ADDED,FaceWindow::AddVertex)
EVT_BUTTON(ID_OTHERFACE, FaceWindow::FaceButton)
EVT_BUTTON(ID_COLOR, FaceWindow::ColorChanged)
//EVT_RADIOBUTTON(wxID_ANY,FaceWindow::RadioClicked) 	
EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, FaceWindow::FaceOrientation)
END_EVENT_TABLE()



FaceWindow::FaceWindow(wxWindow *parent,NodeTree *obj,SimulatedWorld *simu, const wxWindowID id, const wxString& title, const wxPoint& pos,const wxSize& size)
: wxPanel(parent, id, pos, size)
{
	
	world=simu;
	node=obj;
	CreatePanel();
	CreateFace();

}

void FaceWindow::CreatePanel()
{


		wxBoxSizer *fbox=new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer *rbox=new wxBoxSizer(wxVERTICAL);
		wxSplitterWindow *drawFace= new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxSP_LIVE_UPDATE || wxSP_3D);
		wxSplitterWindow *canvas= new wxSplitterWindow(drawFace, ID_DRAG, wxDefaultPosition, wxDefaultSize,wxSP_LIVE_UPDATE || wxSP_3D);
	
		drawFace->SetMinimumPaneSize(200);
		canvas->SetMinimumPaneSize(50);
	
		canvas1=new Canvas(canvas, wxID_ANY, wxDefaultPosition, wxDefaultSize,false);
		canvas2=world->getChild()->getCanvas();
		parentCurrentWorld=canvas2->GetParent();
		canvas2->Reparent(canvas);
		

		PositionableWidget *pw=new PositionableWidget(drawFace,node,world,wxT("Face Set Orientation"),wxDefaultPosition,wxDefaultSize,MainWindow::slider);
		points=new PointsList(this,wxID_ANY,wxT("Face Coordenates"));
		af = new wxButton(this,ID_OTHERFACE,wxT("Add another face"),wxDefaultPosition,wxDefaultSize);
		
		wxBoxSizer *obox=new wxBoxSizer(wxVERTICAL);
		
		roll = new GenericSlider(this,"Face Roll",wxDefaultPosition,wxDefaultSize,true);//true = vertical
		pitch = new GenericSlider(this,"Face Pitch",wxDefaultPosition,wxDefaultSize,true);
		wxString string[2]={wxT("1"), wxT("0")};
		radio = new wxRadioBox(this,wxID_ANY,wxT("Face Transparency"),wxDefaultPosition,wxDefaultSize,2,string);
		wxBitmapButton *color_box = new wxBitmapButton(this,ID_COLOR,wxIcon(colour_xpm),wxDefaultPosition,wxDefaultSize);
		roll->setProperties(-180,180);
		roll->setValue(0);
		pitch->setProperties(-180,180);
		pitch->setValue(0);

		
		obox->Add(roll,1,wxALL|wxEXPAND,5);
		obox->Add(pitch,1,wxALL|wxEXPAND,5);
		obox->Add(radio,0,wxEXPAND|wxALL,5);
		obox->Add(color_box,0,wxEXPAND|wxALL,5);

		drawFace->SplitHorizontally(canvas,pw,0);
		canvas->SplitVertically(canvas2,canvas1,0);
		fbox->Add(obox,0,wxEXPAND);
		fbox->Add(drawFace,1,wxEXPAND );
		rbox->Add(points,1,wxEXPAND|wxALL,5);
		rbox->Add(af,0,wxEXPAND);
		fbox->Add(rbox,0,wxEXPAND);
		
		SetSizer(fbox);
		
	
	
}

void FaceWindow::GetPoint(wxCommandEvent& event)
{
	point=canvas1->getCursorPosition();
	wxSize c_size=canvas1->GetSize();
	point.x-=c_size.GetWidth()/2;
	point.y-=c_size.GetHeight()/2;
	points->SetPoints(point.x,-point.y);
	
}

void FaceWindow::CreateFace()
{
	face=new Face();
	faceCopy=new Face((*face));
	canvas1->scene2D.addObject(faceCopy);

}

void FaceWindow::FaceOrientation(wxCommandEvent& WXUNUSED(event))
{	
	Transformation3D trans;
	trans.orientation.setRPY(roll->getValue(),pitch->getValue(),0);
	face->setBase(trans);

}

void FaceWindow::FaceButton(wxCommandEvent& WXUNUSED(event))
{
	points->RefreshGrid();
	node->pointer.facesetpart->addFace((*face));
	world->getChild()->UpdateWorld();
	roll->setValue(0);
	pitch->setValue(0);
	CreateFace();
	
}

void FaceWindow::AddVertex(wxCommandEvent &event)
{
	wxPoint vertex=points->getLastPointAdded();
	face->addVertex(vertex.x,vertex.y);
	faceCopy->addVertex(vertex.x,vertex.y);
	world->getChild()->UpdateWorld();
	canvas1->Refresh();

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
			if(radio->GetSelection()==1)
				alfa=0;

			face->setColor(red/255,green/255,blue/255,alfa);
			world->getChild()->UpdateWorld();
		
		}		
}