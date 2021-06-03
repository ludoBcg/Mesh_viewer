
//#include <iostream>
//#include <cstdlib>
//#include <algorithm>

#define QT_NO_OPENGL_ES_2
#include <GL/glew.h>



#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedCameraFrame.h>

#include "drawablemesh.h"
#include "trimeshhe.h"
#include "trimeshsoup.h"


class Viewer : public QGLViewer
{
    Q_OBJECT

    public:

        /*------------------------------------------------------------------------------------------------------------+
        |                                        CONSTRUCTORS / DESTRUCTORS                                           |
        +-------------------------------------------------------------------------------------------------------------*/

        Viewer(QWidget *parent);
        Viewer();
        ~Viewer();


        /*------------------------------------------------------------------------------------------------------------+
        |                                                 OTHER METHODS                                               |
        +-------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn loadTriMeshHE
        * \brief load a TriMeshHE from a file
        */
        inline void loadTriMeshHE(QString _fileName)
        {
            if(!_fileName.isEmpty() )
            {
                // Load mesh
                m_triMesh = new TriMeshHE(true, false, false, false);
                m_triMesh->readFile( _fileName.toStdString() );
                m_drawMesh->updateVAO(m_triMesh);
                m_triMesh->computeAABB();
                updateScene();
            }
            else
                std::cerr << "ERROR: [Viewer::loadTriMeshHE()] filename empty" << std::endl;
        }

        /*!
        * \fn loadTriMeshSoup
        * \brief load a TriMeshSoup from a file
        */
        inline void loadTriMeshSoup(QString _fileName)
        {
            if(!_fileName.isEmpty() )
            {
                // Load mesh
                m_triMesh = new TriMeshSoup(true, false, false);
                m_triMesh->readFile( _fileName.toStdString() );
                m_drawMesh->updateVAO(m_triMesh);
                m_triMesh->computeAABB();
                updateScene();
            }
            else
                std::cerr << "ERROR: [Viewer::loadTriMeshSoup()] filename empty" << std::endl;
        }

        /*!
        * \fn saveMesh
        * \brief save the current Mesh into a file
        */
        inline void saveMesh(QString _fileName)
        {
            if(!_fileName.isEmpty() )
            {
                // Load mesh
                m_triMesh->writeFile( _fileName.toStdString() );
            }
            else
                std::cerr << "ERROR: [Viewer::saveMesh()] filename empty" << std::endl;
        }

        /*!
        * \fn setShaderFiles
        * \brief load shader files
        */
        inline void setShaderFiles(std::string _vertShaderFilename, std::string _fragShaderFilename)
        {
            m_drawMesh->setProgram(_vertShaderFilename, _fragShaderFilename);
        }

        /*!
        * \fn setBackColor
        * \brief set Background color
        */
        inline void setBackColor(QColor _color)
        {
            m_backCol = glm::vec3( (float)_color.red()/255.0f, (float)_color.green()/255.0f, (float)_color.blue()/255.0f );
            update();
        }

        /*!
        * \fn setWireColor
        * \brief set Wireframe color
        */
        inline void setWireColor(QColor _color)
        {
            m_drawMesh->setWireColor( _color.red(), _color.green(), _color.blue() );
            update();
        }

        /*!
        * \fn setLightColor
        * \brief set Light color
        */
        inline void setLightColor(QColor _color)
        {
            m_lightCol = glm::vec3( (float)_color.red()/255.0f, (float)_color.green()/255.0f, (float)_color.blue()/255.0f );
            update();
        }

        /*!
        * \fn setAmbientColor
        * \brief set Ambient color
        */
        inline void setAmbientColor(QColor _color)
        {
            m_drawMesh->setAmbientColor( _color.red(), _color.green(), _color.blue() );
            update();
        }

        /*!
        * \fn setDiffuseColor
        * \brief set Diffuse color
        */
        inline void setDiffuseColor(QColor _color)
        {
            m_drawMesh->setDiffuseColor( _color.red(), _color.green(), _color.blue() );
            update();
        }

        /*!
        * \fn setSpecularColor
        * \brief set Specular color
        */
        inline void setSpecularColor(QColor _color)
        {
            m_drawMesh->setSpecularColor( _color.red(), _color.green(), _color.blue() );
            update();
        }

        /*!
        * \fn setAlbedoTex
        * \brief get albedo texture file and send it to DrawableMesh
        */
        inline void setAlbedoTex(QString _fileName)
        {
            if(!_fileName.isEmpty() )
            {
                // Load albedo texture
                m_drawMesh->loadAlbedoTex( _fileName.toStdString() );
            }
            else
                std::cerr << "ERROR: [Viewer::setAlbedoTex()] filename empty" << std::endl; 
        }

        /*!
        * \fn setNormalMap
        * \brief get normal map file and send it to DrawableMesh
        */
        inline void setNormalMap(QString _fileName)
        {
            if(! _fileName.isEmpty() )
            {
                // Load normal map
                m_drawMesh->loadNormalMap( _fileName.toStdString() );
            }
            else
                std::cerr << "ERROR: [Viewer::setNormalMap()] filename empty" << std::endl;
        }

        /*!
        * \fn setMetalMap
        * \brief get metal map file and send it to DrawableMesh
        */
        inline void setMetalMap(QString _fileName)
        {
            if(! _fileName.isEmpty() )
            {
                // Load metal map
                m_drawMesh->loadMetalMap( _fileName.toStdString() );
            }
            else
                std::cerr << "ERROR: [Viewer::setMetalMap()] filename empty" << std::endl;
        }

        /*!
        * \fn setGlossMap
        * \brief get gloss map file and send it to DrawableMesh
        */
        inline void setGlossMap(QString _fileName)
        {
            if(! _fileName.isEmpty() )
            {
                // Load gloss map
                m_drawMesh->loadGlossMap( _fileName.toStdString() );
            }
            else
                std::cerr << "ERROR: [Viewer::setGlossMap()] filename empty" << std::endl;
        }

        /*!
        * \fn setAmbientMap
        * \brief get ambient map file and send it to DrawableMesh
        */
        inline void setAmbientMap(QString _fileName)
        {
            if(! _fileName.isEmpty() )
            {
                // Load gloss map
                m_drawMesh->loadAmbientMap( _fileName.toStdString() );
            }
            else
                std::cerr << "ERROR: [Viewer::setAmbientMap()] filename empty" << std::endl;
        }

        /*!
        * \fn setCubeMap
        * \brief get cube map folder and send it to DrawableMesh
        */
        inline void setCubeMap(QString _dirname)
        {
            if(! _dirname.isEmpty() )
            {
                // Load albedo texture
                m_drawMesh->loadCubeMap( _dirname.toStdString() );
            }
            else
                std::cerr << "ERROR: [Viewer::setCubeMap()] dirname empty" << std::endl; 
        }

        /*!
        * \fn lapSmooth
        * \brief Laplacian smoothing of the mesh
        * \param _nbIter: number of iterations
        * \param _factor: factor applied to the Laplacian vector
        */
        void lapSmooth(int _nbIter, float _factor);


    protected:

        /*------------------------------------------------------------------------------------------------------------+
        |                                                ATTRIBUTES                                                   |
        +-------------------------------------------------------------------------------------------------------------*/

        Mesh* m_triMesh;
        DrawableMesh* m_drawMesh;

        glm::vec3 m_backCol;
        glm::vec3 m_lightPos;
        glm::vec3 m_lightCol;

        /*------------------------------------------------------------------------------------------------------------+
        |                                               OVERRIDDEN METHODS                                            |
        +-------------------------------------------------------------------------------------------------------------*/

        virtual void init();
        virtual void draw();
        void closeEvent(QCloseEvent *e);
        virtual QString helpString() const;
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        void resizeGL(int width, int height);
        void keyPressEvent(QKeyEvent *e);


        /*------------------------------------------------------------------------------------------------------------+
        |                                                 OTHER METHODS                                               |
        +-------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn updateScene
        * \brief update scene and camera parameters according to displayed geometry
        */
        void updateScene();

        /*!
        * \fn lqglToGlmMat4
        * \brief transform 4*4 matrix from libGGLViewer format (i.e. GLdouble [16]) to glm format (i.e. glm::mat4)
        * \param mat : 4*4 matrix represented by a 1D array of 16 doubles
        * \return parameter matrix duplicated as glm::mat4
        */
        glm::mat4 lqglToGlmMat4(GLdouble mat[16]);


    public slots:

        /*------------------------------------------------------------------------------------------------------------+
        |                                                  SLOTS                                                      |
        +-------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn toggleRenderSurf
        * \brief SLOT: activate/deactivate surface rendering
        */
        void toggleRenderSurf(); 
        /*!
        * \fn toggleRenderLines
        * \brief SLOT: activate/deactivate wireframe rendering
        */
        void toggleRenderLines();
        /*!
        * \fn toggleAmbient
        * \brief SLOT: activate/deactivate ambient shading
        */
        void toggleAmbient();
        /*!
        * \fn toggleDiffuse
        * \brief SLOT: activate/deactivate ambient shading
        */
        void toggleDiffuse();
        /*!
        * \fn toggleSpecular
        * \brief SLOT: activate/deactivate ambient shading
        */
        void toggleSpecular();
        /*!
        * \fn toggleShowNormals
        * \brief SLOT: activate/deactivate rendering of normal vectors
        */
        void toggleShowNormals();

        /*!
        * \fn changeSpecPow
        * \brief SLOT: change specular power
        * \param _specPow: new specular power
        */
        void changeSpecPow(double _specPow);

        /*!
        * \fn toggleGammaCorrec
        * \brief SLOT: activate/deactivate gamma correction
        */
        void toggleGammaCorrec();

        /*!
        * \fn toggleAlbedoTex
        * \brief SLOT: activate/deactivate albedo texture mapping
        */
        void toggleAlbedoTex();
        /*!
        * \fn toggleNormalMap
        * \brief SLOT: activate/deactivate normal mapping
        */
        void toggleNormalMap();
        /*!
        * \fn togglePBRMap
        * \brief SLOT: activate/deactivate PBR mapping
        */
        void togglePBRMap();
        /*!
        * \fn toggleAmbientMap
        * \brief SLOT: activate/deactivate ambient mapping
        */
        void toggleAmbientMap();
        /*!
        * \fn toggleEnvMap
        * \brief SLOT: activate/deactivate environment mapping
        */
        void toggleEnvMap();

        /*!
        * \fn compNormals
        * \brief SLOT: recompute geometric normals
        */
        void compNormals();
        /*!
        * \fn compTBs
        * \brief SLOT: compute tangents and bitangents
        */
        void compTBs();

};