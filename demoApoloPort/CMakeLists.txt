cmake_minimum_required (VERSION 2.6)

IF(WIN32)
		INCLUDE_DIRECTORIES(C:/TEMP/mrcore/include/mrcore) 
		LINK_DIRECTORIES(C:/mrcore/lib)
	ELSE(WIN32)
		INCLUDE_DIRECTORIES(/usr/local/mrcore/include/mrcore)
		LINK_DIRECTORIES(/usr/local/mrcore/lib)
	ENDIF(WIN32)


ADD_EXECUTABLE(DEMO_APOLO_PORT demoApoloPort.cpp) 


	IF(WIN32)	
		TARGET_LINK_LIBRARIES(DEMO_APOLO_PORT debug mrcore_d ${wxWidgets_LIBRARIES})	
		TARGET_LINK_LIBRARIES(DEMO_APOLO_PORT optimized mrcore ${wxWidgets_LIBRARIES})
	ELSE(WIN32) #LINUX		
		TARGET_LINK_LIBRARIES(DEMO_APOLO_PORT mrcore ${OPENGL_LIBRARIES} ${wxWidgets_LIBRARIES})
	ENDIF(WIN32)
    	