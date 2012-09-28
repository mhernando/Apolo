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
	if(cad!=0){
		len=(uchar)strlen(cad);
		((uchar *)buffer)[n++]=(uchar)((len>255)?255:len);
		for(int i=0;i<len;i++)buffer[n++]=cad[i];
	}else buffer[n++]=0;
	return n;
}
inline int writeDouble(char *buffer, double val){
	double2byteConversor aux;
	aux.real=val;
	for(int i=0;i<8;i++)buffer[i]=aux.bytes[i];
	return 8;
}

int apoloMessage::writeSetRobotJoints(char *buffer, char *world, char *robot, int num, double *values)
{
	int n=0;
	buffer[n++]=SETJOINTS;//command
	n+=writeString(buffer+n,world);//world
	n+=writeString(buffer+n,robot);//robot
	if(num<0)num=0; 
	if(num>255)num=255;
	((uchar *)buffer)[n++]=(uchar)num;//num joints
	for(int i=0;i<num;i++)
		n+=writeDouble(buffer,values[i]);
	return n;
}
//the same message But changes the command id
int apoloMessage::writeCheckColision(char *buffer, char *world, char *robot, int num, double *values)
{
	int n=writeSetRobotJoints(buffer,world,robot,num,values);
	buffer[0]=CHECKCOLIS;
	return n;
}

apoloMessage::apoloMessage(void)
{
}

apoloMessage::~apoloMessage(void)
{
}
