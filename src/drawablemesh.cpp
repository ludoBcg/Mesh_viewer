#include "drawablemesh.h"


DrawableMesh::DrawableMesh()
{
    /* TODO */
    //m_defaultVAO = 0;
    m_ambientColor = glm::vec3(0.0f, 0.0f, 0.1f);
    m_diffuseColor = glm::vec3(0.95f, 0.5f, 0.25f);
    m_specularColor = glm::vec3(0.0f, 0.8f, 0.0f);

    m_wireColor = glm::vec3(0.5f, 0.5f, 0.5f);

    m_specPow = 128.0f;

    setAmbientFlag(true);
    setDiffuseFlag(true);
    setSpecularFlag(true);
    setAlbedoTexFlag(false);
    setEnvMapFlag(false);
    setNormalMapFlag(false);
    setPBRFlag(false);
    setAmbMapFlag(false);
    setShowNormalFlag(false);
    setUseGammaCorrecFlag(true);

    m_vertexProvided = false;
    m_normalProvided = false;
    m_colorProvided = false;
    m_tangentProvided = false;
    m_bitangentProvided = false;
    m_uvProvided = false;
    m_indexProvided = false;

    m_shadedRenderOn = true;
    m_wireframeRenderOn = false;

    // Load wireframe program
    m_programWF = loadShaderProgram("D:/Documents/dev/test_project/src/shaders/wireframe.vert", "D:/Documents/dev/test_project/src/shaders/wireframe.frag");
}


DrawableMesh::~DrawableMesh()
{
    glDeleteBuffers(1, &(m_vertexVBO));
    glDeleteBuffers(1, &(m_normalVBO));
    glDeleteBuffers(1, &(m_colorVBO));
    glDeleteBuffers(1, &(m_tangentVBO));
    glDeleteBuffers(1, &(m_bitangentVBO));
    glDeleteBuffers(1, &(m_uvVBO));
    glDeleteBuffers(1, &(m_indexVBO));
    glDeleteVertexArrays(1, &(m_meshVAO));
}


void DrawableMesh::createVAO(Mesh* _triMesh)
{
    fillVAO(_triMesh, true);
}


void DrawableMesh::updateVAO(Mesh* _triMesh)
{
    fillVAO(_triMesh, false);
}


void DrawableMesh::fillVAO(Mesh* _triMesh, bool _create)
{
    // mandatory data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;      // !! uint32_t !!

    // optional data
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> texcoords;   // !! vec2 !!
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    _triMesh->getVertices(vertices);
    _triMesh->getNormals(normals);
    _triMesh->getIndices(indices);

    _triMesh->getColors(colors);
    _triMesh->getTexCoords(texcoords);
    _triMesh->getTangents(tangents);
    _triMesh->getBitangents(bitangents);

    // update flags according to data provided
    vertices.size() ?  m_vertexProvided = true :  m_vertexProvided = false;
    normals.size() ?  m_normalProvided = true :  m_normalProvided = false;
    indices.size() ?  m_indexProvided = true :  m_indexProvided = false;
    colors.size() ?  m_colorProvided = true :  m_colorProvided = false;
    texcoords.size() ?  m_uvProvided = true :  m_uvProvided = false;
    tangents.size() ?  m_tangentProvided = true :  m_tangentProvided = false;
    bitangents.size() ?  m_bitangentProvided = true :  m_bitangentProvided = false;

    if(!m_vertexProvided)
        std::cerr << "WARNING: [DrawableMesh::createVAO()] No vertex provided" << std::endl;
    if(!m_normalProvided)
        std::cerr << "WARNING: [DrawableMesh::createVAO()] No normal provided" << std::endl;
    if(!m_indexProvided)
        std::cerr << "WARNING: [DrawableMesh::createVAO()] No index provided" << std::endl;

    // Generates and populates a VBO for vertex coords
    if(_create)
        glGenBuffers(1, &(m_vertexVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    size_t verticesNBytes = vertices.size() * sizeof(vertices[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesNBytes, vertices.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for vertex normals
    if(_create)
        glGenBuffers(1, &(m_normalVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    size_t normalsNBytes = normals.size() * sizeof(normals[0]);
    glBufferData(GL_ARRAY_BUFFER, normalsNBytes, normals.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for the element indices
    if(_create)
        glGenBuffers(1, &(m_indexVBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    auto indicesNBytes = indices.size() * sizeof(indices[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNBytes, indices.data(), GL_STATIC_DRAW);


    // Generates and populates a VBO for vertex colors
    if(_create)
        glGenBuffers(1, &(m_colorVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    if(m_colorProvided)
    {
        size_t colorsNBytes = colors.size() * sizeof(colors[0]);
        glBufferData(GL_ARRAY_BUFFER, colorsNBytes, colors.data(), GL_STATIC_DRAW);
    }
    else
    {
        size_t colorsNBytes = 1.0f * sizeof(colors[0]);
        glBufferData(GL_ARRAY_BUFFER, colorsNBytes, nullptr, GL_STATIC_DRAW);
    }


    // Generates and populates a VBO for UV coords
    if(_create)
        glGenBuffers(1, &(m_uvVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
    if(m_uvProvided) 
    {
        size_t texcoordsNBytes = texcoords.size() * sizeof(texcoords[0]);
        glBufferData(GL_ARRAY_BUFFER, texcoordsNBytes, texcoords.data(), GL_STATIC_DRAW);
    }
    else
    {
        size_t texcoordsNBytes = 1.0f * sizeof(texcoords[0]);
        glBufferData(GL_ARRAY_BUFFER, texcoordsNBytes, nullptr, GL_STATIC_DRAW);
    }

    // Generates and populates a VBO for tangent vectors
    if(_create)
        glGenBuffers(1, &(m_tangentVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
    if(m_tangentProvided) 
    {
        size_t tangentsNBytes = tangents.size() * sizeof(tangents[0]);
        glBufferData(GL_ARRAY_BUFFER, tangentsNBytes, tangents.data(), GL_STATIC_DRAW);
    }
    else
    {
        size_t tangentsNBytes = 1.0f * sizeof(tangents[0]);
        glBufferData(GL_ARRAY_BUFFER, tangentsNBytes, nullptr, GL_STATIC_DRAW);
    }

    // Generates and populates a VBO for bitangent vectors
    if(_create)
        glGenBuffers(1, &(m_bitangentVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_bitangentVBO);
    if(m_bitangentProvided) 
    {
        size_t bitangentsNBytes = bitangents.size() * sizeof(bitangents[0]);
        glBufferData(GL_ARRAY_BUFFER, bitangentsNBytes, bitangents.data(), GL_STATIC_DRAW);
    }
    else
    {
        size_t bitangentsNBytes = 1.0f * sizeof(bitangents[0]);
        glBufferData(GL_ARRAY_BUFFER, bitangentsNBytes, nullptr, GL_STATIC_DRAW);
    }

    // Creates a vertex array object (VAO) for drawing the mesh
    if(_create)
        glGenVertexArrays(1, &(m_meshVAO));
    glBindVertexArray(m_meshVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    glEnableVertexAttribArray(NORMAL);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    glEnableVertexAttribArray(COLOR);
    glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
    glEnableVertexAttribArray(UV);
    glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
    glEnableVertexAttribArray(TANGENT);
    glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_bitangentVBO);
    glEnableVertexAttribArray(BITANGENT);
    glVertexAttribPointer(BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    glBindVertexArray(m_defaultVAO); // unbinds the VAO

    // Additional information required by draw calls
    m_numVertices = vertices.size();
    m_numIndices = indices.size();

    // Clear temporary vectors
    vertices.clear();
    normals.clear();
    indices.clear();
    colors.clear();
    texcoords.clear();
    tangents.clear();
    bitangents.clear();
}


void DrawableMesh::draw(glm::mat4 _mv, glm::mat4 _mvp, glm::vec3 _lightPos, glm::vec3 _lightCol)
{
    if(m_shadedRenderOn)
    {
        // Activate program
        glUseProgram(m_program);

        // Bind textures
        if(m_useAlbedoTex)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_albedoTex);
        }
        if(m_useNormalMap)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_normalMap);
        }
        if(m_usePBR)
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_metalMap);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, m_glossMap);
        }
        if(m_useAmbMap)
        {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, m_ambientMap);
        }
        if(m_useEnvMap)
        {
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
        }
        // ...

        // Pass uniforms
        glUniformMatrix4fv(glGetUniformLocation(m_program, "u_mv"), 1, GL_FALSE, &_mv[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_program, "u_mvp"), 1, GL_FALSE, &_mvp[0][0]);
        glUniform3fv(glGetUniformLocation(m_program, "u_lightPosition"), 1, &_lightPos[0]);

        glUniform3fv(glGetUniformLocation(m_program, "u_lightColor"), 1, &_lightCol[0]);

        glUniform3fv(glGetUniformLocation(m_program, "u_ambientColor"), 1, &m_ambientColor[0]);
        glUniform3fv(glGetUniformLocation(m_program, "u_diffuseColor"), 1, &m_diffuseColor[0]);
        glUniform3fv(glGetUniformLocation(m_program, "u_specularColor"), 1, &m_specularColor[0]);
        glUniform1f(glGetUniformLocation(m_program, "u_specularPower"), m_specPow);
 
        glUniform1i(glGetUniformLocation(m_program, "u_albedoTex"), 0);
        glUniform1i(glGetUniformLocation(m_program, "u_normalMap"), 1);
        glUniform1i(glGetUniformLocation(m_program, "u_metalMap"), 2);
        glUniform1i(glGetUniformLocation(m_program, "u_glossMap"), 3);
        glUniform1i(glGetUniformLocation(m_program, "u_ambientMap"), 4);
        glUniform1i(glGetUniformLocation(m_program, "u_cubemap"), 5);

        if(m_useAmbient)
            glUniform1i(glGetUniformLocation(m_program, "u_useAmbient"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useAmbient"), 0);
        if(m_useDiffuse)
            glUniform1i(glGetUniformLocation(m_program, "u_useDiffuse"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useDiffuse"), 0);
        if(m_useSpecular)
            glUniform1i(glGetUniformLocation(m_program, "u_useSpecular"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useSpecular"), 0);

        if(m_useAlbedoTex)
            glUniform1i(glGetUniformLocation(m_program, "u_useAlbedoTex"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useAlbedoTex"), 0);
        if(m_useNormalMap)
            glUniform1i(glGetUniformLocation(m_program, "u_useNormalMap"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useNormalMap"), 0);
        if(m_usePBR)
            glUniform1i(glGetUniformLocation(m_program, "u_usePBR"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_usePBR"), 0);
         if(m_useAmbMap)
            glUniform1i(glGetUniformLocation(m_program, "u_useAmbMap"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useAmbMap"), 0);
        if(m_useEnvMap)
            glUniform1i(glGetUniformLocation(m_program, "u_useEnvMap"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useEnvMap"), 0);

        if(m_showNormals)
            glUniform1i(glGetUniformLocation(m_program, "u_showNormals"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_showNormals"), 0);

        if(m_useGammaCorrec)
            glUniform1i(glGetUniformLocation(m_program, "u_useGammaCorrec"), 1);
        else
            glUniform1i(glGetUniformLocation(m_program, "u_useGammaCorrec"), 0);
        // ...

        // Draw!
        glBindVertexArray(m_meshVAO);                       // bind the VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);  // do not forget to bind the index buffer AFTER !

        if(m_wireframeRenderOn)
        {
            // polygon offset to avoid z-fighting artefacts when wireframe overlay is activated
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.5, 0.5);
        }
        glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
        if(m_wireframeRenderOn)
            glDisable(GL_POLYGON_OFFSET_FILL);
        glBindVertexArray(m_defaultVAO);


    }
    if(m_wireframeRenderOn)
    {
        // Activate program
        glUseProgram(m_programWF);

        // Pass uniforms
        glUniformMatrix4fv(glGetUniformLocation(m_programWF, "u_mv"), 1, GL_FALSE, &_mv[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programWF, "u_mvp"), 1, GL_FALSE, &_mvp[0][0]);
        glUniform3fv(glGetUniformLocation(m_programWF, "u_lightPosition"), 1, &_lightPos[0]);
        glUniform3fv(glGetUniformLocation(m_programWF, "u_diffuseColor"), 1, &m_wireColor[0]);

        // Draw!
        glBindVertexArray(m_meshVAO);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glBindVertexArray(m_defaultVAO);
    }

    glUseProgram(0);
}


GLuint DrawableMesh::load2DTexture(const std::string& _filename, bool _repeat)
{
    std::vector<unsigned char> data;
    unsigned width, height;
    unsigned error = lodepng::decode(data, width, height, _filename);
    if (error != 0) 
    {
        std::cerr << "ERROR : [DrawableMesh::load2DTexture()] " << lodepng_error_text(error) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if(!_repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );  
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); 
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(data[0]));
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}


GLuint DrawableMesh::loadCubemap(const std::string& _dirname)
{
    bool isValid = true;
    const char *filenames[] = { "posx.png", "negx.png", "posy.png", "negy.png", "posz.png", "negz.png" };
    const GLenum targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
    const unsigned num_sides = 6; 

    std::vector<unsigned char> data[num_sides];
    unsigned width;
    unsigned height;
    for (unsigned i = 0; i < num_sides; ++i) 
    {
        std::string filename = _dirname + "/" + filenames[i];
        unsigned error = lodepng::decode(data[i], width, height, filename);
        if (error != 0) 
        {
            std::cerr << "ERROR: [DrawableMesh::loadCubemap()] " << lodepng_error_text(error) << std::endl;
            isValid = false;
//            std::exit(EXIT_FAILURE);
        }
    }

    GLuint texture;
    if(isValid)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        for (unsigned i = 0; i < num_sides; ++i) 
        {
            glTexImage2D(targets[i], 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(data[i][0]));
        }
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    return texture;

}


GLuint DrawableMesh::loadShaderProgram(const std::string& _vertShaderFilename, const std::string& _fragShaderFilename)
{
    // Load and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderSource = readShaderSource(_vertShaderFilename);
    const char *vertexShaderSourcePtr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, nullptr);

    glCompileShader(vertexShader);
    GLint compiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) 
    {
        std::cerr << "Vertex shader compilation failed:" << std::endl;
        showShaderInfoLog(vertexShader);
        glDeleteShader(vertexShader);
        return 0;
    }

    // Load and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderSource = readShaderSource(_fragShaderFilename);
    const char *fragmentShaderSourcePtr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, nullptr);

    glCompileShader(fragmentShader);
    compiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) 
    {
        std::cerr << "Fragment shader compilation failed:" << std::endl;
        showShaderInfoLog(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // Create program object
    GLuint program = glCreateProgram();

    // Attach shaders to the program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link program
    glLinkProgram(program);

    // Check linking status
    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) 
    {
        std::cerr << "Linking failed:" << std::endl;
        showProgramInfoLog(program);
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // Clean up
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}


std::string DrawableMesh::readShaderSource(const std::string& _filename)
{
    std::ifstream file(_filename);
    std::stringstream stream;
    stream << file.rdbuf();

    return stream.str();
}


void DrawableMesh::showShaderInfoLog(GLuint _shader)
{
    GLint infoLogLength = 0;
    glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> infoLog(infoLogLength);
    glGetShaderInfoLog(_shader, infoLogLength, &infoLogLength, &infoLog[0]);
    std::string infoLogStr(infoLog.begin(), infoLog.end());
    std::cerr << infoLogStr << std::endl;
}


void DrawableMesh::showProgramInfoLog(GLuint _program)
{
    GLint infoLogLength = 0;
    glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> infoLog(infoLogLength);
    glGetProgramInfoLog(_program, infoLogLength, &infoLogLength, &infoLog[0]);
    std::string infoLogStr(infoLog.begin(), infoLog.end());
    std::cerr << infoLogStr << std::endl;
}
