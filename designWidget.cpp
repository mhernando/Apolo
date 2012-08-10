#include "designWidget.h"

BEGIN_EVENT_TABLE(DesignWidget, wxPanel)
	EVT_COMMAND(wxID_ANY, wxEVT_GENERIC_SLIDER_CHANGE,DesignWidget::OnValueChanged)
	EVT_TEXT(wxID_ANY,DesignWidget::OnValueChanged)
END_EVENT_TABLE()

DesignWidget::DesignWidget(wxWindow *window,wxWindowID id,NodeTree *node,SimulatedWorld *world,const wxString label ,const wxPoint& pos ,
									   const wxSize& size,bool sliders,bool orientation):wxPanel( window, wxID_ANY, pos, size) 
									   
{
	slider=sliders;
	auxS=auxC=auxP=false;
	simu=world;
	nodeW=node;
	defRadio=1;
	defVertex=6;
	defHeight=1;
	ObjectConversion();
	CreatePanel(sliders,orientation);	
	//GetSpecificValues(true);
	

}


void DesignWidget::CreatePanel(bool sliders, bool orientation)
{

/////Object's Properties///

	wxBoxSizer *pbox;
	if(orientation)
		pbox=new wxBoxSizer(wxHORIZONTAL);
	else
		pbox=new wxBoxSizer(wxVERTICAL);
if(sliders)
{
	if(auxS || auxC || auxP)
	{	
		c_radio=new GenericSlider(this,"Radio",wxDefaultPosition,wxDefaultSize,orientation);
		c_radio->setProperties(0,10,false);
		c_radio->setValue(defRadio);
		pbox->Add(c_radio,0,wxEXPAND);
	}
	if(auxP || auxC)
	{
		c_height=new GenericSlider(this,"Height",wxDefaultPosition,wxDefaultSize,orientation);
		c_height->setProperties(0,10,false);
		c_height->setValue(defHeight);
		pbox->Add(c_height,0,wxEXPAND);
	}
	
	if(auxP)
	{
		c_vertex=new GenericSlider(this,"BaseVertex",wxDefaultPosition,wxDefaultSize,orientation);
		c_vertex->setProperties(3,10,false);
		c_vertex->setValue(defVertex);
		pbox->Add(c_vertex,0,wxEXPAND);
	}
}
else
	{
	wxString defValue;
	pbox=new wxBoxSizer(wxHORIZONTAL);
	if(auxS || auxC || auxP)
	{	
		r_text = new wxStaticText(this, wxID_ANY, wxT("Radio:"),wxDefaultPosition,wxDefaultSize);
		r_box = new wxTextCtrl(this,wxID_ANY,defValue<<defRadio,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		pbox->Add(r_text,0,wxCENTRE);
		pbox->AddSpacer(5);
		pbox->Add(r_box,1,wxCENTRE);
		pbox->AddSpacer(15);
	}

	if(auxP || auxC)

	{
		h_text = new wxStaticText(this, wxID_ANY, wxT("Height:"),wxDefaultPosition,wxDefaultSize);
		h_box = new wxTextCtrl(this,wxID_ANY,defValue<<defHeight,wxDefaultPosition,wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_CENTRE);
		defValue.clear();
		pbox->Add(h_text,0,wxCENTRE);
		pbox->AddSpacer(5);
		pbox->Add(h_box,1,wxCENTRE);
		pbox->AddSpacer(15);
	}

	if(auxP)
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
void DesignWidget::OnValueChanged(wxCommandEvent& event)
{
	GetSpecificValues();

}

void DesignWidget::GetSpecificValues(bool def)
{

if(auxS)
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
if(auxC)
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
if(auxP)
{
	if(def)
	{
		nodeW->pointer.prismaticpart->setRegularPolygonBase(defRadio,defVertex);
		nodeW->pointer.prismaticpart->setHeight(defHeight);	
	}
	else
	{
		if(slider)
		{
			nodeW->pointer.prismaticpart->setRegularPolygonBase(c_radio->getValue(),c_vertex->getValue());
			nodeW->pointer.prismaticpart->setHeight(c_height->getValue());
		}
		else
		{
			r_box->GetValue().ToDouble(&r);
			v_box->GetValue().ToDouble(&v);
			h_box->GetValue().ToDouble(&h);
			nodeW->pointer.prismaticpart->setRegularPolygonBase(r,v);
			nodeW->pointer.prismaticpart->setHeight(h);
		}
	}
}
 simu->getChild()->UpdateWorld();
	

}

void DesignWidget::ObjectConversion()
{
	if(nodeW->pointer.spherepart=dynamic_cast<SpherePart *>(nodeW->pointer.positionableentity))
	{
		auxS=true;
		defRadio=nodeW->pointer.spherepart->getRadius();
	}
	else if(nodeW->pointer.cylindricalpart=dynamic_cast<CylindricalPart *>(nodeW->pointer.positionableentity))
	{
		auxC=true;
		defHeight=nodeW->pointer.cylindricalpart->getHeight();
	}
	else if(nodeW->pointer.prismaticpart=dynamic_cast<PrismaticPart *>(nodeW->pointer.positionableentity))
	{
		auxP=true;
		defHeight=nodeW->pointer.prismaticpart->getHeight();
		if(nodeW->pointer.prismaticpart->getPolygonalBase().getNumVertex()>3)
			defVertex=nodeW->pointer.prismaticpart->getPolygonalBase().getNumVertex();
	}

	
	nodeW->pointer.solidentity=dynamic_cast<SolidEntity*>(nodeW->pointer.positionableentity);
	GetSpecificValues(true);
}

