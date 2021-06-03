#ifndef SCENE_H
#define SCENE_H

#include "trimeshsoup.h"
#include "drawablemesh.h"


/*!
* \struct Object
* \brief One oject represented by its mesh, VAO, and modelview matrix
*/
struct Object 
{
    Mesh* m_mesh;
    DrawableMesh* m_drawableMesh;
    glm::mat4 m_mMatrix;
};

/*!
* \struct LightSource
* \brief One light source represented by position and color
*/
struct LightSource 
{
    glm::vec3 m_pos;
    glm::vec3 m_col;
    glm::mat4 m_mMatrix;
};



/*!
* \class Scene
* \brief This class represents a scene, containing several objects
*/
class Scene
{
    public:

        /*------------------------------------------------------------------------------------------------------------+
        |                                        CONSTRUCTORS / DESTRUCTORS                                           |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn Scene
        * \brief Default constructor of Scene
        */
        Scene();

        /*!
        * \fn ~Scene
        * \brief Destructor of Scene
        */
        ~Scene();

        /*------------------------------------------------------------------------------------------------------------+
        |                                              GETTERS/SETTERS                                                |
        +-------------------------------------------------------------------------------------------------------------*/


        /*------------------------------------------------------------------------------------------------------------+
        |                                               OTHER METHODS                                                 |
        +-------------------------------------------------------------------------------------------------------------*/

        void initFBOs(int _windowWidth, int _windowHeight);

        void drawScene(glm::mat4 _mv, glm::mat4 _p, glm::vec3 _camPos/*, glm::vec3 _lightPos, glm::vec3 _lightCol*/);

        void updateFrame();

    protected:

        /*------------------------------------------------------------------------------------------------------------+
        |                                                ATTRIBUTES                                                   |
        +-------------------------------------------------------------------------------------------------------------*/

        std::vector<Object> m_listOjects;                   /*!< List of Objects in the scene */
        std::vector<LightSource> m_listLightSources;        /*!< List of LightSources in the scene */

        std::vector<GLuint> m_ListFBOs;                     /*!< List of FBOs for defered rendering */
        std::vector<GLuint> m_listTextures;                 /*!< List of Textures for defered rendering */

        GLuint m_FBO;
        GLuint m_tex;

        GLint m_oldFBO;
        GLint m_oldTex2d;
        GLint m_qtFBO;


        /*------------------------------------------------------------------------------------------------------------+
        |                                               OTHER METHODS                                                 |
        +-------------------------------------------------------------------------------------------------------------*/

        void initTex2D(int _width, int _height, GLuint _texture2D);
        void initFBO(GLuint _texture2D, GLuint _FBO);

};
#endif // SCENE_H