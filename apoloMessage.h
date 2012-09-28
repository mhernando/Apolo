#pragma once

#define SETJOINTS 'J'
#define SETXYZ 'P'
#define SETRPY 'O'
#define CHECKCOLIS 'j'
/*************************************************************/
/*This class implements the protocol for easily connect to apolo
*/
class apoloMessage
{
public:
	static int writeSetRobotJoints(char *buffer, char *world, char *robot, int num, double *values);
	static int writeCheckColision(char *buffer, char *world, char *robot, int num, double *values);

	apoloMessage(void);
	~apoloMessage(void);
};
