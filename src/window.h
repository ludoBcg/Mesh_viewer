#ifndef WINDOW_H
#define WINDOW_H


#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QToolTip>


class Viewer;

/*!
* \class Window
* \brief Main application window
* Inherit from QWidget
*/
class Window : public QWidget
{
    Q_OBJECT

    public:
        Window();
        ~Window();

    private:

        /********************************************* Main Window ********************************************/
        QVBoxLayout* m_globalLayout;        /*!< Global vertical layout of the window */

        QHBoxLayout* m_toolbarLayout;       /*!< Horizontal layout of the toolbar */
        QPushButton* m_buttonShowDB;        /*!< Button to show/hide dialog box */
        QPushButton* m_buttonLoadMeshHE;    /*!< Button to load a TriMeshHE */
        QPushButton* m_buttonLoadMeshSoup;  /*!< Button to load a TriMeshSoup */
        QPushButton* m_buttonSaveMesh;      /*!< Button to save a Mesh */

        Viewer* m_glViewer;                 /*!< QGLViewer, contains the OpenGL context */


        /********************************************* Dialog Box *********************************************/
        QDialog* m_dialogBox;               /*!< Dialog Box */
        QVBoxLayout* m_boxGlobalLayout;     /*!< Global layout of the dialog box */

        QGroupBox* m_groupBoxScene;         /*!< GroupBox for scene setup */
        QVBoxLayout* m_boxSceneLayout;      /*!< Layout for scene setup */
        QPushButton* m_buttonShader;        /*!< Button to select shader files */
        QHBoxLayout* m_backColLayout;       /*!< Horizontal layout for background color */
        QLabel* m_backColLabel;             /*!< Label for background color */
        QPushButton* m_buttonBackCol;       /*!< Button to change background color */
        QHBoxLayout* m_lightColLayout;      /*!< Horizontal layout for light color */
        QLabel* m_lightColLabel;            /*!< Label for light color */
        QPushButton* m_buttonLightCol;      /*!< Button to change light color */
        QCheckBox* m_toggleShowSurf;        /*!< CheckBox to show/hide surface */
        QHBoxLayout* m_wireLayout;          /*!< Horizontal layout for wireframe options */
        QCheckBox* m_toggleShowLines;       /*!< CheckBox to show/hide edges */
        QLabel* m_wireLabel;                /*!< Label for wireframe color */
        QPushButton* m_buttonWireColor;     /*!< Button for wireframe color */

        QGroupBox* m_groupBoxShading;       /*!< GroupBox for shading options */
        QVBoxLayout* m_boxShadingLayout;    /*!< Layout for shading options */
       
        QCheckBox* m_toggleAmbient;         /*!< CheckBox to activate/deactivate ambient shading */
        QHBoxLayout* m_ambientLayout;       /*!< Horizontal layout for ambient shading */
        QLabel* m_ambientLabel;             /*!< Label for ambient color */
        QPushButton* m_buttonAmbientCol;    /*!< Button to change ambient color */ 
        QCheckBox* m_toggleDiffuse;         /*!< CheckBox to activate/deactivate diffuse shading */
        QHBoxLayout* m_diffuseLayout;       /*!< Horizontal layout for diffuse shading */
        QLabel* m_diffuseLabel;             /*!< Label for diffuse color */
        QPushButton* m_buttonDiffuseCol;    /*!< Button to change diffuse color */
        QCheckBox* m_toggleSpecular;        /*!< CheckBox to activate/deactivate specular shading */
        QHBoxLayout* m_specularLayout;      /*!< Horizontal layout for ambient shading */
        QLabel* m_specularLabel;            /*!< Label for specular color */
        QPushButton* m_buttonSpecularCol;   /*!< Button to change specular color */
        QHBoxLayout* m_specPowLayout;       /*!< Horizontal layout for specular power */
        QDoubleSpinBox* m_specPowSpinBox;   /*!< SpinBox to change specular power */
        QLabel* m_specPowLabel;             /*!< Label for specular power */
        QCheckBox* m_toggleShowNormals;     /*!< CheckBox to activate/deactivate rendering of normal vectors */
        QCheckBox* m_toggleGammaCorrec;     /*!< CheckBox to activate/deactivate gamma correction */

        QGroupBox* m_groupBoxTex;           /*!< GroupBox for texture mapping options */
        QVBoxLayout* m_boxTexLayout;        /*!< Layout for texture mapping options */
        QHBoxLayout* m_albedoTexLayout;       /*!< Horizontal layout for albedo texture */
        QPushButton* m_buttonLoadAlbedoTex; /*!< Button to load albedo texture */
        QCheckBox* m_toggleAlbedoTex;       /*!< CheckBox to activate/deactivate albedo texture mapping */
        QHBoxLayout* m_normalMapLayout;       /*!< Horizontal layout for normal map */
        QPushButton* m_buttonLoadNormalMap; /*!< Button to load normal map */
        QCheckBox* m_toggleNormalMap;       /*!< CheckBox to activate/deactivate normal mapping */
        //QPushButton* m_buttonLoadMetalMap;  /*!< Button to load metal map */
        //QHBoxLayout* m_PBRMapLayout;       /*!< Horizontal layout for PBR mapping */
        //QPushButton* m_buttonLoadGlossMap;  /*!< Button to load gloss map */
        //QCheckBox* m_togglePBRMap;          /*!< CheckBox to activate/deactivate PBR mapping */
        //QHBoxLayout* m_ambientMapLayout;       /*!< Horizontal layout for ambient map */
        //QPushButton* m_buttonLoadAmbientMap;/*!< Button to load ambient map */
        //QCheckBox* m_toggleAmbientMap;      /*!< CheckBox to activate/deactivate ambient mapping */
        //QHBoxLayout* m_cubeMapLayout;       /*!< Horizontal layout for cube map */
        //QPushButton* m_buttonLoadCubeMap;   /*!< Button to load cube map */
        //QCheckBox* m_toggleEnvMap;          /*!< CheckBox to activate/deactivate environment mapping */

        QGroupBox* m_groupBoxGeom;          /*!< GroupBox for geometry tools */
        QVBoxLayout* m_boxGeomLayout;       /*!< Layout for geometry tools */
        QPushButton* m_buttonCompNormals;   /*!< Button to recompute geometric normals */
        QPushButton* m_buttonCompTB;        /*!< Button to compute tangents and bitangents */
        QPushButton* m_buttonLapSmooth;     /*!< Button to compute Laplacian smoothing */
        QHBoxLayout* m_smoothParamLayout;   /*!< Horizontal layout for Laplacian smoothing parameters */
        QSpinBox* m_nbIterSpinBox;          /*!< SpinBox to change number of iterations */
        QLabel* m_nbIterLabel;              /*!< Label for number of iterations */
        QDoubleSpinBox* m_factorSpinBox;    /*!< SpinBox to change smoothing factor */
        QLabel* m_factorLabel;              /*!< Label for smoothing factor */


        bool m_albedoTexLoaded;             /*!< True is an albedo texture is loaded */
        bool m_normalMapLoaded;             /*!< True is a normal map is loaded */
        //bool m_metalMapLoaded;              /*!< True is a metal map is loaded */  
        //bool m_glossMapLoaded;              /*!< True is a gloss map is loaded */
        //bool m_ambientMapLoaded;            /*!< True is a ambient map is loaded */  
        //bool m_cubeMapLoaded;               /*!< True is a cube map is loaded */  

        QColor m_ambientCol;                /*!< Current ambient color */
        QColor m_diffuseCol;                /*!< Current diffuse color */
        QColor m_specularCol;               /*!< Current specular color */
        QColor m_lightCol;                  /*!< Current light color */
        QColor m_backCol;                   /*!< Current background color */
        QColor m_wireCol;                   /*!< Current wire color */

        /*!
        * \fn moveEvent
        * \brief Override move event of the main window
        *        Call positionDialog() each time the main window is moved
        */
        void moveEvent(QMoveEvent *e);

        /*!
        * \fn positionDialog
        * \brief Compute position of the dialog box relatively to main window position
        * \param _mainWinGeom: geometry of the main window
        */
        void positionDialog(QRect *_mainWinGeom);

    public slots:

        /*!
        * \fn popDialog
        * \brief SLOT: show/hide dialog box
        */
        void popDialog();

        /*!
        * \fn rejectDialog
        * \brief SLOT: handle dialog box closing using Close [x] button
        */
        void rejectDialog();

        /*!
        * \fn loadMeshHE
        * \brief SLOT: open dialog box to load a TriMeshHE
        */
        void loadMeshHE();

        /*!
        * \fn loadMeshSoup
        * \brief SLOT: open dialog box to load a TriMeshSoup
        */
        void loadMeshSoup();

        /*!
        * \fn saveMesh
        * \brief SLOT: open dialog box to save Mesh
        */
        void saveMesh();

        /*!
        * \fn openShaderDialog
        * \brief SLOT: open dialog box to select shader files
        */
        void openShaderDialog();

        /*!
        * \fn selectBackColor
        * \brief SLOT: open dialog box to select background color
        */
        void selectBackColor();

        /*!
        * \fn selectLightColor
        * \brief SLOT: open dialog box to select ligth color
        */
        void selectLightColor();

        /*!
        * \fn toggleLines
        * \brief SLOT: activate/deactivate wireframe rendering
        */
        void toggleLines();

        /*!
        * \fn selectWireColor
        * \brief SLOT: open dialog box to select wireframe color
        */
        void selectWireColor();

        /*!
        * \fn selectAmbientColor
        * \brief SLOT: open dialog box to select ambient material color
        */
        void selectAmbientColor();

        /*!
        * \fn selectDiffuseColor
        * \brief SLOT: open dialog box to select diffuse material color
        */
        void selectDiffuseColor();

        /*!
        * \fn selectSpecularColor
        * \brief SLOT: open dialog box to select specular material color
        */
        void selectSpecularColor();

        /*!
        * \fn toggleAmbient
        * \brief SLOT: enable/disable ambient color selection according to m_toggleAmbient state
        */
        void toggleAmbient();

        /*!
        * \fn toggleDiffuse
        * \brief SLOT: enable/disable ambient color selection according to m_toggleAmbient state
        */
        void toggleDiffuse();

        /*!
        * \fn toggleSpecular
        * \brief SLOT: enable/disable specular power SpinBox according to m_toggleSpecular state
        */
        void toggleSpecular();

        /*!
        * \fn toggleShowNormals
        * \brief SLOT: enable/disable shading options according to m_toggleShowNormals state
        */
        void toggleShowNormals();

        /*!
        * \fn openTexDialog
        * \brief SLOT: open dialog box to select albedo map texture
        */
        void openAlbedoTexDialog();

        /*!
        * \fn toggleTex
        * \brief SLOT: use texture mapping instead of diffuse + ambient color
        */
        void toggleTex();

        /*!
        * \fn openNormalMapDialog
        * \brief SLOT: open dialog box to select normal map texture
        */
        void openNormalMapDialog();

        /*!
        * \fn openMetalMapDialog
        * \brief SLOT: open dialog box to select metal map texture
        */
        //void openMetalMapDialog();

        /*!
        * \fn openGlossMapDialog
        * \brief SLOT: open dialog box to select gloss map texture
        */
        //void openGlossMapDialog();

        /*!
        * \fn toggleAmbMap
        * \brief SLOT: use ambient mapping instead of ambient default factor
        */
        //void toggleAmbMap();

        /*!
        * \fn openAmbMapDialog
        * \brief SLOT: open dialog box to select ambient map texture
        */
        //void openAmbMapDialog();

        /*!
        * \fn openCubeMapDialog
        * \brief SLOT: open dialog box to select cube map texture set
        */
        //void openCubeMapDialog();

        /*!
        * \fn lapSmooth
        * \brief SLOT: laplacian smoothing of the mesh
        */
        void lapSmooth();
};

#endif // WINDOW_H