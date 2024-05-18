

#include "glwidget.h"




GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent)
    , m_triMesh(nullptr), m_drawMesh(nullptr)
{}

GLWidget::GLWidget() : QOpenGLWidget()
    , m_triMesh(nullptr), m_drawMesh(nullptr)
{}


GLWidget::~GLWidget()
{
    m_drawMesh = nullptr;
    m_triMesh = nullptr;
    std::cout << std::endl << "Bye!" << std::endl;
}


void GLWidget::initializeGL()
{

    // Load OpenGL functions
    glewExperimental = true;
    GLenum status = glewInit();
    if (status != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString(status) << std::endl;
        std::exit(EXIT_FAILURE);
    }


    std::cout << std::endl
        << "Welcome to Mesh_viewer" << std::endl << std::endl
        << "Press H for help" << std::endl
        << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
        << "Vendor: " << glGetString(GL_VENDOR) << std::endl << std::endl
        << "Log:" << std::endl;


    // Load default mesh 
    m_triMesh = std::make_unique<TriMeshSoup>();
    m_triMesh->readFile("../../models/armadillo.obj");
    m_triMesh->computeAABB();

    m_drawMesh = std::make_unique<DrawableMesh>();
    m_drawMesh->setProgram("../../src/shaders/phong.vert", "../../src/shaders/phong.frag");
    m_drawMesh->createVAO(m_triMesh.get());

    m_camera.setFieldOfView(glm::radians(45.0));

    // Setup scene and camera parameters
    updateScene();

    m_backCol = Qt::black;
    m_lightPos = glm::vec3(10.0f, 0.0f, 0.0f);
    m_lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
}

void GLWidget::paintGL()
{
    glClearColor(m_backCol.redF(), m_backCol.greenF(), m_backCol.blueF(), m_backCol.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST); // ensures that polygons overlap correctly

    glm::mat4 mv = m_camera.viewMatrix();
    glm::mat4 p = m_camera.projectionMatrix();
    glm::mat4 mvp = p * mv;

    m_lightPos = m_camera.position();
    m_drawMesh->draw(mv, mvp, m_lightPos, m_lightCol);

}
void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    m_camera.setScreenWidthAndHeight(width, height);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    //lastPos = event->position().toPoint();

    m_camera.mousePressEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent* const event)
{
    m_camera.wheelEvent(event);
    this->update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_camera.mouseMoveEvent(event);

    this->update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* /* event */)
{
    emit clicked();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent* event )
{
    m_camera.mouseDoubleClickEvent(event);

    this->update();
}


void GLWidget::loadTriMeshSoup(QString _fileName)
{
    if (!_fileName.isEmpty())
    {
        qInfo() << "[info] GLWidget::loadTriMeshSoup: Load " <<  _fileName.toStdString();
        // Load mesh
        m_triMesh = std::make_unique<TriMeshSoup>();
        m_triMesh->readFile(_fileName.toStdString());
        m_drawMesh->updateVAO(m_triMesh.get());
        m_drawMesh->setFlatShadingFlag(false);
        m_triMesh->computeAABB();
        updateScene();
        paintGL();
    }
    else
        qCritical() << "[ERROR] Viewer::loadTriMeshSoup: filename empty";
}


void GLWidget::loadTriMeshHE(QString _fileName)
{
    if (!_fileName.isEmpty())
    {
        qInfo() << "[info] GLWidget::loadTriMeshHE: Load " << _fileName.toStdString();
        // Load mesh
        m_triMesh = std::make_unique<TriMeshHE>(true, true, true, true);
        m_triMesh->readFile(_fileName.toStdString());
        m_drawMesh->updateVAO(m_triMesh.get());
        m_drawMesh->setFlatShadingFlag(false);
        m_triMesh->computeAABB();
        updateScene();
        paintGL();
    }
    else
        qCritical() << "[ERROR] Viewer::loadTriMeshSoup: filename empty";
}


void GLWidget::saveMesh(QString _fileName)
{
    if (!_fileName.isEmpty())
    {
        // Load mesh
        m_triMesh->writeFile(_fileName.toStdString());
        qInfo() << "[info] Viewer::saveMesh: saved to file " << _fileName.toStdString();
    }
    else
        qCritical() << "[ERROR] Viewer::saveMesh: filename empty";
}


void GLWidget::updateScene()
{

    // scene AABBox
    glm::vec3 bBoxMin = m_triMesh->getBBoxMin();
    glm::vec3 bBoxMax = m_triMesh->getBBoxMax();

    if (bBoxMin != bBoxMax)
    {
        // scene setup
        glm::vec3 min(bBoxMin.x, bBoxMin.y, bBoxMin.z);
        glm::vec3 max(bBoxMax.x, bBoxMax.y, bBoxMax.z);
        glm::vec3 centerCoords((bBoxMin.x + bBoxMax.x) * 0.5f, (bBoxMin.y + bBoxMax.y) * 0.5f, (bBoxMin.z + bBoxMax.z) * 0.5f);

        // Set scene radius and center from AABBox.
        // Pivot point is set to scene center by default, 
        m_camera.setSceneBoundingBox(min, max);

    }
}


void GLWidget::lapSmooth(int _nbIter, float _factor)
{
    m_triMesh->lapSmooth(_nbIter, _factor);
    m_drawMesh->updateVAO(m_triMesh.get());
    qInfo() << "[info] GLWidget::lapSmooth: Laplacian smoothing applied";
    update();
}



/*------------------------------------------------------------------------------------------------------------+
|                                                  SLOTS                                                      |
+-------------------------------------------------------------------------------------------------------------*/

void GLWidget::toggleRenderSurf()
{
    m_drawMesh->toggleShadedRenderFlag();
    update();
}

void GLWidget::toggleRenderLines()
{
    m_drawMesh->toggleWireframeRenderFlag();
    update();
}

void GLWidget::toggleShadingLines()
{
    m_drawMesh->toggleWireframeShadingFlag();
    update();
}

void GLWidget::toggleAmbient()
{
    // Reverse state of ambient flag
    m_drawMesh->setAmbientFlag(!m_drawMesh->getAmbientFlag());
    update();
}

void GLWidget::toggleDiffuse()
{
    // Reverse state of diffuse flag
    m_drawMesh->setDiffuseFlag(!m_drawMesh->getDiffuseFlag());
    update();
}

void GLWidget::toggleSpecular()
{
    // Reverse state of specular flag
    m_drawMesh->setSpecularFlag(!m_drawMesh->getSpecularFlag());
    update();
}

void GLWidget::toggleShowNormals()
{
    // Reverse state of normal vectors rendering flag
    m_drawMesh->setShowNormalFlag(!m_drawMesh->getShowNormalFlag());
    update();
}

void GLWidget::toggleFlatShading()
{
    // Reverse state of flat shading flag
    m_drawMesh->setFlatShadingFlag(!m_drawMesh->getFlatShadingFlag());
    update();
}

void GLWidget::changeSpecPow(const double _specPow)
{
    m_drawMesh->setSpeculatPower((float)_specPow);
    update();
}

void GLWidget::toggleGammaCorrec()
{
    m_drawMesh->setUseGammaCorrecFlag(!m_drawMesh->getUseGammaCorrecFlag());
    update();
}

void GLWidget::toggleTex()
{
    // Reverse state of texture mapping flag
    m_drawMesh->setTexFlag(!m_drawMesh->getTexFlag());
    update();
}

void GLWidget::toggleNormalMap()
{
    // Reverse state of normal mapping flag
    m_drawMesh->setNormalMapFlag(!m_drawMesh->getNormalMapFlag());
    update();
}

void GLWidget::toggleMeshCol()
{
    // Reverse state of normal mapping flag
    m_drawMesh->setUseMeshColFlag(!m_drawMesh->getUseMeshColFlag());
    update();
}

void GLWidget::duplVertices()
{
    m_triMesh->duplicateVertices();
    m_drawMesh->updateVAO(m_triMesh.get());
    update();
}

void GLWidget::compNormals()
{
    m_triMesh->computeNormals();
    m_drawMesh->updateVAO(m_triMesh.get());
    update();
}

void GLWidget::compTBs()
{
    m_triMesh->computeTB();
    m_drawMesh->updateVAO(m_triMesh.get());
    update();
}


//void GLWidget::compMeanCurv()
//{
//    m_triMesh->computeMeanCurv();
//    m_drawMesh->updateVAO(m_triMesh.get());
//    update();
//}

//void GLWidget::compSurfVar()
//{
//    m_triMesh->computeSurfVar();
//    m_drawMesh->updateVAO(m_triMesh.get());
//    update();
//}



