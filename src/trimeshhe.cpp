#include "trimeshhe.h"



TriMeshHE::TriMeshHE()
{
    // request vertex normals and texcoords
    m_mesh.request_vertex_normals();
    m_mesh.request_vertex_texcoords2D();

    // check we have vertex properties
    if (!m_mesh.has_vertex_normals() )
        std::cerr << "[ERROR] TriMeshHE::TriMeshHE(): Standard vertex property 'Normals' not available!\n";
    if (!m_mesh.has_vertex_texcoords2D() )
        std::cerr << "[ERROR] TriMeshHE::TriMeshHE(): Standard vertex property 'TexCoords2D' not available!\n";

    m_TBComputed = false;

    m_bBoxMin = glm::vec3(0.0f, 0.0f, 0.0f);
    m_bBoxMax = glm::vec3(0.0f, 0.0f, 0.0f);
}


TriMeshHE::TriMeshHE(bool _vertNormals, bool _vertTexCoords2D, bool _vertCol, bool _edgeTexCoords2D)
{
    // request vertex properties
    if(_vertNormals)
        m_mesh.request_vertex_normals();
    if(_vertTexCoords2D)
        m_mesh.request_vertex_texcoords2D();
    if(_vertCol)
        m_mesh.request_vertex_colors();
    if(_edgeTexCoords2D)
        m_mesh.request_halfedge_texcoords2D();


    // check we have vertex properties
    if (!m_mesh.has_vertex_normals() && _vertNormals)
        std::cerr << "[ERROR] TriMeshHE::TriMeshHE(): Standard vertex property 'Normals' not available!\n";
    if (!m_mesh.has_vertex_texcoords2D() && _vertTexCoords2D)
        std::cerr << "[ERROR] TriMeshHE::TriMeshHE(): Standard vertex property 'TexCoords2D' not available!\n";
    if (!m_mesh.has_vertex_colors() && _vertCol)
        std::cerr << "[ERROR] TriMeshHE::TriMeshHE(): Standard vertex property 'Colors' not available!\n";
    if (!m_mesh.has_halfedge_texcoords2D() && _edgeTexCoords2D)
        std::cerr << "[ERROR] TriMeshHE::TriMeshHE(): Standard halfedge property 'TexCoords2D' not available!\n";

    m_TBComputed = false;

    m_bBoxMin = glm::vec3(0.0f, 0.0f, 0.0f);
    m_bBoxMax = glm::vec3(0.0f, 0.0f, 0.0f);
}


TriMeshHE::~TriMeshHE()
{}


void TriMeshHE::getVertices(std::vector<glm::vec3>& _vertices)
{
    if(_vertices.size() != 0)
        _vertices.clear();

    // for each triangle of the mesh...
    for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
    {
        // for each vertex of the triangle
        for (OpMesh::FaceVertexIter fv_it = m_mesh.fv_iter( *f_it ); fv_it.is_valid(); ++fv_it)
        {                
            OpMesh::VertexHandle v_h =  m_mesh.vertex_handle(fv_it);                        // current vertex handle
            v_h = fv_it.handle();
            OpMesh::Point p1 = m_mesh.point( v_h );                                         // current vertex pos
            _vertices.push_back(glm::vec3(p1[0], p1[1], p1[2]));
        }
    }
}


void TriMeshHE::getNormals(std::vector<glm::vec3>& _normals)
{
    if(_normals.size() != 0)
        _normals.clear();

    if(m_mesh.has_vertex_normals())
    {
        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        {
            // for each vertex of the triangle
            for (OpMesh::FaceVertexIter fv_it = m_mesh.fv_iter( *f_it ); fv_it.is_valid(); ++fv_it)
            {                
                OpMesh::VertexHandle v_h = fv_it.handle();                                      // current vertex handle
                OpMesh::Normal n1 = m_mesh.normal( v_h );                                       // current vertex normal
                _normals.push_back(glm::vec3(n1[0], n1[1], n1[2]));
            }
        }
    }
}


void TriMeshHE::getIndices(std::vector<uint32_t>& _indices)
{
    if(_indices.size() != 0)
        _indices.clear();

    int id = 0;
    // for each triangle of the mesh...
    for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
    {
        // for each vertex of the triangle
        for (OpMesh::FaceVertexIter fv_it = m_mesh.fv_iter( *f_it ); fv_it.is_valid(); ++fv_it)
        {         
            _indices.push_back(id);
            id++;
        }
    }
}


void TriMeshHE::getColors(std::vector<glm::vec3>& _colors)
{
    if(_colors.size() != 0)
        _colors.clear();

    if(m_mesh.has_vertex_colors())
    {
        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        {
            // for each vertex of the triangle
            for (OpMesh::FaceVertexIter fv_it = m_mesh.fv_iter( *f_it ); fv_it.is_valid(); ++fv_it)
            {                
                OpMesh::VertexHandle v_h = fv_it.handle();                                      // current vertex handle
                OpMesh::Color c1 = m_mesh.color( v_h );                                         // current vertex color
                _colors.push_back(glm::vec3(c1[0], c1[1], c1[2]));
            }
        }
    }
}


void TriMeshHE::getTexCoords(std::vector<glm::vec2>& _texcoords)
{
    if(_texcoords.size() != 0)
        _texcoords.clear();

    if(m_mesh.has_halfedge_texcoords2D())
    {
        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        {
            // for each halfedge of the triangle
            for (OpMesh::FaceHalfedgeIter fh_it = m_mesh.fh_iter( *f_it ); fh_it.is_valid(); ++fh_it)
            {
                OpMesh::TexCoord2D uv1 = m_mesh.texcoord2D( fh_it );                            // current halfedge uv
                _texcoords.push_back(glm::vec2(uv1[0], uv1[1]));
            }
        }
    }
    else if(m_mesh.has_vertex_texcoords2D())
    {
        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        {
            // for each vertex of the triangle
            for (OpMesh::FaceVertexIter fv_it = m_mesh.fv_iter( *f_it ); fv_it.is_valid(); ++fv_it)
            {                
                OpMesh::VertexHandle v_h = fv_it.handle();                                      // current vertex handle
                OpMesh::TexCoord2D uv1 = m_mesh.texcoord2D( v_h );                              // current vertex uv
                _texcoords.push_back(glm::vec2(uv1[0], uv1[1]));
            }
        }
    }
}


void TriMeshHE::getTangents(std::vector<glm::vec3>& _tangents)
{
    if(_tangents.size() != 0)
        _tangents.clear();

    if(m_TBComputed)
    {
        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        {
            for (OpMesh::FaceHalfedgeIter fh_it = m_mesh.fh_iter( *f_it ); fh_it.is_valid(); ++fh_it)
            {                
                OpenMesh::Vec3f t1 = m_mesh.property(tangents,*fh_it);                    // current halfedge tangent
                _tangents.push_back(glm::vec3(t1[0], t1[1], t1[2]));
            }
        }
    }
}


void TriMeshHE::getBitangents(std::vector<glm::vec3>& _bitangents)
{
    if(_bitangents.size() != 0)
        _bitangents.clear();

    if(m_TBComputed)
    {
        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        {
            for (OpMesh::FaceHalfedgeIter fh_it = m_mesh.fh_iter( *f_it ); fh_it.is_valid(); ++fh_it)
            {                
                OpenMesh::Vec3f bt1 = m_mesh.property(bitangents,*fh_it);                    // current halfedge bitangent
                _bitangents.push_back(glm::vec3(bt1[0], bt1[1], bt1[2]));
            }
        }
    }
}


void TriMeshHE::getFaceNormals(std::vector<glm::vec3>& _facenormals)
{
    if(_facenormals.size() != 0)
        _facenormals.clear();

    if(m_mesh.has_face_normals())
    {
        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        { 
            OpMesh::FaceHandle f_h = f_it.handle();                                      // current face handle
            OpMesh::Normal n1 = m_mesh.normal( f_h );                                    // current face normal
            // for each vertex of the triangle
            for (OpMesh::FaceVertexIter fv_it = m_mesh.fv_iter( *f_it ); fv_it.is_valid(); ++fv_it)
            {   
                // add the normal of the face
                _facenormals.push_back(glm::vec3(n1[0], n1[1], n1[2]));
            }
        }
    }
}


bool TriMeshHE::readFile(std::string _filename)
{
    // read options
    OpenMesh::IO::Options rOpt;

    // add read options for vertex normals, colors, and texcoords
    if(m_mesh.has_vertex_normals())
        rOpt += OpenMesh::IO::Options::VertexNormal;
    if(m_mesh.has_vertex_texcoords2D())
        rOpt += OpenMesh::IO::Options::VertexTexCoord;
    if(m_mesh.has_vertex_colors())
        rOpt += OpenMesh::IO::Options::VertexColor;
    if(m_mesh.has_halfedge_texcoords2D())
        rOpt += OpenMesh::IO::Options::FaceTexCoord;

    // read mesh from stdin
    if ( ! OpenMesh::IO::read_mesh(m_mesh, _filename, rOpt) )
    {
        std::cerr << "[ERROR] TriMeshHE::readFile(): Cannot read mesh from " << _filename << std::endl;
        return false;
    }

    // If the file did not provide vertex normals, then calculate them
    if ( m_mesh.has_vertex_normals() && !rOpt.check( OpenMesh::IO::Options::VertexNormal ) )
    {
        std::cout << "[INFO] TriMeshHE::readFile(): Normals not provided, compute them " << std::endl;
        computeNormals();
    }
    // If the file did not provide vertex texcoords, then release them
    if ( m_mesh.has_vertex_texcoords2D() && !rOpt.check( OpenMesh::IO::Options::VertexTexCoord ) )
        m_mesh.release_vertex_texcoords2D();
    // If the file did not provide halfedge texcoords, then release them
    if ( m_mesh.has_halfedge_texcoords2D() && !rOpt.check( OpenMesh::IO::Options::FaceTexCoord ) )
        m_mesh.release_halfedge_texcoords2D();
    // If the file did not provide vertex colors, then release them
    if ( m_mesh.has_vertex_colors() && !rOpt.check( OpenMesh::IO::Options::VertexColor ) )
        m_mesh.release_vertex_colors();

    return true;
}


bool TriMeshHE::writeFile(std::string _filename)
{
    // write options
    OpenMesh::IO::Options wOpt;

    // add read options for vertex normals, colors, and texcoords
    if(m_mesh.has_vertex_normals())
        wOpt += OpenMesh::IO::Options::VertexNormal;
    if(m_mesh.has_vertex_texcoords2D())
        wOpt += OpenMesh::IO::Options::VertexTexCoord;
    if(m_mesh.has_vertex_colors())
        wOpt += OpenMesh::IO::Options::VertexColor;

    // read mesh from stdin
    if ( ! OpenMesh::IO::write_mesh(m_mesh, _filename, wOpt) )
    {
        std::cerr << "[ERROR] TriMeshHE::writeFile(): Cannot write to from " << _filename << std::endl;
        return false;
    }

    return true;
}


void TriMeshHE::computeAABB()
{
    if(!m_mesh.vertices_empty())
    {
        // compute bounding box
        OpMesh::Point bb_min, bb_max;

        // init values
        OpMesh::VertexIter v_it(m_mesh.vertices_begin());
        bb_min = bb_max = m_mesh.point(v_it);

        for (v_it = m_mesh.vertices_begin(); v_it != m_mesh.vertices_end(); ++v_it)
        {
            bb_min.minimize(m_mesh.point(v_it));
            bb_max.maximize(m_mesh.point(v_it));
        }

        m_bBoxMin = glm::vec3(bb_min[0], bb_min[1], bb_min[2]);
        m_bBoxMax = glm::vec3(bb_max[0], bb_max[1], bb_max[2]);
    }
    else
    {
        std::cerr << "[WARNING] TriMeshHE::computeAABB: Empty vertices array" << std::endl;
        m_bBoxMin = glm::vec3(0.0f, 0.0f, 0.0f);
        m_bBoxMax = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}


void TriMeshHE::computeNormals()
{
    // if vertices do not have normal property...
    if ( !m_mesh.has_vertex_normals() )
    {
        // ...request it
        m_mesh.request_vertex_normals();
    }

    // we need face normals to update the vertex normals
    m_mesh.request_face_normals();
    // let the mesh update the normals
    m_mesh.update_normals();
    // dispose the face normals, as we don't need them anymore
//    m_mesh.release_face_normals(); // @@@ keep face normals

    std::cout << "[INFO] TriMeshHE::computeNormals():] Normals computed" << std::endl;
}


void TriMeshHE::computeTB()
{
    if ( m_mesh.has_vertex_texcoords2D() )
    {
        // add tangent as hafledge property (several tangent per vertex, one for each face)
        m_mesh.add_property(tangents);
        // add bitangent as hafledge property (several tangent per vertex, one for each face)
        m_mesh.add_property(bitangents);

        // for each triangle of the mesh...
        for (OpMesh::FaceIter f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it)
        {
            // for each halfedge of the triangle
            for (OpMesh::FaceHalfedgeIter fh_it = m_mesh.fh_iter( *f_it ); fh_it.is_valid(); ++fh_it)
            {
                OpMesh::HalfedgeHandle hh = m_mesh.prev_halfedge_handle(fh_it);
                OpMesh::VertexHandle v_h =  m_mesh.vertex_handle(m_mesh.opposite_vh(hh).idx()); // current vertex handle
                OpMesh::Point p1 = m_mesh.point( v_h );                                         // current vertex pos
                OpMesh::TexCoord2D uv1 = m_mesh.texcoord2D( fh_it );                            // current vertex uv

                OpMesh::FaceHalfedgeIter fh_it2 = fh_it;
                ++fh_it2;
                hh = m_mesh.prev_halfedge_handle(fh_it2); 
                v_h =  m_mesh.vertex_handle(m_mesh.opposite_vh(hh).idx());                      // second vertex handle
                OpMesh::Point p2 = m_mesh.point( v_h );                                         // second vertex pos
                OpMesh::TexCoord2D uv2 = m_mesh.texcoord2D( fh_it2 );                           // second vertex uv

                ++fh_it2;
                hh = m_mesh.prev_halfedge_handle(fh_it2); 
                v_h =  m_mesh.vertex_handle(m_mesh.opposite_vh(hh).idx());                      // third vertex handle
                OpMesh::Point p3 = m_mesh.point( v_h );                                         // third vertex pos
                OpMesh::TexCoord2D uv3 = m_mesh.texcoord2D( fh_it2 );                           // third vertex uv


                // compute delta vecors (i.e. variation in point coords and uv coords along each edge)
                glm::vec3 tangent, bitangent;
                glm::vec3 _delta_pos1(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
                glm::vec3 _delta_pos2(p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]);
                glm::vec3 _delta_uv1(uv2[0] - uv1[0], uv2[1] - uv1[1], 0.0f);
                glm::vec3 _delta_uv2(uv3[0] - uv1[0], uv3[1] - uv1[1], 0.0f);

                // compute local tangent and bitangent vector
                compTandBT(_delta_pos1, _delta_pos2, _delta_uv1, _delta_uv2, tangent, bitangent);

                // write tangent and bitengent in halfedge property
                m_mesh.property(tangents,*fh_it) = OpenMesh::Vec3f(tangent.x, tangent.y, tangent.z);

                m_mesh.property(bitangents,*fh_it) = OpenMesh::Vec3f(bitangent.x, bitangent.y, bitangent.z);

            }
        }
        m_TBComputed = true;
    }
    else
    {
         std::cerr << "[WARNING] TriMeshHE::computeTB(): vertex UV not available" << std::endl;
    }
}


void TriMeshHE::lapSmooth(unsigned int _nbIter, float _fact)
{
    if ( _fact > 1.0f )
    {
        std::cerr << "[WARNING] TriMeshHE::lapSmooth(): factor larger than 1" << std::endl;
    }

    // this vector stores the computed centers of gravity
    std::vector<OpMesh::Point>  vecCogs;
    std::vector<OpMesh::Point>::iterator cog_it;
    vecCogs.reserve(m_mesh.n_vertices());

    OpMesh::VertexIter          v_it, v_end(m_mesh.vertices_end());     // vertex iterator
    OpMesh::VertexVertexIter    vv_it;                                  // vertex to vertex iterator
    OpMesh::Point               cog;                                    // center of gravity (i.e. barycenter, avg coords of first-ring neighborhood)
    OpMesh::Scalar              valence;                                // valence of the vertex (i.e. nb of direct neighbors)

    for (unsigned int i = 0; i < _nbIter; i++)
    {
        vecCogs.clear();
        // First pass: for each vertex in the mesh ...
        for (v_it = m_mesh.vertices_begin(); v_it != v_end; ++v_it)
        {
            cog[0] = cog[1] = cog[2] = valence = 0.0f;
      
            // for each neighbor vertex ...
            for (vv_it = m_mesh.vv_iter( *v_it ); vv_it.is_valid(); ++vv_it)
            {
                // add coords to sum
                cog += m_mesh.point( *vv_it );
                // increment valence
                valence++;
            }
            // compute avg coords to get cog
            vecCogs.push_back(cog / valence);
        }
    
        // Second pass: for each vertex in the mesh ...
        unsigned int j;
        for (v_it=m_mesh.vertices_begin(), cog_it=vecCogs.begin(), j = 0; v_it!=v_end; ++v_it, ++cog_it, ++j)
        {
            if ( !m_mesh.is_boundary( *v_it ) )
            {
                // if factor == 1.0 then just displace vertex to cog position
                if(_fact == 1.0f)
                    m_mesh.set_point( *v_it, *cog_it );
                else
                {
                    // compute Laplacian vector
                    OpenMesh::Vec3f lapVec = OpenMesh::Vec3f(vecCogs[j] - m_mesh.point( *v_it ));
                    // multiply Laplacian vector by factor
                    lapVec *= _fact;
                    m_mesh.set_point( *v_it, m_mesh.point( *v_it ) + lapVec );
                }
            }
        }
    }

    computeNormals();
}