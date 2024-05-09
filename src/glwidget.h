/*********************************************************************************************************************
 *
 * glwidget.h
 *
 * Qt widget with OpenGL context
 *
 * Mesh_viewer
 * Ludovic Blache
 *
 *********************************************************************************************************************/


#ifndef GLWIDGET_H
#define GLWIDGET_H

#define QT_NO_OPENGL_ES_2
#include <GL/glew.h>

#include "drawablemesh.h"
#include "trimeshsoup.h"

#include "QGLtoolkit/camera.h"


#include <QOpenGLWidget>
#include <QMouseEvent>

// based on Qt Textures Example
// https://doc.qt.io/qt-6/qtopengl-textures-example.html


class GLWidget : public QOpenGLWidget//, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;

    GLWidget(QWidget* parent);
    GLWidget();
    ~GLWidget();

    void loadTriMeshSoup(QString _fileName);

signals:
    void clicked();

protected:

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:

    void updateScene();

    QPoint lastPos;


    /*------------------------------------------------------------------------------------------------------------+
    |                                                ATTRIBUTES                                                   |
    +-------------------------------------------------------------------------------------------------------------*/

    qgltoolkit::Camera m_camera;
    std::unique_ptr<Mesh> m_triMesh = nullptr;
    std::unique_ptr<DrawableMesh> m_drawMesh = nullptr;

    QColor m_backCol = Qt::black;
    glm::vec3 m_lightPos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_lightCol = { 1.0f, 1.0f, 1.0f };



public:

    /*------------------------------------------------------------------------------------------------------------+
    |                                                 OTHER METHODS                                               |
    +-------------------------------------------------------------------------------------------------------------*/


    /*!
    * \fn setShaderFiles
    * \brief load shader files
    */
    inline void setShaderFiles(std::string& _vertShaderFilename, std::string& _fragShaderFilename)
    {
        m_drawMesh->setProgram(_vertShaderFilename, _fragShaderFilename);
    }

    /*!
    * \fn setBackColor
    * \brief set Background color
    */
    inline void setBackColor(const QColor& _color)
    {
        m_backCol = _color;
        update();
    }

    /*!
    * \fn setWireColor
    * \brief set Wireframe color
    */
    inline void setWireColor(const QColor& _color)
    {
        m_drawMesh->setWireColor(_color.red(), _color.green(), _color.blue());
        update();
    }

    /*!
    * \fn setLightColor
    * \brief set Light color
    */
    inline void setLightColor(const QColor& _color)
    {
        m_lightCol = glm::vec3((float)_color.red() / 255.0f, (float)_color.green() / 255.0f, (float)_color.blue() / 255.0f);
        update();
    }

    /*!
    * \fn setAmbientColor
    * \brief set Ambient color
    */
    inline void setAmbientColor(const QColor& _color)
    {
        m_drawMesh->setAmbientColor(_color.red(), _color.green(), _color.blue());
        update();
    }

    /*!
    * \fn setDiffuseColor
    * \brief set Diffuse color
    */
    inline void setDiffuseColor(const QColor& _color)
    {
        m_drawMesh->setDiffuseColor(_color.red(), _color.green(), _color.blue());
        update();
    }

    /*!
    * \fn setSpecularColor
    * \brief set Specular color
    */
    inline void setSpecularColor(const QColor& _color)
    {
        m_drawMesh->setSpecularColor(_color.red(), _color.green(), _color.blue());
        update();
    }

    /*!
    * \fn setTex
    * \brief get texture file and send it to DrawableMesh
    */
    inline void setTex(const QString& _fileName)
    {
        if (!_fileName.isEmpty())
        {
            // Load texture
            m_drawMesh->loadTex(_fileName.toStdString());
        }
        else
            std::cerr << "[ERROR] Viewer::setTex(): filename empty" << std::endl;
    }

    /*!
    * \fn setNormalMap
    * \brief get normal map file and send it to DrawableMesh
    */
    inline void setNormalMap(const QString& _fileName)
    {
        if (!_fileName.isEmpty())
        {
            // Load normal map
            m_drawMesh->loadNormalMap(_fileName.toStdString());
        }
        else
            std::cerr << "[ERROR] Viewer::setNormalMap(): filename empty" << std::endl;
    }

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
        * \fn toggleShadingLines
        * \brief SLOT: activate/deactivate wireframe shading
        */
        void toggleShadingLines();
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
        * \fn toggleFlatShading
        * \brief SLOT: activate/deactivate flat shading
        */
        void toggleFlatShading();

        /*!
        * \fn changeSpecPow
        * \brief SLOT: change specular power
        * \param _specPow: new specular power
        */
        void changeSpecPow(const double _specPow);

        /*!
        * \fn toggleGammaCorrec
        * \brief SLOT: activate/deactivate gamma correction
        */
        void toggleGammaCorrec();

        /*!
        * \fn toggleTex
        * \brief SLOT: activate/deactivate texture mapping
        */
        void toggleTex();
        /*!
        * \fn toggleNormalMap
        * \brief SLOT: activate/deactivate normal mapping
        */
        void toggleNormalMap();

        /*!
        * \fn toggleMeshCol
        * \brief SLOT: activate/deactivate mesh color mapping
        */
        void toggleMeshCol();


        /*!
        * \fn duplVertices
        * \brief SLOT: duplicate vertices
        */
        void duplVertices();
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

        /*!
        * \fn compMeanCurv
        * \brief SLOT: compute mean curvature of mesh surface (TriMeshHE only)
        */
        void compMeanCurv();
        /*!
        * \fn compSurfVar
        * \brief SLOT: compute surface variation of the mesh (TriMeshHE only)
        */
        void compSurfVar();

};

#endif