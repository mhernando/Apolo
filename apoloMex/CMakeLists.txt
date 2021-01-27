 
# Indicamos el nombre que queremos que tenga nuestro proyecto
PROJECT(apoloMex)
set(MATLAB_ADDITIONAL_VERSIONS
    "R2018a=9.4"
    "R2017b=9.3"
  )
FIND_PACKAGE(MATLAB COMPONENTS MX_LIBRARY)
matlab_get_all_valid_matlab_roots_from_registry(FALSE
    matlab_versions
    )

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
