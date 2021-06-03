#ifndef TRIMESHHE_H
#define TRIMESHHE_H

#include "mesh.h"

// -------------------- OpenMesh
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

        /*------------------------------------------------------------------------------------------------------------+
        |                                               OTHER METHODS                                                 |
        +-------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn readFile
        * \brief read a mesh from a file
        * \param _filename : name of the file to read
        * \return false if file extension is not supported, true if it is
        */
        bool readFile(std::string _filename);

        /*!
        * \fn writeFile
        * \brief write a mesh into a file
        * \param _filename : name of the file to write
        * \return false if file extension is not supported, true if it is
        */
        bool writeFile(std::string _filename);

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

    protected:


        /*------------------------------------------------------------------------------------------------------------+
        |                                                ATTRIBUTES                                                   |
        +-------------------------------------------------------------------------------------------------------------*/

        OpMesh m_mesh;                                          /*!< OpenMesh data structure */

        OpenMesh::HPropHandleT<OpenMesh::Vec3f> tangents;       /* tangent property on vertices */ 
        OpenMesh::HPropHandleT<OpenMesh::Vec3f> bitangents;     /* bitangent property on vertices */ 

        /*------------------------------------------------------------------------------------------------------------+
        |                                               OTHER METHODS                                                 |
        +-------------------------------------------------------------------------------------------------------------*/

};
#endif // TRIMESHHE_H