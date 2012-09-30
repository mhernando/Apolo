cmake_minimum_required (VERSION 2.6)

FIND_PACKAGE(wxWidgets COMPONENTS base core gl net aui adv)
IF(wxWidgets_FOUND)
	INCLUDE(${wxWidgets_USE_FILE})

	IF(WIN32)
		INCLUDE_DIRECTORIES(C:/mrcore/include/mrcore) 
		LINK_DIRECTORIES(C:/mrcore/lib)
	ELSE(WIN32)
		INCLUDE_DIRECTORIES(/usr/local/mrcore/include/mrcore)
		LINK_DIRECTORIES(/usr/local/mrcore/lib)
	ENDIF(WIN32)

	ADD_EXECUTABLE(APOLO canvas.cpp childView.cpp genericSliderProperties.cpp mainWindow.cpp genericSlider.cpp apolo.cpp 
	wheeledBasePanel.cpp nodeTree.cpp changeLocationCtrl.cpp robotSimPanel.cpp simulatedWorld.cpp tree.cpp converter.cpp 
	initialProperties.cpp positionableWidget.cpp designWidget.cpp objectSelection.cpp designProperties.cpp faceWindow.cpp pointsList.cpp 
	faceWidget.cpp robotConnection.cpp connectionLog.cpp connectionParameters.cpp apoloPort.cpp apoloMessage.cpp)


	IF(WIN32)
#		SET_TARGET_PROPERTIES(APOLO PROPERTIES
#				COMPILE_FLAGS "${wxWidgets_CXX_FLAGS} ${wxWidgets_DEFINITIONS}"
#				LINK_FLAGS "/SUBSYSTEM:WINDOWS")


		SET_TARGET_PROPERTIES(APOLO PROPERTIES
				COMPILE_FLAGS "/D _CRT_SECURE_NO_WARNINGS"
				LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	ENDIF(WIN32)

	IF(WIN32)	
		TARGET_LINK_LIBRARIES(APOLO debug mrcore_d ${wxWidgets_LIBRARIES})	
		TARGET_LINK_LIBRARIES(APOLO optimized mrcore ${wxWidgets_LIBRARIES})
	ELSE(WIN32) #LINUX		
		TARGET_LINK_LIBRARIES(APOLO mrcore ${OPENGL_LIBRARIES} ${wxWidgets_LIBRARIES})
	ENDIF(WIN32)
    	
ENDIF(wxWidgets_FOUND)

	ADD_EXECUTABLE(DEMO_APOLO_PORT demoApoloPort/demoApoloPort.cpp apoloMessage.cpp) 
	IF(WIN32)	
		TARGET_LINK_LIBRARIES(DEMO_APOLO_PORT debug mrcore_d )	
		TARGET_LINK_LIBRARIES(DEMO_APOLO_PORT optimized mrcore )
	ELSE(WIN32) #LINUX		
		TARGET_LINK_LIBRARIES(DEMO_APOLO_PORT mrcore ${OPENGL_LIBRARIES} ${wxWidgets_LIBRARIES})
	ENDIF(WIN32)
 
