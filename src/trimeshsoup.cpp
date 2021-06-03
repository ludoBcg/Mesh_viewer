#include "trimeshsoup.h"



TriMeshSoup::TriMeshSoup()
{
    m_TBComputed = false;

    m_bBoxMin = glm::vec3(0.0f, 0.0f, 0.0f);
    m_bBoxMax = glm::vec3(0.0f, 0.0f, 0.0f);
}


TriMeshSoup::TriMeshSoup(bool _normals, bool _texCoords2D, bool _col)
{
    m_TBComputed = false;

    m_bBoxMin = glm::vec3(0.0f, 0.0f, 0.0f);
    m_bBoxMax = glm::vec3(0.0f, 0.0f, 0.0f);
}


TriMeshSoup::~TriMeshSoup()
{
    clear();
}


void TriMeshSoup::getVertices(std::vector<glm::vec3>& _vertices)
{
    if(_vertices.size() != 0)
        _vertices.clear();

    if(m_vertices.size() != 0)
    {
        _vertices.assign(m_vertices.begin(), m_vertices.end());
    }
    else
    {
        std::cerr << "WARNING: [TriMeshSoup::getVertices] Empty vertices array" << std::endl;
    }
}


void TriMeshSoup::getNormals(std::vector<glm::vec3>& _normals)
{
    if(_normals.size() != 0)
        _normals.clear();

    if(m_normals.size() != 0)
    {
        _normals.assign(m_normals.begin(), m_normals.end());
    }
    else
    {
        std::cerr << "WARNING: [TriMeshSoup::getNormals] Empty normals array" << std::endl;
    }
}


void TriMeshSoup::getIndices(std::vector<uint32_t>& _indices)
{
    if(_indices.size() != 0)
        _indices.clear();

    if(m_indices.size() != 0)
    {
        _indices.assign(m_indices.begin(), m_indices.end());
    }
    else
    {
        std::cerr << "WARNING: [TriMeshSoup::getIndices] Empty indices array" << std::endl;
    }
}


void TriMeshSoup::getColors(std::vector<glm::vec3>& _colors)
{
    if(_colors.size() != 0)
        _colors.clear();

    if(m_colors.size() != 0)
    {
        _colors.assign(m_colors.begin(), m_colors.end());
    }
}


void TriMeshSoup::getTexCoords(std::vector<glm::vec2>& _texcoords)
{
    if(_texcoords.size() != 0)
        _texcoords.clear();

    if(m_texcoords.size() != 0)
    {
        _texcoords.assign(m_texcoords.begin(), m_texcoords.end());
    }
}


void TriMeshSoup::getTangents(std::vector<glm::vec3>& _tangents)
{
    if(_tangents.size() != 0)
        _tangents.clear();

    if(m_tangents.size() != 0)
    {
        _tangents.assign(m_tangents.begin(), m_tangents.end());
    }
}


void TriMeshSoup::getBitangents(std::vector<glm::vec3>& _bitangents)
{
    if(_bitangents.size() != 0)
        _bitangents.clear();

    if(m_bitangents.size() != 0)
    {
        _bitangents.assign(m_bitangents.begin(), m_bitangents.end());
    }
}


bool TriMeshSoup::readFile(std::string _filename)
{
    if(_filename.substr(_filename.find_last_of(".") + 1) == "obj")
    {
        importOBJ(_filename);
        return true;
    }
    else if(_filename.substr(_filename.find_last_of(".") + 1) == "stl") 
    {
        importSTL(_filename);
        return true;
    }
    else
    {
        std::cerr << " ERROR: [TriMeshSoup::readFile()] Invalid file extension: only .obj and .stl are supported" << std::endl;
    }
    return false;
}


bool TriMeshSoup::writeFile(std::string _filename)
{
    if(_filename.substr(_filename.find_last_of(".") + 1) == "obj")
    {
        exportOBJ(_filename);
        return true;
    }
    else if(_filename.substr(_filename.find_last_of(".") + 1) == "stl") 
    {
        exportSTL(_filename);
        return true;
    }
    else
    {
        std::cerr << " ERROR: [TriMeshSoup::writeFile()] Invalid file extension: only .obj and .stl are supported" << std::endl;
    }
    return false;
}


void TriMeshSoup::computeAABB()
{
    if(m_vertices.size() != 0)
    {
        // init values
        glm::vec3 min = m_vertices[0];
        glm::vec3 max = m_vertices[0];

        for (unsigned int i = 0; i <m_vertices.size(); i++) 
        {
            if(m_vertices[i].x < min.x) { min.x = m_vertices[i].x; }
            if(m_vertices[i].y < min.y) { min.y = m_vertices[i].y; }
            if(m_vertices[i].z < min.z) { min.z = m_vertices[i].z; }
            if(m_vertices[i].x > max.x) { max.x = m_vertices[i].x; }
            if(m_vertices[i].y > max.y) { max.y = m_vertices[i].y; }
            if(m_vertices[i].z > max.z) { max.z = m_vertices[i].z; }
        }
        m_bBoxMin = min;
        m_bBoxMax = max;
    }
    else
    {
        std::cerr << "WARNING: [TriMeshSoup::computeAABB] Empty vertices array" << std::endl;
        m_bBoxMin = glm::vec3(0.0f, 0.0f, 0.0f);
        m_bBoxMax = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}


void TriMeshSoup::computeNormals()
{
    m_normals.clear();
    m_normals.resize(m_vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

    // Compute per-vertex normals by averaging the unnormalized face normals
    std::int32_t vertexIndex0, vertexIndex1, vertexIndex2;
    glm::vec3 normal;
    int numIndices = m_indices.size();
    for (int i = 0; i < numIndices; i += 3) 
    {
        vertexIndex0 = m_indices[i];
        vertexIndex1 = m_indices[i + 1];
        vertexIndex2 = m_indices[i + 2];
        normal = glm::cross(m_vertices[vertexIndex1] - m_vertices[vertexIndex0],  m_vertices[vertexIndex2] - m_vertices[vertexIndex0]);
        m_normals[vertexIndex0] += normal;
        m_normals[vertexIndex1] += normal;
        m_normals[vertexIndex2] += normal;
    }

    int numNormals = m_normals.size();
    for (int i = 0; i < numNormals; i++) 
    {
        m_normals[i] = glm::normalize(m_normals[i]);
    }
}


void TriMeshSoup::computeTB()
{
    if( m_normals.size() == 0)
    {
        std::cerr << "WARNING: [TriMeshSoup::computeTB()] normals not available" << std::endl; 
        computeNormals();
    }

    if( m_texcoords.size() != 0)
    {
        m_tangents.resize( m_normals.size() );
        m_bitangents.resize( m_normals.size() );
        //for each triangle
        for (int i = 0; i < m_indices.size(); i += 3)
        {
            int vertexIndex0 = m_indices[i];
            int vertexIndex1 = m_indices[i + 1];
            int vertexIndex2 = m_indices[i + 2];

            glm::vec3 tangent, bitangent;
            compTandBT( m_vertices[vertexIndex1] - m_vertices[vertexIndex0], 
                        m_vertices[vertexIndex2] - m_vertices[vertexIndex0], 
                        glm::vec3( m_texcoords[vertexIndex1].x,  m_texcoords[vertexIndex1].y, 0.0f ) - glm::vec3( m_texcoords[vertexIndex0].x,  m_texcoords[vertexIndex0].y, 0.0f ), 
                        glm::vec3( m_texcoords[vertexIndex2].x,  m_texcoords[vertexIndex2].y, 0.0f ) - glm::vec3( m_texcoords[vertexIndex0].x,  m_texcoords[vertexIndex0].y, 0.0f ), 
                        tangent, bitangent);
            m_tangents[vertexIndex0] = tangent;
            m_bitangents[vertexIndex0] = bitangent;
            compTandBT( m_vertices[vertexIndex2] - m_vertices[vertexIndex1], 
                        m_vertices[vertexIndex0] - m_vertices[vertexIndex1], 
                        glm::vec3( m_texcoords[vertexIndex2].x,  m_texcoords[vertexIndex2].y, 0.0f ) - glm::vec3( m_texcoords[vertexIndex1].x,  m_texcoords[vertexIndex1].y, 0.0f ), 
                        glm::vec3( m_texcoords[vertexIndex0].x,  m_texcoords[vertexIndex0].y, 0.0f ) - glm::vec3( m_texcoords[vertexIndex1].x,  m_texcoords[vertexIndex1].y, 0.0f ),
                        tangent, bitangent);
            m_tangents[vertexIndex1] = tangent;
            m_bitangents[vertexIndex1] = bitangent;
            compTandBT( m_vertices[vertexIndex0] - m_vertices[vertexIndex2], 
                        m_vertices[vertexIndex1] - m_vertices[vertexIndex2], 
                        glm::vec3( m_texcoords[vertexIndex0].x,  m_texcoords[vertexIndex0].y, 0.0f ) - glm::vec3( m_texcoords[vertexIndex2].x,  m_texcoords[vertexIndex2].y, 0.0f ), 
                        glm::vec3( m_texcoords[vertexIndex1].x,  m_texcoords[vertexIndex1].y, 0.0f ) - glm::vec3( m_texcoords[vertexIndex2].x,  m_texcoords[vertexIndex2].y, 0.0f ),
                        tangent, bitangent);
            m_tangents[vertexIndex2] = tangent;
            m_bitangents[vertexIndex2] = bitangent;
        }
    }
    else
    {
        std::cerr << "WARNING: [TriMeshSoup::computeTB()] texcoords not available" << std::endl; 
    }
}


// Read an Mesh from an .obj file. This function can read texture
// coordinates and/or normals, in addition to vertex positions.
bool TriMeshSoup::importOBJ(const std::string &_filename)
{
    struct uvec3Less 
    {
        bool operator() (const glm::uvec3 &a, const glm::uvec3 &b) const
        {
            return (a.x < b.x) |
                    ((a.x == b.x) & (a.y < b.y)) |
                    ((a.x == b.x) & (a.y == b.y) & (a.z < b.z));
        }
    };

    const std::string VERTEX_LINE("v ");
    const std::string TEXCOORD_LINE("vt ");
    const std::string NORMAL_LINE("vn ");
    const std::string FACE_LINE("f ");

    std::string line;
    glm::vec3 vertex;
    glm::vec3 normal;
    glm::vec3 texcoord;
    std::uint32_t vindex[3];
    std::uint32_t tindex[3];
    std::uint32_t nindex[3];

    // Open OBJ file
    std::ifstream f(_filename.c_str());
    if(!f.is_open()) 
    {
        std::cerr << "ERROR: [TriMeshSoup::importOBJ()] Could not open " << _filename << std::endl;
        return false;
    }

    // First pass: read vertex data into temporary mesh
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> texcoords;
    while(!f.eof()) 
    {
        std::getline(f, line);
        if (line.substr(0, 2) == VERTEX_LINE) 
        {
            std::istringstream ss(line.substr(2));
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (line.substr(0, 3) == TEXCOORD_LINE) 
        {
            std::istringstream ss(line.substr(3));
            ss >> texcoord.x >> texcoord.y;// >> texcoord.z;
            texcoords.push_back(texcoord);
        }
        else if (line.substr(0, 3) == NORMAL_LINE) 
        {
            std::istringstream ss(line.substr(3));
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else 
        {
            // Ignore line
        }
    }

    // Rewind file
    f.clear();
    f.seekg(0);

    // Clear old mesh and pre-allocate space for new mesh data
    m_vertices.clear();
    m_vertices.reserve(vertices.size());
    m_texcoords.clear();
    m_texcoords.reserve(texcoords.size());
    m_normals.clear();
    m_normals.reserve(normals.size());
    m_indices.clear();

    // Set up dictionary for mapping unique tuples to indices
    std::map<glm::uvec3, unsigned, uvec3Less> visited;
    unsigned next_index = 0;
    glm::uvec3 key;

    // Second pass: read faces and construct per-vertex texcoords/normals.
    // Note: OBJ-indices start at one, so we need to subtract indices by one.
    while (!f.eof()) 
    {
        std::getline(f, line);
        if (line.substr(0, 2) == FACE_LINE) 
        {
            if (std::sscanf(line.c_str(), "f %d %d %d", &vindex[0], &vindex[1], &vindex[2]) == 3) 
            {
                for (unsigned i = 0; i < 3; ++i) 
                {
                    key = glm::uvec3(vindex[i], 0, 0);
                    if (visited.count(key) == 0) 
                    {
                        visited[key] = next_index++;
                        m_vertices.push_back(vertices[vindex[i] - 1]);
                    }
                    m_indices.push_back(visited[key]);
                }
            }
            else if (std::sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &vindex[0], &tindex[0], &vindex[1], &tindex[1], &vindex[2], &tindex[2]) == 6) 
            {
                for (unsigned i = 0; i < 3; ++i) 
                {
                    key = glm::uvec3(vindex[i], tindex[i], 0);
                    if (visited.count(key) == 0) 
                    {
                        visited[key] = next_index++;
                        m_vertices.push_back(vertices[vindex[i] - 1]);
                        m_texcoords.push_back( glm::vec2(texcoords[tindex[i] - 1].x, texcoords[tindex[i] - 1].y) );
                    }
                    m_indices.push_back(visited[key]);
                }
            }
            else if (std::sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &vindex[0], &nindex[0], &vindex[1], &nindex[1], &vindex[2], &nindex[2]) == 6) 
            {
                for (unsigned i = 0; i < 3; ++i) 
                {
                    key = glm::uvec3(vindex[i], nindex[i], 0);
                    if (visited.count(key) == 0) 
                    {
                        visited[key] = next_index++;
                        m_vertices.push_back(vertices[vindex[i] - 1]);
                        m_normals.push_back(normals[nindex[i] - 1]);
                    }
                    m_indices.push_back(visited[key]);
                }
            }
            else if(std::sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vindex[0], &tindex[0], &nindex[0], &vindex[1], &tindex[1], &nindex[1], &vindex[2], &tindex[2], &nindex[2]) == 9) 
            {
                for(unsigned i = 0; i < 3; ++i) 
                {
                    key = glm::uvec3(vindex[i], tindex[i], nindex[i]);
                    if (visited.count(key) == 0) 
                    {
                        visited[key] = next_index++;
                        m_vertices.push_back(vertices[vindex[i] - 1]);
                        m_texcoords.push_back( glm::vec2(texcoords[tindex[i] - 1].x, texcoords[tindex[i] - 1].y) );
                        m_normals.push_back(normals[nindex[i] - 1]);
                    }
                    m_indices.push_back(visited[key]);
                }
            }
        }
    }

    // Compute normals (if OBJ-file did not contain normals)
    if(m_normals.size() == 0) 
    {
        std::cout << "Normals not provided, compute them " << std::endl;
        computeNormals();
    }

    return true;
}


void TriMeshSoup::exportOBJ(const std::string &_filename)
{
    // Open the file
    FILE* file = fopen(_filename.c_str(), "w");
    if( !file) 
    {
        std::cerr << " ERROR [TriMeshSoup::wriexportOBJteOBJ()] Cannot open file to write" << _filename << std::endl;
    }

    unsigned int nb_triangles = m_indices.size()/3;
    if( m_indices.size() % 3 != 0)
    {
        std::cerr << " ERROR [TriMeshSoup::wriexportOBJteOBJ()] Number of vertices is not a multiple of 3" << std::endl;
    }

    unsigned int i;

    // Write vertices
    fprintf(file, "\n");
    fprintf(file, "# %d vertices\n", m_vertices.size() );
    for (i = 0; i <m_vertices.size(); i++) 
    {
        fprintf(file, "v %f %f %f\n", m_vertices[i].x, m_vertices[i].y, m_vertices[i].z);
    }

    // Write normals
    fprintf(file, "\n");
    fprintf(file, "# %d normals\n", m_normals.size() );
    for (i = 0; i <m_normals.size(); i++) 
    {
        fprintf(file, "vn %f %f %f\n", m_normals[i].x, m_normals[i].y, m_normals[i].z);
    }

    // Write texcoords
    fprintf(file, "\n");
    fprintf(file, "# %d texcoords\n", m_texcoords.size() );
    for (i = 0; i <m_texcoords.size(); i++) 
    {
        fprintf(file, "vt %f %f \n", m_texcoords[i].x, m_texcoords[i].y);
    }

    // Write facets (triangles)
    fprintf(file, "\n");
    fprintf(file, "# %d faces (triangles)\n", nb_triangles );

    for (i = 0; i <nb_triangles; i++) 
    {
        int vertId0 = m_indices[ i * 3 ] + 1; // !! in wavefront files, vertex indices starts at 1
        int vertId1 = m_indices[ i * 3 + 1 ] + 1;
        int vertId2 = m_indices[ i * 3 + 2 ] + 1;

        if(m_texcoords.size() != 0 && m_normals.size() != 0)
            fprintf(file, "f %d/%d/%d %d/%d/%d %d/%d/%d \n", vertId0, vertId0, vertId0, vertId1, vertId1, vertId1, vertId2, vertId2, vertId2);
        else if(m_texcoords.size() != 0 || m_normals.size() != 0)
            fprintf(file, "f %d//%d %d//%d %d//%d \n", vertId0, vertId0, vertId1, vertId1, vertId2, vertId2);
        else
            fprintf(file, "f %d %d %d \n", vertId0, vertId1, vertId2);
    }

    fclose(file);
}


void TriMeshSoup::readSTLAscii(FILE * const _file, std::vector<float> * const _vertex_data,  std::vector<float> * const _normal_data)
{
    assert( _file );
    assert( _vertex_data );
    assert( 0 == _vertex_data->size() );

    const uint32_t max_line_length = 256;

    char line[ max_line_length ];
    while ( std::fgets( line, max_line_length, _file ) ) 
    {
        if ( std::strstr( line, "facet normal" ) ) 
        {
            double normal[3];

            char str1[10];
            char str2[10];
            std::sscanf( line, " %10s %10s %lf %lf %lf", str1, str2, &normal[0], &normal[1], &normal[2] );
            _normal_data->push_back( normal[0] );
            _normal_data->push_back( normal[1] );
            _normal_data->push_back( normal[2] );

        }
        if ( std::strstr( line, "vertex" ) ) 
        {

            double vertex[3];
            char str[16];

            std::sscanf( line, " %15s %lf %lf %lf", str, &vertex[0], &vertex[1], &vertex[2] );

            _vertex_data->push_back( vertex[0] );
            _vertex_data->push_back( vertex[1] );
            _vertex_data->push_back( vertex[2] );

        }
    }
}


void TriMeshSoup::readSTLBinary(FILE * const _file, std::vector<float> * const _vertex_data)
{ 
    assert( _file );
    assert( _vertex_data );
    assert( 0 == _vertex_data->size() );

    const uint32_t header_length = 80;

    uint8_t header[ header_length ];
    std::fread( header, sizeof( uint8_t ), header_length, _file );
    uint32_t num_triangles = 0;
    std::fread( &num_triangles, sizeof( uint32_t ), 1, _file );
    for ( uint32_t i = 0; i < num_triangles; ++i ) 
    {
        float normal[3];
        std::fread( normal, sizeof( float ), 3, _file );
        for ( uint32_t j = 0; j < 3; ++j )
        {
            float vertex[3];
            std::fread( vertex, sizeof( float ), 3, _file );
            _vertex_data->push_back( vertex[ 0 ] );
            _vertex_data->push_back( vertex[ 1 ] );
            _vertex_data->push_back( vertex[ 2 ] );
        }
        uint16_t metadata;
        std::fread( &metadata, sizeof( uint16_t ), 1, _file );
    }
}


//! Reads mesh from STL file (ASCII or binary). The format, ASCII or
//! binary, is detected automatically by checking the content of the file.
bool TriMeshSoup::readSTL(const std::string &_filename, std::vector<float> * const _vertex_data, std::vector<float> * const _normal_data)
{
    FILE *file = std::fopen( _filename.c_str(), "rb" );
    if ( file ) 
    {
        uint8_t buf[ 5 ];
        std::fread( buf, sizeof( uint8_t ), 5, file );
        std::rewind( file );
        if ( 0 == std::strncmp( "solid", (const char *)buf, 5 ) ) 
        {
            readSTLAscii( file, _vertex_data, _normal_data );
        }
        else 
        {
            readSTLBinary( file, _vertex_data );
        }
        std::fclose( file );
    }
    else 
    {
        std::cerr << " ERROR [TriMeshSoup::readSTL()] Cannot open file " << _filename << std::endl;
        return false;
    }
    return true;
}

//! Reads mesh from STL file (ASCII or binary)
bool TriMeshSoup::importSTL(const std::string &_filename)
{
    if (!m_vertices.empty())
        m_vertices.clear();
    if (!m_normals.empty())
        m_normals.clear();

    // Read STL file.
    std::vector<float> buffer_vertices;
    std::vector<float> buffer_normals;

    bool status = readSTL(_filename.c_str(), &buffer_vertices, &buffer_normals);
    if (!status)
        return false;

    const int32_t num_vertices = buffer_vertices.size() / 3;
    const int32_t num_normals = buffer_normals.size() / 3;

    // Store vertex positions.
    m_vertices.reserve(num_vertices);

    for (int32_t i = 0; i < num_vertices; ++i)
    {
        glm::vec3 vertex;
        vertex[0] = buffer_vertices[3 * i + 0];
        vertex[1] = buffer_vertices[3 * i + 1];
        vertex[2] = buffer_vertices[3 * i + 2];
        m_vertices.push_back(vertex);

        m_indices.push_back(i);
    }

    // Store face normals.
    if (!buffer_normals.empty() && (num_normals == num_vertices/3) )
    {
        std::cout<<"read normals"<<std::endl;
        std::cout<<num_normals<<" "<<num_vertices<<std::endl;
        for (int32_t i = 0; i < num_normals; ++i)
        { 
            glm::vec3 normal;
            normal[0] = buffer_normals[3 * i + 0];
            normal[1] = buffer_normals[3 * i + 1];
            normal[2] = buffer_normals[3 * i + 2];

            // one normal per facet, duplicate it to get one normal per vertex
            m_normals.push_back( normal );
            m_normals.push_back( normal );
            m_normals.push_back( normal );
        }
    }
    else
    {
        std::cout<<"compute normals"<<std::endl;
        m_normals.reserve(num_vertices);
        for (int32_t i = 0; i < num_vertices; i += 3)
        { 
            glm::vec3 v01 = m_vertices[i + 1] - m_vertices[i];
            glm::vec3 v02 = m_vertices[i + 2] - m_vertices[i];
            glm::vec3 normal = glm::normalize( (glm::cross(v01, v02)) );
            m_normals.push_back(normal);
            m_normals.push_back(normal);
            m_normals.push_back(normal);
        }
    }

    return true;
}


void TriMeshSoup::exportSTL(const std::string &_filename)
{
    if( m_vertices.empty() || m_indices.empty() )
        std::cerr<<"ERROR: [TriMeshSoup::exportSTL()] empty data"<<std::endl;

    struct STL_TRIANGLE
    {
        float n[3];
        float v1[3];
        float v2[3];
        float v3[3];
        std::uint16_t abc;
    };

    std::uint8_t header[80] = "Exported STL";
    std::uint32_t num_triangles =  m_indices.size() / 3;

    FILE *file = std::fopen(_filename.c_str(), "wb+");
    if( !file) 
    {
        std::cerr << " ERROR [TriMeshSoup::exportSTL()] Cannot open file to write" << _filename << std::endl;
    }

    std::fwrite(&header[0], sizeof(header), 1, file);
    std::fwrite(&num_triangles, sizeof(num_triangles), 1, file);

    for (int i = 0; i < m_indices.size(); i += 3)
    {
        const glm::vec3 &v1 = m_vertices[ m_indices[i + 0] ];
        const glm::vec3 &v2 = m_vertices[ m_indices[i + 1] ];
        const glm::vec3 &v3 = m_vertices[ m_indices[i + 2] ];

        STL_TRIANGLE triangle;
        std::memset(&triangle.n[0], 0, sizeof(float) * 3);
        std::memcpy(&triangle.v1[0], &v1[0], sizeof(float) * 3);
        std::memcpy(&triangle.v2[0], &v2[0], sizeof(float) * 3);
        std::memcpy(&triangle.v3[0], &v3[0], sizeof(float) * 3);
        std::memset(&triangle.abc, 0, sizeof(std::uint16_t));

        size_t triangleNBytes = 50;  // Actual size without padding
        std::fwrite(&triangle, triangleNBytes, 1, file);
    }
    std::fclose(file);
}


void TriMeshSoup::clear()
{
    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();

    m_colors.clear();
    m_texcoords.clear();
    m_tangents.clear();
    m_bitangents.clear();
}