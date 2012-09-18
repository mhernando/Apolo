// Prueba Clientes.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "mrcore.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Socket s;
	char name[20];
	char message[100];
	char order;
	int world;
	float x=0,y=0,z=0,u=0,v=0,w=0;
	printf("Bienvenido a la prueba de conexions clientes \n\n");
	
		while(1)
		{
		
			printf("Introduce command: (name,order,world,parameters)\n"); 
			scanf("%s %c %d %f %f %f %f %f %f",&name,&order,&world,&x,&y,&z,&u,&v,&w);
			sprintf(message,"%s %c %d %.2f %.2f %.2f %.2f %.2f %.2f",name,order,--world,x,y,z,u,v,w);
			if(s.connect("127.0.0.1",12000))
			{
				printf("Connected to Apolo\n\n");
				int e=s.Send(message,100);

				if(order=='p')
				{
					char response[100];
					int lenght=s.Receive(response,100);
					printf("%s",response);
					
				}

				if(order=='g')
				{
					char data[5000];
					int lenght=s.Receive(data,5000);
					printf("Sensor Data\n");
					printf("%s",data);


				}

				if(order=='r')
				{
					char response[100];
					int lenght=s.Receive(response,100);
					printf("%s",response);

				}
				printf("\n");
				s.close();
			}
			

	
	}
	return 0;

}
