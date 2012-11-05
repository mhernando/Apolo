
#pragma once

#define AP_NONE 0
#define AP_SETJOINTS 'J'
#define AP_PLACE 'P'
#define AP_CHECKJOINTS 'j'
#define AP_UPDATEWORLD 'U'
#define AP_TRUE 'T'
#define AP_FALSE 'F'
#define AP_PLACE_WB 'p'
#define AP_MOVE_WB 'm'
#define AP_GETLOCATION 'G'
#define AP_GETLOCATION_WB 'g'
#define AP_DVECTOR 'D'
#define AP_LINK_TO_ROBOT_TCP 'L'
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
	static int writePlaceObject(char *buffer, char *world,char *object, double *xyzrpy);
	static int writePlaceWheeledBase(char *buffer, char *world,char *robot, double *xyzy);
	static int writeMoveWheeledBase(char *buffer, char *world,char *robot, double *sp_rs_t); //speed,rot_speed,time
	static int writeGetLocation(char *buffer, char *world,char *object);
	static int writeGetLocationWheeledBase(char *buffer, char *world,char *robot);
	static int writeDoubleVector(char *buffer, int num, double *d);
	static int writeLinkToRobotTCP(char *buffer, char *world,char *robot,char *object);
	static ApoloMessage *getApoloMessage(char **buffer, int max);

	char *getWorld(){return world;}
	char *getObjectName(){return name;}
	char getType(){return type;}
	int getSize(){return size;}
	int getIntAt(int offset);
	int getUInt16At(int offset);
	double getDoubleAt(int offset);
	char getCharAt(int offset);
	char *getStringAt(int offset);


};
