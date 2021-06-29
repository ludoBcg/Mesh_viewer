
//#include <QMouseEvent>
//#include <QTimer>
//#include <QElapsedTimer>
//#include <QImage>

#include "viewer.h"



using namespace std;


    /*------------------------------------------------------------------------------------------------------------+
    |                                        CONSTRUCTORS / DESTRUCTORS                                           |
    +-------------------------------------------------------------------------------------------------------------*/


Viewer::Viewer(QWidget *parent) : QGLViewer(parent)
{ }

Viewer::Viewer() : QGLViewer()
{ }

Viewer::~Viewer()
{
    delete m_drawMesh;
    delete m_triMesh;
    std::cout << std::endl << "Bye!" << std::endl;
}


    /*------------------------------------------------------------------------------------------------------------+
    |                                               OVERRIDDEN METHODS                                            |
    +-------------------------------------------------------------------------------------------------------------*/


void Viewer::init()
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
//    m_triMesh = new TriMeshHE(true, false, false, false);
    m_triMesh = new TriMeshSoup(true, false, false);
    m_triMesh->readFile("../../models/misc/teapot.obj");
    m_triMesh->computeAABB();

    m_drawMesh = new DrawableMesh;
    m_drawMesh->setProgram("../../src/shaders/phong_2.vert", "../../src/shaders/phong_2.frag");
    m_drawMesh->createVAO(m_triMesh);

    // Setup scene and camera parameters
    updateScene();

    m_backCol = glm::vec3(0.0f, 0.0f, 0.0f);
    m_lightPos = glm::vec3(10.0f, 0.0f, 0.0f);
    m_lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
}


void Viewer::draw()
{
    glClearColor(m_backCol.r, m_backCol.g, m_backCol.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST); // ensures that polygons overlap correctly

    // get modelview matrix
    GLdouble MWMat[16];
    this->camera()->getModelViewMatrix(MWMat);
    // get projection matrix
    GLdouble PMat[16];
    this->camera()->getProjectionMatrix(PMat);


    glm::mat4 mv = lqglToGlmMat4(MWMat);
    glm::mat4 projection = lqglToGlmMat4(PMat);
    glm::mat4 mvp = projection * mv;

    // get camera position
    glm::vec3 cam_pos(this->camera()->position().x, this->camera()->position().y, this->camera()->position().z);

    m_lightPos = cam_pos;
    m_drawMesh->draw(mv, mvp, m_lightPos, m_lightCol);

}


void Viewer::closeEvent(QCloseEvent *e)
{
    // Removed: saveStateToFile(); (caused crashing when closing window)
	QOpenGLWidget::closeEvent(e);
}


QString Viewer::helpString() const
{
    QString text("<h2>S i m p l e V i e w e r</h2>");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the viewer.";
    return text;
}


void Viewer::mousePressEvent(QMouseEvent *e)
{
    QGLViewer::mousePressEvent(e);
}


void Viewer::mouseReleaseEvent(QMouseEvent *e)
{
    QGLViewer::mouseReleaseEvent(e);
    // avoid camera spinning
    camera()->frame()->stopSpinning();
}


void Viewer::mouseMoveEvent(QMouseEvent *e)
{
    QGLViewer::mouseMoveEvent(e);  
}


void Viewer::resizeGL(int width, int height)
{
    QGLViewer::resizeGL(width, height);
}


void Viewer::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F)
    {
        std::cout<<"pressed F"<<std::endl;
    }
    QGLViewer::keyPressEvent(e);
}


    /*------------------------------------------------------------------------------------------------------------+
    |                                                 OTHER METHODS                                               |
    +-------------------------------------------------------------------------------------------------------------*/

void Viewer::updateScene()
{
    // scene setup
    glm::vec3 bBoxMin = m_triMesh->getBBoxMin();
    glm::vec3 bBoxMax = m_triMesh->getBBoxMax();
//    glm::vec3 bBoxDiag = bBoxMax - bBoxMin;

    if(bBoxMin != bBoxMax)
    {
        // scene setup
        qglviewer::Vec min(bBoxMin.x, bBoxMin.y, bBoxMin.z);
        qglviewer::Vec max(bBoxMax.x, bBoxMax.y, bBoxMax.z);
        qglviewer::Vec centerCoords( (bBoxMin.x + bBoxMax.x) * 0.5f, (bBoxMin.y + bBoxMax.y) * 0.5f, (bBoxMin.z + bBoxMax.z) * 0.5f );

//        this->setSceneCenter(/*centerCoords*/qglviewer::Vec(0.0f, 0.0f, 0.0f));
//        this->setSceneRadius( glm::length(bBoxDiag)*/*0.5f*/50 );
// equivalent to :
// setSceneCenter((min+max) / 2.0);
// setSceneRadius((max-min).norm() / 2.0);
this->setSceneBoundingBox(min, max);

        // camera setup
        this->camera()->setPosition( centerCoords + qglviewer::Vec(0.0f, 0.0f, this->sceneRadius()*2.5f) );
        this->camera()->setViewDirection(centerCoords - this->camera()->position() );
        this->camera()->setUpVector( qglviewer::Vec(0.0f, 1.0f, 0.0f) );

        update();
    }
    else
        std::cerr << "[ERROR] Viewer::updateScene(): bounding box is not computed" << std::endl;
}


glm::mat4 Viewer::lqglToGlmMat4(GLdouble mat[16])
{
    return glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9],
                     mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
}


void Viewer::lapSmooth(int _nbIter, float _factor)
{
    m_triMesh->lapSmooth(_nbIter, _factor);
    m_drawMesh->updateVAO(m_triMesh);
    update();
}


    /*------------------------------------------------------------------------------------------------------------+
    |                                                  SLOTS                                                      |
    +-------------------------------------------------------------------------------------------------------------*/

void Viewer::toggleRenderSurf() 
{ 
    m_drawMesh->toggleShadedRenderFlag(); 
    update(); 
}

void Viewer::toggleRenderLines() 
{ 
    m_drawMesh->toggleWireframeRenderFlag(); 
    update(); 
}

void Viewer::toggleShadingLines() 
{ 
    m_drawMesh->toggleWireframeShadingFlag(); 
    update(); 
}

void Viewer::toggleAmbient()
{
    // Reverse state of ambient flag
    m_drawMesh->setAmbientFlag( !m_drawMesh->getAmbientFlag() );
    update();
}

void Viewer::toggleDiffuse()
{
    // Reverse state of diffuse flag
    m_drawMesh->setDiffuseFlag( !m_drawMesh->getDiffuseFlag() );
    update();
}

void Viewer::toggleSpecular()
{
    // Reverse state of specular flag
    m_drawMesh->setSpecularFlag( !m_drawMesh->getSpecularFlag() );
    update();
}

void Viewer::toggleShowNormals()
{
    // Reverse state of normal vectors rendering flag
    m_drawMesh->setShowNormalFlag( !m_drawMesh->getShowNormalFlag() );
    update();
}

void Viewer::toggleFlatShading()
{
    // Reverse state of flat shading flag
    m_drawMesh->setFlatShadingFlag( !m_drawMesh->getFlatShadingFlag() );
    update();
}

void Viewer::changeSpecPow(double _specPow)
{
    m_drawMesh->setSpeculatPower( (float)_specPow );
    update();
}

void Viewer::toggleGammaCorrec()
{
    m_drawMesh->setUseGammaCorrecFlag( !m_drawMesh->getUseGammaCorrecFlag() );
    update();
}

void Viewer::toggleTex()
{
    // Reverse state of texture mapping flag
    m_drawMesh->setTexFlag( !m_drawMesh->getTexFlag() );
    update();
}

void Viewer::toggleNormalMap()
{
    // Reverse state of normal mapping flag
    m_drawMesh->setNormalMapFlag( !m_drawMesh->getNormalMapFlag() );
    update();
}

void Viewer::toggleMeshCol()
{
    // Reverse state of normal mapping flag
    m_drawMesh->setUseMeshColFlag( !m_drawMesh->getUseMeshColFlag() );
    update();
}

void Viewer::duplVertices()
{
    m_triMesh->duplicateVertices();
    m_drawMesh->updateVAO(m_triMesh);
    update();
}

void Viewer::compNormals()
{
    m_triMesh->computeNormals();
    m_drawMesh->updateVAO(m_triMesh);
    update();
}

void Viewer::compTBs()
{
    m_triMesh->computeTB();
    m_drawMesh->updateVAO(m_triMesh);
    update();
}

