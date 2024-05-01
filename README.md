# Mesh_viewer

A simple OpenGL viewer, for quick and handy mesh visualization.  

![Mesh_viewer_fig_small](https://user-images.githubusercontent.com/84736834/124169237-4f03af80-daa6-11eb-8d80-12ca22b02764.png)

Uses [Qt](https://www.qt.io/) and [libQGLViewer](http://libqglviewer.com/) for GUI, and [OpenMesh](https://www.graphics.rwth-aachen.de/software/openmesh/) for winged edge data structure.  

Other external dependencies:
* [GLEW (The OpenGL Extension Wrangler Library)](http://glew.sourceforge.net/)
* [GLM (OpenGL Mathematics)](https://glm.g-truc.net/0.9.9/)
* [Eigen](https://eigen.tuxfamily.org/)
* [LodePNG](https://lodev.org/lodepng/)  
  https://github.com/lvandeve/lodepng


*Update 2024:*

Compatiblity with Qt 6.7.0 (WIP), using VisualStudio 2022 and the [Qt open source](https://www.qt.io/download-open-source) installer for Windows.

Workaround for the *Could not find the Qt platform plugin "windows"* error for VisualStudio project:
Got to *C:\path\to\Qt\6.7.0\msvc2019_64\plugins\* and copy the *platform* folder into your executable folder (same for the dlls located in *C:\path\to\Qt\6.7.0\msvc2019_64\bin*) 
