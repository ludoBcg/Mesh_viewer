/*********************************************************************************************************************
 *
 * trimeshhe.cpp
 *
 * Mesh_viewer
 * Ludovic Blache
 *
 *********************************************************************************************************************/


#include <QColor>

#include "trimeshhe.h"



TriMeshHE::TriMeshHE() : Mesh()
{
    // request vertex normals and texcoords
    m_mesh.request_vertex_normals();
    m_mesh.request_vertex_texcoords2D();

    // check that we have vertex properties
    if (!m_mesh.has_vertex_normals() )
        qCritical() << "[ERROR] TriMeshHE::TriMeshHE: Standard vertex property 'Normals' not available!";
    if (!m_mesh.has_vertex_texcoords2D() )
        qCritical() << "[ERROR] TriMeshHE::TriMeshHE: Standard vertex property 'TexCoords2D' not available!";
}


TriMeshHE::TriMeshHE(bool _vertNormals, bool _vertTexCoords2D, bool _vertCol, bool _edgeTexCoords2D) : Mesh()
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
        qCritical() << "[ERROR] TriMeshHE::TriMeshHE: Standard vertex property 'Normals' not available!";
    if (!m_mesh.has_vertex_texcoords2D() && _vertTexCoords2D)
        qCritical() << "[ERROR] TriMeshHE::TriMeshHE: Standard vertex property 'TexCoords2D' not available!";
    if (!m_mesh.has_vertex_colors() && _vertCol)
        qCritical() << "[ERROR] TriMeshHE::TriMeshHE: Standard vertex property 'Colors' not available!";
    if (!m_mesh.has_halfedge_texcoords2D() && _edgeTexCoords2D)
        qCritical() << "[ERROR] TriMeshHE::TriMeshHE: Standard halfedge property 'TexCoords2D' not available!";
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
            //OpMesh::VertexHandle v_h =  m_mesh.vertex_handle(fv_it);              // current vertex handle
            //v_h = fv_it.handle();
            //OpMesh::Point p1 = m_mesh.point( v_h);                                // handle() is deprecated
            OpMesh::Point p1 = m_mesh.point(*fv_it);                                // current vertex pos
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
                //OpMesh::VertexHandle v_h = fv_it.handle();                          // current vertex handle
                //OpMesh::Normal n1 = m_mesh.normal( v_h );                           // handle() is deprecated
                OpMesh::Normal n1 = m_mesh.normal(*fv_it);                           // current vertex normal
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
                //OpMesh::VertexHandle v_h = fv_it.handle();                       // current vertex handle
                //OpMesh::Color c1 = m_mesh.color( v_h );                          // handle() is deprecated
                OpMesh::Color c1 = m_mesh.color(*fv_it);                              // current vertex color
                _colors.push_back(glm::vec3(c1[0], c1[1], c1[2]) / 256.0f);        // normalize color ([0;255] -> [0;1])
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
                OpMesh::TexCoord2D uv1 = m_mesh.texcoord2D(*fh_it);               // current halfedge uv
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
                //OpMesh::VertexHandle v_h = fv_it.handle();                       // current vertex handle
                //OpMesh::TexCoord2D uv1 = m_mesh.texcoord2D( v_h );               // handle() is deprecated
                OpMesh::TexCoord2D uv1 = m_mesh.texcoord2D(*fv_it);                 // current vertex uv
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
                OpenMesh::Vec3f t1 = m_mesh.property(tangents,*fh_it);             // current halfedge tangent
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
                OpenMesh::Vec3f bt1 = m_mesh.property(bitangents,*fh_it);          // current halfedge bitangent
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
            //OpMesh::FaceHandle f_h = f_it.handle();                              // current face handle
            //OpMesh::Normal n1 = m_mesh.normal( f_h );                            // handle() is deprecated
            OpMesh::Normal n1 = m_mesh.normal(*f_it);                              // current face normal
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
        qCritical() << "[ERROR] TriMeshHE::readFile: Cannot read mesh from " << _filename;
        return false;
    }

    // If the file did not provide vertex normals, then calculate them
    if ( m_mesh.has_vertex_normals() && !rOpt.check( OpenMesh::IO::Options::VertexNormal ) )
    {
        qInfo() << "[info] TriMeshHE::readFile: Normals not provided, compute them ";
        computeNormals();
    }
    // If the file did not provide vertex texcoords, then release them
    if ( m_mesh.has_vertex_texcoords2D() && !rOpt.check( OpenMesh::IO::Options::VertexTexCoord ) )
        m_mesh.release_vertex_texcoords2D();
    // If the file did not provide halfedge texcoords, then release them
    if ( m_mesh.has_halfedge_texcoords2D() && !rOpt.check( OpenMesh::IO::Options::FaceTexCoord ) )
        m_mesh.release_halfedge_texcoords2D();
    // If the file did not provide vertex colors, then release them
//    if ( m_mesh.has_vertex_colors() && !rOpt.check( OpenMesh::IO::Options::VertexColor ) )
//        m_mesh.release_vertex_colors();

    qInfo() << "[info] TriMeshHE::readFile: finished ";

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
        qCritical() << "[ERROR] TriMeshHE::writeFile: Cannot write to " << _filename;
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
        bb_min = bb_max = m_mesh.point(*v_it);

        for (v_it = m_mesh.vertices_begin(); v_it != m_mesh.vertices_end(); ++v_it)
        {
            bb_min.minimize(m_mesh.point(*v_it));
            bb_max.maximize(m_mesh.point(*v_it));
        }

        m_bBoxMin = glm::vec3(bb_min[0], bb_min[1], bb_min[2]);
        m_bBoxMax = glm::vec3(bb_max[0], bb_max[1], bb_max[2]);
    }
    else
    {
        qWarning() << "[Warning] TriMeshHE::computeAABB: Empty vertices array";
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

    qInfo() << "[info] TriMeshHE::computeNormals: Normals computed";
}


void TriMeshHE::computeTB()
{
    if ( m_mesh.has_vertex_texcoords2D() && m_mesh.has_halfedge_texcoords2D() )
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
                OpMesh::HalfedgeHandle hh = m_mesh.prev_halfedge_handle(*fh_it);
                OpMesh::VertexHandle v_h =  m_mesh.vertex_handle(m_mesh.opposite_vh(hh).idx()); // current vertex handle
                OpMesh::Point p1 = m_mesh.point( v_h );                                         // current vertex pos
                OpMesh::TexCoord2D uv1 = m_mesh.texcoord2D(*fh_it);                             // current vertex uv

                OpMesh::FaceHalfedgeIter fh_it2 = fh_it;
                ++fh_it2;
                hh = m_mesh.prev_halfedge_handle(*fh_it2); 
                v_h =  m_mesh.vertex_handle(m_mesh.opposite_vh(hh).idx());                      // second vertex handle
                OpMesh::Point p2 = m_mesh.point( v_h );                                         // second vertex pos
                OpMesh::TexCoord2D uv2 = m_mesh.texcoord2D(*fh_it2);                            // second vertex uv

                ++fh_it2;
                hh = m_mesh.prev_halfedge_handle(*fh_it2); 
                v_h =  m_mesh.vertex_handle(m_mesh.opposite_vh(hh).idx());                      // third vertex handle
                OpMesh::Point p3 = m_mesh.point( v_h );                                         // third vertex pos
                OpMesh::TexCoord2D uv3 = m_mesh.texcoord2D(*fh_it2);                            // third vertex uv


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
        qInfo() << "[info] TriMeshHE::computeTB: Tangents and Bitangents computed";
        m_TBComputed = true;
    }
    else
    {
        qWarning() << "[Warning] TriMeshHE::computeTB: vertex UV not available";
    }
}


void TriMeshHE::lapSmooth(unsigned int _nbIter, float _fact)
{
    if ( _fact > 1.0f )
    {
        qWarning() << "[Warning] TriMeshHE::lapSmooth: factor larger than 1";
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

    qInfo() << "[info] TriMeshHE::lapSmooth: Laplacian smoothing finished";
}

void TriMeshHE::duplicateVertices()
{
    // todo
}


void TriMeshHE::computeSurfVar()
{
    // Surface variation is calculated using the algorithm described in :
    // Pauly et al., "Efficient Simplification of Point-Sampled Surfaces", IEEE Visualization, 2002
    // https://www.graphics.rwth-aachen.de/media/papers/p_Pau021.pdf

    qInfo() << "[info] TriMeshHE::computeSurfVar: mesh color will be overwritten by surface variation ";

    OpMesh::VertexIter v_it, v_end(m_mesh.vertices_end());

    std::vector<double> values, sortedVal;

    double maxCurv = 0.0f;
    for (v_it = m_mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        double curv = compLocalVariation(v_it) ;

        values.push_back(curv);
    }

    // sort vector and ignore the last 5% of higher curvatures to remove outliers
    sortedVal = values;
    std::sort(sortedVal.begin(), sortedVal.end() );
    float maxBound = sortedVal[(int)( (float)sortedVal.size() *0.95f )];
    maxCurv = maxBound;

    unsigned int i = 0;
    for (v_it = m_mesh.vertices_begin(), i=0; v_it != v_end; ++v_it, ++i)
    {
        double H = 0.0f;
        if(values[i] > 0.0f && values[i] < maxBound)
        {
            // normalize cuvature using maximum
            double curvNormalized = values[i] / maxCurv;
            // H in [0; 120] degrees (red to green)
            H = 120 - curvNormalized * 120;
        }

        // build HSV color and transform to RGB 
        QColor rgb = QColor::fromHsv(  (int)H, 255, 255);
        OpMesh::Color col( rgb.red() , rgb.green(), rgb.blue() );
        m_mesh.set_color(*v_it, col );

    }
    qInfo() << "[info] TriMeshHE::computeSurfVar: Surface variation calculation finished, show mesh color to see the result ";
}


double TriMeshHE::compLocalVariation(OpMesh::VertexIter _hi)
{
    // N = number on point in the cloud (i.e. the current vertex and his 1-ring neighborhood
    int N = m_mesh.valence(*_hi) + 1;

    // Compute the centroid (center of gravity)
    OpMesh::Point cog(0, 0, 0);
    OpMesh::Point vi = m_mesh.point(*_hi);
    cog += vi;
    int cpt = 1;
    for(OpMesh::VertexVertexIter vv_it = m_mesh.vv_iter(*_hi); vv_it; ++vv_it)
    {
        OpMesh::Point vj = m_mesh.point(vv_it);
        cog += vj;
        cpt++;
    }
    cog /= cpt;

    // M matrix = difference between centroid and each vertex
    Eigen::MatrixX3d M;
    M.resize(N,3);
    // Fill M matrix
    cpt = 0;
    OpMesh::Point var = (vi - cog);
    Eigen::Vector3d varb;
    varb(0) = var[0]; varb(1) = var[1]; varb(2) = var[2];
    M.row(cpt) =  varb;
    for(OpMesh::VertexVertexIter vv_it = m_mesh.vv_iter(*_hi); vv_it; ++vv_it)
    {
        cpt++;
        OpMesh::Point vj = m_mesh.point(vv_it);
        var = (vj - cog);
        varb(0) = var[0]; varb(1) = var[1]; varb(2) = var[2];
        M.row(cpt) =  varb;
    }

    // Compute C matrix (C = M^t * M)
    Eigen::Matrix3d C = M.transpose() * M;

    // eigen values decomposition of C
    Eigen::VectorXcd eigenVals = C.eigenvalues();
    std::vector< double > Lambda;
    Lambda.push_back(eigenVals(0).real());
    Lambda.push_back(eigenVals(1).real());
    Lambda.push_back(eigenVals(2).real());

    // lambda0 < lambda1 < lambda2
    std::sort(Lambda.begin(), Lambda.end());
    double lambda0 = Lambda[0];
    double lambda1 = Lambda[1];
    double lambda2 = Lambda[2];

    // Compute surface variation sigma = lambda0 / (lambda0 + lambda1 + lambda2)
    double sigma = lambda0 / (lambda0 + lambda1 + lambda2);
    return sigma;

}


void TriMeshHE::computeMeanCurv()
{
    // Discrete Mean Curvature is calculated using the algorithm described in :
    // Using Meyer et al., "Discrete Differential-Geometry Operators for Triangulated 2-Manifolds", Visualization and Mathematics III, 2003
    // http://multires.caltech.edu/pubs/diffGeoOps.pdf

    qInfo() << "[info] TriMeshHE::computeMeanCurv: mesh color will be overwritten by surface variation ";

    OpMesh::VertexIter v_it, v_end(m_mesh.vertices_end());

    std::vector<double> values, sortedVal;

    double maxCurv = 0.0f;
    for (v_it = m_mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        double curv = vertMeanCurv(v_it) ;

        values.push_back(curv);
    }

    // sort vector and ignore the last 5% of higher curvatures to remove outliers
    sortedVal = values;
    std::sort(sortedVal.begin(), sortedVal.end() );
    float maxBound = sortedVal[(int)( (float)sortedVal.size() *0.95f )];
    maxCurv = maxBound;

    unsigned int i = 0;
    for (v_it = m_mesh.vertices_begin(), i=0; v_it != v_end; ++v_it, ++i)
    {
        double H = 0.0f;
        if(values[i] > 0.0f && values[i] < maxBound)
        {
            // normalize cuvature using maximum
            double curvNormalized = values[i] / maxCurv;
            // H in [0; 120] degrees (red to green)
            H = 120 - curvNormalized * 120;
        }

        // build HSV color and transform to RGB 
        QColor rgb = QColor::fromHsv(  (int)H, 255, 255);
        OpMesh::Color col( rgb.red() , rgb.green(), rgb.blue() );
        m_mesh.set_color(*v_it, col );

    }

    qInfo() << "[info] TriMeshHE::computeMeanCurv: Mean curvature calculation finished, show mesh color to see the result ";
}

double TriMeshHE::vertMeanCurv(OpMesh::VertexIter _xi)
{
    if(!m_mesh.is_manifold(*_xi ) )
        return 0.0f;

    OpMesh::VertexVertexIter vv_it, next_it, prev_it;

    glm::vec3 K(0.0f);
    // for each neighbor vertex ...
    for (vv_it = m_mesh.vv_iter( *_xi ); vv_it.is_valid(); ++vv_it)
    {
        // previous neighbor
        prev_it = vv_it;
        --prev_it;
        // next neighbor
        next_it = vv_it;
        ++next_it;

        OpMesh::Point xi = m_mesh.point( *_xi );
        OpMesh::Point xj = m_mesh.point( *vv_it );
        glm::vec3 x_i(xi[0], xi[1], xi[2]);
        glm::vec3 x_j(xj[0], xj[1], xj[2]);

        //// get handle to triangle
        OpMesh::VertexHandle v_h = vv_it.handle(), prev_h = prev_it.handle(), next_h = next_it.handle();

        OpMesh::VertexIter v_Vit(m_mesh, v_h);
        OpMesh::VertexIter prev_Vit(m_mesh, prev_h);
        OpMesh::VertexIter next_Vit(m_mesh, next_h);

        K += (float)compSumCot( _xi, v_Vit, prev_Vit, next_Vit) * (x_i - x_j);
        /*K += (float)compSumCot(_xi, (vv_it.handle()), (prev_it.handle()), (next_it.handle()) ) * (x_i - x_j) ;*/
    }

    float A = (float)compAreaMixed(_xi);
    glm::vec3 K2;
    if(A > std::numeric_limits<float>::epsilon() )
        K2 = K / (2.0f * A );
    else
        return 0.0f;


    return  0.5f * glm::length(K2);

}

double TriMeshHE::compAreaMixed(OpMesh::VertexIter _xi)
{
    double sumA = 0.0f;

    // for each neighbor triangle ...
    OpMesh::VertexFaceIter vf_it;
    for (vf_it = m_mesh.vf_iter( *_xi ); vf_it.is_valid(); ++vf_it)
    {
        if(m_mesh.is_manifold(*_xi ) )
        {
            // get handle to triangle
            OpMesh::FaceHandle f_h = vf_it.handle();

            OpMesh::FaceIter f_it(m_mesh, f_h);
        
            if( !isTriangleObtuse(f_it) )
            {
                double AV = compAreaVoronoi(_xi, f_it);
                if(AV > 0.0f)
                    sumA += AV;
               
            }
            else if( isAngleObtuse(f_it, _xi) )
            {
                double AT = compAreaTriangle(f_it);
                if(AT > 0.0f)
                    sumA += AT / 2.0f;
                
            }
            else
            {
                double AT = compAreaTriangle(f_it);
                if(AT > 0.0f)
                    sumA += AT / 4.0f;
                
            }
        }

    }

    return sumA;
}

float TriMeshHE::compAreaVoronoi(OpMesh::VertexIter _xi, OpMesh::FaceIter _f)
{
    // get face iter
    OpMesh::FaceIter f_it = m_mesh.faces_begin() + _f->idx();
    // circulate over vertices
    OpMesh::FaceVertexIter fv_it = m_mesh.fv_iter( *f_it );
    OpMesh::Point pA = m_mesh.point( *fv_it ); ++fv_it;
    OpMesh::Point pB = m_mesh.point( *fv_it ); ++fv_it;
    OpMesh::Point pC = m_mesh.point( *fv_it );

    OpMesh::Point p1 ,p2, p3;

    OpMesh::Point px = m_mesh.point( *_xi );
    if(px == pA)
    {
        p1 = pA;
        p2 = pB;
        p3 = pC;
    }
    else if(px == pB)
    {
        p1 = pB;
        p2 = pA;
        p3 = pC;
    }
    else
    {
        p1 = pC;
        p2 = pA;
        p3 = pB;
    }


    // get edges
    glm::vec3 p2p1(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
    glm::vec3 p3p1(p1[0] - p3[0], p1[1] - p3[1], p1[2] - p3[2]);
    glm::vec3 p2p3(p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2]);

    if( glm::length(p2p1) == 0.0f || glm::length(p3p1) == 0.0f || glm::length(p2p3) == 0.0f)
        return 0.0f;

    float cosP2 = glm::dot( glm::normalize(p2p1) , glm::normalize(p2p3));     // A.B = ||A|| * ||B|| * cos(AB)
    float sinP2 = glm::length( glm::cross( glm::normalize(p2p1) , glm::normalize(p2p3) ) );    // ||AxB|| = ||A|| * ||B|| * sin(AB)
    float cosP3 = glm::dot( glm::normalize(p3p1) , glm::normalize(-p2p3) );
    float sinP3 = glm::length( glm::cross( glm::normalize(p3p1), glm::normalize(-p2p3) ) );


    // cot alpha = cos alpha / sin alpha
    float cotP2 = cosP2 / sinP2;
    float cotP3 = cosP3 / sinP3;
    
    if( glm::length(p3p1) > std::numeric_limits<float>::epsilon() && glm::length(p2p1) > std::numeric_limits<float>::epsilon() && cotP3 != 0.0f && cotP2 != 0.0f)
        return ( pow(glm::length(p3p1), 2) * cotP2 + pow(glm::length(p2p1), 2) * cotP3 ) / 8.0f;
    
    return 0.0f;
}

double TriMeshHE::compSumCot(OpMesh::VertexIter _xi, OpMesh::VertexIter _xj, OpMesh::VertexIter _xjm, OpMesh::VertexIter _xjp)
{

    //  _xjm --- _xi
    //     \      |  \
    //      \     |   \
    //       \    |    \
    //        \   |     \
    //          _xj --- _xjp
    //
    // _xi the "central" vertex
    // _xj is a neighbor of _xi
    //_xjm is the "previous" neighbor (_xj-1) 
    //_xjp is the "next" neighbor (_xp-1)
    // Alpha_ij = (_xj, _xjm, _xi) angle
    // Beta_ij = (_xi, _xjp, _xj) angle

    //cot Alpha_ij + cot Beta_ij

    OpMesh::Point x_i = m_mesh.point( *_xi );
    OpMesh::Point x_j = m_mesh.point( *_xj );
    OpMesh::Point x_jm = m_mesh.point( *_xjm );
    OpMesh::Point x_jp = m_mesh.point( *_xjp );

    glm::vec3 a_xi = glm::vec3( x_i[0] - x_jm[0], x_i[1] - x_jm[1], x_i[2] - x_jm[2] ); // vector x_j-1 -> x_i
    glm::vec3 a_xj = glm::vec3( x_j[0] - x_jm[0], x_j[1] - x_jm[1], x_j[2] - x_jm[2] ); // vector x_j-1 -> x_j
    glm::vec3 b_xi = glm::vec3( x_i[0] - x_jp[0], x_i[1] - x_jp[1], x_i[2] - x_jp[2] ); // vector x_j+1 -> x_i
    glm::vec3 b_xj = glm::vec3( x_j[0] - x_jp[0], x_j[1] - x_jp[1], x_j[2] - x_jp[2] ); // vector x_j+1 -> x_j
    if(glm::length(a_xi) > 0.0f && glm::length(a_xj) > 0.0f && glm::length(b_xi) > 0.0f && glm::length(b_xj) > 0.0f)
    {
        a_xi = glm::normalize( a_xi );
        a_xj = glm::normalize( a_xj );
        b_xi = glm::normalize( b_xi );
        b_xj = glm::normalize( b_xj );
    }
    else
        return 0.0f;

    double cosA = glm::dot(a_xi, a_xj);     // A.B = ||A|| * ||B|| * cos(AB)
    double sinA = glm::length( glm::cross(a_xi, a_xj) );    // ||AxB|| = ||A|| * ||B|| * sin(AB)
    double cosB = glm::dot(b_xi, b_xj);
    double sinB = glm::length( glm::cross(b_xi, b_xj) );

    // cot alpha = cos alpha / sin alpha
    double cotA = cosA / sinA;
    double cotB = cosB / sinB;

    return cotA + cotB;
}

double TriMeshHE::compAreaTriangle(OpMesh::FaceIter _f)
{
    // get face iter
    OpMesh::FaceIter f_it = m_mesh.faces_begin() + _f->idx();
    // circulate over vertices
    OpMesh::FaceHalfedgeIter fh_it = m_mesh.fh_iter( *f_it );
    // point from
    OpMesh::Point p1 = m_mesh.point( m_mesh.from_vertex_handle(fh_it) );
    // point to
    OpMesh::Point p2 = m_mesh.point( m_mesh.to_vertex_handle(fh_it) );
    // previous point
    OpMesh::Point p3 = m_mesh.point( m_mesh.from_vertex_handle( m_mesh.prev_halfedge_handle(fh_it) ) );

    // get 2 edges
    glm::vec3 e1(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
    glm::vec3 e2(p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]);

    if( glm::length(e1) > std::numeric_limits<float>::epsilon() && glm::length(e2) > std::numeric_limits<float>::epsilon() )
    {
        // area = (1/2) * || e1 x e2 ||
        if(abs(glm::dot( glm::normalize(e1), glm::normalize(e2))) > std::numeric_limits<float>::epsilon())
        {
            double normCross = glm::length( glm::cross(e1, e2) );
                return normCross / 2.0f;
        }

    }

    return 0.0f;
}

bool TriMeshHE::isTriangleObtuse(OpMesh::FaceIter _f)
{
    bool res = false;

    // get face iter
    OpMesh::FaceIter f_it = m_mesh.faces_begin() + _f->idx();
    // circulate over vertices
    for (OpMesh::FaceHalfedgeIter fh_it = m_mesh.fh_iter( *f_it ); fh_it.is_valid(); ++fh_it)
    {   
        // point from
        OpMesh::Point p1 = m_mesh.point( m_mesh.from_vertex_handle(fh_it) );
        // point to
        OpMesh::Point p2 = m_mesh.point( m_mesh.to_vertex_handle(fh_it) );
        // previous point
        OpMesh::Point p3 = m_mesh.point( m_mesh.from_vertex_handle( m_mesh.prev_halfedge_handle(fh_it) ) );

        // get two edge vectors
        glm::vec3 p1p2(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
        glm::vec3 p1p3(p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]);

        
        if( glm::dot( glm::normalize(p1p2), glm::normalize(p1p3) ) <= 0.0f)
            res = true;
    }


    return res;
}

bool TriMeshHE::isAngleObtuse(OpMesh::FaceIter _f, OpMesh::VertexIter _xi)
{
    OpMesh::Point px = m_mesh.point( *_xi );

    bool res = false;
    // get face iter
    OpMesh::FaceIter f_it = m_mesh.faces_begin() + _f->idx();
    // circulate over vertices
    for (OpMesh::FaceHalfedgeIter fh_it = m_mesh.fh_iter( *f_it ); fh_it.is_valid(); ++fh_it)
    {   
        // point from
        OpMesh::Point p1 = m_mesh.point( m_mesh.from_vertex_handle(fh_it) );
        // point to
        OpMesh::Point p2 = m_mesh.point( m_mesh.to_vertex_handle(fh_it) );
        // previous point
        OpMesh::Point p3 = m_mesh.point( m_mesh.from_vertex_handle( m_mesh.prev_halfedge_handle(fh_it) ) );

        if(px == p1)
        {
            // get two edge vectors
            glm::vec3 p1p2(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
            glm::vec3 p1p3(p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]);

        
            if( glm::dot( glm::normalize(p1p2), glm::normalize(p1p3) ) <= 0.0f)
                res = true;
        }
    }


    return res;
}
