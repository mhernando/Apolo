
#include  <windows.h>
#include <winsock.h>
#include <stdio.h>
#include "mex.h"


class Conexion
{

	
	SOCKET s;
	SOCKET ns;
	SOCKADDR_IN saun, fsaun;
public:
	Conexion();
	void send(const char *mens,int size);
	void connect(char *direcc,short puerto);
	void close();
	int receive(const char *mens, int max);
	~Conexion();

};

Conexion::Conexion()
{
	s=-1;
	ns=-1;
}
Conexion::~Conexion()
{
	close();
}




void Conexion::connect(char *direcc, short puerto)
{
// si no esta en modo servidor o ya conectado, entonces... intenta
//	unsigned long hostaddr;
	int optval = 0;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err, len;
    char nombre[200];
	struct hostent *direccion;
	int miError;

	miError=0;

	// verifico valores de Socket

	if(s!=-1)return;
	if(ns!=-1)return;

	// Verificacion de la version
	
	wVersionRequested = MAKEWORD(1,1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		
		miError=1;
		//"No se encuentra winsock.dll"
	}
	if (LOBYTE (wsaData.wVersion) != 1 || HIBYTE (wsaData.wVersion) != 1)
	{
	    miError=1;
		//"No se encuentra winsock.dll (2)"
		WSACleanup();
	}
	

	
	// Creacion del socket
	
	if ((ns = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		miError=1;
		exit(1);
	}
	saun.sin_family = AF_INET;
	// Configuracion del socket 
	gethostname(nombre,200);
	direccion=gethostbyname(nombre);
	saun.sin_addr.s_addr = inet_addr(direcc);
	saun.sin_port = htons(puerto);
	len = sizeof(saun);
	if(miError)mexErrMsgTxt("Error en la apertura 3");
	if (connect(ns,(const struct sockaddr *) &saun,len)<0) {
		int val=WSAGetLastError();
		if(verbose){
		char mensaje[100];
		sprintf(mensaje,"mensaje error:%d",val);
		mexErrMsgTxt(mensaje);
		}
		miError=1;
		cierraCliente();
		mexErrMsgTxt("Error while connecting to Apolo");
	}
	
  if(miError)mexErrMsgTxt("Error en la apertura 4");
}

void Conexion::close()
{

	if(ns!=-1)closesocket(ns);
	if(s!=-1)closesocket(s);	
	s=-1;
	ns=-1;
	WSACleanup ();

	
}

int Conexion::send(const char *mens,int num)
{
	int res;
	if(ns<0)return;
	res = send(ns, mens, tama, 0);
	return res;
}


int Conexion::receive(char *mens, int max)
{
	
	int num=recv(ns, mens, max, 0); 	
			
	if (num == SOCKET_ERROR){
		mexErrMsgTxt(" "Error de lectura del socket");
		return num;	
		}
	return num;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/












#ifdef __cplusplus
extern "C" { // use the C fcn-call standard for all functions  
#endif       // defined within this scope                     


/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */


#define MODO_ARG  (ssGetSFcnParam(S, 0))
#define NOMBRE_ARG  (ssGetSFcnParam(S, 1))
#define NUM_IO_ARG  (ssGetSFcnParam(S, 2))


#define IS_STRING(pVal) (mxIsChar(pVal))
#define IS_ENTERO(pVal) (mxIsNumeric(pVal)/*&& !mxIsDouble(pVal)&& !mxIsSingle(pVal)*/)
#define OBTEN_CONEXION Conexion *conexion = (Conexion *) ssGetPWork(S)[0]
/*====================*
 * S-function methods *
 *====================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS)  && defined(MATLAB_MEX_FILE)

static void mdlCheckParameters(SimStruct *S)
{

    const mxArray *pVal0 = MODO_ARG;
	const mxArray *pVal1 = NOMBRE_ARG;
	const mxArray *pVal2 = NUM_IO_ARG;
	int mod;
	int io;
	if ( !IS_ENTERO(pVal0)) 
		{
        ssSetErrorStatus(S, "El primer parámetro debe ser entero");
        return;
		} 
    if ( !IS_STRING(pVal1)) 
		{
        ssSetErrorStatus(S, "El segundo parámetro debe ser una cadena");
        return;
		} 
	if ( !IS_ENTERO(pVal0)) 
		{
        ssSetErrorStatus(S, "El tercer parámetro debe ser entero");
        return;
		} 
	mod=(int)mxGetScalar(MODO_ARG);
	io=(int)mxGetScalar(NUM_IO_ARG);
	if(mod<0 || mod>3)
		{
		ssSetErrorStatus(S, "El primer parámetro debe ser 0,1,2,3");
        return;
		}
	if(io<1)
		{
		ssSetErrorStatus(S, "El tercer parámetro debe ser mayor que cero");
        return;
		}
}
#endif


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    /* See sfuntmpl.doc for more details on the macros below */

    ssSetNumSFcnParams(S, 3);  /* Number of expected parameters */
#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) 
		{
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL)return;
		} 
	else 
		{
		ssSetErrorStatus(S, "Son necesarios 3 parámetros: modo (int), identificador (string), numero (int)");
        return; /* Parameter mismatch will be reported by Simulink */
		}
#endif
    int i;
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
	int mod=(int)mxGetScalar(MODO_ARG);
	int io=(int)mxGetScalar(NUM_IO_ARG);

	switch(mod)
	    {
	    case 3:
	        if (!ssSetNumInputPorts(S, 0)) return;
	       // ssSetInputPortWidth(S, 0, 1);
	        if (!ssSetNumOutputPorts(S, io)) return;
	        for(i=0;i<io;i++)ssSetOutputPortWidth(S, i, 1);
	    break;
	    case 2:
	    case 1:
	        if (!ssSetNumInputPorts(S, io)) return;
            if (!ssSetNumOutputPorts(S, io)) return;
        	for(i=0;i<io;i++)
		        {
		        ssSetOutputPortWidth(S, i, 1);
		        ssSetInputPortWidth(S, i, 1);
		        ssSetInputPortDirectFeedThrough(S, i, 1);
		        //ssSetInputPortDataType(S, i, dtype );
		        }
	    break;

	    }


    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1); // reserve element in the pointers vector
    ssSetNumModes(S, 0); // to store a C++ object
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, mxGetScalar(ssGetSFcnParam(S, 0)));
    ssSetOffsetTime(S, 0, 0.0);

}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   */
  static void mdlStart(SimStruct *S)
  {
	  Conexion *conexion;
	  
      ssGetPWork(S)[0] = (void *)(conexion= new Conexion); // store new C++ object in the
	  conexion->iniciaCliente("127.0.0.1",3000,S);
                                              // pointers vector
	  conexion->modo=(int)mxGetScalar(MODO_ARG);
	  conexion->num_io=(int)mxGetScalar(NUM_IO_ARG);
	  if(mxGetString(NOMBRE_ARG, conexion->nombre, LONG_NOMBRE))
		{
		  ssSetErrorStatus(S, "ERROR en el nombre");
		  return;
		}
	  conexion->entradas=new double[conexion->num_io];
	  conexion->salidas=new double[conexion->num_io];

  }
  
#endif /*  MDL_START */

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block. Generally outputs are placed in the output vector, ssGetY(S).
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    OBTEN_CONEXION;   // retrieve C++ object from
	char mens[400],aux[100];
	double val;
	int i;
    char *pos;
    real_T  *y;
    (conexion->numllamadas)++;
    switch(conexion->modo)
        {
        case 1: //control de posicion
        case 2: //control de velocidad
	        for (i = 0; i < conexion->num_io; i++) //lectura de las entradas
	            {
	            InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,i);
	            conexion->entradas[i]=(real_T)(*uPtrs[0]);
	            }    
	        //en el ejemplo sfunc_stype_param.c viene como tratar tipos variables

            //contruccion del mensaje de peticion al servidor
            if(conexion->modo==1)
	        sprintf(mens,"p %s %d ",conexion->nombre,conexion->num_io);
	        if(conexion->modo==2)
	        sprintf(mens,"v %s %d ",conexion->nombre,conexion->num_io);
  	        for(i=0;i<conexion->num_io;i++)
	        	{
	        	sprintf(aux,"* %.2f ",conexion->entradas[i]);
	        	strcat(mens,aux);
	        	}
            //peticion al servidor     	
            conexion->enviar(mens,S);
            //lectura de la respuesta
            conexion->recibir(mens,S);
  
            //interpretacion
            
	        pos=mens;
  	        for(i=0;i<conexion->num_io;i++)
		        {
		        while((pos[0]!='*')&&(pos[0]!='\0'))pos++;
		        if(pos[0]=='*')
			        {
			        sscanf(++pos,"%lf",&(val));
			        (conexion->salidas)[i]=val;
			        }
		        }
            //asignacion a las salidas
	       
	        for(i=0;i<conexion->num_io;i++)
		        {
		        y= ssGetOutputPortRealSignal(S,i); 
		        y[0]=(conexion->salidas)[i];
		        }
		    //fin de control de posicion y velocidad
		break; 
		case 3: //info de sensores
		    //mensaje de peticion
		    sprintf(mens,"s %s %d *******",conexion->nombre,conexion->num_io);
		    //peticion al servidor     	
            conexion->enviar(mens,S);
            //lectura de la respuesta
            conexion->recibir(mens,S);
            //interpretacion
            
	        pos=mens;
  	        for(i=0;i<conexion->num_io;i++)
		        {
		        while((pos[0]!='*')&&(pos[0]!='\0'))pos++;
		        if(pos[0]=='*')
			        {
			        sscanf(++pos,"%lf",&(val));
			        (conexion->salidas)[i]=val;
			        }
		        }  
            //asignacion a las salidas
	        
	        for(i=0;i<conexion->num_io;i++)
		        {

		        y= ssGetOutputPortRealSignal(S,i); 
		        y[0]=(conexion->salidas)[i];
		        }
		    //fin de entrada de sensores
		break;
		}
}
/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
    OBTEN_CONEXION; // retrieve and destroy C++
 
	conexion->cierraCliente();
	delete conexion;                           // object in the termination
}                                              // function
/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

#ifdef __cplusplus
} // end of extern "C" scope
#endif

