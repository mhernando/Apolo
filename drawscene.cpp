#include "drawscene.h"
#include "base\streamfile.h"
#include "world\world.h"
#include <math.h>
#include <fstream>
#include <iostream>
#include "gl\gltools.h"
#include "math\mrmath.h"
#include <gl\GLU.h>






bool DrawScene::save(string filename)//without extension
{
	StreamFile str(filename+".glscene2d",false);
	if(!str.good())return false;
	writeToStream(str);
	return true;
}
bool DrawScene::load(string filename)//without extension
{
	StreamFile str(filename+".glscene2d",true);
	if(!str.good())return false;
	readFromStream(str);
	return true;
}
void DrawScene::writeToStream(Stream& stream)
{
	int n=(int)object.size();
	stream<<n;
	for(int i=0;i<n;i++)
		stream.write(object[i]);
}
void DrawScene::readFromStream(Stream& stream)
{
	int n;
	stream>>n;
	for(int i=0;i<n;i++)
	{
		Object* obj=stream.read();
		GLObject* globj=dynamic_cast<GLObject*>(obj);
		if(globj)
			loadedObjects.push_back(globj);
		else
			delete obj;
	}
}

DrawScene::DrawScene()
{
	this->initX=0;
	this->initY=0;
	finalX=1;
	finalY=1;
	showGrid=true;
	showFrame=true;
	gridsize=2;	
}

DrawScene::~DrawScene()
{
	for(unsigned int i=0;i<loadedObjects.size();i++)
	{
		delete loadedObjects[i];
	}
}
void DrawScene::KeyDown(unsigned char key)
{
	//cout << "key: " << (int)key << endl;
	if(key=='g')
		showGrid=!showGrid;
	if(key=='f')
		showFrame=!showFrame;
	if(key=='+'){
		if (finalX-initX>2){	initX+=1; finalX-=1; }
		if (finalY-initY>2){ initY+=1; finalY-=1; }
		initX+=1; initY+=1;
		finalX-=1; finalY-=1;
	}
	if(key=='-'){
		initX-=1; finalX+=1; 
		initY-=1; finalY+=1;
	}
		
	if(key==UP_ARROW)
	{
		initY+=1;
		finalY+=1;
	}
	if(key==DOWN_ARROW)
	{
		initY-=1;
		finalY-=1;
	}
	if(key==LEFT_ARROW)
	{
		initX-=1;
		finalX-=1;
	}
	if(key==RIGHT_ARROW)
	{
		initX+=1;
		finalX+=1;
	}

	
}
void DrawScene::init()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer
	//Para definir el punto de vista
	glMatrixMode(GL_PROJECTION); // Proyection mode
	glLoadIdentity(); 
	glOrtho(initX , finalX , initY , finalY ,0.0, 1.0); // Proyección ortográfica, dentro del cubo señalado

}





void DrawScene::Draw()
{
	//Borrado de la pantalla	
	glClearColor(0,0,0.9,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer

	//Para definir el punto de vista
	glMatrixMode(GL_PROJECTION); // Proyection mode
	glLoadIdentity(); 
	glOrtho(initX , finalX , initY , finalY ,0.0, object.size()); // Proyección ortográfica, dentro del cubo señalado
	glMatrixMode(GL_MODELVIEW); // Model mode
	glLoadIdentity();							// Reset The Current Matrix
	glPushMatrix();
	
	
	if(showGrid)
		GLTools::DrawGrid(90,gridsize,200);
	if(showFrame)
		GLTools::DrawFrame();
	
	for(unsigned int i=0;i<object.size();i++)
	{
		glLoadIdentity();
		glTranslatef((GLfloat)0,(GLfloat)0,(GLfloat)i);
		object[i]->drawGL();
	}
	for(unsigned int i=0;i<loadedObjects.size();i++)
	{
		loadedObjects[i]->drawGL();
	}
	
	glPopMatrix();
}
void DrawScene::MouseButton(int x,int y,int button,bool down,bool sKey, bool ctrlKey)
{
	
	if(down)
	{
		controlKey=ctrlKey;
		shiftKey=sKey;
	}
	else
		controlKey=shiftKey=false;
}
void DrawScene::MouseMove(int x,int y)
{
	if(controlKey)
	{
		if(leftButton)
		{		
			//y_previo=y;	
		}
		if(rightButton)
		{	
			//y_previo=y;	
		}
	}
	else
	{
		if(leftButton)
		{
			//y_previo=y;	
		}
		if(rightButton)
		{
			//y_previo=y;	
		}	
	}
}


void DrawScene:: clearObjects()
{
	object.clear();
}


void DrawScene::setObjects(vector<GLObject*> v)
{
	object=v;
}


void DrawScene::addObject(GLObject *o)
{
	object.push_back(o);
}


void DrawScene::setViewSize(int width, int height)
{

	finalX=initX+width;
	finalY=initY+height;
}


void DrawScene::setMap(GLObject *o, double initX, double initY, double width, double height)
{
	this->initX=initX;
	this->initY=initY;
	finalX=initX+width;
	finalY=initY+height;
	if (object.size()>0){
		object.erase(object.begin());
		object.insert(object.begin(),o);
	}else{
		object.push_back(o);
	}
}


void DrawScene::removeObject(GLObject* obj)
{
	for(unsigned int i=0;i<object.size();i++)
	{
		if(object[i]==obj)
		{
		//	delete obj;
			object.erase(object.begin()+i);
			break;
		}
	}
}




