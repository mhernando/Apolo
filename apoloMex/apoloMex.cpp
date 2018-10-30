
//#include "apoloMex.h" //de momento inutil ...habr� que borrarlo 
#include "../apoloMessage.h"
#include "mrcore.h"

#ifdef _CHAR16T
#define CHAR16_T
#endif
#include "mex.h"
#include "matrix.h"

static mr::Socket  *conection=0;

/* Here is the exit function, which gets run when the MEX-file is
   cleared and when the user exits MATLAB. The mexAtExit function
   should always be declared as static. */
static void CloseConection(void){

  mexPrintf("Closing Apolo conexion .\n");
  conection->close();
  delete conection;
}
// Definimos la funci�n de interfaz entre Matlab y C++
static  ApoloMessage *waitForCompleteMessage(char **buffer, int max, int &size)
{
	ApoloMessage *m = 0;
	int numintentos = 0;
	while (m == 0) {
		size += conection->Receive((*buffer)+size, max-size,1);
		m = ApoloMessage::getApoloMessage(buffer, size);
		numintentos++;
		//char errorText[100];
		//mexPrintf( "leido %d=", size);
		//mexErrMsgTxt("Mensaje de respuesta no identificado");
		if (size == max)return 0;
		if (numintentos > 1000)return 0;
	}
	return m;
}
void createMexLandMarkInfoFromMessage(ApoloMessage *m, mxArray **p)
{
	if (m) {
		if (m->getType() == AP_LM_INFO) {
			int num = m->getUInt16At(0);
			const char *fieldnames[3] = { "id","angle","distance" };		
			mxArray *m_id,*m_ang, *m_dist;
			m_id = mxCreateNumericMatrix(1, num, mxINT32_CLASS, mxREAL);
			m_ang = mxCreateDoubleMatrix(1, num, mxREAL);
			m_dist = mxCreateDoubleMatrix(1, num, mxREAL);
			int *id = (int *)mxGetData(m_id);
			double *angle = mxGetPr(m_ang);
			double *dist = mxGetPr(m_dist);
			for (int i = 0; i < num; i++) {
				id[i] = m->getUInt16At(2 + i * (18));
				angle[i] = m->getDoubleAt(4 + i * (18));
				dist[i] = m->getDoubleAt(12 + i * (18));
			}
			p[0]= mxCreateStructMatrix(1, 1, 3, fieldnames);	
			mxSetFieldByNumber(p[0], 0, 0, m_id);
			mxSetFieldByNumber(p[0], 0, 1, m_ang);
			mxSetFieldByNumber(p[0], 0, 2, m_dist);
		}
		else mexErrMsgTxt("Mensaje de respuesta erroneo");
		delete m;
	}
	else mexErrMsgTxt("Mensaje de respuesta no identificado");
}
void createMexDoubleArrayFromMessage(ApoloMessage *m, mxArray **p)
{
	if (m) {
		//prepara vector de retorno
		if (m->getType() == AP_DVECTOR) {

			int num = m->getUInt16At(0);

			p[0] = mxCreateDoubleMatrix(1, num, mxREAL);
			double *z = mxGetPr(p[0]);
			for (int i = 0; i<num; i++)z[i] = m->getDoubleAt(2 + i * 8);
			//mexPrintf("Create double: %d %f %f %f", num, z[0], z[1], z[2]);
		}
		else mexErrMsgTxt("Mensaje de respuesta erroneo");
		delete m;
	}
	else mexErrMsgTxt("Mensaje de respuesta no identificado");
	
}
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

//1, obtengo el nombre de la funci�n a ejecutar y hago un switch
//el primer argumento es el nombre de la funcion
	char *command;
    if(nrhs<1)mexErrMsgTxt("One input required.");
    if ( mxIsChar(prhs[0]) != 1)mexErrMsgTxt("Input must be a string.");

    /* input must be a row vector */
    if (mxGetM(prhs[0])!=1)mexErrMsgTxt("Input must be a row vector.");
 
    /* copy the string data from prhs[0] into a C string input_ buf. 
	OJO, hace la reserva por lo que hay que llamar a mxFree(input_buf) despues de usarlo*/
    command = mxArrayToString(prhs[0]);
    
	//si el enlace no est� abierto, lo abro y registro que al salir de matlab se cierre si no se ha cerrado ya
	if(conection==0)conection=new mr::Socket;
	if(conection->IsConnected()==0)conection->connect("127.0.0.1",12000);
	if(conection->IsConnected()==0)mexErrMsgTxt("Apolo is not running at the localhost");
	/* Register an exit function. You should only register the
	   exit function after the conexion has been opened successfully*/
	mexAtExit(CloseConection);



	//--------------gets the input parameters that are common to most cases
	char *world=0,*name=0,*auxname=0;
	char message[500];
	switch(command[0])
	{
	//commands with world id, and name
	case AP_SETJOINTS:
	case AP_CHECKJOINTS:
	case AP_PLACE:
	case AP_PLACE_WB:
	case AP_MOVE_WB:
	case AP_GETLOCATION:
	case AP_GETLOCATION_WB:
	case AP_LINK_TO_ROBOT_TCP:
	case AP_GET_LASER_DATA:
	case AP_GET_LASER_LM:
	case AP_GET_WB_ODOMETRY:
	case AP_RESET_ODOMETRY:
	case AP_GET_USENSOR:
	case AP_GET_DEP_USENSORS:
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
	char resp[10000];
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
	case AP_PLACE_WB:
		//get the double vector
		if ( mxIsDouble(prhs[3]) != 1)mexErrMsgTxt("location must be a vector of doubles.");
		dvalues = mxGetPr(prhs[3]);
		//  get the dimension of the row vector 
		if(mxGetM(prhs[3])!=1)mexErrMsgTxt("location must be row vector.");
		if(mxGetN(prhs[3])!=4)mexErrMsgTxt("position must be a 4-row vector.");
						
		size=ApoloMessage::writePlaceWheeledBase(message,world,name,dvalues);
		if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		else{
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
		case AP_MOVE_WB:
		//get the double vector
		if ( mxIsDouble(prhs[3]) != 1)mexErrMsgTxt("speeds and time must be a vector of doubles.");
		dvalues = mxGetPr(prhs[3]);
		//  get the dimension of the row vector 
		if(mxGetM(prhs[3])!=1)mexErrMsgTxt("speeds must be row vector.");
		if(mxGetN(prhs[3])!=3)mexErrMsgTxt("speeds and time must be a 3-row vector.");
						
		size=ApoloMessage::writeMoveWheeledBase(message,world,name,dvalues);
		if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		else {
			mxLogical val = 0;
			size = conection->Receive(resp, 100, 100);
			char *auxb = resp;
			ApoloMessage *m = ApoloMessage::getApoloMessage(&auxb, size);

			if (m) {
				//prepara vector de retorno
				if (m->getType() == AP_TRUE)val = 1;
				delete m;
			}
			plhs[0] = mxCreateLogicalScalar(val);
		}
		break;

	case AP_GETLOCATION:
	case AP_GETLOCATION_WB:
		if(command[0]==AP_GETLOCATION)
			size=ApoloMessage::writeGetLocation(message,world,name);
		else size=ApoloMessage::writeGetLocationWheeledBase(message,world,name);
		if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		else{
			
			size = 0;
			char *auxb = resp;
			ApoloMessage *m = waitForCompleteMessage(&auxb, 10000, size);
			createMexDoubleArrayFromMessage(m, plhs);
			
		}
		break;
	case AP_LINK_TO_ROBOT_TCP:
		//get the object name vector
		if(nrhs<4)mexErrMsgTxt(" target name parameter not present");
		if (( mxIsChar(prhs[3]) != 1)&&(mxGetM(prhs[3])!=1))mexErrMsgTxt("target name must be a string.");
		auxname=mxArrayToString(prhs[3]);
		if(auxname[0]==0){
			mxFree(auxname);
			auxname=0;
		}
		else{				
			size=ApoloMessage::writeLinkToRobotTCP(message,world,name,auxname);
			if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		}
		break;
	case AP_GET_LASER_DATA:
		size = ApoloMessage::writeGetLaserData(message, world, name);
		if (conection->Send(message, size)<size)mexErrMsgTxt(" Socket Bad Send");
		else {
			size = 0;
			char *auxb = resp;
			ApoloMessage *m = waitForCompleteMessage(&auxb, 10000, size);
			createMexDoubleArrayFromMessage(m, plhs);

		}
		break;
	case AP_GET_LASER_LM:
		size = ApoloMessage::writeGetLaserLandMarks(message, world, name);
		if (conection->Send(message, size)<size)mexErrMsgTxt(" Socket Bad Send");
		else {
			size = 0;
			char *auxb = resp;
			ApoloMessage *m = waitForCompleteMessage(&auxb, 10000, size);
			//aqu� ser�a la creaci�n de una matriz no un double array TODO
			createMexLandMarkInfoFromMessage(m, plhs);

		}
		break;
	case AP_GET_WB_ODOMETRY:
		size = ApoloMessage::writeGetOdometry(message, world, name);
		if (conection->Send(message, size)<size)mexErrMsgTxt(" Socket Bad Send");
		else {//resp
			size = 0;
			char *auxb = resp;
			ApoloMessage *m = waitForCompleteMessage(&auxb, 10000, size);
			createMexDoubleArrayFromMessage(m, plhs);
		}
		break;
	case AP_RESET_ODOMETRY:
		//get the double vector
		if (mxIsDouble(prhs[3]) != 1)mexErrMsgTxt("new odometry value must be a vector of doubles.");
		dvalues = mxGetPr(prhs[3]);
		//  get the dimension of the row vector 
		if (mxGetM(prhs[3]) != 1)mexErrMsgTxt("odometry must be row vector.");
		if (mxGetN(prhs[3]) != 3)mexErrMsgTxt("odometry must be a 3-row vector.");

		size = ApoloMessage::writeResetOdometry(message, world, name, dvalues);
		if (conection->Send(message, size)<size)mexErrMsgTxt(" Socket Bad Send");
		break;
	case AP_GET_USENSOR:
	case AP_GET_DEP_USENSORS:
		if (command[0] == AP_GET_USENSOR)size = ApoloMessage::writeGetUltrasonicSensor(message, world, name);
		else size = ApoloMessage::writeGetDependentUltrasonicSensors(message, world, name);

		if (conection->Send(message, size)<size)mexErrMsgTxt(" Socket Bad Send");
		else {
			size = 0;
			char *auxb = resp;
			ApoloMessage *m = waitForCompleteMessage(&auxb, 10000, size);
			createMexDoubleArrayFromMessage(m, plhs);

		}
		break;
	//commands with world only
	case AP_UPDATEWORLD:
		//ApoloUpdate
		size=ApoloMessage::writeUpdateWorld(message,world);
		if(conection->Send(message,size)<size)mexErrMsgTxt(" Socket Bad Send");
		break;
	}


	


	if(command)mxFree(command);
	if(name)mxFree(name);
	if(auxname)mxFree(auxname);
	if(world)mxFree(world);
}




