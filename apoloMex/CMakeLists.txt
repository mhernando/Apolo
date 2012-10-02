# Indicamos el nombre que queremos que tenga nuestro proyecto
PROJECT(apoloMex)

# Si estamos en Windows, buscamos en el registro donde se encuentran instalado Matlab
# Dependiendo de la versión de Matlab de que dispongamos tendremos que modificar el 
# numero de versión (en este archivo está buscando solamente las versiones 7.7, 7.8 y 7.11
# tanto en 32 como en 64 bits)
IF(WIN32)	# Windows
	
	SET(MATLAB_LIB [HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.11;MATLABROOT]/extern/lib/win64/microsoft 
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.8;MATLABROOT]/extern/lib/win64/microsoft
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.7;MATLABROOT]/extern/lib/win64/microsoft
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.6;MATLABROOT]/extern/lib/win64/microsoft 
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.11;MATLABROOT]/extern/lib/win32/microsoft
    		[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.8;MATLABROOT]/extern/lib/win32/microsoft
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.7;MATLABROOT]/extern/lib/win32/microsoft
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.6;MATLABROOT]/extern/lib/win32/microsoft)
	SET(MATLAB_INC "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.11;MATLABROOT]/extern/include"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.8;MATLABROOT]/extern/include"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.7;MATLABROOT]/extern/include"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.6;MATLABROOT]/extern/include")

	FIND_LIBRARY(MATLAB_MEX_LIBRARY
    		libmex
		${MATLAB_LIB}
    	)
  	FIND_LIBRARY(MATLAB_MX_LIBRARY
    		libmx
    		${MATLAB_LIB}
    	)
  	FIND_LIBRARY(MATLAB_ENG_LIBRARY
    		libeng
    		${MATLAB_LIB}
    	)

	FIND_PATH(MATLAB_INCLUDE
		"mex.h"
		${MATLAB_INC}
	)

# Si estamos en Linux, ponemos directamente el directorio donde se encuentra instalado Matlab
# (en este caso esta buscando en /home/usuario/Matlab y en /opt/Matlab)
ELSE(WIN32)	# Linux

	SET(MATLAB_LIB $ENV{HOME}/Matlab/bin/glnxa64 
		/opt/Matlab/bin/glnxa64
		$ENV{HOME}/Matlab/bin/glnx86
		/opt/Matlab/bin/glnx86)
	SET(MATLAB_INC "$ENV{HOME}/Matlab/extern/include/"
		"/opt/Matlab/extern/include/")

	FIND_LIBRARY(MATLAB_MEX_LIBRARY
    		mex
		${MATLAB_LIB}
    	)
  	FIND_LIBRARY(MATLAB_MX_LIBRARY
    		mx
    		${MATLAB_LIB}
    	)
  	FIND_LIBRARY(MATLAB_ENG_LIBRARY
    		eng
    		${MATLAB_LIB}
    	)

	FIND_PATH(MATLAB_INCLUDE
		"mex.h"
		${MATLAB_INC}
	)

ENDIF(WIN32)

# Incluimos al proyecto los ficheros .h que definen las funciones de interfaz entre Matlab y C++
# (Básicamente el fichero mex.h)
INCLUDE_DIRECTORIES(${MATLAB_INCLUDE})

# Indicamos que el proyecto va a crear una librería
# e indicamos los archivos que van a dar lugar a la libreria
# Deben estar creados antes de ejecutar CMake
ADD_LIBRARY(apoloMex MODULE

	apoloMex.cpp
	apoloMex.h
	../apoloMessage.cpp
	../apoloMessage.h
	
)

# Indicamos la extensión que debe tener la librería en función de la arquitectura y del S.O. en que la compilemos
IF(WIN32)	# Windows
	IF(CMAKE_SIZEOF_VOID_P EQUAL 4)		# 32 bits
    		SET_TARGET_PROPERTIES(apoloMex PROPERTIES SUFFIX ".mexw32")

	ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)	# 64 bits
    		SET_TARGET_PROPERTIES(apoloMex PROPERTIES SUFFIX ".mexw64")
  	ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)

ELSE(WIN32)	# Linux

	IF(CMAKE_SIZEOF_VOID_P EQUAL 4)		# Linux x86
    		SET_TARGET_PROPERTIES(apoloMex PROPERTIES SUFFIX ".mexglx" PREFIX "")

	ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)	# Linux x64
    		SET_TARGET_PROPERTIES(apoloMex PROPERTIES SUFFIX ".mexa64" PREFIX "")
  	ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
		
ENDIF(WIN32)

# Indicamos que la libreria creada va a ser de tipo MEX
ADD_DEFINITIONS(-DMATLAB_MEX_FILE)
IF(WIN32)
	SET_TARGET_PROPERTIES(apoloMex
		PROPERTIES
		LINK_FLAGS "/export:mexFunction"
	)
ENDIF(WIN32)

# Añadimos al proyecto las librerias interfaz entre Matlab y C++
SET(MATLAB_LIBRARIES
  ${MATLAB_MEX_LIBRARY}
  ${MATLAB_MX_LIBRARY}
  ${MATLAB_ENG_LIBRARY}
)

IF(WIN32)	
	TARGET_LINK_LIBRARIES(apoloMex debug mrcore_d ${MATLAB_LIBRARIES})	
	TARGET_LINK_LIBRARIES(apoloMex optimized mrcore ${MATLAB_LIBRARIES})
ELSE(WIN32) #LINUX		
	TARGET_LINK_LIBRARIES(apoloMex mrcore ${MATLAB_LIBRARIES})
ENDIF(WIN32)


CMAKE_MINIMUM_REQUIRED(VERSION 2.6)