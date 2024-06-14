/*********************************************************************************************************************
 *
 * camera.h
 *
 * A perspective or orthographic camera
 * 
 * QGL_toolkit
 * Ludovic Blache
 *
 *********************************************************************************************************************/

#ifndef QGLTOOLKIT_CAMERA_H
#define QGLTOOLKIT_CAMERA_H

#include "cameraFrame.h"

namespace qgltoolkit 
{


/*!
* \class Camera
* \brief A perspective or orthographic camera.
*/
class Camera : public qgltoolkit::CameraFrame
{
    //Q_OBJECT

    private:

        // mutable: can be modfied in a function foo() const
        mutable glm::mat4 m_viewMatrix = glm::mat4(1.0);        /*!< view matrix */
        mutable bool m_viewMatrixIsUpToDate = false;            /*!< false if view matrix has been modified */
        mutable glm::mat4 m_projectionMatrix = glm::mat4(1.0);  /*!< projection matrix */
        mutable bool m_projectionMatrixIsUpToDate = false ;     /*!< false if projection matrix has been modified*/

        glm::vec3 m_position = glm::vec3(0.0);                  /*!< position of camera in 3D space*/
        glm::vec3 m_viewDirection = glm::vec3(0.0);             /*!< view direction vector */
        glm::vec3 m_upVector = glm::vec3(0.0);                  /*!< up direction vector*/
        glm::vec3 m_sceneCenter = glm::vec3(0.0);               /*!< center of 3D scene to look at */
        double m_orthoCoef = 1.0;                               /*!< defines dimensions for orthogonal projection */
        //double m_zClippingCoef;                                 /*!< defines margin between scene radius and frustum borders  */

        //QPointF m_prevPos;

   public:


       /*------------------------------------------------------------------------------------------------------------+
        |                                               CONSTRUCTORS                                                  |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn ~Camera
        * \brief Default contructor of Camera.
        */
       Camera() :
           m_viewMatrix(1.0),
           m_viewMatrixIsUpToDate(false),
           m_projectionMatrix(1.0),
           m_projectionMatrixIsUpToDate(false),
           m_position(0.0),
           m_viewDirection(0.0),
           m_upVector(0.0),
           m_sceneCenter(0.0),
           m_orthoCoef(1.0)
           //m_zClippingCoef(1.0)
       {
           computeProjectionMatrix();
           computeViewMatrix();
       }

       /*!
       * \fn CameraFrame
       * \brief Copy constructor of Camera.
       */
       Camera(const Camera& _camera)
           : CameraFrame(_camera),
           m_position(_camera.m_position),
           m_viewDirection(_camera.m_viewDirection),
           m_upVector(_camera.m_upVector),
           m_sceneCenter(_camera.m_sceneCenter),
           m_orthoCoef(_camera.m_orthoCoef),
           m_viewMatrixIsUpToDate(false),
           m_projectionMatrixIsUpToDate(false)
           //m_zClippingCoef(_camera.m_zClippingCoef)
       {
           computeProjectionMatrix();
           computeViewMatrix();
       }

       /*!
       * \fn operator=
       * \brief Copy assignment operator of Camera
       */
       Camera& operator=(const Camera& _camera)
       {
           CameraFrame::operator=(_camera);

           m_position = _camera.m_position;
           m_viewDirection = _camera.m_viewDirection;
           m_upVector = _camera.m_upVector;
           m_orthoCoef = _camera.m_orthoCoef;
           m_sceneCenter = _camera.m_sceneCenter;
           //m_zClippingCoef = _camera.m_zClippingCoef;

           m_viewMatrixIsUpToDate = false;
           m_projectionMatrixIsUpToDate = false;

           computeProjectionMatrix();
           computeViewMatrix();

           return *this;
       }

       /*!
       * \fn ~Camera
       * \brief Destructor of Camera.
       */
       virtual ~Camera()
       {
       }

        /*------------------------------------------------------------------------------------------------------------+
        |                                                  GETTERS                                                    |
        +------------------------------------------------------------------------------------------------------------*/



        /*!
        * \fn screenWidth
        * \brief Returns screen width.
        */
        //int screenWidth() const {  }

        /*!
        * \fn screenHeight
        * \brief Returns screen height.
        */
        //int screenHeight() const {  }

        /*!
        * \fn aspectRatio
        * \brief Calculates and returns aspect ratio.
        */
        //double aspectRatio() const {  }

        /*!
        * \fn fieldOfView
        * \brief Returns FOV.
        */
        //double fieldOfView() const {  }

        /*!
        * \fn projType
        * \brief Returns projection type.
        */
        //ProjectionType projType() const {  }

        /*!
        * \fn projectionMatrix
        * \brief Returns projection matrix.
        */
        glm::mat4 projectionMatrix() const { return m_projectionMatrix; }
        /*!
        * \fn viewMatrix
        * \brief Returns view matrix.
        */

        glm::mat4 viewMatrix() const { return m_viewMatrix; }

        /*!
        * \fn viewProjectionMatrix
        * \brief Calculates and returns view projection Matrix.
        */
        glm::mat4 viewProjectionMatrix() const { return  m_projectionMatrix * m_viewMatrix; }

        /*!
        * \fn sceneCenter
        * \brief Returns cords of scene center.
        */
        //glm::vec3 sceneCenter() const { return m_sceneCenter; }

        /*!
        * \fn sceneRadius
        * \brief Returns scene radius.
        */
        //double sceneRadius() const {  }

        /*!
        * \fn distanceToSceneCenter
        * \brief Calculates and returns distance between camera and scene center.
        */
        //double distanceToSceneCenter() const {  }

        /*!
        * \fn zClippingCoefficient
        * \brief Returns zClippingCoefficient.
        */
        //double zClippingCoefficient() const { return m_zClippingCoef; }

        /*!
        * \fn zOrthoCoefficient
        * \brief Returns zOrthoCoefficient.
        */
        double zOrthoCoefficient() const { return m_orthoCoef; }

        /*!
        * \fn position
        * \brief Returns camera frame position.
        */
        glm::vec3 position() const 
        { 
            return m_position; 
        }


        /*!
        * \fn upVector
        * \brief Returns up vector.
        */
        glm::vec3 upVector() const { return m_upVector; }

        /*!
        * \fn viewDirection
        * \brief Returns view direction.
        */
        glm::vec3 viewDirection() const { return m_viewDirection; } 

        /*!
        * \fn orientation
        * \brief Returns camera frame orientation.
        */
        //Quaternion orientation() const { return m_frame.orientation(); }


        /*------------------------------------------------------------------------------------------------------------+
        |                                                  MATRICES                                                   |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn zNear
        * \brief Calculates and returns z-near distance.
        */
        //virtual double zNear() const
        //{

        //    double zNear = distanceToSceneCenter() - zClippingCoefficient() * sceneRadius();

        //    // Prevents negative or null zNear values.
        //    const double zMin = 0.1f;
        //    if (zNear < zMin)
        //        zNear = zMin;

        //    return zNear;
        //}

        /*!
        * \fn zFar
        * \brief Calculates and returns z-far distance.
        */
        //virtual double zFar() const
        //{
        //    return distanceToSceneCenter() + zClippingCoefficient() * sceneRadius();
        //}

        /*!
        * \fn computeProjectionMatrix
        * \brief Calculates projection matrix.
        */
        void computeProjectionMatrix()
        {
            if (m_projectionMatrixIsUpToDate)
                return;

            //const double ZNear = zNear();
            //const double ZFar = zFar();

            //switch (projType()) 
            //{
                //case CameraFrame::PERSPECTIVE: 
                //{
                //    const double f = 1.0 / tan(fieldOfView() / 2.0);

                //    m_projectionMatrix = glm::perspective(fieldOfView(), aspectRatio(), zNear(), zFar() ); 
                //    
                //    break;
                //}
                //case CameraFrame::ORTHOGRAPHIC: 
                //{
                //    double halfWidth = m_orthoCoef * ((aspectRatio() < 1.0) ? 1.0 : aspectRatio());
                //    double halfHeight = m_orthoCoef * ((aspectRatio() < 1.0) ? 1.0 / aspectRatio() : 1.0);

                //    m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, zNear(), zFar() ); 
                //    break;
                //}
            //}

            double zNear = glm::distance( m_sceneCenter, m_position) - m_sceneRadius * 1.01f;
            double zFar = glm::distance(m_sceneCenter, m_position) + m_sceneRadius * 1.01f;

            m_projectionMatrix = glm::perspective(fieldOfView(), aspectRatio(), zNear, zFar);

            m_projectionMatrixIsUpToDate = true;
        }

        /*!
        * \fn computeViewMatrix
        * \brief Calculates view matrix.
        */
        void computeViewMatrix() const
        {
            if (m_viewMatrixIsUpToDate)
                return;

            const Quaternion q = orientation();
            const glm::vec3 t = q.inverseRotate(position());

            glm::vec3 diff = glm::normalize(glm::vec3(m_sceneCenter - position()));
            glm::vec3 axis = glm::normalize(orientation().axis());

            const float q00 = 2.0 * q[0] * q[0];
            const float q11 = 2.0 * q[1] * q[1];
            const float q22 = 2.0 * q[2] * q[2];

            const float q01 = 2.0 * q[0] * q[1];
            const float q02 = 2.0 * q[0] * q[2];
            const float q03 = 2.0 * q[0] * q[3];

            const float q12 = 2.0 * q[1] * q[2];
            const float q13 = 2.0 * q[1] * q[3];

            const float q23 = 2.0 * q[2] * q[3];
            glm::vec3 quatZ = glm::normalize(glm::vec3(q02 + q13, q12 - q03, 1.0f - q11 - q00));
            glm::vec3 quatU = glm::normalize(glm::vec3(q01 - q23, 1.0 - q22 - q00, q12 + q03));
            m_viewMatrix = glm::lookAt(position(), /*m_pivotPoint*/ position() - quatZ , quatU );

            //m_viewMatrix = glm::lookAt(m_position, m_sceneCenter, m_upVector);

            m_viewMatrixIsUpToDate = true; 
        }

        /*!
        * \fn setScreenWidthAndHeight
        * \brief Set windows' dimensions.
        */
        void setScreenWidthAndHeight(int _width, int _height) 
        {
            CameraFrame::setScreenWidthAndHeight(_width, _height);

            m_projectionMatrixIsUpToDate = false;
            computeProjectionMatrix();
        }

        /*!
        * \fn setViewDirection
        * \brief Set view direction vector.
        * \param _direction: direction as a 3D vector
        */
        void setViewDirection(glm::vec3& _direction)
        {
            if (_direction == glm::vec3(0.0f) )
                return;

            m_viewDirection = glm::normalize(_direction);
        }

        /*!
        * \fn lookAt
        * \brief Set view direction vector, given a target position.
        * The view direction is calculated so it is oriented towards (or "look at") the target point.
        * \param _target: 3D coords of a point in scene to look at
        */
        void lookAt(glm::vec3& _target)
        {
            glm::vec3 dir = _target - position();
            setViewDirection(dir);
        }

        /*!
        * \fn cameraCoordinatesOf
        * \brief Transfers coordinates of a point from world space to camera space.
        * \param _src: point coords in world space
        * \return point coords in camera space
        */
        //glm::vec3 cameraCoordinatesOf(const glm::vec3& _src) const {  }

        /*!
        * \fn worldCoordinatesOf
        * \brief Transfers coordinates of a point from camera space to world space.
        * \param _src: point coords in camera space
        * \return point coords in world space
        */
        //glm::vec3 worldCoordinatesOf(const glm::vec3& _src) const {  }

        /*!
        * \fn getOrthoWidthHeight
        * \brief Calculates half diemrensions of window when orthographic projection is used.
        * \param _halfWidth, _halfHeight: half dimensions to be returned
        */
        //virtual void getOrthoWidthHeight(double& _halfWidth, double& _halfHeight) const 
        //{
        //}


        /*------------------------------------------------------------------------------------------------------------+
        |                                                  SETTERS                                                    |
        +------------------------------------------------------------------------------------------------------------*/


        /*! \fn setPivotPoint 
        * \brief Set pivot point.
        */
        //void setPivotPoint(const glm::vec3& _point)
        //{
        //}

        /*! \fn setAspectRatio 
        * \brief Set dimesnions from aspect ratio.
        */
        /*void setAspectRatio(double _aspect) 
        {
        }*/

        /*! \fn setFieldOfView 
        * \brief Set FOV.
        */
        //void setFieldOfView(double _fov)
        //{
        //}

        /*! \fn setFOVToFitScene 
        * \brief Set FOV so it contains the entire scene.
        */
        /*void setFOVToFitScene()
        {
        }*/

        /*! \fn setProjType 
        * \brief Set type of projection.
        */
        //void setProjType(CameraFrame::ProjectionType _type)
        //{
        //}

        /*! \fn setSceneCenter 
        * \brief Set scene center.
        */
        void setSceneCenter(const glm::vec3& _center)
        {
            m_sceneCenter = _center;
            m_viewMatrixIsUpToDate = false;
            m_projectionMatrixIsUpToDate = false;
        }

        /*! \fn setSceneRadius 
        * \brief Set scene radius.
        */
        void setSceneRadius(double _radius)
        {
            m_sceneRadius = _radius;
            m_viewMatrixIsUpToDate = false;
            m_projectionMatrixIsUpToDate = false;
        }

        /*! \fn setSceneBoundingBox 
        * \brief Set scene center and radius given an axis aligned bounding box.
        * \param _min, _max: min amd max corners of the AABBox
        */
        void setSceneBoundingBox(glm::vec3& _min, glm::vec3& _max)
        {
            m_q = qgltoolkit::Quaternion();

            setSceneCenter((_min + _max) / 2.0f);
            setPivotPoint(m_sceneCenter);
            setSceneRadius(0.5f * glm::length( _max - _min ) );

            setPosition(m_sceneCenter + glm::vec3(0.0f, 0.0f, m_sceneRadius * 2.0f) );
            glm::vec3 dir = m_sceneCenter - position();
            setViewDirection(dir);
            setUpVector(glm::vec3(0.0f, 1.0f, 0.0f));

            m_viewMatrixIsUpToDate = false;
            m_projectionMatrixIsUpToDate = false;
            computeViewMatrix();
            computeProjectionMatrix();
            
        }

        /*! \fn setZClippingCoefficient 
        * \brief Set ZClippingCoefficient.
        */
        //void setZClippingCoefficient(double _coef) 
        //{
        //    m_zClippingCoef = _coef;
        //    m_projectionMatrixIsUpToDate = false;
        //}

        /*! \fn setPosition 
        * \brief Set camera frame position.
        */
        void setPosition(const glm::vec3& _pos) 
        { 
            m_position = _pos;
            m_t = m_position;
        }


        /*! \fn setOrientation 
        * \brief Set camera frame orientation from quaternion.
        * \param _q: orientation as quaternion
        */
        //void setOrientation(const Quaternion& _q)
        //{
        //}

        /*! \fn setOrientation 
        * \brief Set camera frame orientation from two angles.
        * \param _theta, _phi: orientation as angles
        */
        //void setOrientation(double _theta, double _phi)
        //{
        //}

        /*! \fn setUpVector 
        * \brief Set up vector
        */
        void setUpVector(const glm::vec3& _up)
        {
            m_upVector = _up;
        }


        /*------------------------------------------------------------------------------------------------------------+
        |                                                   MISC.                                                     |
        +------------------------------------------------------------------------------------------------------------*/

        /*! \fn fitSphere 
        * \brief Moves the Camera so that the sphere defined by _center 
        * and _radius is visible and fits in the frustum.
        * \param _center: sphere center
        * \param _radius: sphere radius
        */
        //void fitSphere(const glm::vec3& _center, double _radius)
        //{
        //    float distance = 0.0;
        //    switch (projType()) 
        //    {
        //        case CameraFrame::PERSPECTIVE: 
        //        {
        //            distance = _radius / sin(fieldOfView() / 2.0);
        //            break;
        //        }
        //        case CameraFrame::ORTHOGRAPHIC: 
        //        {
        //            distance = glm::dot( (_center - pivotPoint()), viewDirection()) + (_radius / m_orthoCoef);
        //            break;
        //        }
        //    }
        //    glm::vec3 newPos(_center - (viewDirection() * distance) );

        //    m_frame.setPosition(newPos);
        //}

        /*! \fn showEntireScene 
        * \brief Moves the Camera so that the entire scene is visible.
        * The scene is described by its center coords and its radius.
        */
        //void showEntireScene() { fitSphere(sceneCenter(), sceneRadius()); }

        /*! \fn fitBoundingBox 
        * \brief Moves the Camera so that the entire scene is visible.
        * The scene is described by a axis-aligned bounding box.
        * \param _min, _max: min amd max corners of the AABBox
        */
        //void fitBoundingBox(const glm::vec3& _min, const glm::vec3& _max)
        //{
        //    float diameter = std::max( std::abs(_max[1] - _min[1]),  std::abs(_max[0] - _min[0]));
        //    diameter = std::max( std::abs(_max[2] - _min[2]), diameter);
        //    fitSphere(0.5f * (_min + _max), 0.5f * diameter);
        //}

        /*! \fn centerScene 
        * \brief Moves the Camera so that its sceneCenter() is projected on the 
        * center of the window. 
        * The orientation() and fieldOfView() are unchanged.
        * Simply projects the current position on a line passing through sceneCenter(). 
        */
        //void centerScene(){ m_frame.projectOnLine(sceneCenter(), viewDirection()); }





        virtual void wheelEvent(QWheelEvent* const _event)
        {
            CameraFrame::wheelEvent(_event);

            m_position = m_t;
            m_viewMatrixIsUpToDate = false;
            computeViewMatrix();
            m_projectionMatrixIsUpToDate = false;
            computeProjectionMatrix();
        }

        virtual void mouseMoveEvent(QMouseEvent* const _event)
        {
            CameraFrame::mouseMoveEvent( _event);

            m_position = m_t;
            m_viewMatrixIsUpToDate = false;
            computeViewMatrix();
            m_projectionMatrixIsUpToDate = false;
            computeProjectionMatrix();

        }

        virtual void mouseDoubleClickEvent(QMouseEvent* event)
        {
            CameraFrame::mouseDoubleClickEvent(event);

            m_position = m_t;
            m_viewMatrixIsUpToDate = false;
            computeViewMatrix();
        }

}; // class Camera


} // namespace qgltoolkit

#endif // QGLTOOLKIT_CAMERA_H
