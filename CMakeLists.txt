# check CMAKE version
cmake_minimum_required(VERSION 3.0)

# create project
project(Mesh_viewer)

# C++ 20
set(CMAKE_CXX_STANDARD 20)

# add files
set(SRCS
	src/main.cpp
	src/glwidget.cpp
	src/window.cpp
	src/trimeshsoup.cpp
	src/trimeshhe.cpp
	src/drawablemesh.cpp
    )
    
set(HEADERS
	src/glwidget.h
	src/window.h
	src/mesh.h
	src/trimeshsoup.h
	src/trimeshhe.h
	src/drawablemesh.h
    )
	
	
# Add include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src" )
# Dependencies folder
set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs")
include_directories(SYSTEM "${LIBS_DIR}")


################################# EXTERNAL LIBS #################################


# OpenMP
find_package(OpenMP)


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
# flag for conditional compilation
add_compile_definitions(USE_OPENGL)


# GLEW (download binaries for windows)
set(GLEW_DIR "${LIBS_DIR}/third_party/glew-2.1.0")
include_directories(${GLEW_DIR}/include)
link_directories(${GLEW_DIR}/lib/Release/x64)
# use glew32s only
SET(GLEW_LIBS glew32s.lib)
add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)

	
# GLM (Header only!)
include_directories(SYSTEM "${LIBS_DIR}/third_party/glm-1.0.1")

# Eigen (Header only)
include_directories(SYSTEM "${LIBS_DIR}/third_party/eigen-3.4.0")

# OpenMesh
# build VS solution using provided Cmake script
# uncheck "BUILD_APPS" to avoid Qt5 search
add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DNOMINMAX)
set(OPENMESH_DIR "${LIBS_DIR}/third_party/OpenMesh-11.0.0")
include_directories ( ${OPENMESH_DIR}/src)
link_directories(${OPENMESH_DIR}/build/Build/lib)	



################################## VS SOLUTION ##################################

# Add executable for project
add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})

# link libraries (different openmesh libs for release and debug)
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX 
					  debug OpenMeshCored OpenMeshToolsd ${QT_LIBRARIES} ${GLEW_LIBS} ${OPENGL_LIBRARIES} 
					  optimized OpenMeshCore OpenMeshTools ${QT_LIBRARIES} ${GLEW_LIBS} ${OPENGL_LIBRARIES} )

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
