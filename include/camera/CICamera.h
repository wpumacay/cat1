#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

namespace engine
{

    enum class eCameraType
    {
        BASE = 0,
        FIXED, FPS, ORBIT
    };

    std::string toString( const eCameraType& type );

    enum class eCameraProjection
    {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC,
    };

    std::string toString( const eCameraProjection& proj );

    struct CCameraProjData
    {
        eCameraProjection projection;
        /* perspective-camera parameters */
        float fov;
        float aspect;
        /* orthographics-camera parameters */
        float width;
        float height;
        /* common to both types*/
        float zNear;
        float zFar;
    };

    std::string toString( const CCameraProjData& projData );

    class CICamera
    {
    public :

        CICamera( const std::string& name,
                  const CVec3& position,
                  const CVec3& targetPoint,
                  const eAxis& upAxis,
                  const CCameraProjData& projData );

        virtual ~CICamera();

        static eCameraType GetStaticType() { return eCameraType::BASE; }

        void setPosition( const CVec3& position );
        void setActiveMode( bool activeMode );
        void setProjectionData( const CCameraProjData& projData );
        void update();
        void resize( int width, int height );
        std::string toString();

        std::string name() const { return m_name; }
        eCameraType type() const { return m_type; }
        eAxis upAxis() const { return m_upAxis; }
        bool active() const { return m_active; }

        CVec3 targetPoint() const { return m_targetPoint; }
        CVec3 targetDir() const { return m_targetDir; }
        CVec3 position() const { return m_position; }

        CCameraProjData projData() const { return m_projData; }

        CMat4 matView() const { return m_matView; }
        CMat4 matProj() const { return m_matProj; }

    protected :

        void _buildViewMatrix();
        void _buildProjMatrix();

        virtual void _positionChangedInternal() = 0;
        virtual void _updateInternal() = 0;
        virtual std::string _toStringInternal() = 0;

    protected :

        std::string m_name;
        eCameraType m_type;
        eAxis m_upAxis;

        CVec3 m_position;
        CVec3 m_worldUp;
        CVec3 m_targetPoint;
        CVec3 m_targetDir;
        
        CVec3 m_front;
        CVec3 m_up;
        CVec3 m_right;
        
        CMat4 m_matView;
        CMat4 m_matProj;

        CCameraProjData m_projData;

        int m_viewportWidth;
        int m_viewportHeight;
        bool m_active;
    };

}