#include "CreateComposed.h"
#include <wx/button.h>
#include <wx/colordlg.h>
#include <wx/textdlg.h>


BEGIN_EVENT_TABLE(CreateComposed,wxPanel)

EVT_MENU(ID_CYLINDCOMPOSED,CreateComposed::AddNewObject)
EVT_MENU(ID_SPHERECOMPOSED,CreateComposed::AddNewObject)
EVT_MENU(ID_IRRPRISMCOMPOSED,CreateComposed::AddNewObject)
EVT_MENU(ID_INSERTJOINT,CreateComposed::AddNewObject)
EVT_MENU(ID_ADDCOMPPART,CreateComposed::AddNewComposed)
EVT_MENU(ID_ADDANOTHERITEM,CreateComposed::AddActualItem)
EVT_MENU(ID_ADDCOMPOSEDTCP,CreateComposed::AddNewObject)
EVT_MENU(ID_REGPRISMCOMPOSED,CreateComposed::AddNewObject)
EVT_BUTTON(ID_COLOR,CreateComposed::ChangeDesignProperties)
EVT_BUTTON(ID_ADDOWNFACE, CreateComposed::ChangeDesignProperties)
EVT_COMBOBOX(ID_SELECTCOMPOSEDTOVIEW,CreateComposed::SelectComposed)
EVT_COMBOBOX(ID_SELECTCURRENTCOMPOSED,CreateComposed::SelectComposed)
EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE, CreateComposed::ChangeDesignProperties)
END_EVENT_TABLE()


CreateComposed::CreateComposed(wxWindow *parent,NodeTree* nod,wxWindowID id,wxPoint Pos,wxSize siz)
:wxPanel(parent,wxID_ANY,Pos,siz)
{
	CreatePanel();
	type=0;
	creation=nod->pointer.composedentity;
	currentComp=creation;
	checkAddition=true;
}

void CreateComposed::CreatePanel()
{
	VComposedNames.push_back("View All");
	ComposedIntegratedNames.push_back("Main");
	wxBoxSizer* Screen=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* total=new wxBoxSizer(wxHORIZONTAL);	
	wxBoxSizer* Select=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* controls=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* SelectView=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* SelectComposed=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* CoLFin=new wxBoxSizer(wxHORIZONTAL);	

	wxColour colour(193,221,224);
	this->SetBackgroundColour(colour);
	canvas3d=new Canvas(this,wxID_ANY,wxDefaultPosition,wxSize(600,600));
	canvas3d->DrawGrid();

	wxStaticText* SelectCompToViewText=new wxStaticText(this,wxID_ANY,wxT("View Composed"),wxDefaultPosition,wxSize(115,25));
	selectComposedtoView=new wxComboBox();
	selectComposedtoView->Create(this,ID_SELECTCOMPOSEDTOVIEW,wxEmptyString,wxDefaultPosition,wxSize(130,20),VComposedNames,wxCB_DROPDOWN);
	SelectView->Add(SelectCompToViewText,0,wxSHAPED);
	SelectView->Add(selectComposedtoView,0,wxSHAPED);
	selectComposedtoView->SetStringSelection(VComposedNames[0]);

	wxStaticText* CurrentComposedText=new wxStaticText(this,wxID_ANY,wxT("Current Composed"),wxDefaultPosition,wxSize(115,25));
	CurrentComposed=new wxComboBox();
	CurrentComposed->Create(this,ID_SELECTCURRENTCOMPOSED,wxEmptyString,wxDefaultPosition,wxSize(130,20),ComposedIntegratedNames,wxCB_DROPDOWN);
	SelectComposed->Add(CurrentComposedText,0,wxSHAPED);
	SelectComposed->Add(CurrentComposed,0,wxSHAPED);
	CurrentComposed->SetStringSelection(ComposedIntegratedNames[0]);


	wxBitmap bitmaps[9];
	bitmaps[0] = wxBitmap (cylinder_xpm);
	bitmaps[1]=wxBitmap (sphereCom_xpm);
	bitmaps[2]=wxBitmap (prismCom_xpm);
	bitmaps[3]=wxBitmap(coloursComp_xpm);
	bitmaps[4]=wxBitmap(AddComposedIcon_xpm);
	bitmaps[5]=wxBitmap(CreateJoint_xpm);
	bitmaps[6]=wxBitmap(AddAnotherItem_xpm);
	bitmaps[7]=wxBitmap(ComposedTCP_xpm);
	bitmaps[8]=wxBitmap(IrrPrismCom_xpm);



	MainToolbar=new wxToolBar();
	MainToolbar->Create(this,wxID_ANY,wxDefaultPosition,wxSize(650,40),wxBORDER_NONE|wxTB_HORIZONTAL);
	MainToolbar->AddTool(ID_ADDCOMPPART,bitmaps[4], wxT("Add new Composed"));
	MainToolbar->AddSeparator();
	MainToolbar->AddTool(ID_ADDANOTHERITEM,bitmaps[6], wxT("Add New Item"));
	MainToolbar->SetBackgroundColour(wxColour(255,255,131));
	MainToolbar->Realize();

	Toolbar=new wxToolBar();
	Toolbar->Create(this,wxID_ANY,wxDefaultPosition,wxSize(300,30),wxBORDER_NONE|wxTB_HORIZONTAL);
	Toolbar->AddTool(ID_CYLINDCOMPOSED,bitmaps[0], wxT("Create Cylinder"));
	Toolbar->AddTool(ID_SPHERECOMPOSED,bitmaps[1], wxT("Create Sphere"));
	Toolbar->AddTool(ID_REGPRISMCOMPOSED,bitmaps[2],wxT("Create Regular Prism"));
	Toolbar->AddTool(ID_IRRPRISMCOMPOSED,bitmaps[8], wxT("Create Irregular Prism"));
	Toolbar->AddTool(ID_INSERTJOINT,bitmaps[5], wxT("Create Joint"));
	Toolbar->AddTool(ID_ADDCOMPOSEDTCP,bitmaps[7], wxT("Insert TCP"));
	Toolbar->SetBackgroundColour(*wxWHITE);
	Toolbar->Realize();

	Screen->Add(MainToolbar,5,wxEXPAND);
	Select->Add(SelectView,5,wxEXPAND);
	Select->AddSpacer(10);
	Select->Add(SelectComposed,5,wxEXPAND);
	Screen->Add(Select,0,wxEXPAND);
	Screen->Add(Toolbar,5,wxEXPAND);
	Screen->Add(canvas3d,0,wxEXPAND);

	PositionX=new GenericSlider(this,wxT("PositionX"),wxDefaultPosition,wxDefaultSize,false);
	PositionY=new GenericSlider(this,wxT("PositionY"),wxDefaultPosition,wxDefaultSize,false);
	PositionZ=new GenericSlider(this,wxT("PositionZ"),wxDefaultPosition,wxDefaultSize,false);
	Roll=new GenericSlider(this,wxT("Roll Orientation"),wxDefaultPosition,wxDefaultSize,false);
	Pitch=new GenericSlider(this,wxT("Pitch Orientation"),wxDefaultPosition,wxDefaultSize,false);
	Yaw=new GenericSlider(this,wxT("Yaw orientation"),wxDefaultPosition,wxDefaultSize,false);
	Radio=new GenericSlider(this,wxT("Radio"),wxDefaultPosition,wxDefaultSize,false);
	Height=new GenericSlider(this,wxT("Height"),wxDefaultPosition,wxDefaultSize,false);
	BaseVertex=new GenericSlider(this,wxT("Base Vertex"),wxDefaultPosition,wxDefaultSize,false);
	PositionX->setProperties(-20,20);
	PositionY->setProperties(-20,20);
	PositionZ->setProperties(-20,20);
	Roll->setProperties(-180,180);
	Pitch->setProperties(-180,180);
	Yaw->setProperties(-180,180);
	Radio->setProperties(0,20);
	Height->setProperties(0,20);
	BaseVertex->setProperties(3,10);

	wxStaticText* ColourText=new wxStaticText(this,wxID_ANY,wxT("Colour"),wxDefaultPosition,wxSize(35,15));
	wxBitmapButton* colo = new wxBitmapButton(this,ID_COLOR,wxIcon(coloursComp_xpm),wxDefaultPosition,wxDefaultSize);

	wxBitmapButton* FinishComp = new wxBitmapButton(this,ID_FINISHCOMPOSED,wxIcon(FinishComp_xpm),wxDefaultPosition,wxDefaultSize);
	wxBoxSizer *FinishEdition=new wxBoxSizer(wxHORIZONTAL);
	FinishEdition->Add(FinishComp,0,wxSHAPED);
	
	ResetValues();
	controls->Add(PositionX,0,wxEXPAND);
	controls->Add(PositionY,0,wxEXPAND);
	controls->Add(PositionZ,0,wxEXPAND);
	controls->Add(Roll,0,wxEXPAND);
	controls->Add(Pitch,0,wxEXPAND);
	controls->Add(Yaw,0,wxEXPAND);
	controls->Add(Radio,0,wxEXPAND);
	controls->Add(Height,0,wxEXPAND);
	controls->Add(BaseVertex,0,wxEXPAND);

	controls->Add(ColourText,0,wxEXPAND);
	CoLFin->Add(colo,0,wxSHAPED);
	CoLFin->Add(FinishEdition,0,wxSHAPED|wxALIGN_RIGHT);
	controls->Add(CoLFin,0,wxEXPAND);
	total->Add(Screen,5,wxEXPAND);
	total->AddSpacer(5);
	total->Add(controls,0,wxEXPAND|wxALIGN_RIGHT);
	SetSizer(total);
	total->SetSizeHints(this);
	ResetValues();
}

void CreateComposed::AddNewObject(wxCommandEvent& event)
{
	if(checkAddition==true)
	{
		int id=event.GetId();
	
		if (id==ID_CYLINDCOMPOSED)
		{
			Height->Enable();
			Radio->Enable();
			BaseVertex->Disable();
			Temp= new CylindricalPart();
			canvas3d->AddObject(Temp);
			canvas3d->Refresh();
			ResetValues();
			type=1;
		}

		if (id==ID_SPHERECOMPOSED)
		{
			Temp= new SpherePart();
			Height->Disable();
			Radio->Enable();
			BaseVertex->Disable();
			canvas3d->AddObject(Temp);
			canvas3d->Refresh();
			ResetValues();
			type=2;
		}

		if (id==ID_IRRPRISMCOMPOSED)
		{
			Temp=new PrismaticPart();
			Height->Enable();
			Radio->Disable();
			BaseVertex->Disable();
			ResetValues();
			type=3;
			CreateFig=new globalView(this,wxID_ANY,wxT("Create Base"));
			CreateFig->Show(true);
		}

		if (id==ID_INSERTJOINT)
		{
			Temp=new SimpleJoint();
			type=4;
			Height->Disable();
			Radio->Disable();
			BaseVertex->Disable();
		}

		if (id==ID_ADDCOMPOSEDTCP)
		{
			Temp=new Tcp();
			Temp->setName("Tcp");
			type=5;
			Height->Disable();
			Radio->Disable();
			BaseVertex->Disable();
		}

		if (id==ID_REGPRISMCOMPOSED)
		{
			Temp=new PrismaticPart();
			prism=dynamic_cast<PrismaticPart*>(Temp);
			prism->setRegularPolygonBase(1,4);	
			prism->setColor(0.5,0.5,0.5);
			canvas3d->AddObject(Temp);
			canvas3d->Refresh();
			Height->Enable();
			Radio->Enable();
			BaseVertex->Enable();
			ResetValues();
			type=6;
		}
		checkAddition=false;
	}
	else 
	{
		wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Last item needs to be added"), wxT("Error"),wxOK | wxICON_EXCLAMATION);
		dial->ShowModal();
	}
}

void CreateComposed::ChangeDesignProperties(wxCommandEvent &event)
{
	int id=event.GetId();
	if(type==1) //Creating a cylindricalPart
	{
		CylindricalPart* cyl=dynamic_cast<CylindricalPart* >(Temp);
		if (id==ID_COLOR)
		{
			wxColour ItemColour=wxGetColourFromUser(this);
			double red,green,blue;
			if(ItemColour.IsOk())
			{
				red = ItemColour.Red();
				green = ItemColour.Green();
				blue = ItemColour.Blue();
			}
			cyl->setColor((unsigned char)red/255,(unsigned char)green/255,(unsigned char)blue/255);
			canvas3d->Refresh();
			return;
		}
		
		Transformation3D t;
		Vector3D orientation;
		t.position.x=PositionX->getValue();	
		t.position.y=PositionY->getValue();		
		t.position.z=PositionZ->getValue();	
		orientation.x=Roll->getValue();	
		orientation.y=Pitch->getValue();
		orientation.z=Yaw->getValue();
		t.orientation.setRPY(deg2rad(orientation.x),deg2rad(orientation.y),deg2rad(orientation.z));
		Temp->setRelativeT3D(t);
		double radio=Radio->getValue();
		double height=Height->getValue();
		cyl->setHeightAndRadius(height,radio);
		canvas3d->Refresh();
	}

	if(type==2)//Creating a spherePart
	{
		sphere=dynamic_cast<SpherePart*>(Temp);
		if (id==ID_COLOR)
		{
			wxColour ItemColour=wxGetColourFromUser(this);
			double red,green,blue;
			if(ItemColour.IsOk())
			{
				red = ItemColour.Red();
				green = ItemColour.Green();
				blue = ItemColour.Blue();
			}
			sphere->setColor((unsigned char)red/255,(unsigned char)green/255,(unsigned char)blue/255);
			canvas3d->Refresh();
			return;
		}
		
		Transformation3D t;
		t.position.x=PositionX->getValue();	
		t.position.y=PositionY->getValue();		
		t.position.z=PositionZ->getValue();	
		Vector3D orientation;
		orientation.x=Roll->getValue();	
		orientation.y=Pitch->getValue();
		orientation.z=Yaw->getValue();
		t.orientation.setRPY(deg2rad(orientation.x),deg2rad(orientation.y),deg2rad(orientation.z));
		Temp->setRelativeT3D(t);
		double radio=Radio->getValue();
		sphere->setRadius(radio);
		canvas3d->Refresh();
	}


	if(type==3)//Creating a IrregularPrismaticPart
	{
		if (id==ID_COLOR)
		{
			wxColour ItemColour=wxGetColourFromUser(this);
			double red,green,blue;
			if(ItemColour.IsOk())
			{
				red = ItemColour.Red();
				green = ItemColour.Green();
				blue = ItemColour.Blue();
			}
			prism->setColor((unsigned char)red/255,(unsigned char)green/255,(unsigned char)blue/255);
			canvas3d->Refresh();
			return;
		}

		if (id==ID_ADDOWNFACE)
		{
			Face* base=CreateFig->GetFace();
			CreateFig->Show(false);
			prism=dynamic_cast<PrismaticPart*>(Temp);
			prism->setPolygonalBase(*base);
			prism->setHeight(1);
			canvas3d->AddObject(Temp);
			canvas3d->Refresh();
			return;
		}
		
		Transformation3D t;
		t.position.x=PositionX->getValue();	
		t.position.y=PositionY->getValue();		
		t.position.z=PositionZ->getValue();	
		Vector3D orientation;
		orientation.x=Roll->getValue();	
		orientation.y=Pitch->getValue();
		orientation.z=Yaw->getValue();
		t.orientation.setRPY(deg2rad(orientation.x),deg2rad(orientation.y),deg2rad(orientation.z));
		prism->setRelativeT3D(t);
		double height=Height->getValue();
		prism->setHeight(height);
		canvas3d->Refresh();
	}


	if(type==4) //Creating a SimpleJoint
	{
		joint=dynamic_cast<SimpleJoint*>(Temp);
		Transformation3D t;
		t.position.x=PositionX->getValue();	
		t.position.y=PositionY->getValue();		
		t.position.z=PositionZ->getValue();	
		Vector3D orientation;
		orientation.x=Roll->getValue();	
		orientation.y=Pitch->getValue();
		orientation.z=Yaw->getValue();
		t.orientation.setRPY(deg2rad(orientation.x),deg2rad(orientation.y),deg2rad(orientation.z));
		joint->setRelativeT3D(t);
		canvas3d->Refresh();
	}

	if(type==5) //Creating  TCP
	{
		TCP=dynamic_cast<Tcp*>(Temp);
		Transformation3D t;
		t.position.x=PositionX->getValue();	
		t.position.y=PositionY->getValue();		
		t.position.z=PositionZ->getValue();	
		Vector3D orientation;
		orientation.x=Roll->getValue();	
		orientation.y=Pitch->getValue();
		orientation.z=Yaw->getValue();
		t.orientation.setRPY(deg2rad(orientation.x),deg2rad(orientation.y),deg2rad(orientation.z));
		TCP->setRelativeT3D(t);
		canvas3d->AddObject(Temp);
		TCP->setDrawReferenceSystem(true);
		canvas3d->Refresh();
	}

	if(type==6)
	{
		prism=dynamic_cast<PrismaticPart*>(Temp);
		if (id==ID_COLOR)
		{
			wxColour ItemColour=wxGetColourFromUser(this);
			double red,green,blue;
			if(ItemColour.IsOk())
			{
				red = ItemColour.Red();
				green = ItemColour.Green();
				blue = ItemColour.Blue();
			}
			prism->setColor((unsigned char)red/255,(unsigned char)green/255,(unsigned char)blue/255);
			canvas3d->Refresh();
			return;
		}
		Transformation3D t;
		t.position.x=PositionX->getValue();	
		t.position.y=PositionY->getValue();		
		t.position.z=PositionZ->getValue();	
		Vector3D orientation;
		orientation.x=Roll->getValue();	
		orientation.y=Pitch->getValue();
		orientation.z=Yaw->getValue();
		t.orientation.setRPY(deg2rad(orientation.x),deg2rad(orientation.y),deg2rad(orientation.z));
		Temp->setRelativeT3D(t);
		double radio=Radio->getValue();
		int numVertex=BaseVertex->getValue();
		double height=Height->getValue();
		prism->setHeight(height);
		prism->setRegularPolygonBase(radio,numVertex);
		canvas3d->Refresh();
	}
}


void CreateComposed::AddNewComposed(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_ADDCOMPPART)
	{
		ComposedEntity* Comp=new ComposedEntity();
		string NameSet;
		wxTextEntryDialog myDialog(this,"Introduce name");
		if (myDialog.ShowModal()==wxID_OK )
		{
			wxString Name=myDialog.GetValue();
			VComposedNames.push_back(Name);
			ComposedIntegratedNames.push_back(Name);
			NameSet=std::string(Name.mb_str());
			Comp->setName(NameSet);
			selectComposedtoView->AppendString(Name);
			CurrentComposed->AppendString(Name);
		}
		IntegratedParts.push_back(Comp);
		ActualComp=IntegratedParts.size()-1;
		(*currentComp)+=IntegratedParts[IntegratedParts.size()-1];
		currentComp=IntegratedParts[IntegratedParts.size()-1];
	}
	wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Composed added"), wxT("Confirmation"), wxOK | wxICON_INFORMATION);
	dial->ShowModal();
}


void CreateComposed::AddActualItem(wxCommandEvent& event)
{
	if((type<4)||(type==6))
	{
		(*currentComp)+=Temp;
		ResetValues();
	}
	else if(type==4) 
	{
		Temp->LinkTo(currentComp);
		actuator=new Actuator();
		actuator->linkTo(Temp);
		Actuators.push_back(actuator);
		Joints.push_back(joint);
		ResetValues();
	}
	else if(type==5) 
	{
		Temp->LinkTo(currentComp);
		ResetValues();
	}
	wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Item added"), wxT("Confirmation"), wxOK | wxICON_INFORMATION);
	dial->ShowModal();
	checkAddition=true;

}


void CreateComposed::SelectComposed(wxCommandEvent& event)
{
	int id=event.GetId();
	if (id==ID_SELECTCOMPOSEDTOVIEW)
	{
		if(selectComposedtoView->GetCurrentSelection()==0)
		{
			canvas3d->ClearObjects();
			canvas3d->AddObject(creation);
			canvas3d->Refresh();
		}
		else
		{
			ActualComp=selectComposedtoView->GetCurrentSelection()-1;
			canvas3d->ClearObjects();
			canvas3d->Refresh();
			canvas3d->AddObject(IntegratedParts[ActualComp]);
			canvas3d->Refresh();
		}
	}


	if (id==ID_SELECTCURRENTCOMPOSED)
	{
		if(CurrentComposed->GetCurrentSelection()==0)
		{
			currentComp=creation;
		}
		else
		{
			int index=CurrentComposed->GetCurrentSelection()-1;
			currentComp=IntegratedParts[index];
		}
	}
}


void CreateComposed::ResetValues()
{
	PositionX->setValue(0);
	PositionY->setValue(0);
	PositionZ->setValue(0);
	Roll->setValue(0);
	Pitch->setValue(0);
	Yaw->setValue(0);
	Radio->setValue(1);
	Height->setValue(1);
	BaseVertex->setValue(3);
}