//#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "scene.h"


Scene::Scene()
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO); std::cout<<"oldFBO = "<<m_oldFBO<<std::endl;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_oldTex2d);

    Object object, object2, object3;

    object.m_mesh = new TriMeshSoup(true, false, false);
    object.m_mesh->readFile("D:/Documents/dev/CG_chips/3dmodels/gargo.obj");
    object.m_mesh->computeAABB();

    object.m_drawableMesh = new DrawableMesh;
    object.m_drawableMesh->setProgram("D:/Documents/dev/CG_chips/scene_viewer/src/shaders/phong.vert", "D:/Documents/dev/CG_chips/scene_viewer/src/shaders/phong.frag");
    object.m_drawableMesh->createVAO(object.m_mesh);
    object.m_drawableMesh->setAmbientColor(10, 10, 10);
    object.m_drawableMesh->setDiffuseColor(255, 0, 0);
    object.m_drawableMesh->setSpecularColor(200, 200, 200);

    glm::mat4 Model;
    Model = glm::rotate(Model, /*(glm::mediump_float)1.0*/90.f, glm::vec3(0, 1, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
    //Model = glm::rotate( 3.141592f / 1.0f, glm::vec3(0, 1, 0)); // angle in radian because macro GLM_FORCE_RADIANS
    object.m_mMatrix = Model;

    m_listOjects.push_back(object);



    object2.m_mesh = new TriMeshSoup(true, false, false);
    object2.m_mesh->readFile("D:/Documents/dev/CG_chips/3dmodels/bunny.obj");
    object2.m_mesh->computeAABB();

    object2.m_drawableMesh = new DrawableMesh;
    object2.m_drawableMesh->setProgram("D:/Documents/dev/CG_chips/scene_viewer/src/shaders/phong.vert", "D:/Documents/dev/CG_chips/scene_viewer/src/shaders/phong.frag");
    object2.m_drawableMesh->createVAO(object2.m_mesh);
    object2.m_drawableMesh->setAmbientColor(10, 10, 10);
    object2.m_drawableMesh->setDiffuseColor(0, 255, 0);
    object2.m_drawableMesh->setSpecularColor(200, 200, 200);
    glm::mat4 Model2;
    Model2 = glm::translate( glm::vec3(1.0f, 0.0f, 0.0f) );
    object2.m_mMatrix = Model2;

    m_listOjects.push_back(object2);



    object3.m_mesh = new TriMeshSoup(true, false, false);
    object3.m_mesh->readFile("D:/Documents/dev/CG_chips/3dmodels/armadillo.obj");
    object3.m_mesh->computeAABB();

    object3.m_drawableMesh = new DrawableMesh;
    object3.m_drawableMesh->setProgram("D:/Documents/dev/CG_chips/scene_viewer/src/shaders/phong.vert", "D:/Documents/dev/CG_chips/scene_viewer/src/shaders/phong.frag");
    object3.m_drawableMesh->createVAO(object3.m_mesh);
    object3.m_drawableMesh->setAmbientColor(10, 10, 10);
    object3.m_drawableMesh->setDiffuseColor(0, 0, 255);
    object3.m_drawableMesh->setSpecularColor(200, 200, 200);
    glm::mat4 Model3;
    object3.m_mMatrix = Model3;

    m_listOjects.push_back(object3);


    // light source
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::mat4 ModelL;
//    ModelL = glm::translate( glm::vec3(10.0f, 0.0f, 0.0f) );
    LightSource LS;
    LS.m_col = lightCol;
    LS.m_pos = lightPos;
    LS.m_mMatrix = ModelL;
    m_listLightSources.push_back(LS);

}

Scene::~Scene()
{
    m_listOjects.clear();
    m_listLightSources.clear();
    m_ListFBOs.clear();
    m_listTextures.clear();
}

void Scene::initFBOs(int _windowWidth, int _windowHeight)
{
    //initTex2D(_windowWidth, _windowHeight, m_tex);
    //initFBO(m_tex, m_FBO);
}

void Scene::initTex2D(int _width, int _height, GLuint _texture2D)
{
    // create a 2D texture with screen's dimension for redering the col of voxels computed by ray casting
    glDeleteTextures(1, &_texture2D);
    glGenTextures(1, &_texture2D);
    glBindTexture(GL_TEXTURE_2D, _texture2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, _width, _height, 0, GL_RGBA, GL_UNSIGNED_SHORT, nullptr);
    glBindTexture(GL_TEXTURE_2D, m_oldTex2d);
}

void Scene::initFBO(GLuint _texture2D, GLuint _FBO)
{
    // create back face frame buffer
    glDeleteFramebuffers(1, &_FBO);
    glGenFramebuffers(1, &_FBO);
    // attach back face texture to back face frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture2D, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { std::cerr << "[Scene::initFBO] ERROR: Framebuffer is not complete" << std::endl; }
    glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);
}

void Scene::drawScene(glm::mat4 _mv, glm::mat4 _p, glm::vec3 _camPos/*, glm::vec3 _lightPos, glm::vec3 _lightCol*/)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_qtFBO);


    glBindFramebuffer(GL_FRAMEBUFFER, m_qtFBO);
    glDrawBuffer(GL_BACK);

    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    

    glm::mat4 tMat;
    tMat = glm::translate( glm::vec3(10.0f, 0.0f, 0.0f) );
    glm::mat4 rMat;
    rMat = m_listLightSources[0].m_mMatrix;

    glm::vec3 Lightpos = m_listLightSources[0].m_pos;
    glm::vec4 Lightpos2 = glm::vec4(Lightpos.x, Lightpos.y, Lightpos.z, 1.0f);

//    rMat = rMat * tMat;
    
    Lightpos2 = rMat * Lightpos2;
    Lightpos2 = tMat * Lightpos2;
    Lightpos2 = _mv * Lightpos2;
    
    Lightpos = glm::vec3(Lightpos2.x, Lightpos2.y, Lightpos2.z);

    for(unsigned int i = 0; i < m_listOjects.size(); i++)
    {
        //m_listOjects[i].m_drawableMesh->setShowNormalFlag(true);

        glm::mat4 mv = _mv * m_listOjects[i].m_mMatrix;
        glm::mat4 mvp = _p * mv;

        m_listOjects[i].m_drawableMesh->draw(mv, mvp, Lightpos, m_listLightSources[0].m_col);
    }

}

void Scene::updateFrame()
{
    glm::mat4 Model = m_listOjects[0].m_mMatrix;
    Model = glm::rotate(Model, 0.25f, glm::vec3(0, 1, 0));
    m_listOjects[0].m_mMatrix = Model;


    //glm::mat4 rMat;
    //rMat = m_listLightSources[0].m_mMatrix;
    //rMat = glm::rotate(rMat, 1.0f, glm::vec3(0, 1, 0));
    //m_listLightSources[0].m_mMatrix = rMat;

}