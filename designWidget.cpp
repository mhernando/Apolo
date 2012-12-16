#include "designWidget.h"


BEGIN_EVENT_TABLE(DesignWidget, wxPanel)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE,DesignWidget::OnValueChanged)
	EVT_TEXT(wxID_ANY,DesignWidget::OnValueChanged)
END_EVENT_TABLE()

DesignWidget::DesignWidget(wxWindow *window,NodeTree *node,const wxString label ,const wxPoint& pos ,
									   const wxSize& size,bool sliders,bool orient):wxPanel( window, wxID_ANY, pos, size) 
									   
{
	slider=sliders;
	orientation=orient;
	nodeW=node;
	type=node->getTipo();
	GetDefaultValues();
	CreatePanel(sliders,orientation);	

	

}


void DesignWidget::CreatePanel(bool sliders,bool orientation)
{

/////Object's Properties///

	wxBoxSizer *pbox;
	wxString defValue;
	
	if(!slider || orientation)
		pbox=new wxBoxSizer(wxHORIZONTAL);
	else
		pbox=new wxBoxSizer(wxVERTICAL);
	

	
	if(type==N_SpherePart || type==N_CylindricalPart || type==N_PrismaticPart)
	{	
		if(slider)
		{
			c_radio=new GenericSlider(this,wxT("Radio"),wxDefaultPosition,wxDefaultSize,orientation);
			c_radio->setProperties(0,10,false);
			c_radio->setValue(defRadio);
			pbox->Add(c_radio,1,wxEXPAND);
		}
		else
		{
			r_text = new wxStaticText(this, wxID_ANY, wxT("Radio:"),wxDefaultPosition,wxDefaultSize);
			r_box = new wxTextCtrl(this,wxID_ANY,defValue<<defRadio,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
			defValue.clear();
			pbox->Add(r_text,0,wxCENTRE);
			pbox->AddSpacer(5);
			pbox->Add(r_box,1,wxCENTRE);
			pbox->AddSpacer(15);
		}

	}
	if(type==N_CylindricalPart || type==N_PrismaticPart || type==N_IrregularPrismaticPart)
	{
		if(slider)
		{
			c_height=new GenericSlider(this,wxT("Height"),wxDefaultPosition,wxDefaultSize,orientation);
			c_height->setProperties(0,10,false);
			c_height->setValue(defHeight);
			pbox->Add(c_height,1,wxEXPAND);
		}
		else
		{
			h_text = new wxStaticText(this, wxID_ANY, wxT("Height:"),wxDefaultPosition,wxDefaultSize);
			h_box = new wxTextCtrl(this,wxID_ANY,defValue<<defHeight,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
			defValue.clear();
			pbox->Add(h_text,0,wxCENTRE);
			pbox->AddSpacer(5);
			pbox->Add(h_box,1,wxCENTRE);
			pbox->AddSpacer(15);

		}
	}
	
	
	if( type==N_PrismaticPart)
	{
		if(slider)
		{
			c_vertex=new GenericSlider(this,wxT("BaseVertex"),wxDefaultPosition,wxDefaultSize,orientation);
			c_vertex->setProperties(3,10,false);
			c_vertex->setValue(defVertex);
			pbox->Add(c_vertex,1,wxEXPAND);
		}
		else
		{
			v_text = new wxStaticText(this, wxID_ANY, wxT("Vertex:"),wxDefaultPosition,wxDefaultSize);
			v_box = new wxTextCtrl(this,wxID_ANY,defValue<<defVertex,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
			defValue.clear();
			pbox->Add(v_text,0,wxCENTRE);
			pbox->AddSpacer(5);
			pbox->Add(v_box,1,wxCENTRE);
		
		}
	}

	


	SetSizer(pbox);
	
	
}


void DesignWidget::SetSpecificValues(bool def)
{

	if(type==N_SpherePart)
	{	
		if(def)
			nodeW->pointer.spherepart->setRadius(defRadio);
		else
			if(slider)
				nodeW->pointer.spherepart->setRadius(c_radio->getValue());
			else
			{
				r_box->GetValue().ToDouble(&r);
				nodeW->pointer.spherepart->setRadius(r);
			}

	}
	if( type==N_CylindricalPart)
	{
		if(def)
			nodeW->pointer.cylindricalpart->setHeightAndRadius(defHeight,defRadio);
		else
			if(slider)
				nodeW->pointer.cylindricalpart->setHeightAndRadius(c_height->getValue(),c_radio->getValue());
			else 
			{
				h_box->GetValue().ToDouble(&h);
				r_box->GetValue().ToDouble(&r);
				nodeW->pointer.cylindricalpart->setHeightAndRadius(h,r);
			}
	}
	if(type==N_PrismaticPart || type==N_IrregularPrismaticPart)
	{
		if(def)
		{
			if(type==N_PrismaticPart)nodeW->pointer.prismaticpart->setRegularPolygonBase(defRadio,defVertex);
			nodeW->pointer.prismaticpart->setHeight(defHeight);	
		}
		else
		{
			if(slider)
			{
				if(type==N_PrismaticPart)nodeW->pointer.prismaticpart->setRegularPolygonBase(c_radio->getValue(),c_vertex->getValue());
				nodeW->pointer.prismaticpart->setHeight(c_height->getValue());
			}
			else
			{
				r_box->GetValue().ToDouble(&r);
				v_box->GetValue().ToDouble(&v);
				h_box->GetValue().ToDouble(&h);
				if(type==N_PrismaticPart)nodeW->pointer.prismaticpart->setRegularPolygonBase(r,v);
				nodeW->pointer.prismaticpart->setHeight(h);
			}
		}
	}
	nodeW->getSimu()->getChild()->UpdateWorld();
	

}

void DesignWidget::GetDefaultValues()
{
	if(type==N_SpherePart)
		defRadio=nodeW->pointer.spherepart->getRadius();

	else if(type==N_CylindricalPart)
	{
		defRadio=nodeW->pointer.cylindricalpart->getRadius();
		defHeight=nodeW->pointer.cylindricalpart->getHeight();
	}
	
	else if( type==N_PrismaticPart || type==N_IrregularPrismaticPart)
	{
		defHeight=nodeW->pointer.prismaticpart->getHeight();
		if(type==N_PrismaticPart){ defVertex=4; defRadio=1; nodeW->pointer.prismaticpart->setRegularPolygonBase(defRadio,defVertex);}
		else{defVertex=nodeW->pointer.prismaticpart->getPolygonalBase().getNumVertex(); defRadio=0;}
	}
	nodeW->pointer.solidentity=dynamic_cast<SolidEntity*>(nodeW->pointer.positionableentity);
	
}

