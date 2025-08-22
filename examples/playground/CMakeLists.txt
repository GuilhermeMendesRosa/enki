if (SDL2_FOUND)
	message(STATUS "SDL2 found, enabling joystick support")
	include_directories(${SDL2_INCLUDE_DIR})
	set(EXTRA_LIBS ${SDL2_LIBRARY} ${EXTRA_LIBS})
	add_definitions("-DUSE_SDL")
endif ()

if (Qt5Widgets_FOUND AND Qt5OpenGL_FOUND)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTORCC ON)
	
	set(viewer_playground_SRCS Playground.cpp)
	
	include_directories(${PROJECT_SOURCE_DIR})
	
	# Configurar propriedades específicas do MOC para este target
	add_executable(enkiplayground ${viewer_playground_SRCS})
	
	# Garantir que o MOC seja processado corretamente
	set_target_properties(enkiplayground PROPERTIES
		AUTOMOC ON
		AUTOMOC_MOC_OPTIONS ""
	)
	
	target_link_libraries(enkiplayground enkiviewer enki ${OPENGL_gl_LIBRARY} ${EXTRA_LIBS} Qt5::OpenGL Qt5::Widgets)
endif ()