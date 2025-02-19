cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(CG_Fase_2)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(generator Generator/main.cpp
		Generator/plane.cpp
        Utils/point.cpp
		Utils/point.h
		Generator/primitive.cpp
		Generator/headers/box.h
		Generator/box.cpp
		Generator/headers/sphere.h
		Generator/sphere.cpp
		Generator/cone.cpp
		Generator/headers/cone.h
		Generator/torus.cpp
		Generator/headers/torus.h)

add_executable(engine Engine/main.cpp
		Engine/tinyxml2.cpp
		Engine/headers/tinyxml2.h
        Utils/point.cpp
		Utils/point.h
		Engine/Model.cpp
		Engine/headers/Model.h
		Engine/headers/Transformation.h
		Engine/Transformation.cpp
		Engine/headers/Group.h
		Engine/Group.cpp)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
	
	include_directories(${TOOLKITS_FOLDER}/glut )

	target_link_libraries(generator ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)
	target_link_libraries(engine ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)

	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )	

	
else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

	target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
	
endif(WIN32)








