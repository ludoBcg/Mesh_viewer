/*********************************************************************************************************************
 *
 * window.h
 *
 * Main Qt window
 *
 * Mesh_viewer
 * Ludovic Blache
 *
 *********************************************************************************************************************/


#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"

#include <QWidget>
#include <QtWidgets>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QToolTip>
#include <QDropEvent>


// based on Qt Textures Example
// https://doc.qt.io/qt-6/qtopengl-textures-example.html

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

    private slots:

    private:

        /*!
        * \fn buildVisDialogBox
        * \brief Build visualization dialog box.
        */
        void buildVisDialogBox();
        
        /*!
        * \fn buildGeomDialogBox
        * \brief Build geometry dialog box.
        */
        void buildGeomDialogBox();

        /********************************************* Main Window ********************************************/

        QVBoxLayout* m_globalLayout;        /*!< Global vertical layout of the window */

        QHBoxLayout* m_toolbarLayout;       /*!< Horizontal layout of the toolbar */
        QPushButton* m_buttonShowVisDB;     /*!< Button to show/hide visualization dialog box */
        QPushButton* m_buttonShowGeomDB;    /*!< Button to show/hide geometry dialog box */
        QPushButton* m_buttonLoadMeshHE;    /*!< Button to load a TriMeshHE */
        QPushButton* m_buttonLoadMeshSoup;  /*!< Button to load a TriMeshSoup */
        QPushButton* m_buttonSaveMesh;      /*!< Button to save a Mesh */
        QPushButton* m_buttonHelp;          /*!< Button to show/hide help message box */

        GLWidget* m_glViewer;                 /*!< QGLViewer, contains the OpenGL context */


        /******************************************* Vis Dialog Box *******************************************/

        QDialog* m_visDialogBox;            /*!< Dialog Box for visualization tools */
        QVBoxLayout* m_visBoxGlobalLayout;  /*!< Global layout of the dialog box */

        QGroupBox* m_groupBoxScene;         /*!< GroupBox for scene setup */
        QVBoxLayout* m_boxSceneLayout;      /*!< Layout for scene setup */
        QHBoxLayout* m_sceneColLayout;      /*!< Layout for scene colors */
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
        QHBoxLayout* m_wireShadingLayout;   /*!< Horizontal layout for wireframe shading */
        QCheckBox* m_toggleShadingLines;    /*!< CheckBox to activate/deactivate wireframe shading */

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
        QHBoxLayout* m_boxMeshColLayout;    /*!< Horizontal layout for mesh coloring */
        QCheckBox* m_toggleShowNormals;     /*!< CheckBox to activate/deactivate rendering of normal vectors */
        QCheckBox* m_toggleMeshCol;         /*!< CheckBox to activate/deactivate mesh color rendering */
        QCheckBox* m_toggleFlatShading;     /*!< CheckBox to activate/deactivate flat shading */
        QCheckBox* m_toggleGammaCorrec;     /*!< CheckBox to activate/deactivate gamma correction */

        QGroupBox* m_groupBoxTex;           /*!< GroupBox for texture mapping options */
        QVBoxLayout* m_boxTexLayout;        /*!< Layout for texture mapping options */
        QHBoxLayout* m_texLayout;           /*!< Horizontal layout for texture */
        QPushButton* m_buttonLoadTex;       /*!< Button to load texture */
        QCheckBox* m_toggleTex;             /*!< CheckBox to activate/deactivate texture mapping */
        QHBoxLayout* m_normalMapLayout;     /*!< Horizontal layout for normal map */
        QPushButton* m_buttonLoadNormalMap; /*!< Button to load normal map */
        QCheckBox* m_toggleNormalMap;       /*!< CheckBox to activate/deactivate normal mapping */

        QGroupBox* m_groupBoxGeom;          /*!< GroupBox for geometry tools */
        QVBoxLayout* m_boxGeomLayout;       /*!< Layout for geometry tools */
        QPushButton* m_buttonDuplVertices;  /*!< Button to duplicate vertices */
        QPushButton* m_buttonCompNormals;   /*!< Button to recompute geometric normals */
        QPushButton* m_buttonCompTB;        /*!< Button to compute tangents and bitangents */
        QPushButton* m_buttonLapSmooth;     /*!< Button to compute Laplacian smoothing */
        QHBoxLayout* m_smoothParamLayout;   /*!< Horizontal layout for Laplacian smoothing parameters */
        QSpinBox* m_nbIterSpinBox;          /*!< SpinBox to change number of iterations */
        QLabel* m_nbIterLabel;              /*!< Label for number of iterations */
        QDoubleSpinBox* m_factorSpinBox;    /*!< SpinBox to change smoothing factor */
        QLabel* m_factorLabel;              /*!< Label for smoothing factor */
        QPushButton* m_buttonMeanCurv;      /*!< Button to compute mean curvature */
        QPushButton* m_buttonSurfVar;       /*!< Button to compute surface variation */


        /******************************************* Geom Dialog Box ******************************************/
        QDialog* m_geomDialogBox;            /*!< Dialog Box for geometry tools */
        QVBoxLayout* m_geomBoxGlobalLayout;  /*!< Global layout of the dialog box */


        bool m_texLoaded;                   /*!< True if a texture is loaded */
        bool m_normalMapLoaded;             /*!< True if a normal map is loaded */

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
        void moveEvent(QMoveEvent* e);

        /*!
        * \fn resizeEvent
        * \brief Override resize event of the main window
        *        Call positionDialog() each time the main window is resized
        */
        void resizeEvent(QResizeEvent* e);

        /*!
        * \fn positionDialog
        * \brief Compute position of the dialog box relatively to main window position
        * \param _mainWinGeom: geometry of the main window
        */
        void positionDialog(QRect* _mainWinGeom);


    public slots:

            /*!
            * \fn popVisDialog
            * \brief SLOT: show/hide visualization dialog box
            */
            void popVisDialog();

            /*!
            * \fn rejectVisDialog
            * \brief SLOT: handle visualization dialog box closing using Close [x] button
            */
            void rejectVisDialog();

            /*!
            * \fn popGeomDialog
            * \brief SLOT: show/hide geometry dialog box
            */
            void popGeomDialog();

            /*!
            * \fn rejectGeomDialog
            * \brief SLOT: handle geometry dialog box closing using Close [x] button
            */
            void rejectGeomDialog();

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
            * \fn help
            * \brief SLOT: open help message box
            */
            void help();

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
            * \brief SLOT: enable/disable normal coords rendering according to m_toggleShowNormals state
            */
            void toggleShowNormals();

            /*!
            * \fn toggleMeshCol
            * \brief SLOT: enable/disable mesh colors rendering according to m_toggleMeshCol state
            */
            void toggleMeshCol();

            /*!
            * \fn openTexDialog
            * \brief SLOT: open dialog box to select map texture
            */
            void openTexDialog();

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
            * \fn compNormals
            * \brief SLOT: duplicate vertices and compute normals
            */
            void compNormals();
            /*!
            * \fn lapSmooth
            * \brief SLOT: laplacian smoothing of the mesh
            */
            void lapSmooth();
};

#endif