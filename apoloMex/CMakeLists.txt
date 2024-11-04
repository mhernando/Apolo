 
# Indicamos el nombre que queremos que tenga nuestro proyecto
PROJECT(apoloMex)

# Lo ideal es tener instalada la ultima versión de CMake para asegurar que se detecta la 
#instalación de Matlab automáticamente. Pero si es una versión mas antigua que la de Matlab
#siempre es posible agregarla a mano a traves de esta variable. Basta con poner versión 
#y el número identificativo para que el findpackage lo encuentre de manera automática
set(MATLAB_ADDITIONAL_VERSIONS
	"R2024b=24.2"
	"R2023b=23.2"
	"R2023a=9.14"
	"R2022b=9.13"
	"R2022a=9.12"
    "R2018a=9.4"
    "R2017b=9.3"
  )
FIND_PACKAGE(MATLAB COMPONENTS MX_LIBRARY)
matlab_get_all_valid_matlab_roots_from_registry(FALSE
    matlab_versions
    )
IF(WIN32)
		INCLUDE_DIRECTORIES(C:/mrcore/include) 
		LINK_DIRECTORIES(C:/mrcore/lib)
ELSE(WIN32)
		INCLUDE_DIRECTORIES(/usr/local/mrcore/include)
		LINK_DIRECTORIES(/usr/local/mrcore/lib)
ENDIF(WIN32)	


MESSAGE("SEARCHING MATLAB *******************************************************************")
If(MATLAB_FOUND)
MESSAGE("MATLAB FOUND!!********************************************************************")

matlab_add_mex(NAME apoloMex MODULE
	SRC apoloMex.cpp apoloMex.h	../apoloMessage.cpp	../apoloMessage.h
    )


IF(WIN32)	
	TARGET_LINK_LIBRARIES(apoloMex debug mrcore_d )	
	TARGET_LINK_LIBRARIES(apoloMex optimized mrcore)
ELSE(WIN32) #LINUX		
	TARGET_LINK_LIBRARIES(apoloMex mrcore)
ENDIF(WIN32)
ENDIF(MATLAB_FOUND)
