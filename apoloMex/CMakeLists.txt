
# Indicamos el nombre que queremos que tenga nuestro proyecto
PROJECT(apoloMex)

INCLUDE("findMatlab.cmake")
MESSAGE("SEARCHING MATLAB *******************************************************************")
If(MATLAB_FOUND)
MESSAGE("MATLAB FOUND!!********************************************************************")
INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR})
if(TARGET miguel_mrcore)
get_property(apolo_inc_dirs TARGET miguel_mrcore PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
INCLUDE_DIRECTORIES(${apolo_inc_dirs})
get_property(apolo_inc_dirs2 TARGET ${BII_apolo_TARGET} PROPERTY INCLUDE_DIRECTORIES)
INCLUDE_DIRECTORIES(${apolo_inc_dirs2})
message("APOLO_INC_DIRS: ${apolo_inc_dirs}")
endif()
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
IF(NOT BICODE)
	IF(WIN32)	
		TARGET_LINK_LIBRARIES(apoloMex debug mrcore_d ${MATLAB_LIBRARIES})	
		TARGET_LINK_LIBRARIES(apoloMex optimized mrcore ${MATLAB_LIBRARIES})
	ELSE(WIN32) #LINUX		
		TARGET_LINK_LIBRARIES(apoloMex mrcore ${MATLAB_LIBRARIES})
	ENDIF(WIN32)
ELSE()
		TARGET_LINK_LIBRARIES(apoloMex miguel_mrcore)
ENDIF()
ENDIF()