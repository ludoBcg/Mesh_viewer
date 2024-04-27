# check CMAKE version
cmake_minimum_required(VERSION 3.0)

# create project
project(Mesh_viewer)

# add files
set(SRCS
	src/main.cpp
	src/viewer.cpp
	src/window.cpp
	src/trimeshhe.cpp
	src/trimeshsoup.cpp
	src/drawablemesh.cpp
    )
    
set(HEADERS
	src/viewer.h
	src/window.h
	src/mesh.h
	src/trimeshhe.h
	src/trimeshsoup.h
	src/drawablemesh.h
    )
	

	
################################# EXTERNAL LIBS ######################

# Add include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src" )

# QT 
# OpenSource version 6.7.0 (used binaries installer for windows)
# https://www.qt.io/download-open-source
set(CMAKE_PREFIX_PATH C:/Qt/6.7.0/msvc2019_64/lib/cmake)
find_package(Qt6Widgets)
find_package(Qt6OpenGLWidgets)
set(CMAKE_AUTOMOC ON)

SET(QT_LIBRARIES Qt6::Core
				 Qt6::Gui
				 Qt6::OpenGL 
				 Qt6::Widgets 
				 Qt6::OpenGLWidgets )

# OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
endif(OPENGL_FOUND)


# GLEW (download binaries for windows)
set(GLEW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0")
include_directories(${GLEW_DIR}/include)
link_directories(${GLEW_DIR}/lib/Release/x64)
# use glew32s only
SET(GLEW_LIBS glew32s.lib)
add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)

	
# GLM (Header only!)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/external/glm")

	


# Add executable for project
add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${GLEW_LIBS} ${OPENGL_LIBRARIES} )

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
