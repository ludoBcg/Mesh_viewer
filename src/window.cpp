#include <QFileDialog>
#include <QColorDialog>

#include "window.h"
#include "viewer.h"

Window::Window() : QWidget()
{
    // Init flags
    m_albedoTexLoaded = false;
    m_normalMapLoaded = false;
    m_metalMapLoaded = false;
    m_glossMapLoaded = false;
    m_ambientMapLoaded = false;
    m_cubeMapLoaded = false;

    this->setWindowTitle("=][=");
    this->move(300,100);

    /*******************************************************************************************/
    /************************************** Global Layout **************************************/
    /*******************************************************************************************/

    m_globalLayout = new QVBoxLayout(this);


    /************************************* ToolBar Layouts *************************************/

    m_toolbarLayout = new QHBoxLayout;
    m_toolbarLayout->setAlignment(Qt::AlignLeft);

    // Show dialog box button
    m_buttonShowDB = new QPushButton("show Toolbox", this);
    m_buttonShowDB->setToolTip("rendering and geometry tools");
    m_buttonShowDB->setFixedSize(100, 25);
    m_buttonShowDB->setCheckable(true);
    m_buttonShowDB->setChecked(false);
    QObject::connect(m_buttonShowDB, SIGNAL(clicked()), this, SLOT(popDialog()));
    m_toolbarLayout->addWidget(m_buttonShowDB);

    // Load TriMeshHE button
    m_buttonLoadMeshHE = new QPushButton("load TriMeshHE", this);
    m_buttonLoadMeshHE->setToolTip("load a triangle mesh using half-edge data structure");
    m_buttonLoadMeshHE->setFixedSize(100, 25);
    QObject::connect(m_buttonLoadMeshHE, SIGNAL(clicked()), this, SLOT(loadMeshHE()));
    m_toolbarLayout->addWidget(m_buttonLoadMeshHE);

    // Load TriMeshSoup button
    m_buttonLoadMeshSoup = new QPushButton("load TriMeshSoup", this);
    m_buttonLoadMeshSoup->setToolTip("load a triangle mesh using a polygon soup data structure");
    m_buttonLoadMeshSoup->setFixedSize(100, 25);
    QObject::connect(m_buttonLoadMeshSoup, SIGNAL(clicked()), this, SLOT(loadMeshSoup()));
    m_toolbarLayout->addWidget(m_buttonLoadMeshSoup);

    // Save mesh button
    m_buttonSaveMesh = new QPushButton("save mesh", this);
    m_buttonSaveMesh->setToolTip("save mesh to a file (warning: file format depends on data structure)");
    m_buttonSaveMesh->setFixedSize(100, 25);
    QObject::connect(m_buttonSaveMesh, SIGNAL(clicked()), this, SLOT(saveMesh()));
    m_toolbarLayout->addWidget(m_buttonSaveMesh);

    m_globalLayout->addLayout(m_toolbarLayout);


    /**************************************** GL Viewer ****************************************/

    m_glViewer = new Viewer(this);
    m_glViewer->setMinimumSize(1024, 768);

    m_globalLayout->addWidget(m_glViewer);


    /*******************************************************************************************/
    /*************************************** Dialog Box ****************************************/
    /*******************************************************************************************/

    m_dialogBox = new QDialog(this);
    m_dialogBox->setModal(false);
    m_dialogBox->setVisible(false);
    m_dialogBox->setGeometry(this->geometry().x() + 20 , this->geometry().y() + 50, 250, 500 );
    m_dialogBox->setWindowTitle("Toolbox");
    QObject::connect(m_dialogBox, SIGNAL(rejected()), this, SLOT(rejectDialog()));

    // Main dialog box layout
    m_boxGlobalLayout = new QVBoxLayout;


    /*************************************** Scene setup ****************************************/
    // GroupBox Scene setup
    m_groupBoxScene = new QGroupBox("Scene setup", this);
    // Scene setup layout
    m_boxSceneLayout = new QVBoxLayout;


    // Shader button;
    m_buttonShader = new QPushButton("load shader files", this);
    m_buttonShader->setFixedSize(100, 25);
    QObject::connect(m_buttonShader, SIGNAL(clicked()), this, SLOT(openShaderDialog()));
    m_boxSceneLayout->addWidget(m_buttonShader);

    // Background color button
    m_backColLayout = new QHBoxLayout;
    m_backColLayout->setAlignment(Qt::AlignRight);
    m_backColLabel = new QLabel("background color");
    m_backColLayout->addWidget(m_backColLabel);
    m_buttonBackCol = new QPushButton(this);
    m_buttonBackCol->setToolTip("change background color");
    m_buttonBackCol->setFixedSize(100, 25);
    m_backCol = QColor(0, 0, 0);
    QString styleSheet("background: #" + QString(m_backCol.red() < 16? "0" : "") + QString::number(m_backCol.red(),16)
                                       + QString(m_backCol.green() < 16? "0" : "") + QString::number(m_backCol.green(),16)
                                       + QString(m_backCol.blue() < 16? "0" : "") + QString::number(m_backCol.blue(),16) + ";");
    m_buttonBackCol->setStyleSheet(styleSheet);
    m_buttonBackCol->setFixedSize(25, 20);
    QObject::connect(m_buttonBackCol, SIGNAL(clicked()), this, SLOT(selectBackColor()));
    m_backColLayout->addWidget(m_buttonBackCol);
    m_boxSceneLayout->addLayout(m_backColLayout);

    // Light color
    m_lightColLayout = new QHBoxLayout;
    m_lightColLayout->setAlignment(Qt::AlignRight);
    m_lightColLabel = new QLabel("light color");
    m_lightColLayout->addWidget(m_lightColLabel);
    m_buttonLightCol = new QPushButton(this);
    m_lightCol = QColor(255, 255, 255);
    styleSheet = QString("background: #" + QString(m_lightCol.red() < 16? "0" : "") + QString::number(m_lightCol.red(),16)
                                         + QString(m_lightCol.green() < 16? "0" : "") + QString::number(m_lightCol.green(),16)
                                         + QString(m_lightCol.blue() < 16? "0" : "") + QString::number(m_lightCol.blue(),16) + ";");
    m_buttonLightCol->setStyleSheet(styleSheet);
    m_buttonLightCol->setFixedSize(25, 20);
    QObject::connect(m_buttonLightCol, SIGNAL(clicked()), this, SLOT(selectLightColor()));
    m_lightColLayout->addWidget(m_buttonLightCol);
    m_boxSceneLayout->addLayout(m_lightColLayout);

    // Render surface
    m_toggleShowSurf = new QCheckBox;
    m_toggleShowSurf->setText("show surface");
    m_toggleShowSurf->setChecked(true);
    QObject::connect(m_toggleShowSurf, SIGNAL(clicked()), m_glViewer, SLOT(toggleRenderSurf()));
    m_boxSceneLayout->addWidget(m_toggleShowSurf);

    // wireframe
    m_wireLayout = new QHBoxLayout;
    // Render edges button
    m_toggleShowLines = new QCheckBox;
    m_toggleShowLines->setText("show wireframe");
    m_toggleShowLines->setChecked(false);
    QObject::connect(m_toggleShowLines, SIGNAL(clicked()), m_glViewer, SLOT(toggleRenderLines()));
    QObject::connect(m_toggleShowLines, SIGNAL(clicked()), this, SLOT(toggleLines()));
    m_wireLayout->addWidget(m_toggleShowLines);
    m_wireLayout->addStretch();
        //Edge color
        m_wireLabel = new QLabel("edges color");
        m_wireLabel->setEnabled(false);
        m_wireLayout->addWidget(m_wireLabel);
        m_buttonWireColor = new QPushButton(this);
        m_buttonWireColor->setFixedSize(25, 20);
        m_buttonWireColor->setEnabled(false);
        m_wireCol = QColor(127, 127, 127);
        styleSheet = QString("background: #" + QString(m_wireCol.red() < 16? "0" : "") + QString::number(m_wireCol.red(),16)
                                             + QString(m_wireCol.green() < 16? "0" : "") + QString::number(m_wireCol.green(),16)
                                             + QString(m_wireCol.blue() < 16? "0" : "") + QString::number(m_wireCol.blue(),16) + ";");
        m_buttonWireColor->setStyleSheet(styleSheet);
        QObject::connect(m_buttonWireColor, SIGNAL(clicked()), this, SLOT(selectWireColor()));
    m_wireLayout->addWidget(m_buttonWireColor);
    m_boxSceneLayout->addLayout(m_wireLayout);

    m_groupBoxScene->setLayout(m_boxSceneLayout);
    m_boxGlobalLayout->addWidget(m_groupBoxScene);

    /************************************* Shading options **************************************/
    // GroupBox Shading
    m_groupBoxShading = new QGroupBox("Shading options", this);
    // Shading layout
    m_boxShadingLayout = new QVBoxLayout;


    // Toggle Ambient button
    m_ambientLayout = new QHBoxLayout;
    m_toggleAmbient = new QCheckBox;
    m_toggleAmbient->setText("ambient shading");
    m_toggleAmbient->setChecked(true);
    QObject::connect(m_toggleAmbient, SIGNAL(clicked()), m_glViewer, SLOT(toggleAmbient()));
    QObject::connect(m_toggleAmbient, SIGNAL(clicked()), this, SLOT(toggleAmbient()));
    m_ambientLayout->addWidget(m_toggleAmbient);
        // Select ambient color
        m_ambientLayout->addStretch();
        m_ambientLabel = new QLabel("ambient color");
        m_ambientLayout->addWidget(m_ambientLabel);
        m_buttonAmbientCol = new QPushButton(this);
        m_ambientCol = QColor(0, 0, 25);
        styleSheet = QString("background: #" + QString(m_ambientCol.red() < 16? "0" : "") + QString::number(m_ambientCol.red(),16)
                                                + QString(m_ambientCol.green() < 16? "0" : "") + QString::number(m_ambientCol.green(),16)
                                                + QString(m_ambientCol.blue() < 16? "0" : "") + QString::number(m_ambientCol.blue(),16) + ";");
        m_buttonAmbientCol->setStyleSheet(styleSheet);
        m_buttonAmbientCol->setFixedSize(25, 20);
        QObject::connect(m_buttonAmbientCol, SIGNAL(clicked()), this, SLOT(selectAmbientColor()));
        m_ambientLayout->addWidget(m_buttonAmbientCol);
        m_boxShadingLayout->addLayout(m_ambientLayout);

    // Toggle Diffuse button
    m_diffuseLayout = new QHBoxLayout;
    m_toggleDiffuse = new QCheckBox;
    m_toggleDiffuse->setText("diffuse shading");
    m_toggleDiffuse->setChecked(true);
    QObject::connect(m_toggleDiffuse, SIGNAL(clicked()), m_glViewer, SLOT(toggleDiffuse()));
    QObject::connect(m_toggleDiffuse, SIGNAL(clicked()), this, SLOT(toggleDiffuse()));
    m_diffuseLayout->addWidget(m_toggleDiffuse);
        // Select diffuse color
        m_diffuseLayout->addStretch();
        m_diffuseLabel = new QLabel("diffuse color");
        m_diffuseLayout->addWidget(m_diffuseLabel);
        m_buttonDiffuseCol = new QPushButton(this);
        m_diffuseCol = QColor(242, 128, 64);
        styleSheet = QString("background: #" + QString(m_diffuseCol.red() < 16? "0" : "") + QString::number(m_diffuseCol.red(),16)
                                             + QString(m_diffuseCol.green() < 16? "0" : "") + QString::number(m_diffuseCol.green(),16)
                                             + QString(m_diffuseCol.blue() < 16? "0" : "") + QString::number(m_diffuseCol.blue(),16) + ";");
        m_buttonDiffuseCol->setStyleSheet(styleSheet);
        m_buttonDiffuseCol->setFixedSize(25, 20);
        QObject::connect(m_buttonDiffuseCol, SIGNAL(clicked()), this, SLOT(selectDiffuseColor()));
        m_diffuseLayout->addWidget(m_buttonDiffuseCol);
        m_boxShadingLayout->addLayout(m_diffuseLayout);

    // Toggle Specular button
    m_specularLayout = new QHBoxLayout;
    m_toggleSpecular = new QCheckBox;
    m_toggleSpecular->setText("specular shading");
    m_toggleSpecular->setChecked(true);
    QObject::connect(m_toggleSpecular, SIGNAL(clicked()), m_glViewer, SLOT(toggleSpecular()));
    QObject::connect(m_toggleSpecular, SIGNAL(clicked()), this, SLOT(toggleSpecular()));
    m_specularLayout->addWidget(m_toggleSpecular);
        // Select specular color
        m_specularLayout->addStretch();
        m_specularLabel = new QLabel("specular color");
        m_specularLayout->addWidget(m_specularLabel);
        m_buttonSpecularCol = new QPushButton(this);
        m_specularCol = QColor(0, 204, 0);
        styleSheet = QString("background: #" + QString(m_specularCol.red() < 16? "0" : "") + QString::number(m_specularCol.red(),16)
                                             + QString(m_specularCol.green() < 16? "0" : "") + QString::number(m_specularCol.green(),16)
                                             + QString(m_specularCol.blue() < 16? "0" : "") + QString::number(m_specularCol.blue(),16) + ";");
        m_buttonSpecularCol->setStyleSheet(styleSheet);
        m_buttonSpecularCol->setFixedSize(25, 20);
        QObject::connect(m_buttonSpecularCol, SIGNAL(clicked()), this, SLOT(selectSpecularColor()));
        m_specularLayout->addWidget(m_buttonSpecularCol);
        m_boxShadingLayout->addLayout(m_specularLayout);

        // Specular power SpinBox
        m_specPowLayout = new QHBoxLayout;
        m_specPowLabel = new QLabel("specular power");
        m_specPowLayout->addWidget(m_specPowLabel);
        m_specPowSpinBox = new QDoubleSpinBox(this);
        m_specPowSpinBox->setMinimum(0.1);
        m_specPowSpinBox->setMaximum(2048.0);
        m_specPowSpinBox->setSingleStep(0.5);
        m_specPowSpinBox->setValue(128.0);
        m_specPowSpinBox->setFixedWidth(60);
        m_specPowSpinBox->setFixedHeight(20);
        QObject::connect(m_specPowSpinBox, SIGNAL(valueChanged(double)), m_glViewer, SLOT(changeSpecPow(double)));
        m_specPowLayout->addWidget(m_specPowSpinBox);
        m_specPowLayout->setAlignment(Qt::AlignRight);
        m_boxShadingLayout->addLayout(m_specPowLayout);

    // Toggle show normals button
    m_toggleShowNormals = new QCheckBox;
    m_toggleShowNormals->setText("show normals");
    m_toggleShowNormals->setChecked(false);
    QObject::connect(m_toggleShowNormals, SIGNAL(clicked()), m_glViewer, SLOT(toggleShowNormals()));
    QObject::connect(m_toggleShowNormals, SIGNAL(clicked()), this, SLOT(toggleShowNormals()));
    m_boxShadingLayout->addWidget(m_toggleShowNormals);

    // Toggle gamma correction button
    m_toggleGammaCorrec = new QCheckBox;
    m_toggleGammaCorrec->setText("gamma correction");
    m_toggleGammaCorrec->setChecked(true);
    QObject::connect(m_toggleGammaCorrec, SIGNAL(clicked()), m_glViewer, SLOT(toggleGammaCorrec()));
    m_boxShadingLayout->addWidget(m_toggleGammaCorrec);

    m_groupBoxShading->setLayout(m_boxShadingLayout);
    m_boxGlobalLayout->addWidget(m_groupBoxShading);


    /************************************* Texture mapping **************************************/
    // GroupBox Texture mapping
    m_groupBoxTex = new QGroupBox("Texture mapping", this);
    // Texture mapping layout
    m_boxTexLayout = new QVBoxLayout;

    // Load Albedo texture button
    m_albedoTexLayout = new QHBoxLayout;
    m_buttonLoadAlbedoTex = new QPushButton("load albedo texture", this);
    m_buttonLoadAlbedoTex->setFixedSize(150, 25);
    QObject::connect(m_buttonLoadAlbedoTex, SIGNAL(clicked()), this, SLOT(openAlbedoTexDialog()));
    m_albedoTexLayout->addWidget(m_buttonLoadAlbedoTex);
    m_toggleAlbedoTex = new QCheckBox;
    m_toggleAlbedoTex->setText("use albedo texture");
    m_toggleAlbedoTex->setChecked(false);
    m_toggleAlbedoTex->setEnabled(false);
    QObject::connect(m_toggleAlbedoTex, SIGNAL(clicked()), m_glViewer, SLOT(toggleAlbedoTex()));
    QObject::connect(m_toggleAlbedoTex, SIGNAL(clicked()), this, SLOT(toggleTex()));
    m_albedoTexLayout->addWidget(m_toggleAlbedoTex);
    m_boxTexLayout->addLayout(m_albedoTexLayout);

    // Load Normal map button
    m_normalMapLayout = new QHBoxLayout;
    m_buttonLoadNormalMap = new QPushButton("load normal map", this);
    m_buttonLoadNormalMap->setFixedSize(150, 25);
    QObject::connect(m_buttonLoadNormalMap, SIGNAL(clicked()), this, SLOT(openNormalMapDialog()));
    m_normalMapLayout->addWidget(m_buttonLoadNormalMap);
    m_toggleNormalMap = new QCheckBox;
    m_toggleNormalMap->setText("use normal map");
    m_toggleNormalMap->setChecked(false);
    m_toggleNormalMap->setEnabled(false);
    QObject::connect(m_toggleNormalMap, SIGNAL(clicked()), m_glViewer, SLOT(toggleNormalMap()));
    m_normalMapLayout->addWidget(m_toggleNormalMap);
    m_boxTexLayout->addLayout(m_normalMapLayout);

    // Load Metal map button
    m_buttonLoadMetalMap = new QPushButton("load metal map", this);
    m_buttonLoadMetalMap->setFixedSize(150, 25);
    QObject::connect(m_buttonLoadMetalMap, SIGNAL(clicked()), this, SLOT(openMetalMapDialog()));
    m_boxTexLayout->addWidget(m_buttonLoadMetalMap);
    // Load Gloss map button
    m_PBRMapLayout = new QHBoxLayout;
    m_buttonLoadGlossMap = new QPushButton("load gloss map", this);
    m_buttonLoadGlossMap->setFixedSize(150, 25);
    QObject::connect(m_buttonLoadGlossMap, SIGNAL(clicked()), this, SLOT(openGlossMapDialog()));
    m_PBRMapLayout->addWidget(m_buttonLoadGlossMap);
    m_togglePBRMap = new QCheckBox;
    m_togglePBRMap->setText("use PBR maps");
    m_togglePBRMap->setChecked(false);
    m_togglePBRMap->setEnabled(false);
    QObject::connect(m_togglePBRMap, SIGNAL(clicked()), m_glViewer, SLOT(togglePBRMap()));
    m_PBRMapLayout->addWidget(m_togglePBRMap);
    m_boxTexLayout->addLayout(m_PBRMapLayout);

    // Load Ambient map button
    m_ambientMapLayout = new QHBoxLayout;
    m_buttonLoadAmbientMap = new QPushButton("load ambient occlusion map", this);
    m_buttonLoadAmbientMap->setFixedSize(150, 25);
    QObject::connect(m_buttonLoadAmbientMap, SIGNAL(clicked()), this, SLOT(openAmbMapDialog()));
    m_ambientMapLayout->addWidget(m_buttonLoadAmbientMap);
    m_toggleAmbientMap = new QCheckBox;
    m_toggleAmbientMap->setText("use ambient occlusion");
    m_toggleAmbientMap->setChecked(false);
    m_toggleAmbientMap->setEnabled(false);
    QObject::connect(m_toggleAmbientMap, SIGNAL(clicked()), m_glViewer, SLOT(toggleAmbientMap()));
    QObject::connect(m_toggleAmbientMap, SIGNAL(clicked()), this, SLOT(toggleAmbMap()));
    m_ambientMapLayout->addWidget(m_toggleAmbientMap);
    m_boxTexLayout->addLayout(m_ambientMapLayout);

    // Load Cube map button
    m_cubeMapLayout = new QHBoxLayout;
    m_buttonLoadCubeMap = new QPushButton("load cube map", this);
    m_buttonLoadCubeMap->setFixedSize(150, 25);
    QObject::connect(m_buttonLoadCubeMap, SIGNAL(clicked()), this, SLOT(openCubeMapDialog()));
    m_cubeMapLayout->addWidget(m_buttonLoadCubeMap);
    m_toggleEnvMap = new QCheckBox;
    m_toggleEnvMap->setText("use environment mapping");
    m_toggleEnvMap->setChecked(false);
    m_toggleEnvMap->setEnabled(false);
    QObject::connect(m_toggleEnvMap, SIGNAL(clicked()), m_glViewer, SLOT(toggleEnvMap()));
    m_cubeMapLayout->addWidget(m_toggleEnvMap);
    m_boxTexLayout->addLayout(m_cubeMapLayout);

    m_groupBoxTex->setLayout(m_boxTexLayout);
    m_boxGlobalLayout->addWidget(m_groupBoxTex);


    /************************************* Geometry tools ***************************************/
    // GroupBox Geometry tools
    m_groupBoxGeom = new QGroupBox("Geometry tools", this);
    // Geometry tools layout
    m_boxGeomLayout = new QVBoxLayout;

    // Recompute normals button
    m_buttonCompNormals = new QPushButton("recompute normals", this);
    m_buttonCompNormals->setFixedSize(175, 25);
    QObject::connect(m_buttonCompNormals, SIGNAL(clicked()), m_glViewer, SLOT(compNormals()));
    m_boxGeomLayout->addWidget(m_buttonCompNormals);

    // Compute tangents + bitangents button
    m_buttonCompTB = new QPushButton("compute tangents and bitangents", this);
    m_buttonCompTB->setFixedSize(175, 25);
    QObject::connect(m_buttonCompTB, SIGNAL(clicked()), m_glViewer, SLOT(compTBs()));
    m_boxGeomLayout->addWidget(m_buttonCompTB);

    // Apply Laplacian smoothing
    m_buttonLapSmooth = new QPushButton("Laplacian smoothing", this);
    m_buttonLapSmooth->setFixedSize(175, 25);
    m_buttonLapSmooth->setVisible(false);
    QObject::connect(m_buttonLapSmooth, SIGNAL(clicked()), this, SLOT(lapSmooth()));
    m_boxGeomLayout->addWidget(m_buttonLapSmooth);

        // Smoothing parameters
        m_smoothParamLayout = new QHBoxLayout;
        m_nbIterSpinBox = new QSpinBox(this);
        m_nbIterSpinBox->setVisible(false);
        m_nbIterSpinBox->setMinimum(1);
        m_nbIterSpinBox->setMaximum(100);
        m_nbIterSpinBox->setSingleStep(1);
        m_nbIterSpinBox->setValue(1);
        m_nbIterSpinBox->setFixedWidth(40);
        m_nbIterSpinBox->setFixedHeight(20);
        m_smoothParamLayout->addWidget(m_nbIterSpinBox);
        m_nbIterLabel = new QLabel("iterations");
        m_nbIterLabel->setVisible(false);
        m_smoothParamLayout->addWidget(m_nbIterLabel);
        m_factorSpinBox = new QDoubleSpinBox(this);
        m_factorSpinBox->setVisible(false);
        m_factorSpinBox->setMinimum(0.1);
        m_factorSpinBox->setMaximum(1.0);
        m_factorSpinBox->setSingleStep(0.1);
        m_factorSpinBox->setValue(1.0);
        m_factorSpinBox->setFixedWidth(45);
        m_factorSpinBox->setFixedHeight(20);
        m_smoothParamLayout->addWidget(m_factorSpinBox);
        m_factorLabel = new QLabel("factor");
        m_factorLabel->setVisible(false);
        m_smoothParamLayout->addWidget(m_factorLabel);
        m_smoothParamLayout->setAlignment(Qt::AlignRight);
        m_boxGeomLayout->addLayout(m_smoothParamLayout);


    m_groupBoxGeom->setLayout(m_boxGeomLayout);
    m_boxGlobalLayout->addWidget(m_groupBoxGeom);


    m_dialogBox->setLayout(m_boxGlobalLayout);

    m_globalLayout->addWidget(m_dialogBox);

}

Window::~Window()
{
    //--- Delete tool box widgets ------------

    // Delete scene setup
    delete m_buttonShader;
    delete m_backColLabel;
    delete m_buttonBackCol;
    delete m_backColLayout;
    delete m_lightColLabel;
    delete m_buttonLightCol;
    delete m_lightColLayout;
    delete m_toggleShowSurf;
    delete m_toggleShowLines;
    delete m_wireLabel;
    delete m_wireLayout;
    delete m_buttonWireColor;
    delete m_boxSceneLayout;
    delete m_groupBoxScene;
    // Delete shading options
    delete m_toggleAmbient;
    delete m_ambientLabel;
    delete m_buttonAmbientCol;
    delete m_ambientLayout;
    delete m_toggleDiffuse;
    delete m_diffuseLabel;
    delete m_buttonDiffuseCol;
    delete m_diffuseLayout;
    delete m_toggleSpecular;
    delete m_specularLabel;
    delete m_buttonSpecularCol;
    delete m_specularLayout;
    delete m_specPowLabel;
    delete m_specPowSpinBox;
    delete m_specPowLayout;
    delete m_toggleShowNormals;
    delete m_toggleGammaCorrec;
    delete m_boxShadingLayout;
    delete m_groupBoxShading;
    // Delete texture mapping options
    delete m_buttonLoadAlbedoTex;
    delete m_toggleAlbedoTex; 
    delete m_albedoTexLayout;
    delete m_buttonLoadNormalMap;
    delete m_toggleNormalMap;
    delete m_normalMapLayout;
    delete m_buttonLoadMetalMap;
    delete m_buttonLoadGlossMap;
    delete m_togglePBRMap;
    delete m_PBRMapLayout;
    delete m_buttonLoadAmbientMap;
    delete m_toggleAmbientMap;
    delete m_ambientMapLayout;
    delete m_buttonLoadCubeMap;
    delete m_toggleEnvMap;
    delete m_cubeMapLayout;
    delete m_boxTexLayout;
    delete m_groupBoxTex;
    // Delete geometry tools
    delete m_buttonCompNormals;
    delete m_buttonCompTB;
    delete m_buttonLapSmooth;
    delete m_nbIterSpinBox;
    delete m_nbIterLabel;
    delete m_factorSpinBox;
    delete m_factorLabel;
    delete m_smoothParamLayout;
    delete m_boxGeomLayout;
    delete m_groupBoxGeom;
    //--- Delete tool box --------------------
    delete m_boxGlobalLayout;
    delete m_dialogBox;

    //--- Delete other main window widgets ---
    delete m_glViewer;
    delete m_buttonShowDB;
    delete m_buttonLoadMeshHE;
    delete m_buttonLoadMeshSoup;
    delete m_buttonSaveMesh;
    delete m_toolbarLayout;
    //--- Delete main window layout-----------
    delete m_globalLayout;
}

void Window::moveEvent(QMoveEvent *e)
{
    QWidget::moveEvent(e);

    QRect geom = this->geometry();
    positionDialog(&geom);
}

void Window::positionDialog(QRect *_mainWinGeom)
{
    // Move dialog box to the top left corner of the main window
    m_dialogBox->move(_mainWinGeom->x() + 20 , _mainWinGeom->y() + 50);
}

        /*------------------------------------------------------------------------------------------------------------+
        |                                                   SLOTS                                                     |
        +-------------------------------------------------------------------------------------------------------------*/

void Window::popDialog()
{
    if( m_buttonShowDB->isChecked() )
    {
        m_dialogBox->setVisible(true);
        m_buttonShowDB->setText("hide Toolbox");
    }
    else
    {
        m_dialogBox->setVisible(false);
        m_buttonShowDB->setText("show Toolbox");
    }
}

void Window::rejectDialog()
{
    m_dialogBox->setVisible(false);
    m_buttonShowDB->setText("show Toolbox");
    m_buttonShowDB->setChecked(false);
}

void Window::loadMeshHE()
{
    QString file = QFileDialog::getOpenFileName(this, "open file", "../../3dmodels", "Mesh (*.obj *.off)");
    if( !file.isEmpty() )
    {
        m_glViewer->loadTriMeshHE(file);
        m_buttonLapSmooth->setVisible(true);
        m_nbIterSpinBox->setVisible(true);
        m_nbIterLabel->setVisible(true);
        m_factorSpinBox->setVisible(true);
        m_factorLabel->setVisible(true);
    }
}

void Window::loadMeshSoup()
{
    QString file = QFileDialog::getOpenFileName(this, "open file", "../../3dmodels", "Mesh (*.obj *.stl)");
    if( !file.isEmpty() )
    {
        m_glViewer->loadTriMeshSoup(file);
        m_buttonLapSmooth->setVisible(false);
        m_nbIterSpinBox->setVisible(false);
        m_nbIterLabel->setVisible(false);
        m_factorSpinBox->setVisible(false);
        m_factorLabel->setVisible(false);
    }
}

void Window::saveMesh()
{
    QString file = QFileDialog::getSaveFileName(this, "save file", "../../results", "Mesh (*.obj *.off *.stl)");
    if( !file.isEmpty() )
        m_glViewer->saveMesh(file);
}

void Window::openShaderDialog()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "open file", "../../src/shaders", "Shader (*.vert *.frag)");
    if( !files.isEmpty() )
    {
        std::string vertFile;
        std::string fragFile;

        for(int i = 0; i < files.size(); i++)
        {
            std::string filename = files[i].toStdString();
            if(filename.substr(filename.find_last_of(".") + 1) == "vert")
                vertFile = filename;
            else if(filename.substr(filename.find_last_of(".") + 1) == "frag")
                fragFile = filename;
        }
        if( !vertFile.empty() && !fragFile.empty() )
            m_glViewer->setShaderFiles(vertFile, fragFile);
        else
            std::cerr << "ERROR: [Window::openShaderDialog()] selected files do not correspond to fragment and vertex shaders" << std::endl;
    }
}

void Window::selectBackColor()
{
    QColor color = QColorDialog::getColor(m_backCol);
    if( color.isValid() )
    {
        m_glViewer->setBackColor(color);
        // update current background color
        m_backCol = color;
        // change color of the button accordingly
        QString styleSheet("background: #" + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                                           + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                                           + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
        m_buttonBackCol->setStyleSheet(styleSheet);
        m_buttonBackCol->update();
    }
}

void Window::selectLightColor()
{
    QColor color = QColorDialog::getColor(m_lightCol);
    if( color.isValid() )
    {
        m_glViewer->setLightColor(color);

        // update current ambient color
        m_lightCol = color;
        // change color of the button accordingly
        QString styleSheet("background: #" + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                                           + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                                           + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
        m_buttonLightCol->setStyleSheet(styleSheet);
        m_buttonLightCol->update();
    }
}


void Window::toggleLines()
{
    if( m_toggleShowLines->isChecked() )
    {
        m_buttonWireColor->setEnabled(true);
        m_wireLabel->setEnabled(true);
    }
    else
    {
        m_buttonWireColor->setEnabled(false);
        m_wireLabel->setEnabled(false);
    }
}

void Window::selectWireColor()
{
    QColor color = QColorDialog::getColor(m_wireCol);
    if( color.isValid() )
    {
        m_glViewer->setWireColor(color);
        // update current wireframe color
        m_wireCol = color;
        // change color of the button accordingly
        QString styleSheet("background: #" + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                                           + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                                           + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
        m_buttonWireColor->setStyleSheet(styleSheet);
        m_buttonWireColor->update();
    }
}

void Window::selectAmbientColor()
{
    QColor color = QColorDialog::getColor(m_ambientCol);
    if( color.isValid() )
    {
        m_glViewer->setAmbientColor(color);

        // update current ambient color
        m_ambientCol = color;
        // change color of the button accordingly
        QString styleSheet("background: #" + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                                           + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                                           + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
        m_buttonAmbientCol->setStyleSheet(styleSheet);
        m_buttonAmbientCol->update();
    }
}

void Window::selectDiffuseColor()
{
    QColor color = QColorDialog::getColor(m_diffuseCol);
    if( color.isValid() )
    {
        m_glViewer->setDiffuseColor(color);

        // update current diffuse color
        m_diffuseCol = color;
        // change color of the button accordingly
        QString styleSheet("background: #" + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                                           + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                                           + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
        m_buttonDiffuseCol->setStyleSheet(styleSheet);
        m_buttonDiffuseCol->update();
    }
}

void Window::selectSpecularColor()
{
    QColor color = QColorDialog::getColor(m_specularCol);
    if( color.isValid() )
    {
        m_glViewer->setSpecularColor(color);

        // update current specular color
        m_specularCol = color;
        // change color of the button accordingly
        QString styleSheet("background: #" + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                                           + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                                           + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
        m_buttonSpecularCol->setStyleSheet(styleSheet);
        m_buttonSpecularCol->update();
    }
}

void Window::toggleAmbient()
{
    if( m_toggleAmbient->isChecked() && !m_toggleAlbedoTex->isChecked() )
    {
        m_buttonAmbientCol->setEnabled(true);
        m_ambientLabel->setEnabled(true);
    }
    else
    {
        m_buttonAmbientCol->setEnabled(false);
        m_ambientLabel->setEnabled(false);
    }
}

void Window::toggleDiffuse()
{
    if( m_toggleDiffuse->isChecked() && !m_toggleAlbedoTex->isChecked() )
    {
        m_buttonDiffuseCol->setEnabled(true);
        m_diffuseLabel->setEnabled(true);
    }
    else
    {
        m_buttonDiffuseCol->setEnabled(false);
        m_diffuseLabel->setEnabled(false);
    }
}

void Window::toggleSpecular()
{
    if( m_toggleSpecular->isChecked() )
    {
        m_specPowSpinBox->setEnabled(true);
        m_specPowLabel->setEnabled(true);
        m_buttonSpecularCol->setEnabled(true);
        m_specularLabel->setEnabled(true);
    }
    else
    {
        m_specPowSpinBox->setEnabled(false);
        m_specPowLabel->setEnabled(false);
        m_buttonSpecularCol->setEnabled(false);
        m_specularLabel->setEnabled(false);
    }
}

void Window::toggleShowNormals()
{
    if(m_toggleShowNormals->isChecked() )
    {
        m_toggleAmbient->setEnabled(false);
        m_toggleDiffuse->setEnabled(false);
        m_toggleSpecular->setEnabled(false);

        m_lightColLabel->setEnabled(false);
        m_buttonLightCol->setEnabled(false);

        m_buttonAmbientCol->setEnabled(false);
        m_ambientLabel->setEnabled(false);
        m_buttonDiffuseCol->setEnabled(false);
        m_diffuseLabel->setEnabled(false);
        m_buttonSpecularCol->setEnabled(false);
        m_specularLabel->setEnabled(false);

        m_specPowSpinBox->setEnabled(false);
        m_specPowLabel->setEnabled(false);

        m_toggleAlbedoTex->setEnabled(false);
        m_togglePBRMap->setEnabled(false);
        m_toggleEnvMap->setEnabled(false);
        m_toggleAmbientMap->setEnabled(false);
    }
    else
    {
        m_toggleAmbient->setEnabled(true);
        m_toggleDiffuse->setEnabled(true);
        m_toggleSpecular->setEnabled(true);

        m_lightColLabel->setEnabled(true);
        m_buttonLightCol->setEnabled(true);

        if( !m_toggleAlbedoTex->isChecked() )
        {
            toggleAmbient();
            toggleDiffuse();
        }
        toggleSpecular();

        if(m_albedoTexLoaded)
            m_toggleAlbedoTex->setEnabled(true);
        if(m_metalMapLoaded || m_glossMapLoaded)
            m_togglePBRMap->setEnabled(true);
        if(m_cubeMapLoaded)
            m_toggleEnvMap->setEnabled(true);
        if(m_ambientMapLoaded)
            m_toggleAmbientMap->setEnabled(true);
    }
}

void Window::openAlbedoTexDialog()
{
    QString file = QFileDialog::getOpenFileName(this, "open file", "../../3dmodels", "Image (*.png)");
    if( !file.isEmpty() )
    {
        m_glViewer->setAlbedoTex(file);
        m_toggleAlbedoTex->setEnabled(true);
        m_albedoTexLoaded = true;
    }
}

void Window::toggleTex()
{
    if( m_toggleAlbedoTex->isChecked() )
    {
        m_buttonAmbientCol->setEnabled(false);
        m_ambientLabel->setEnabled(false);
        m_buttonDiffuseCol->setEnabled(false);
        m_diffuseLabel->setEnabled(false);
    }
    else
    {
        toggleAmbient();
        toggleDiffuse();
    }
}

void Window::openNormalMapDialog()
{
    QString file = QFileDialog::getOpenFileName(this, "open file", "../../3dmodels", "Image (*.png)");
    if( !file.isEmpty() )
    {
        m_glViewer->setNormalMap(file);
        m_toggleNormalMap->setEnabled(true);
        m_normalMapLoaded = true;
    }
}

void Window::openMetalMapDialog()
{
    QString file = QFileDialog::getOpenFileName(this, "open file", "../../3dmodels", "Image (*.png)");
    if( !file.isEmpty() )
    {
        m_glViewer->setMetalMap(file);
        m_togglePBRMap->setEnabled(true);
        m_metalMapLoaded = true;
    }
}

void Window::openGlossMapDialog()
{
    QString file = QFileDialog::getOpenFileName(this, "open file", "../../3dmodels", "Image (*.png)");
    if( !file.isEmpty() )
    {
        m_glViewer->setGlossMap(file);
        m_togglePBRMap->setEnabled(true);
        m_glossMapLoaded = true;
    }
}

void Window::openAmbMapDialog()
{
    QString file = QFileDialog::getOpenFileName(this, "open file", "../../3dmodels", "Image (*.png)");
    if( !file.isEmpty() )
    {
        m_glViewer->setAmbientMap(file);
        m_toggleAmbientMap->setEnabled(true);
        m_ambientMapLoaded = true;
    }
}

void Window::toggleAmbMap()
{
    if( m_toggleAmbientMap->isChecked() )
    {
        m_buttonAmbientCol->setEnabled(false);
        m_ambientLabel->setEnabled(false);
    }
    else
    {
        toggleAmbient();
    }
}

void Window::openCubeMapDialog()
{
    // Select directory instead of files
    QString folder = QFileDialog::getExistingDirectory(this, "Open  folder", "../../3dmodels/cubemaps", QFileDialog::ShowDirsOnly );
    if( !folder.isEmpty() )
    {
        m_glViewer->setCubeMap(folder);
        m_toggleEnvMap->setEnabled(true);
        m_cubeMapLoaded = true;

        // Adapt specular power to prefiltered cubemap
        std::string dirname = folder.toStdString();
        std::string value = dirname.substr(dirname.find_last_of("/") + 1);

        double valueD = -1.0f;
        try
        {
            valueD = std::stod(value);
        }
        catch(const std::invalid_argument&)
        {
            std::cerr << "WARNING: [Window::openCubeMapDialog()] No prefiltered value in cube map directory name, specular power is not modified " << std::endl;
        }
        catch(const std::out_of_range&)
        {
            std::cerr << "WARNING: [Window::openCubeMapDialog()] Prefiltered value in cube map directory name is out of range, specular power is not modified " << std::endl;
        }

        if(valueD > 0.0f)
            m_specPowSpinBox->setValue(valueD);
    }
}

void Window::lapSmooth()
{
    m_glViewer->lapSmooth( m_nbIterSpinBox->value(), m_factorSpinBox->value() );
}