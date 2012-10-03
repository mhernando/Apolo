
#include "apoloMex.h" //de momento inutil ...habrá que borrarlo 
#include "../apoloMessage.h"
#include "mrcore.h"
static mr::Socket  *conection=0;

/* Here is the exit function, which gets run when the MEX-file is
   cleared and when the user exits MATLAB. The mexAtExit function
   should always be declared as static. */
static void CloseConection(void){

  mexPrintf("Closing Apolo conexion .\n");
  conection->close();
  delete conection;
}
// Definimos la función de interfaz entre Matlab y C++
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

//1, obtengo el nombre de la función a ejecutar y hago un switch
//el primer argumento es el nombre de la funcion
	char *command;
    if(nrhs<1)mexErrMsgTxt("One input required.");
    if ( mxIsChar(prhs[0]) != 1)mexErrMsgTxt("Input must be a string.");

    /* input must be a row vector */
    if (mxGetM(prhs[0])!=1)mexErrMsgTxt("Input must be a row vector.");
 
    /* copy the string data from prhs[0] into a C string input_ buf. 
	OJO, hace la reserva por lo que hay que llamar a mxFree(input_buf) despues de usarlo*/
    command = mxArrayToString(prhs[0]);
    
	//si el enlace no está abierto, lo abro y registro que al salir de matlab se cierre si no se ha cerrado ya
	if(conection==0)conection=new mr::Socket;
	if(conection->IsConnected()==0)conection->connect("127.0.0.1",12000);
	if(conection->IsConnected()==0)mexErrMsgTxt("Apolo is not running at the localhost");
	/* Register an exit function. You should only register the
	   exit function after the conexion has been opened successfully*/
	mexAtExit(CloseConection);

/*#define AP_SETJOINTS 'J'
#define AP_SETXYZ 'P'
#define AP_SETRPY 'O'
#define AP_CHECKJOINTS 'j'
#define AP_UPDATEWORLD 'U'*/

	//--------------gets the input parameters that are common to most cases
	char *world=0,*name=0;
	char message[500];
	switch(command[0])
	{
	//commands with world id, and name
	case AP_SETJOINTS:
	case AP_CHECKJOINTS:
	case AP_PLACE:
		if(nrhs<3)mexErrMsgTxt(" name parameter not present");
		if (( mxIsChar(prhs[2]) != 1)&&(mxGetM(prhs[2])!=1))mexErrMsgTxt("name must be a string.");
		name=mxArrayToString(prhs[2]);
		if(name[0]==0){
			mxFree(name);
			name=0;
		}
	//commands with world only
	case AP_UPDATEWORLD:
		if(nrhs<2)mexErrMsgTxt("world parameter not present");
		if (( mxIsChar(prhs[1]) != 1)&&(mxGetM(prhs[1])!=1))mexErrMsgTxt("world must be a string.");
		world=mxArrayToString(prhs[1]);
		if(world[0]==0){
			mxFree(world);
			world=0;
		}
	}

	//--------------gets the specific input parameters and execute the command
	char *aux=message;
	int size;
	int num;
	double *dvalues;
	char resp[100];
	switch(command[0])
	{
	//commands with world id, and name
	case AP_SETJOINTS:
	case AP_CHECKJOINTS:
		//get the double vector
		if ( mxIsDouble(prhs[3]) != 1)mexErrMsgTxt("joints must be a vector of doubles.");
		dvalues = mxGetPr(prhs[3]);
		//  get the dimension of the row vector 
		if(mxGetM(prhs[3])!=1)mexErrMsgTxt("joints must be row vector.");
		num = mxGetN(prhs[3]);
		if(command[0]==AP_SETJOINTS)				
			size=ApoloMessage::writeSetRobotJoints(message,world,name,num,dvalues);
		else size=ApoloMessage::writeCheckColision(message,world,name,num,dvalues);
		
		if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		else if(command[0]==AP_CHECKJOINTS){
			mxLogical val=0;
			size=conection->Receive(resp,100,100);
			char *auxb=resp;
			ApoloMessage *m=ApoloMessage::getApoloMessage(&auxb,size);
			
			if(m){
				//prepara vector de retorno
				if(m->getType()==AP_TRUE)val=1;
				delete m;
			}
			plhs[0] = mxCreateLogicalScalar(val);
			
		}
		break;
	case AP_PLACE:
		//get the double vector
		if ( mxIsDouble(prhs[3]) != 1)mexErrMsgTxt("location must be a vector of doubles.");
		dvalues = mxGetPr(prhs[3]);
		//  get the dimension of the row vector 
		if(mxGetM(prhs[3])!=1)mexErrMsgTxt("location must be row vector.");
		if(mxGetN(prhs[3])!=6)mexErrMsgTxt("location must be a 6-row vector.");
						
		size=ApoloMessage::writePlaceObject(message,world,name,dvalues);
		if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		
		break;

	//commands with world only
	case AP_UPDATEWORLD:
		//ApoloUpdate
		size=ApoloMessage::writeUpdateWorld(message,0);
		if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		break;
	}


	


	if(command)mxFree(command);
	if(name)mxFree(name);
	if(world)mxFree(world);
}




