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

# Define variable for linked libraries
set(PROJECT_LIBRARIES)


# OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
  set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)

# GLEW
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/external/glew/src" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include")
add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)

# GLM
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/external/glm")

# lodepng
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng")


################################# QT #################################
# find QT packages
set(CMAKE_PREFIX_PATH C:/Qt/5.11.2/msvc2015_64/lib/cmake)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
set(CMAKE_AUTOMOC ON)
find_package(OpenGL)

################################# LibQGLViewer #######################
include_directories(C:/Qt/5.11.2/msvc2017_64/include/QtXml)
SET(LIBQGLVIEWER_INSTALL_DIR D:/Documents/dev/libraries/libQGLViewer-2.7.1)
include_directories(${LIBQGLVIEWER_INSTALL_DIR})
link_directories(${LIBQGLVIEWER_INSTALL_DIR}/QGLViewer)
SET(LIBQGLVIEWER_LIBS_PATHS QGLViewer2.lib)
SET(PROJECT_LIBRARIES ${SOFA_LIBS_PATHS}  ${BOOST_LIBS} ${LIBQGLVIEWER_LIBS_PATHS})

################################## OpenMesh ##########################
add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DNOMINMAX)
include_directories ( D:/Documents/dev/libraries/OpenMesh-7.1/src)
link_directories(D:/Documents/dev/libraries/OpenMesh-7.1/build/Build/lib)


SET(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} ${LIBQGLVIEWER_LIBS_PATHS})###
	
#add_executable(soft_tissues_3  ${SRCS} ${HEADERS})
# Add executable for project
add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS} ${PROJECT_SRCS})

qt5_use_modules(Mesh_viewer Widgets OpenGL)

target_link_libraries(Mesh_viewer OpenMeshCore OpenMeshTools ${PROJECT_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

