# Mesh_viewer

A simple OpenGL viewer, for quick and handy mesh visualization.  

![img1](img1.png)


Uses [Qt](https://www.qt.io/) for GUI, and [OpenMesh](https://www.graphics.rwth-aachen.de/software/openmesh/) for winged-edge data structure. 


## Prerequisite

This project has been developed on Windows, using the following setup:

* [VisualStudio 2022](https://visualstudio.microsoft.com) (community edition)
* Qt 6.7.0, using the [Qt open source](https://www.qt.io/download-open-source) installer for Windows
* [CMake]( https://cmake.org/ )


**QGL_toolkit** and other libraries are provided in the [libs](https://github.com/ludoBcg/libs) repository. 

External dependencies used for this project are:

* [GLEW (The OpenGL Extension Wrangler Library)](http://glew.sourceforge.net/)
  
* [GLM (OpenGL Mathematics)](https://github.com/g-truc/glm)

* [OpenMesh](https://www.graphics.rwth-aachen.de/software/openmesh)

* [Eigen](https://gitlab.com/libeigen/eigen)
 


## Compilation

1. Use Cmake to generate a VS solution, using the CMakeLists provided.

2. Open the solution, then set the *Mesh_viewer* project as startup project (right-click on the project in the solution explorer panel).
   In the project properties, go to *Configuration properties -> Debugging* and set the *Working directory* to *.\Release* or *.\Debug*, depending on your current configuration.
   
3. Workaround for the *Could not find the Qt platform plugin "windows"* error for VisualStudio project:
   Go to *C:\path\to\Qt\6.7.0\msvc2019_64\plugins* and copy the *platform* folder into your executable folder (same for the dlls located in *C:\path\to\Qt\6.7.0\msvc2019_64\bin*) 


![img2](img2.png)


