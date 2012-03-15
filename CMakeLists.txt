cmake_minimum_required (VERSION 2.6)

FIND_PACKAGE(wxWidgets COMPONENTS base core gl net aui )
IF(wxWidgets_FOUND)
	INCLUDE(${wxWidgets_USE_FILE})

	IF(WIN32)
		INCLUDE_DIRECTORIES(C:/usr/include/mrcore) 
		LINK_DIRECTORIES(C:/usr/lib)
	ELSE(WIN32)
		INCLUDE_DIRECTORIES(/usr/local/include/mrcore)
		LINK_DIRECTORIES(/usr/local/lib)
	ENDIF(WIN32)

	ADD_EXECUTABLE(APOLO canvas.cpp childView.cpp genericSliderProperties.cpp mainWindow.cpp genericSlider.cpp apolo.cpp wheeledBasePanel.cpp nodeTree.cpp changeLocationCtrl.cpp robotSimPanel.cpp simulatedWorld.cpp tree.cpp converter.cpp)
	IF(WIN32)
		SET_TARGET_PROPERTIES(APOLO PROPERTIES
				COMPILE_FLAGS "${wxWidgets_CXX_FLAGS} ${wxWidgets_DEFINITIONS}"
				LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	ENDIF(WIN32)

	IF(WIN32)
    		TARGET_LINK_LIBRARIES(APOLO  mrcore_d ${wxWidgets_LIBRARIES})
    	ELSE(WIN32)
		TARGET_LINK_LIBRARIES(APOLO  mrcores GL GLU ${wxWidgets_LIBRARIES})
	ENDIF(WIN32)
    	
ENDIF(wxWidgets_FOUND)
