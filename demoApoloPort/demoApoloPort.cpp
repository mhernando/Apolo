// Prueba Clientes.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "mrcore.h"
	
void generateMessage(char order,char nameWorld[],char nameObject[],double objectPosition[],double objectOrientation[],unsigned char jointsNumber,unsigned char jointN[],double jointOrientation[],char buffer[]);

union floatConversor
{
		unsigned char bytes[8];
		double real;
};
int _tmain(int argc, _TCHAR* argv[])
{
	Socket s;
	

	
	char message[500];
	char order;
	char worldName[30];
	char objectName[30];
	double objectPosition[3]={0,0,0};
	double objectOrientation[3]={0,0,0};
	unsigned char jointsNumber=0;
	unsigned char jointN[6]={0,0,0,0,0,0};
	double jointOrientation[6]={0,0,0,0,0,0};
	
	printf("Bienvenido a la prueba de conexions clientes \n");
	
		while(1)
		{
		
			printf("\nIntroduce command:\n 1-Move object(p).\n 2-Move robot joints(j).\n 3-Get sensor data(g).\n"); 
			scanf("%c",&order);

			printf("\nIntroduce object`s world (0 = Search object in all worlds)\n"); 
			scanf("%s",worldName);


			printf("\nIntroduce object`s name\n"); 
			scanf("%s",objectName);

			if(order=='p')
			{
				printf("\nIntroduce object`s  position\n"); 
				scanf("%lf %lf %lf",&objectPosition[0],&objectPosition[1],&objectPosition[2]);

				printf("\nIntroduce object`s  orientation\n"); 
				scanf("%lf %lf %lf",&objectOrientation[0],&objectOrientation[1],&objectOrientation[2]);
			}

			else if(order=='j')
			{

				printf("\nIntroduce joints number to move:\n"); 
				scanf("%c",jointsNumber);

				for(int i=0;i<jointsNumber;i++)
				{
					printf("\nIntroduce joint to move and its new position\n"); 
					scanf("%c %lf",jointN,jointOrientation);
				}
			//else if(message[0]=='g')

			}
			generateMessage(order,worldName,objectName,objectPosition,objectOrientation,jointsNumber,jointN,jointOrientation,message);
		}




}



void generateMessage(char order,char nameWorld[],char nameObject[],double objectPosition[],double objectOrientation[],unsigned char jointsNumber,unsigned char jointN[],double jointOrientation[],char buffer[] )
{
	int n=0;
	unsigned char h;
	floatConversor conv;
	
	buffer[n++]=order;
	h=buffer[n++]=(unsigned char)strlen(nameWorld);
	for(int i=0;i<h;i++)
		buffer[n++]=nameWorld[i];
		
	
	h=buffer[n++]=(unsigned char)strlen(nameObject);
	for(int i=0;i<h;i++)
		buffer[n++]=nameObject[i];
	
	
	if(order=='p')
	{
	
	for(int i=0;i<3;i++)
	{
		conv.real=objectPosition[i];
		for(int i=0;i<8;i++)
			buffer[n++]=conv.bytes[i];

	}

	for(int i=0;i<3;i++)
	{
		conv.real=objectOrientation[i];
		for(int i=0;i<8;i++)
			buffer[n++]=conv.bytes[i];

	}
	
	}





}





