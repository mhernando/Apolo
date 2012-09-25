// Prueba Clientes.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "mrcore.h"
	
void  generateMessage(char order,char nameWorld[],char nameObject[],double objectPosition[],double objectOrientation[],unsigned char jointsNumber,unsigned char jointN[],double jointOrientation[], char buffer[]);

union double2byteConversor
{
		unsigned char bytes[8];
		double real;
};

union int2byteConversor
{
		unsigned char bytes[4];
		int integer;
};
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
	double2byteConversor dConv;
	int2byteConversor iConv;
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
			printf("\nIntroduce command:\n 1-Move object(p).\n 2-Move robot joints(j).\n 3-Get sensor data(g).\n"); 
			scanf("%c",&order);
			fflush(stdin);
			printf("\nIntroduce object`s world (0 = Search object in all worlds)\n"); 
			gets(worldName);
			
			

			printf("\nIntroduce object`s name\n"); 
			gets(objectName);
			
	
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
				scanf("%uc",&jointsNumber);
				fflush(stdin);
				for(int i=0;i<jointsNumber;i++)
				{
					fflush(stdin);
					printf("\nIntroduce joint to move\n"); 
					scanf("%uc",&jointN[i]);
					printf("\nIntroduce its new position\n"); 
					scanf("%lf",&jointOrientation[i]);
				}
		

			}
			
			
			if(s.IsConnected())
				generateMessage(order,worldName,objectName,objectPosition,objectOrientation,jointsNumber,jointN,jointOrientation,message);
				
			
			if(0<s.Send(message,500))
			{
			
				if(order=='g')
				{	
					int i=0;
				
					s.Receive(response,6000,-1);
					
					for(int j =0;j<4;j++)
						iConv.bytes[j]=response[i++];
					int nPoints=iConv.integer;
					char data[8];
					double range;
				
					printf("\nSensor Data\n");
					
					for(int j=0; j<nPoints; j++)
						{
							for(int z=0;z<8;z++)
								dConv.bytes[z]=response[i++];
							range=dConv.real;
							printf("%.2f\t",range);
						}
					s.Send("\nSucces\n",100);
					
			
				}
			int err=s.Receive(response,50,-1);
			if(err>0)
				printf("%s",response);
			
			else
			{
				LOG_ERROR("Receive error");//sock.Receive outputs LOG
				break;
			}
			
			}

			else
			{
				printf("\nIt wasn,t possible to send the message\n");
				break;
			}
			
			fflush(stdin);
		}
		s.close();
		
	}

}



void generateMessage(char order,char nameWorld[],char nameObject[],double objectPosition[],double objectOrientation[],unsigned char jointsNumber,unsigned char jointN[],double jointOrientation[],char buffer[])
{
	int n=0;
	
	unsigned char h;
	double2byteConversor dConv;
	int2byteConversor iConv;
	buffer[n++]=order;
	h=buffer[n++]=(unsigned char)strlen(nameWorld)+1;
	for(int i=0;i<h;i++)
		buffer[n++]=nameWorld[i];
		
	
	h=buffer[n++]=(unsigned char)strlen(nameObject)+1;
	for(int i=0;i<h;i++)
		buffer[n++]=nameObject[i];
	
	
	if(order=='p')
	{
	
		for(int i=0;i<3;i++)
		{
			dConv.real=objectPosition[i];
			for(int i=0;i<8;i++)
				buffer[n++]=dConv.bytes[i];

		}

		for(int i=0;i<3;i++)
		{
			dConv.real=objectOrientation[i];
			for(int i=0;i<8;i++)
				buffer[n++]=dConv.bytes[i];
		}
	}

	else if(order=='j')
	{
		buffer[n++]=jointsNumber;
		for(int i=0;i<jointsNumber;i++)
			buffer[n++]=jointN[i];
		for(int i=0;i<jointsNumber;i++)
		{
			dConv.real=jointOrientation[i];
			for(int i=0;i<8;i++)
				buffer[n++]=dConv.bytes[i];
		}
			
		
	}

	
}





