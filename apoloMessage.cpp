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
inline int Apolo_writeString(char *buffer, char *cad){
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
inline int Apolo_writeDouble(char *buffer, double val){
	double2byteConversor aux;
	aux.real=val;
	for(int i=0;i<8;i++)buffer[i]=aux.bytes[i];
	return 8;
}
inline int Apolo_writeUInt16(char *message, int &num)
{
	if(num>65535)num=65535;
	if(num<0)num=0;
	((uchar *)message)[0]=(uchar)(num%256);
	((uchar *)message)[1]=(uchar)(num/256);
	return 2;
}
inline void Apolo_insertSize(char *message, int size)//size including the header
{
	((uchar *)message)[2]=(uchar)(size%256);
	((uchar *)message)[3]=(uchar)(size/256);
}
//tamaño minimo de mensaje es 5
inline int Apolo_writeHeader(char*buffer,char command) //escribe la cabecera 
{
	int n=0;
	buffer[0]='a';
	buffer[1]='a';
	Apolo_insertSize(buffer,5);
	buffer[4]=command;
	return 5;
}

int ApoloMessage::writeSetRobotJoints(char *buffer, char *world, char *robot, int num, double *values)
{
	int n=0;
	n+= Apolo_writeHeader(buffer,AP_SETJOINTS);//command
	n+= Apolo_writeString(buffer+n,world);//world
	n+= Apolo_writeString(buffer+n,robot);//robot
	if(num<0)num=0; 
	if(num>255)num=255;
	((uchar *)buffer)[n++]=(uchar)num;//num joints
	for(int i=0;i<num;i++)
		n+= Apolo_writeDouble(buffer+n,values[i]);
	Apolo_insertSize(buffer,n);
	return n;
}
int ApoloMessage::writePlaceObject(char *buffer, char *world,char *object, double *xyzrpy)
{
	int n=0,i;
	n+= Apolo_writeHeader(buffer,AP_PLACE);//command
	n+= Apolo_writeString(buffer+n,world);//world
	n+= Apolo_writeString(buffer+n,object);//object
	for(i=0;i<6;i++)
		n+= Apolo_writeDouble(buffer+n,xyzrpy[i]);
	Apolo_insertSize(buffer,n);
	return n;
}
int ApoloMessage::writeMoveWheeledBase(char *buffer, char *world,char *robot, double *sp_rs_t)
{
	int n=0,i;
	n+= Apolo_writeHeader(buffer,AP_MOVE_WB);//command
	n+= Apolo_writeString(buffer+n,world);//world
	n+= Apolo_writeString(buffer+n,robot);//robot
	for(i=0;i<3;i++)//speed, rot speed, time
		n+= Apolo_writeDouble(buffer+n,sp_rs_t[i]);
	Apolo_insertSize(buffer,n);
	return n;
}
int  ApoloMessage::writePlaceWheeledBase(char *buffer, char *world,char *robot, double *xyzy)
{
	int n=0,i;
	n+= Apolo_writeHeader(buffer,AP_PLACE_WB);//command
	n+= Apolo_writeString(buffer+n,world);//world
	n+= Apolo_writeString(buffer+n,robot);//robot
	for(i=0;i<4;i++)//x,y,z, rot z
		n+= Apolo_writeDouble(buffer+n,xyzy[i]);
	Apolo_insertSize(buffer,n);
	return n;
}
//the same message But changes the command id
int ApoloMessage::writeCheckColision(char *buffer, char *world, char *robot, int num, double *values)
{
	int n=writeSetRobotJoints(buffer,world,robot,num,values);
	buffer[4]=AP_CHECKJOINTS;
	return n;
}
int ApoloMessage::writeGetLocation(char *buffer, char *world,char *object)
{
	int n=0,i;
	n+= Apolo_writeHeader(buffer,AP_GETLOCATION);//command
	n+= Apolo_writeString(buffer+n,world);//world
	n+= Apolo_writeString(buffer+n,object);//robot
	Apolo_insertSize(buffer,n);
	return n;
}
int ApoloMessage::writeGetLocationWheeledBase(char *buffer, char *world,char *robot)
{
	int n=0,i;
	n+= Apolo_writeHeader(buffer,AP_GETLOCATION_WB);//command
	n+= Apolo_writeString(buffer+n,world);//world
	n+= Apolo_writeString(buffer+n,robot);//robot
	Apolo_insertSize(buffer,n);
	return n;
}
int ApoloMessage::writeUpdateWorld(char *buffer, char *world)
{
	int n=0;
	n+= Apolo_writeHeader(buffer,AP_UPDATEWORLD);//command
	n+= Apolo_writeString(buffer+n,world);//world
	Apolo_insertSize(buffer,n);
	return n;
}
int ApoloMessage::writeLinkToRobotTCP(char *buffer, char *world,char *robot,char *object)
{
	int n=0,i;
	n+= Apolo_writeHeader(buffer,AP_LINK_TO_ROBOT_TCP);//command
	n+= Apolo_writeString(buffer+n,world);//world
	n+= Apolo_writeString(buffer+n,robot);//robot
	n+= Apolo_writeString(buffer+n,object);//robot
	Apolo_insertSize(buffer,n);
	return n;
}
int ApoloMessage::writeGetLaserData(char *buffer, char *world, char *laser)
{
	int n = 0;
	n += Apolo_writeHeader(buffer, AP_GET_LASER_DATA);//command
	n += Apolo_writeString(buffer + n, world);//world
	n += Apolo_writeString(buffer + n, laser);//laser
	Apolo_insertSize(buffer, n);
	return n;
}
int ApoloMessage::writeGetLaserLandMarks(char *buffer, char *world, char *laser)//AP_GET_LASER_LM
{
	int n = 0;
	n += Apolo_writeHeader(buffer, AP_GET_LASER_LM);//command
	n += Apolo_writeString(buffer + n, world);//world
	n += Apolo_writeString(buffer + n, laser);//laser
	Apolo_insertSize(buffer, n);
	return n;
}
int ApoloMessage::writeGetOdometry(char  *buffer, char *world, char *robot)
{
	int n = 0;
	n += Apolo_writeHeader(buffer, AP_GET_WB_ODOMETRY);//command
	n += Apolo_writeString(buffer + n, world);//world
	n += Apolo_writeString(buffer + n, robot);//robot
	Apolo_insertSize(buffer, n);
	return n;
}
int ApoloMessage::writeGetUltrasonicSensor(char *buffer, char *world, char* name)
{
	int n = 0;
	n += Apolo_writeHeader(buffer, AP_GET_USENSOR);//command
	n += Apolo_writeString(buffer + n, world);//world
	n += Apolo_writeString(buffer + n, name);//laser
	Apolo_insertSize(buffer, n);
	return n;
}
int ApoloMessage::writeGetDependentUltrasonicSensors(char *buffer, char *world, char* object) 
{
	int n = 0;
	n += Apolo_writeHeader(buffer, AP_GET_DEP_USENSORS);//command
	n += Apolo_writeString(buffer + n, world);//world
	n += Apolo_writeString(buffer + n, object);//laser
	Apolo_insertSize(buffer, n);
	return n;
}
int ApoloMessage::writeResetOdometry(char *buffer, char *world, char *robot, double *xyt) //AP_RESET_ODOMETRY
{
	int n = 0, i;
	n += Apolo_writeHeader(buffer, AP_RESET_ODOMETRY);//command
	n += Apolo_writeString(buffer + n, world);//world
	n += Apolo_writeString(buffer + n, robot);//robot
	for (i = 0; i<3; i++)//x,y, rot z
		n += Apolo_writeDouble(buffer + n, xyt[i]);
	Apolo_insertSize(buffer, n);
	return n;
}

int ApoloMessage::writeDoubleVector(char *buffer, int num, double *d)
{
	int n=0;

	n+= Apolo_writeHeader(buffer,AP_DVECTOR);//command
	n+= Apolo_writeUInt16(buffer+n,num);
	for(int i=0;i<num;i++)
		n+= Apolo_writeDouble(buffer+n,d[i]);
	Apolo_insertSize(buffer,n);
	return n;
}
int ApoloMessage::writeDoubleVector(char *buffer, std::vector<double> v)
{
	int n = 0;
	n += Apolo_writeHeader(buffer, AP_DVECTOR);//command
	int num = (int)v.size();
	n += Apolo_writeUInt16(buffer + n, num);
	for (int i=0; i<num; ++i) 
		n += Apolo_writeDouble(buffer + n, v[i]);
	Apolo_insertSize(buffer, n);
	return n;
}

int ApoloMessage::writeLandMarkInfoVector(char *buffer, std::vector<mr::LaserSensorSim::LandMarkInfo> &v)
{
	int n = 0;
	n += Apolo_writeHeader(buffer, AP_LM_INFO);//command
	int num = (int)v.size();
	n += Apolo_writeUInt16(buffer + n, num);
	for (int i = 0; i < num; ++i) {
		n += Apolo_writeUInt16(buffer + n, v[i].ID);
		n += Apolo_writeDouble(buffer + n, v[i].ang);
		n += Apolo_writeDouble(buffer + n, v[i].dist);
	}
	Apolo_insertSize(buffer, n);
	return n;
}
int ApoloMessage::writeBOOL(char *buffer, bool val)
{
	int n=0;
	char command=AP_FALSE;
	if(val)command=AP_TRUE;
	n+= Apolo_writeHeader(buffer,command);//command
	Apolo_insertSize(buffer,n);
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
		case AP_CHECKJOINTS:
		case AP_PLACE:
		case AP_PLACE_WB:
		case AP_MOVE_WB:
		case AP_GETLOCATION_WB:
		case AP_GETLOCATION:
		case AP_LINK_TO_ROBOT_TCP:
		case AP_GET_LASER_DATA:
		case AP_GET_WB_ODOMETRY:
		case AP_GET_USENSOR:
		case AP_GET_DEP_USENSORS:
		case AP_GET_LASER_LM:
		case AP_RESET_ODOMETRY:
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
		break;
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
int ApoloMessage::getUInt16At(int offset)
{
	if(offset+(bindata-pData)+2>size)return 0;
	return (((uchar *)(bindata))[offset])+(((uchar *)(bindata))[offset+1])*255;
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
char *ApoloMessage::getStringAt(int offset)
{
	if(offset+(bindata-pData)+1>size)return 0;
	uchar tam=((uchar *)(bindata))[offset];
	if(tam==0)return 0;
	if(offset+(bindata-pData)+tam+1>size)return 0;
	else return bindata+offset+1;
}
