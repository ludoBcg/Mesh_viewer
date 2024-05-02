/*********************************************************************************************************************
 *
 * frame.h
 *
 * A Frame represents a coordinate system, defined by a position and an orientation
 * 
 * QGL_toolkit
 * Ludovic Blache
 *
 *
 * Based on the libQGLViewer library by Gilles Debunne
 * https://github.com/GillesDebunne/libQGLViewer
 *
 *********************************************************************************************************************/

#ifndef QGLTOOLKIT_FRAME_H
#define QGLTOOLKIT_FRAME_H


#include <QObject>
#include <QString>


#include "quaternion.h"


namespace qgltoolkit 
{


/*!
* \class Frame
* \brief A coordinate system, defined by a position and an orientation.
*
* A Frame is a 3D coordinate system, represented by a position() and an
* orientation(). The order of these transformations is important: 
* the Frame is first translated and  then rotated around the new translated origin.
*/
class Frame : public QObject 
{

    //Q_OBJECT


    protected:

        glm::vec3 m_t;               /*!< position (i.e., translation vector) */
        qgltoolkit::Quaternion m_q;  /*!< orientation (i.e., quaternion rotation) */


    //Q_SIGNALS:
  
    //    /*!
    //    * \fn modified
    //    * \brief Signal to send when Frame is modified.
    //    */
    //    void modified();



    public:

        /*------------------------------------------------------------------------------------------------------------+
        |                                                CONSTRUCTORS                                                 |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn Frame
        * \brief Destructor of Frame.
        */
        Frame() : QObject(), m_t(), m_q() {}

        /*!
        * \fn ~Frame
        * \brief Destructor of Frame.
        */
        virtual ~Frame() {}

        /*!
        * \fn Frame
        * \brief Constructor of Frame from position and orientation.
        * \param _position : position as 3D vector
        * \param _orientation : orientation as quaternion
        */
        Frame(const glm::vec3& _position, const Quaternion& _orientation)
            : m_t(_position), m_q(_orientation)
        {}
                
        /*!
        * \fn operator=
        * \brief Equal operator.
        */
        Frame& operator=(const Frame& _frame)
        {
            // Automatic compiler generated version would not emit the modified() signals
            // as is done in setTranslationAndRotation.
            setTranslationAndRotation(_frame.translation(), _frame.rotation());

            return *this;
        }

        /*!
        * \fn Frame
        * \brief Copy constructor of Frame.
        */
        Frame(const Frame& _frame) 
            : m_t(_frame.m_t), m_q(_frame.m_q) 
        {}



        /*------------------------------------------------------------------------------------------------------------+
        |                                                  GETTERS                                                    |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn translation
        * \brief Returns the Frame translation.
        * Similar to position().
        */
        glm::vec3 translation() const { return m_t; }

        /*!
        * \fn position
        * \brief Returns the Frame translation.
        * Similar to translation().
        */
        glm::vec3 position() const { return translation(); }

        /*!
        * \fn rotation
        * \brief Returns the Frame rotation.
        * Similar to orientation().
        */
        Quaternion rotation() const { return m_q; }

        /*!
        * \fn orientation
        * \brief Returns the Frame orientation.
        * Similar to rotation().
        */
        Quaternion orientation() const { return rotation(); }



        /*------------------------------------------------------------------------------------------------------------+
        |                                                  SETTERS                                                    |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn setTranslation
        * \brief Sets the translation of the frame.
        * Similar to setPosition()
        * \param _translation: translation 3D vector.
        */
        void setTranslation(const glm::vec3 _translation) 
        {
            m_t = _translation;
            //Q_EMIT modified();
        }

        /*!
        * \fn setTranslation
        * \brief Sets the translation of the frame.
        * Similar to setPosition()
        * \param _x, _y, _z: coordinates of 3D translation vector.
        */
        void setTranslation(double _x, double _y, double _z)
        {
            setTranslation( glm::vec3(_x, _y, _z) );
        }


        /*!
        * \fn setRotation
        * \brief Sets the rotation of the frame.
        * Similar to setOrientation().
        * \param _rotation: rotation quaternion.
        */
        void setRotation(const Quaternion& _rotation) 
        {
            m_q = _rotation;
            //Q_EMIT modified();
        }

        /*!
        * \fn setRotation
        * \brief Sets the rotation of the frame.
        * Similar to setOrientation().
        * \param _q0, _q1, _q2, _q3: coordinates of rotation quaternion.
        */
        void setRotation(double _q0, double _q1, double _q2, double _q3)
        {
            setRotation(Quaternion(_q0, _q1, _q2, _q3));
        }



        /*!
        * \fn setTranslationAndRotation
        * \brief Sets translation and rotation of the frame.
        * Similar to setPositionAndOrientation().
        * \param _translation: translation 3D vector.
        * \param _rotation: rotation quaternion.
        */
        void setTranslationAndRotation(const glm::vec3& _translation, const Quaternion& _rotation)
        {
            m_t = _translation;
            m_q = _rotation;
            //Q_EMIT modified();
        }


        /*!
        * \fn setPosition
        * \brief Sets the position of the frame.
        * Similar to setTranslation().
        * \param _position: position 3D vector.
        */
        void setPosition(const glm::vec3& _position)
        {
            setTranslation(_position);
        }

        /*!
        * \fn setPosition
        * \brief Sets the position of the frame.
        * Similar to setTranslation().
        * \param _x, _y, _z: coordinates of 3position 3D vector.
        */
        void setPosition(double _x, double _y, double _z)
        {
            setPosition( glm::vec3(_x, _y, _z) );
        }


        /*!
        * \fn setOrientation
        * \brief Sets the orientation of the frame.
        * Similar to setRotation().
        * \param _rotation: orientation quaternion.
        */
        void setOrientation(const Quaternion& _orientation)
        {
            setRotation(_orientation);
        }

        /*!
        * \fn setOrientation
        * \brief Sets the orientation of the frame.
        * Similar to setRotation().
        * \param _q0, _q1, _q2, _q3: coordinates of orientation quaternion.
        */
        void setOrientation(double _q0, double _q1, double _q2, double _q3)
        {
            setOrientation(Quaternion(_q0, _q1, _q2, _q3));
        }


        /*!
        * \fn setPositionAndOrientation
        * \brief Sets position and orientation of the frame.
        * Similar to setTranslationAndRotation().
        * \param _position: position 3D vector.
        * \param _orientation: orientation quaternion.
        */
        void setPositionAndOrientation(const glm::vec3& _position, const Quaternion& _orientation)
        {

            m_t = _position;
            m_q = _orientation;

            //Q_EMIT modified();
        }


        /*------------------------------------------------------------------------------------------------------------+
        |                                              TRANSFORMATIONS                                                |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn translate
        * \brief Translates frame by a given vector.
        * \param _t: translation vector
        */
        void translate(glm::vec3& _t)
        {
            m_t += _t;
            //Q_EMIT modified();
        }

        /*!
        * \fn translate
        * \brief Translates frame by a given vector.
        * \param _x, _y, _z: coordinates of translation vector
        */
        void translate(double _x, double _y, double _z) 
        {
            glm::vec3 t(_x, _y, _z);
            translate(t);
        }

        /*!
        * \fn rotate
        * \brief Rotates frame by a given quaternion.
        * \param _q: rotation quaternion
        */
        void rotate(Quaternion& _q)
        {
            m_q *= _q;
            m_q.normalize(); // Prevents numerical drift
            //Q_EMIT modified();
        }

        /*!
        * \fn rotate
        * \brief Rotates frame by a given quaternion.
        * \param _q0, _q1, _q2, _q3: coordinates of rotation quaternion.
        */
        void rotate(double _q0, double _q1, double _q2, double _q3) 
        {
            Quaternion q(_q0, _q1, _q2, _q3);
            rotate(q);
        }

        /*!
        * \fn transformOf
        * \brief Returns the Frame transform of a vector _src defined in the world
        * coordinate system (converts vectors from world to Frame).
        * inverseTransformOf() performs the inverse transformation. 
        * coordinatesOf() converts 3D coordinates instead of 3D vectors 
        * (here only the rotational part of the transformation is taken into account).
        */
        glm::vec3 transformOf(const glm::vec3 _src) const
        {
            return rotation().inverseRotate(_src);
        }

        /*!
        * \fn inverseTransformOf
        * \brief Returns the world transform of the vector whose coordinates in the Frame
        * coordinate system is _src (converts vectors from Frame to world).
        * transformOf() performs the inverse transformation. 
        * Use inverseCoordinatesOf() to transform 3D coordinates instead of 3D vectors.
        */
        glm::vec3 inverseTransformOf(const glm::vec3 _src) const
        {
            return rotation().rotate(_src);
        }

        /*!
        * \fn coordinatesOf
        * \brief Returns the Frame coordinates of a point _src defined in the world
        * coordinate system (converts from world to Frame).
        * inverseCoordinatesOf() performs the inverse convertion. 
        * transformOf() converts 3D vectors instead of 3D coordinates.
        */
        glm::vec3 coordinatesOf(const glm::vec3& _src) const
        {
            return rotation().inverseRotate(_src - translation());
        }

        /*!
        * \fn inverseCoordinatesOf
        * \brief Returns the world coordinates of the point whose position in the Frame
        * coordinate system is _src (converts from Frame to world).
        * coordinatesOf() performs the inverse convertion. 
        * Use inverseTransformOf() to transform 3D vectors instead of 3D coordinates.
        */
        glm::vec3 inverseCoordinatesOf(const glm::vec3& _src) const
        {
            return rotation().rotate(_src) + translation();
        }

        /*!
        * \fn rotateAroundPoint
        * \brief Rotates frame around a given point.
        * _point is defined in the world coordinate system, while the _rotation axis
        * is defined in the Frame coordinate system.
        * \param _rotation: rotation quaternion
        * \param _point: rotation center
        */
        void rotateAroundPoint(Quaternion& _rotation, const glm::vec3& _point)
        {
            m_q *= _rotation;
            m_q.normalize(); // Prevents numerical drift
            glm::vec3 trans = _point + 
                Quaternion(inverseTransformOf(_rotation.axis()), _rotation.angle()).rotate(position() - _point)
                - m_t;

            m_t += trans;
            
            //Q_EMIT modified();
        }

};


} // namespace qgltoolkit

#endif // QGLTOOLKIT_FRAME_H
