
// Prueba Clientes.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "mrcore.h"
	



#include "../apoloMessage.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	Socket s;
	char order;
	unsigned char jointsNumber;
	char message[500];

	char worldName[30];
	char response[6000];
	char objectName[30];
	unsigned char jointN[6];	
	double objectPosition[3];
	double objectOrientation[3];
	double jointOrientation[6];

	printf("Bienvenido a la prueba de conexions clientes \n");
	
	while(1)
	{
	printf("Waiting for connection \n");
	
	s.connect("127.0.0.1",12000);
	
	while(s.IsConnected())
		{
			printf("\nConnected\n");
			printf("\nIntroduce command:\n 1-Move robot joints."); 
			printf("\n 2-Mueve Puma 560 continuamente.\n 3-Place xyz.\n"); 
			scanf("%c",&order);
			fflush(stdin);
			printf("\nIntroduce object`s world (Push ENTER to search object in all worlds)\n"); 
			gets(worldName);
			printf("\nIntroduce object`s name\n"); 
			gets(objectName);
			


			if(order=='1')
			{
				
				printf("\nIntroduce joints number to move:\n"); 
				scanf("%uc",&jointsNumber);
				fflush(stdin);
				for(int i=0;i<jointsNumber;i++)
				{
					fflush(stdin);
					printf("\nIntroduce its new position\n"); 
					scanf("%lf",&(jointOrientation[i]));
				}
				char *aux=message;
				int size=ApoloMessage::writeSetRobotJoints(message,worldName,objectName,jointsNumber,jointOrientation);
	
				if(s.Send(message,size)<size)printf("\n--ERROR---\n");
				else printf("\nEnvio correcto de %d bytes\n",size);
				

			}
			else if(order=='2')
			{
				for(int i=-200;i<200;i++)
				{
					jointOrientation[0]=i*0.01;
	
					int size=ApoloMessage::writeCheckColision(message,0,"Puma 560",1,jointOrientation);
				if(s.Send(message,size)<0)printf("\n--ERROR---\n");
				else {
					//printf("\nEnvio correcto de %d bytes\n",size);
					s.Receive(response,6000,-1);
					char *auxb=response;
					ApoloMessage *m=ApoloMessage::getApoloMessage(&auxb,6000);
					if(m){
						if(m->getType()==AP_TRUE)printf("\tHAY COLISION %lf |",jointOrientation[0]);
						else if(m->getType()==AP_FALSE)printf("\tLIBRE");
						else printf("\nNI IDEA");
						delete m;
					}
				}


				
				size=ApoloMessage::writeUpdateWorld(message,0);
				if(s.Send(message,size)<size)
				/*printf("\n--ERROR-UPDATE-\n")*/;
				else /*printf("\nEnvio de UPDATE\n")*/;
				}


			}
			else if(order=='3')
			{
				
				printf("\nIntroduce x y z:\n"); 
			
				double xyz[6]={0,0,0,0,0,0};
				for(int i=0;i<3;i++)
				{
					fflush(stdin);
					printf("\nIntroduce value:"); 
					scanf("%lf",&(xyz[i]));
				}
				char *aux=message;
				int size=ApoloMessage::writePlaceObject(message,worldName,objectName,xyz);
	
				if(s.Send(message,size)<size)printf("\n--ERROR---\n");
				else printf("\nEnvio correcto de %d bytes\n",size);
				
				size=ApoloMessage::writeUpdateWorld(message,0);
				if(s.Send(message,size)<size)
				/*printf("\n--ERROR-UPDATE-\n")*/;
				else /*printf("\nEnvio de UPDATE\n")*/;

			}
			
				

			fflush(stdin);
		}
		s.close();
		
	}

}








