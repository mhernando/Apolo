#include "apoloMessage.h"
#include <string.h>
/**UTILITY UNIONS FOR CONVERSIONS**/
union double2byteConversor
{
		char bytes[8];
		double real;
};

union int2byteConversor
{
		char bytes[4];
		int integer;
};
typedef unsigned char uchar;

/**
writes intp buffer the message for moving a robot in a worl
if world=null... is equivalent to any.
returns the message size
**/
inline int writeString(char *buffer, char *cad){
	int n=0,len;
	if(cad!=0){ //not null
		if(cad[0]==0)buffer[n++]=0;//empty string
		else{
			len=1+(uchar)strlen(cad);
			((uchar *)buffer)[n++]=(uchar)((len>255)?255:len);
			for(int i=0;i<len-1;i++)buffer[n++]=cad[i];
			buffer[n++]=0;
		}
	}else buffer[n++]=0;
	return n;
}
inline int writeDouble(char *buffer, double val){
	double2byteConversor aux;
	aux.real=val;
	for(int i=0;i<8;i++)buffer[i]=aux.bytes[i];
	return 8;
}
inline void insertSize(char *message, int size)//size including the header
{
	((uchar *)message)[2]=(uchar)(size%255);
	((uchar *)message)[3]=(uchar)(size/255);
}
//tamaño minimo de mensaje es 5
inline int writeHeader(char*buffer,char command) //escribe la cabecera 
{
	int n=0;
	buffer[0]='a';
	buffer[1]='a';
	insertSize(buffer,5);
	buffer[4]=command;
	return 5;
}

int ApoloMessage::writeSetRobotJoints(char *buffer, char *world, char *robot, int num, double *values)
{
	int n=0;
	n+=writeHeader(buffer,AP_SETJOINTS);//command
	n+=writeString(buffer+n,world);//world
	n+=writeString(buffer+n,robot);//robot
	if(num<0)num=0; 
	if(num>255)num=255;
	((uchar *)buffer)[n++]=(uchar)num;//num joints
	for(int i=0;i<num;i++)
		n+=writeDouble(buffer+n,values[i]);
	insertSize(buffer,n);
	return n;
}
//the same message But changes the command id
int ApoloMessage::writeCheckColision(char *buffer, char *world, char *robot, int num, double *values)
{
	int n=writeSetRobotJoints(buffer,world,robot,num,values);
	buffer[4]=AP_CHECKJOINTS;
	return n;
}
int ApoloMessage::writeUpdateWorld(char *buffer, char *world)
{
	int n=0;
	n+=writeHeader(buffer,AP_UPDATEWORLD);//command
	n+=writeString(buffer+n,world);//world
	insertSize(buffer,n);
	return n;
}
int ApoloMessage::writeBOOL(char *buffer, bool val)
{
	int n=0;
	char command=AP_FALSE;
	if(val)command=AP_TRUE;
	n+=writeHeader(buffer,command);//command
	insertSize(buffer,n);
	return n;
}
ApoloMessage::ApoloMessage(char *buffer,int size,char type)
{
	char *aux;
	pData=buffer; 
	this->size=size;
	this->type=type;
	if(type==AP_NONE)return;
	world=bindata=name=0;
	switch(type)
	{//command with world and name
		case AP_SETJOINTS:
		case AP_SETXYZ:
		case AP_SETRPY:
		case AP_CHECKJOINTS:
			if(pData[5]!=0){
				world=pData+6;
				aux=world+((uchar *)pData)[5];
			}else aux=pData+6;
			if(aux[0]!=0){
				name=aux+1;
				aux=name+((uchar *)aux)[0];
			}else aux++;
			bindata=aux;
		break;
		case AP_UPDATEWORLD://commands with world only
			if(pData[5]!=0){
				world=pData+6;
				aux=world+((uchar *)pData)[5];
			}else aux=pData+6;
			bindata=aux;
		break;
		default: //commands without world neither
			bindata=pData+5;
	}
	
}
//se considera que el buffer contiene mensajes completos (pueden ser varios)... si son parciales, se desecharán
ApoloMessage *ApoloMessage::getApoloMessage(char **buffer, int max)
{
	int i=0;
	while(i+4<max){
		if(((*buffer)[i]=='a')&&((*buffer)[i+1]=='a'))
		{
			int size=(((uchar *)(*buffer))[i+2])+(((uchar *)(*buffer))[i+3])*255;
			char  type=(*buffer)[i+4];
			//si el mensaje es correcto... crea el mensaje y situa el puntero al final
			//ojo... el mensaje mantiene unos punteros sobre el buffer original. El mensaje no reserva memoria
			if(i+size<=max){
				ApoloMessage *message=new ApoloMessage((*buffer)+i,size,type);
				*buffer=*buffer+size;
				return message;
			}//si no lo es retorna null
			else return 0;
			
		}
	i++;
	}
	return 0;
}
int ApoloMessage::getIntAt(int offset)
{
	int2byteConversor aux;
	if(offset+(bindata-pData)+4>size)return 0;
	for(int i=0;i<4;i++)aux.bytes[i]=bindata[offset+i];
	return aux.integer;
}
double ApoloMessage::getDoubleAt(int offset)
{
	double2byteConversor aux;
	if(offset+(bindata-pData)+8>size)return 0;
	for(int i=0;i<8;i++)aux.bytes[i]=bindata[offset+i];
	return aux.real;
}
char ApoloMessage::getCharAt(int offset)
{
	if(offset+(bindata-pData)+1>size)return 0;
	return bindata[offset];
}

