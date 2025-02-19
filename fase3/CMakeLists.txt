cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(CG_Fase_3)

set(OpenGL_GL_PREFERENCE "GLVND")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(generator 
    Generator/main.cpp
	Generator/plane.cpp
    Utils/point.cpp
	Utils/point.h
	Utils/MatrixOperations.cpp
	Utils/MatrixOperations.h
	Generator/primitive.cpp
	Generator/headers/box.h
	Generator/box.cpp
	Generator/headers/sphere.h
	Generator/sphere.cpp
	Generator/cone.cpp
	Generator/headers/cone.h
	Generator/torus.cpp
	Generator/headers/torus.h
	Generator/patches.cpp
	Generator/headers/patches.h
)

add_executable(engine 
    Engine/main.cpp
	Engine/tinyxml2.cpp
	Engine/headers/tinyxml2.h
    Utils/point.cpp
	Utils/point.h
	Utils/MatrixOperations.cpp
	Utils/MatrixOperations.h
	Engine/Model.cpp
	Engine/headers/Model.h
	Engine/headers/Transformation.h
	Engine/Transformation.cpp
	Engine/headers/Group.h
	Engine/Group.cpp
)

# Encontre GLEW
find_package(GLEW REQUIRED)

# Encontre o OpenGL
find_package(OpenGL REQUIRED)

# Inclua as diretivas de inclusão e definição necessárias
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
add_definitions(${OPENGL_DEFINITIONS})

# Verificar se o OpenGL foi encontrado
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if(WIN32)
    message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    
    # Verifique a existência de GLUT
    if(NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(ERROR ": GLUT not found")
    endif(NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")   
    
    include_directories(${TOOLKITS_FOLDER}/glut)

    # Vincule as bibliotecas necessárias
    target_link_libraries(generator ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib ${GLEW_LIBRARIES})
    target_link_libraries(engine ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib ${GLEW_LIBRARIES})

    if(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")    
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

    # Encontre o GLUT
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})

    # Vincule as bibliotecas necessárias
    target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
    target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
    
    # Verifique se o GLUT foi encontrado
    if(NOT GLUT_FOUND)
       message(ERROR ": GLUT not found!")
    endif(NOT GLUT_FOUND)
endif(WIN32)
