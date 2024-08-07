/*********************************************************************************************************************
 *
 * cameraFrame.h
 *
 * A Frame with Camera specific mouse bindings
 * 
 * QGL_toolkit
 * Ludovic Blache
 *
 *
 * Based on the libQGLViewer library by Gilles Debunne
 * http://www.libqglviewer.com
 *
 *********************************************************************************************************************/

#ifndef QGLTOOLKIT_CAMERA_FRAME_H
#define QGLTOOLKIT_CAMERA_FRAME_H

#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QObject>
#include <QMap>
#include <QPoint>
#include <QMouseEvent>


#include "frame.h"



namespace qgltoolkit 
{


/*!
* \class CameraFrame
* \brief A Frame that can be rotated and translated using the mouse.
*  It converts the mouse motion into a translation and an orientation updates.
*
* Emits a manipulated() signal each time its state is modified by the mouse. 
* This signal is automatically connected to the QGLViewer::update() slot when the 
* ManipulatedFrame is attached to a viewer using QGLViewer::setManipulatedFrame().
*
* A camera frame rotates around its pivotPoint(), which corresponds to the associated Camera::pivotPoint().
*/
class CameraFrame : public qgltoolkit::Frame 
{

    //Q_OBJECT


    public:

        // Camera projection type
        enum ProjectionType { PERSPECTIVE, ORTHOGRAPHIC };

        // Mouse actions
        enum MouseAction {
            NO_ACTION,
            ROTATE,
            ZOOM,
            TRANSLATE
            };


    protected:

        // interactions sensitivity
        double m_rotationSensitivity = 1.0;                     /*!< rotation sensitivity factor */
        double m_translationSensitivity = 1.0;                  /*!< translation sensitivity factor */
        double m_wheelSensitivity = 1.0;                        /*!< wheel sensitivity factor */
        double m_zoomSensitivity = 1.0;                         /*!< zoom sensitivity factor */

        // flags
        bool m_rotatesAroundUpVector = false;                   /*!< rotates around pivot point if false (defaulf behavior) */
        bool m_zoomsOnPivotPoint = true;                        /*!< zoom on pivot point if true (defaulf behavior) */

        // scene parameters
        glm::vec3 m_sceneUpVector = glm::vec3(0.0, 1.0, 0.0);   /*!< up direction vector */
        glm::vec3 m_pivotPoint = glm::vec3(0.0, 0.0, 0.0);      /*!< pivot point coords */
        double m_sceneRadius = 1.0;                             /*!< scene radius */

        // camera parameters
        int m_screenWidth = 1;                                  /*!< windows width (in pixels) */
        int m_screenHeight = 1;                                 /*!< windows height (in pixels) */          
        double m_fieldOfView = 0.0;                             /*!< fov angle (in radians) */
        double m_orthoCoef = 1.0;                               /*!< defines dimensions for orthogonal projection */
        ProjectionType m_projType = PERSPECTIVE;                /*!< camera projection type */
        
        // UI event
        MouseAction m_action = NO_ACTION;                       /*!< mouse action type */
        QPoint m_prevPos;                                       /*!< previous mouse cursor position */


    public:

        /*------------------------------------------------------------------------------------------------------------+
        |                                                CONSTRUCTORS                                                 |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn CameraFrame
        * \brief Default constructor of CameraFrame.
        */
        CameraFrame() 
            : m_rotationSensitivity(1.0)
            , m_translationSensitivity(1.0)
            , m_wheelSensitivity(1.0)
            , m_zoomSensitivity(1.0)
            , m_rotatesAroundUpVector(false)
            , m_zoomsOnPivotPoint(true)
            , m_sceneUpVector(0.0, 1.0, 0.0)
            , m_pivotPoint(0.0, 0.0, 0.0)
            , m_sceneRadius(1.0)
            , m_screenWidth(1)
            , m_screenHeight(1)
            , m_fieldOfView(0.0)
            , m_orthoCoef(1.0)
            , m_projType(PERSPECTIVE)
            , m_action(NO_ACTION)
        { }

        /*!
        * \fn CameraFrame
        * \brief Copy constructor of CameraFrame.
        */
        CameraFrame(const CameraFrame& _cf)
            : Frame(_cf)
            , m_rotationSensitivity(_cf.m_rotationSensitivity)
            , m_translationSensitivity(_cf.m_translationSensitivity)
            , m_wheelSensitivity(_cf.m_wheelSensitivity)
            , m_zoomSensitivity(_cf.m_zoomSensitivity)
            , m_rotatesAroundUpVector(_cf.m_rotatesAroundUpVector)
            , m_zoomsOnPivotPoint(_cf.m_zoomsOnPivotPoint)
            , m_sceneUpVector(_cf.m_sceneUpVector)
            , m_pivotPoint(_cf.m_pivotPoint)
            , m_sceneRadius(_cf.m_sceneRadius)
            , m_screenWidth(_cf.m_screenWidth)
            , m_screenHeight(_cf.m_screenHeight)
            , m_fieldOfView(_cf.m_fieldOfView)
            , m_orthoCoef(_cf.m_orthoCoef)
            , m_projType(_cf.m_projType)
            , m_action(NO_ACTION)
        { }

        /*!
        * \fn operator=
        * \brief Copy assignment operator of CameraFrame.
        */
        CameraFrame& operator=(const CameraFrame& _cf)
        {
            Frame::operator=(_cf);

            setRotationSensitivity(_cf.rotationSensitivity());
            setTranslationSensitivity(_cf.translationSensitivity());
            setWheelSensitivity(_cf.wheelSensitivity());
            setZoomSensitivity(_cf.zoomSensitivity());

            m_action = NO_ACTION;

            setSceneUpVector(_cf.sceneUpVector());
            setRotatesAroundUpVector(_cf.m_rotatesAroundUpVector);
            setZoomsOnPivotPoint(_cf.m_zoomsOnPivotPoint);

            setScreenWidthAndHeight(_cf.m_screenWidth, _cf.m_screenHeight);
            setProjType(_cf.m_projType);
            setPivotPoint(_cf.m_pivotPoint);
            setSceneRadius(_cf.m_sceneRadius);
            setFieldOfView(_cf.m_fieldOfView);
            setOrthoCoefficient(_cf.m_orthoCoef);

            return *this;
        }

        /*!
        * \fn ~CameraFrame
        * \brief Destructor of CameraFrame.
        */
        virtual ~CameraFrame() {}

        //Q_SIGNALS:

        //    /*! \fn manipulated 
        //    * \brief Signal when camera frame is being manipulated.
        //    */
        //    void manipulated();


        /*------------------------------------------------------------------------------------------------------------+
        |                                             GETTERS / SETTERS                                               |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn screenWidth
        * \brief Returns screen width.
        */
        int screenWidth() const { return m_screenWidth; }

        /*!
        * \fn screenHeight
        * \brief Returns screen height.
        */
        int screenHeight() const { return m_screenHeight; }

        /*!
        * \fn setScreenWidthAndHeight
        * \brief Set windows' dimensions.
        */
        void setScreenWidthAndHeight(int _width, int _height) 
        {
            // Prevent negative and zero dimensions that would cause divisions by zero.
            m_screenWidth = _width > 0 ? _width : 1;
            m_screenHeight = _height > 0 ? _height : 1;
        }

        /*!
        * \fn aspectRatio
        * \brief Get aspect ratio.
        */
        double aspectRatio()
        {
            return (double)screenWidth() / (double)screenHeight();
        }

        /*!
        * \fn sceneRadius
        * \brief Returns scene radius.
        */
        double sceneRadius() const { return m_sceneRadius; }

        /*! \fn setSceneRadius 
        * \brief Set scene radius.
        */
        void setSceneRadius(double _radius)
        {
            if (_radius <= 0.0) 
            {
                qCritical() << "[ERROR] CameraFrame::setSceneRadius: Scene radius must be positive - Ignoring value: "<< _radius;
                return;
            }
            m_sceneRadius = _radius;
        }

        /*!
        * \fn projType
        * \brief Returns projection type.
        */
        CameraFrame::ProjectionType projType() const { return m_projType; }

        /*! \fn setProjType 
        * \brief Set type of projection.
        */
        void setProjType(CameraFrame::ProjectionType _type) { m_projType = _type; }

        /*!
        * \fn fieldOfView
        * \brief Returns FOV.
        */
        double fieldOfView() const { return m_fieldOfView; }

        /*! \fn setFieldOfView 
        * \brief Set FOV.
        */
        void setFieldOfView(double _fov) { m_fieldOfView = _fov; }

        /*!
        * \fn orthoCoefficient
        * \brief Returns orthoCoefficient.
        */
        double orthoCoefficient() const { return m_orthoCoef; }

        /*! \fn setOrthoCoefficient
        * \brief Set orthoCoefficient.
        */
        void setOrthoCoefficient(double _orthoCoef) { m_orthoCoef = _orthoCoef; }

        /*!
        * \fn viewDirection
        * \brief Returns the normalized view direction of the Camera, 
        * defined in the world coordinate system.
        * This corresponds to the negative Z axis of the frame().
        */
        glm::vec3 viewDirection() const { return inverseTransformOf(glm::vec3(0.0f, 0.0f, -1.0f)); } 

        /*! \fn pivotPoint 
        * \brief Get pivot point.
        */
        glm::vec3 pivotPoint() const { return m_pivotPoint; }

        /*! \fn setPivotPoint 
        * \brief Set pivot point.
        */
        void setPivotPoint(const glm::vec3& _point) { m_pivotPoint = _point; }

        /*! \fn rotatesAroundUpVector 
        * \brief Get rotatesAroundUpVector flag.
        */
        bool rotatesAroundUpVector() const { return m_rotatesAroundUpVector; }

        /*! \fn setRotatesAroundUpVector
        * \brief Set rotatesAroundUpVector flag.
        */
        void setRotatesAroundUpVector(bool constrained) { m_rotatesAroundUpVector = constrained; }

        /*! \fn zoomsOnPivotPoint 
        * \brief Get zoomsOnPivotPoint flag.
        */
        bool zoomsOnPivotPoint() const { return m_zoomsOnPivotPoint; }

        /*! \fn setZoomsOnPivotPoint
        * \brief Set zoomsOnPivotPoint flag.
        */
        void setZoomsOnPivotPoint(bool _enabled) { m_zoomsOnPivotPoint = _enabled; }

        /*! \fn sceneUpVector 
        * \brief Get up vector.
        */
        glm::vec3 sceneUpVector() const { return m_sceneUpVector; }

        /*! \fn setSceneUpVector
        * \brief Set up vector.
        */
        void setSceneUpVector(const glm::vec3& _up) { m_sceneUpVector = _up; }

        /*! \fn rotationSensitivity 
        * \brief Get rotation sensitivity.
        */
        double rotationSensitivity() const { return m_rotationSensitivity; }

        /*! \fn setRotationSensitivity
        * \brief Set rotation sensitivity.
        */
        void setRotationSensitivity(double sensitivity) { m_rotationSensitivity = sensitivity; }
 
        /*! \fn translationSensitivity 
        * \brief Get translation sensitivity.
        */
        double translationSensitivity() const { return m_translationSensitivity; }

        /*! \fn setTranslationSensitivity
        * \brief Set translation sensitivity.
        */
        void setTranslationSensitivity(double sensitivity) { m_translationSensitivity = sensitivity; }
  
        /*! \fn zoomSensitivity 
        * \brief Get zoom sensitivity.
        */
        double zoomSensitivity() const { return m_zoomSensitivity; }

        /*! \fn setZoomSensitivity
        * \brief Set zoom sensitivity.
        */
        void setZoomSensitivity(double sensitivity) { m_zoomSensitivity = sensitivity; }
  
        /*! \fn wheelSensitivity 
        * \brief Get wheel sensitivity.
        */
        double wheelSensitivity() const { return m_wheelSensitivity; }

        /*! \fn setWheelSensitivity
        * \brief Set wheel sensitivity.
        */
        void setWheelSensitivity(double sensitivity) { m_wheelSensitivity = sensitivity; }

        /*! \fn currentMouseAction 
        * \brief Get mouse action.
        */
        MouseAction currentMouseAction() const { return m_action; }

        /*! \fn isManipulated 
        * \brief Returns true if camera frame is being manupilated.
        */
        bool isManipulated() const { return m_action != NO_ACTION; }
 

        void updateSceneUpVector()
        {
            m_sceneUpVector = inverseTransformOf( glm::vec3(0.0, 1.0, 0.0) );
        }


    private:


        /*------------------------------------------------------------------------------------------------------------+
        |                                      TRACKBALL / ZOOM TRANSFORMATIONS                                       |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn zoom
        * \brief Translates camera according to zoom delta
        * \param 
        */
        void zoom(double _delta)
        {
            if (projType() == ORTHOGRAPHIC)
            {
                if( (m_orthoCoef > 0.1f && _delta < 0.0) || (m_orthoCoef < 2.0f && _delta > 0.0) )
                    m_orthoCoef += _delta;
            }
            else if (projType() == PERSPECTIVE)
            {
                if (m_zoomsOnPivotPoint)
                {
                    glm::vec3 direction = position() - m_pivotPoint;

                    if ((glm::length(direction) > 0.01 * m_sceneRadius || _delta > 0.0) && (glm::length(direction) < 10.0 * m_sceneRadius || _delta < 0.0))
                    {
                        direction *= (float)_delta;
                        this->translate(direction);
                    }
                }
                else
                {
                    const float coef = std::max(std::abs(this->coordinatesOf(pivotPoint()).z), 0.2f * (float)m_sceneRadius);
                    glm::vec3 trans(0.0, 0.0, -coef * _delta);
                    trans = inverseTransformOf(trans);
                    this->translate(trans);
                }
            }
        }

        /*!
        * \fn projectOnBall
        * \brief Returns "pseudo-distance" from (_x,_y) to unit ball.
        * For a point inside the ball, it is proportional to the euclidean distance to the ball.
        * For a point outside the ball, it is proportional to the inverse of this distance 
        * (tends to zero) on the ball, the function is continuous.
        * \param _x, _y: 2D coords of point to project
        * \return distance to ball
        */
        static double projectOnBall(double _x, double _y) 
        {
            // If you change the size value, change angle computation in
            // deformedBallQuaternion().
            const double size = 1.0;
            const double size2 = size * size;
            const double size_limit = size2 * 0.5;

            const double d = _x * _x + _y * _y;
            return d < size_limit ? sqrt(size2 - d) : size_limit / sqrt(d);
        }

        /*!
        * \fn deformedBallQuaternion
        * \brief Returns a quaternion computed according to the mouse motion. 
        * Mouse positions are projected on a deformed ball, centered on (_cx, _cy).
        * \param _x, _y: 2D coords of point on screen
        * \param _cx, _cy: 2D coords of center point
        * \return trackball rotation as quaternion
        */
        Quaternion deformedBallQuaternion(int _x, int _y, double _cx, double _cy)
        {
            // Points on the deformed ball
            double px = rotationSensitivity() * (m_prevPos.x() - _cx) / screenWidth();
            double py = rotationSensitivity()  * (_cy - m_prevPos.y()) / screenHeight();
            double dx = rotationSensitivity() * (_x - _cx) / screenWidth();
            double dy = rotationSensitivity() * (_cy - _y) / screenHeight();

            const glm::vec3 p1(px, py, projectOnBall(px, py));
            const glm::vec3 p2(dx, dy, projectOnBall(dx, dy));
            // Approximation of rotation angle
            // Should be divided by the projectOnBall size, but it is 1.0
            const glm::vec3 axis = glm::cross(p2, p1);
            const double angle = 5.0 * asin(sqrt(squaredNorm(axis) / squaredNorm(p1) / squaredNorm(p2)));

            return Quaternion(axis, angle);
        }

        /*!
        * \fn deltaWithPrevPos
        * \brief Returns a screen scaled delta from event's position to m_prevPos,
        * along the X or Y direction, whichever has the largest magnitude.
        * \param _event: UI event
        */
        double deltaWithPrevPos(QMouseEvent *const _event) const
        {
            double dx = double(_event->position().x() - m_prevPos.x()) / screenWidth() ;
            double dy = double(_event->position().y() - m_prevPos.y()) / screenHeight() ;

            double value = std::abs(dx) > std::abs(dy) ? dx : dy;
            return value * zoomSensitivity();
        }

        /*!
        * \fn wheelDelta
        * \brief Returns a normalized wheel delta, proportionnal to wheelSensitivity().
        * \param _event: UI event
        */
        double wheelDelta(const QWheelEvent *_event) const
        {
            static const double WHEEL_SENSITIVITY_COEF = 1E-3;
            return _event->angleDelta().y() * wheelSensitivity() * WHEEL_SENSITIVITY_COEF;
        }


    public:


        /*------------------------------------------------------------------------------------------------------------+
        |                                                   EVENTS                                                    |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn mousePressEvent
        * \brief Initiates the mouse manipulation.
        * Previous mouse position is updated with current position.
        * \param _event: UI event
        */
        virtual void mousePressEvent(QMouseEvent *const _event) 
        {
            switch (_event->button())
            {
                case Qt::LeftButton:
                    m_action = ROTATE;
                    break;

                case Qt::MiddleButton:
                    m_action = TRANSLATE;
                    break;

                case Qt::RightButton:
                    m_action = ZOOM;
                    break;

                default:
                    m_action = NO_ACTION;
                    break;
            }
            m_prevPos = _event->pos();
        }

        /*!
        * \fn mouseReleaseEvent
        * \brief Stops the mouse manipulation.
        * \param _event: UI event
        */
        virtual void mouseReleaseEvent(QMouseEvent *const _event)
        {
            //Q_UNUSED(_event);

            m_action = NO_ACTION;
        }
        
        /*!
        * \fn mouseMoveEvent
        * \brief Modifies the camera frame according to the mouse motion.
        * Actual behavior depends on mouse action type.
        * Emits the manipulated() signal.
        * \param _event: UI event
        */
        virtual void mouseMoveEvent(QMouseEvent *const _event )
        {
            switch (m_action) 
            {
                case TRANSLATE: 
                {
                    //const QPoint delta = prevPos_ - event->pos();
                    const QPoint delta = _event->pos() - m_prevPos;
                    glm::vec3 trans(delta.x(), -delta.y(), 0.0);
                    // Scale to fit the screen mouse displacement
                    switch (m_projType) 
                    {
                        case PERSPECTIVE:
                        {
                            trans *= 2.0 * tan(fieldOfView() / 2.0) * std::abs((this->coordinatesOf(pivotPoint())).z) / screenHeight();
                            break;
                        }
                        case ORTHOGRAPHIC: 
                        {
                            trans[0] *= 2.0 / m_screenWidth;
                            trans[1] *= 2.0 / m_screenHeight;
                            break;
                        }
                    }
                    trans = inverseTransformOf((float)translationSensitivity() * -trans);
                    this->translate(trans);

                    break;
                }

                case ZOOM: 
                {
                    zoom(deltaWithPrevPos(_event) );
                    break;
                }

                case ROTATE: 
                {
                    Quaternion rot;
                    if (m_rotatesAroundUpVector) 
                    {
                        // Multiply by 2.0 to get on average about the same speed as with the
                        // deformed ball
                        double dx = 2.0 * rotationSensitivity() * (m_prevPos.x() - _event->position().x()) / screenWidth() ;
                        double dy = 2.0 * rotationSensitivity() * (m_prevPos.y() - _event->position().y()) / screenHeight() ;
                        dx = -dx;
                        dy = -dy;
                        glm::vec3 verticalAxis = transformOf(m_sceneUpVector);
                        rot = Quaternion(verticalAxis, dx) * Quaternion( glm::vec3(1.0, 0.0, 0.0), dy);
                        rotate(rot);
                    } 
                    else 
                    {   
                        glm::vec3 trans = this->coordinatesOf(pivotPoint()); //camera->projectedCoordinatesOf(pivotPoint());
                        rot = deformedBallQuaternion(_event->position().x(), _event->position().y(), trans[0], trans[1]);
                        
                        rotateAroundPoint(rot, pivotPoint());
                    }

                    break;
                }

                case NO_ACTION:
                    break;
            }

            if (m_action != NO_ACTION) 
            {
                m_prevPos = _event->pos();

                //Q_EMIT manipulated();
            }
        }


        virtual void mouseDoubleClickEvent(QMouseEvent* event)
        {
            Frame* frame = new Frame();
            frame->setTranslation(pivotPoint());
            
            alignWithFrame(frame, true);
        }
        

        /*!
        * \fn wheelEvent
        * \brief Call zoom acording to wheel delta
        * \param _event: UI event
        */
        virtual void wheelEvent(QWheelEvent *const _event )
        {
            zoom(-wheelDelta(_event) );
            //Q_EMIT manipulated();
        }

        /*!
        * \fn startAction
        * \brief Handles mouse events.
        * \param _ma: type of mouse action
        */
        virtual void startAction( int _ma) // int is really a QGLViewer::MouseAction
        {
            m_action = (MouseAction)(_ma);
        }

      

}; // class CameraFrame


} // namespace qgltoolkit

#endif // QGLTOOLKIT_CAMERA_FRAME_H
