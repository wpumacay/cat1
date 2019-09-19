
#include <camera/CFpsCamera.h>

namespace engine
{

    CFpsCamera::CFpsCamera( const std::string& name,
                            const LVec3& position,
                            const LVec3& targetPoint,
                            const eAxis& upAxis,
                            const CCameraProjData& projData,
                            float sensitivity,
                            float camSpeed,
                            float camMaxDelta )
        : CICamera( name, position, targetPoint, upAxis, projData )
    {
        m_type = eCameraType::FPS;

        m_sensitivity   = sensitivity;
        m_camSpeed      = camSpeed;
        m_camSpeedFront = 0.0f;
        m_camSpeedRight = 0.0f;
        m_camMaxDelta   = camMaxDelta;

        m_roll  = 0.0f;
        m_pitch = 0.0f;
        m_yaw   = -90.0f;

        m_lastCursorPos = { 0.0f, 0.0f };

        // initialize looking at the target
        m_front = LVec3::normalize( m_targetPoint - m_position );
        m_right = LVec3::normalize( LVec3::cross( m_front, m_worldUp ) );
        m_up    = LVec3::normalize( LVec3::cross( m_right, m_front ) );
        _updateCameraAngles();

        // initialize view matrix to this orientation
        _buildViewMatrix();
    }

    void CFpsCamera::_positionChangedInternal()
    {
        // nothing for now, use the same front, right and up vectors
    }

    void CFpsCamera::_updateInternal()
    {
        if ( !m_active )
            return;

        m_camSpeedFront = 0.0f;
        m_camSpeedRight = 0.0f;

        if ( InputSystem::isKeyDown( ENGINE_KEY_W ) )
            m_camSpeedFront = m_camSpeed;

        else if ( InputSystem::isKeyDown( ENGINE_KEY_S ) )
            m_camSpeedFront = -m_camSpeed;

        else if ( InputSystem::isKeyDown( ENGINE_KEY_D ) )
            m_camSpeedRight = m_camSpeed;

        else if ( InputSystem::isKeyDown( ENGINE_KEY_A ) )
            m_camSpeedRight = -m_camSpeed;

        /* compute camera angles from user cursor */
        LVec2 _currentCursorPos = InputSystem::getCursorPosition();
        float _xOff = _currentCursorPos.x - m_lastCursorPos.x;
        float _yOff = m_lastCursorPos.y - _currentCursorPos.y;

        _xOff = m_sensitivity * std::min( m_camMaxDelta, std::max( -m_camMaxDelta, _xOff ) );
        _yOff = m_sensitivity * std::min( m_camMaxDelta, std::max( -m_camMaxDelta, _yOff ) );

        m_yaw = m_yaw + _xOff;
        m_pitch = std::min( std::max( m_pitch + _yOff, -89.0f ), 89.0f );

        // update camera frame of reference using these angles
        _updateCameraVectors();

        // and mode a bit using this new reference frame
        auto _dFront = m_front * ( m_camSpeedFront * engine::CTime::GetTimeStep() );
        auto _dRight = m_right * ( m_camSpeedRight * engine::CTime::GetTimeStep() );
        m_position = m_position + _dFront + _dRight;

        // some book keeping for next calculation
        m_lastCursorPos = _currentCursorPos;
    }

    std::string CFpsCamera::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "front   : " + m_front.toString() + "\n\r";
        _strRep += "right   : " + m_right.toString() + "\n\r";
        _strRep += "up      : " + m_up.toString() + "\n\r";
        _strRep += "roll    : " + std::to_string( m_roll ) + "\n\r";
        _strRep += "pitch   : " + std::to_string( m_pitch ) + "\n\r";
        _strRep += "yaw     : " + std::to_string( m_yaw ) + "\n\r";
        _strRep += "senst.  : " + std::to_string( m_sensitivity ) + "\n\r";
        _strRep += "speed   : " + std::to_string( m_camSpeed ) + "\n\r";
        _strRep += "speed-f : " + std::to_string( m_camSpeedFront ) + "\n\r";
        _strRep += "speed-r : " + std::to_string( m_camSpeedRight ) + "\n\r";

        return _strRep;
    }

    void CFpsCamera::_updateCameraVectors()
    {
        if ( m_upAxis == eAxis::X )
        {
            m_front.x = std::sin( toRadians( m_pitch ) );
            m_front.y = std::cos( toRadians( m_pitch ) ) * std::sin( toRadians( m_yaw ) );
            m_front.z = std::cos( toRadians( m_pitch ) ) * std::cos( toRadians( m_yaw ) );
        }
        else if ( m_upAxis == eAxis::Y )
        {
            m_front.x = std::cos( toRadians( m_pitch ) ) * std::cos( toRadians( m_yaw ) );
            m_front.y = std::sin( toRadians( m_pitch ) );
            m_front.z = std::cos( toRadians( m_pitch ) ) * std::sin( toRadians( m_yaw ) );
        }
        else if ( m_upAxis == eAxis::Z )
        {
            m_front.x = std::cos( toRadians( m_pitch ) ) * std::sin( toRadians( m_yaw ) );
            m_front.y = std::cos( toRadians( m_pitch ) ) * std::cos( toRadians( m_yaw ) );
            m_front.z = std::sin( toRadians( m_pitch ) );
        }

        m_front = LVec3::normalize( m_front );
        m_right = LVec3::normalize( LVec3::cross( m_front, m_worldUp ) );
        m_up    = LVec3::normalize( LVec3::cross( m_right, m_front ) );

        _buildViewMatrix();
    }

    void CFpsCamera::_updateCameraAngles()
    {
        m_roll  = 0.0f;
        m_pitch = toDegrees( std::asin( m_front.z ) );
        m_yaw   = toDegrees( std::atan2( m_front.y, m_front.x ) );
    }

}