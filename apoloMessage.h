#pragma once

#define AP_NONE 0
#define AP_SETJOINTS 'J'
#define AP_SETXYZ 'P'
#define AP_SETRPY 'O'
#define AP_CHECKJOINTS 'j'
#define AP_UPDATEWORLD 'U'
#define AP_TRUE 'T'
#define AP_FALSE 'F'
/*************************************************************/
/*This class implements the protocol for easily connect to apolo
  An apolo message pointers to an external buffer. Is simply an interpreter
  of raw data. Therefore, use have to be carefull.
*/
class ApoloMessage
{
	char *pData;//pointer to a byte secuence that has a message (header+size+type+specific data)
	char *world,*name,*bindata; //utility fields to avoid reinterpretation
	int size;
	char type;

	ApoloMessage(char *buffer,int size,char type);
public:
	static int writeSetRobotJoints(char *buffer, char *world, char *robot, int num, double *values);
	static int writeCheckColision(char *buffer, char *world, char *robot, int num, double *values);
	static int writeUpdateWorld(char *buffer, char *world);
	static int writeBOOL(char *buffer, bool val);

	static ApoloMessage *getApoloMessage(char **buffer, int max);

	char *getWorld(){return world;}
	char *getObjectName(){return name;}
	char getType(){return type;}
	int getSize(){return size;}
	int getIntAt(int offset);
	double getDoubleAt(int offset);
	char getCharAt(int offset);


};
