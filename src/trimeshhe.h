/*********************************************************************************************************************
 *
 * triMeshhe.h
 *
 * Mesh class for triangle mesh, using OpenMesh half-edge sata structure
 *
 * Mesh_viewer
 * Ludovic Blache
 *
 *********************************************************************************************************************/


#ifndef TRIMESHHE_H
#define TRIMESHHE_H

#include "mesh.h"

#include <Eigen/Dense>
#include <Eigen/SVD>
#include <Eigen/LU>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>


// simple triangle mesh, using default traits
typedef OpenMesh::TriMesh_ArrayKernelT<>  OpMesh;


/*!
* \class TriMeshHE
* \brief Triangle mesh using HalfEdge data struture
* This class encapsulates the OpenMesh data structure
* Handling multiple UV cords and/or normals vectors per vertex mught be ddifficult, use triMeshSoup instead
*/
class TriMeshHE : public Mesh 
{
    public:

        /*------------------------------------------------------------------------------------------------------------+
        |                                        CONSTRUCTORS / DESTRUCTORS                                           |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn TriMeshHE
        * \brief Default constructor of TriMeshHE
        */
        TriMeshHE();

        /*!
        * \fn TriMeshHE
        * \brief Constructor of TriMeshHE with various vertex properties.
        *        Other properties can still be requested when needed.
        * \param _vertNormals : true if mesh needs vertex normals
        * \param _vertTexCoords2D : true if mesh needs vertex texCoords2D
        * \param _vertCol : true if mesh needs vertex colors
        * \param _edgeTexCoords2D : true if mesh needs halfedge texCoords2D
        */
        TriMeshHE(bool _vertNormals, bool _vertTexCoords2D, bool _vertCol, bool _edgeTexCoords2D);

        /*!
        * \fn ~TriMeshHE
        * \brief Destructor of TriMeshHE
        */
        ~TriMeshHE();


        /*------------------------------------------------------------------------------------------------------------+
        |                                              GETTERS/SETTERS                                                |
        +-------------------------------------------------------------------------------------------------------------*/

        /*! \fn getVertices */
        void getVertices(std::vector<glm::vec3>& _vertices);
        /*! \fn getNormals */
        void getNormals(std::vector<glm::vec3>& _normals);
        /*! \fn getIndices */
        void getIndices(std::vector<uint32_t>& _indices);

        /*! \fn getColors */
        void getColors(std::vector<glm::vec3>& _colors);
        /*! \fn getTexCoords */
        void getTexCoords(std::vector<glm::vec2>& _texcoords);
        /*! \fn getTangents */
        void getTangents(std::vector<glm::vec3>& _tangents);
        /*! \fn getBitangents */
        void getBitangents(std::vector<glm::vec3>& _bitangents);

        /*! \fn getFaceNormals */
        void getFaceNormals(std::vector<glm::vec3>& _facenormals);

        /*------------------------------------------------------------------------------------------------------------+
        |                                               OTHER METHODS                                                 |
        +-------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn readFile
        * \brief read a mesh from a file
        * \param _filename : name of the file to read
        * \return false if file extension is not supported, true if it is
        */
        bool readFile(const std::string& _filename);

        /*!
        * \fn writeFile
        * \brief write a mesh into a file
        * \param _filename : name of the file to write
        * \return false if file extension is not supported, true if it is
        */
        bool writeFile(const std::string& _filename);

        /*!
        * \fn computeAABB
        * \brief compute Axis Oriented Bounding Box
        */
        void computeAABB();

        /*!
        * \fn computeNormals
        * \brief recompute the triangle normals and update vertex normals
        */
        void computeNormals();

        /*!
        * \fn computeTB
        * \brief Compute tangent and bitangent vectors for all vertices of the mesh
        *        http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
        */
        void computeTB();

        /*!
        * \fn lapSmooth
        * \brief Apply a Laplacian smoothing of the mesh
        * \param _nbIter : number of iterations
        * \param _fact : factor applied to the Laplacian vector for the displacement of the vertices 
        *                (if _fact == 1 then the vertex is displaced by the complete Lapacian vector)
        */
        void lapSmooth(unsigned int _nbIter = 1, float _fact = 1.0f );

        void duplicateVertices();


        /*------------------------------------------------------------------------------------------------------------+
        |                                                 CURVATURE                                                   |
        +-------------------------------------------------------------------------------------------------------------*/


        /*!
        * \fn computeMeanCurv
        * \brief Compute mean curvature of the mesh
        * Using Meyer et al., "Discrete Differential-Geometry Operators for Triangulated 2-Manifolds", Visualization and Mathematics III, 2003
        * http://multires.caltech.edu/pubs/diffGeoOps.pdf
        */
        void computeMeanCurv();

        /*!
        * \fn computeMeanCurv
        * \brief Compute surface variation of the mesh
        * Using Pauly et al., "Efficient Simplification of Point-Sampled Surfaces", IEEE Visualization, 2002
        * https://www.graphics.rwth-aachen.de/media/papers/p_Pau021.pdf
        */
        void computeSurfVar();


    protected:


        /*------------------------------------------------------------------------------------------------------------+
        |                                                ATTRIBUTES                                                   |
        +-------------------------------------------------------------------------------------------------------------*/

        OpMesh m_mesh;                                          /*!< OpenMesh data structure */

        OpenMesh::HPropHandleT<OpenMesh::Vec3f> tangents;       /* tangent property on vertices */ 
        OpenMesh::HPropHandleT<OpenMesh::Vec3f> bitangents;     /* bitangent property on vertices */ 


        /*------------------------------------------------------------------------------------------------------------+
        |                                                 CURVATURE                                                   |
        +-------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn compLocalVariation
        * \brief surface variation  at a given vertex
        * \param _hi: iterator of the vertex where surface variation is calculated
        * \return local variation at vertex _hi
        */
        double compLocalVariation(OpMesh::VertexIter _hi);

        /*!
        * \fn compLocalVariation
        * \brief discrete mean curvature at a given vertex
        * \param _hi: iterator of the vertex where surface variation is calculated
        * \return mean curvature at vertex _xi
        */
        double vertMeanCurv(OpMesh::VertexIter _xi);

        /*!
        * \fn compAreaMixed
        * \brief compute the "mixed area" of triangles connected to a given vertex.
        * see Fig 4. in  http://multires.caltech.edu/pubs/diffGeoOps.pdf 
        * \param _xi: iterator of the vertex, which area of connected triangles is computed
        * \return mixed area of 1-ring neighborhood of _xi
        */
        double compAreaMixed(OpMesh::VertexIter _xi);

        /*!
        * \fn compAreaVoronoi
        * \brief compute area of a Voronoi region , defined by a triangle facet _f and one of its vertices _xi.
        * The Voronoi region is the the part of the Triangle _f that isbelongs to the Voronoi cell of the vertex _xi.
        * see Fig 3. in  http://multires.caltech.edu/pubs/diffGeoOps.pdf 
        * \param _f: iterator of the triangle face
        * \param _xi: iterator of the vertex (Voronoi cell's seed)
        * \return area of the fraction of triangle _f that is the Voronoi cell of _xi
        */
        float compAreaVoronoi(OpMesh::VertexIter _xi, OpMesh::FaceIter _f);

        /*!
        * \fn compSumCot
        * \brief compute cotan(alpha_ij)+cotan(beta_ij) , for two connected vertices x_i and x_j. 
        * The edge (x_i, x_j) connects two triangle facets. 
        * The opposit angle on one triangle is alpha_ij.
        * The opposit angle on the second triangle is beta_ij.
        * For a given vertex x_i, x_j is accessed through a vertex-vertex iterator (circulates around all vertices connected to x_i, i.e., 1-st ring neighborhood)
        * The angle alpha_ij is therefore located at the "previous" vertex on the ring (x_j-1).
        * The angle beta_ij is similarly located at the "next" vertex on the ring (x_j+1).
        * \param _xi, _xj: iterators of vertices x_i and x_j
        * \param _xjm: iterator of vertex where alpha_ij is located (x_j-1).
        * \param _xjp: iterator of vertex where beta_ij is located (x_j+1).                  
        * \return value of cotan(alpha_ij)+cotan(beta_ij)
        */
        double compSumCot(OpMesh::VertexIter _xi, OpMesh::VertexIter _xj, OpMesh::VertexIter _xjm, OpMesh::VertexIter _xjp);

        /*!
        * \fn compAreaTriangle
        * \brief compute area of a triangle facet 
        * \param _f: iterator of the triangle face
        * \return area of triangle
        */
        double compAreaTriangle(OpMesh::FaceIter _f);

        /*!
        * \fn isTriangleObtuse
        * \brief test if a facet is an obtuse triangle (i.e., contains an obtuse angle). 
        * \param _f: iterator of the triangle face to test
        * \return true if triangle is obtuse
        */
        bool isTriangleObtuse(OpMesh::FaceIter _f);

        /*!
        * \fn isAngleObtuse
        * \brief test if an angle is obtuse. 
        * The angle is defined by the facet and vertex to which it is connected.
        * \param _f: iterator of the triangle face in which the angle is located
        * \param _xi: iterator of the vertex which corresponds to the angle corner in _f
        * \return true if angle is obtuse
        */
        bool isAngleObtuse(OpMesh::FaceIter _f, OpMesh::VertexIter _xi);

};
#endif // TRIMESHHE_H