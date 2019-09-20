#pragma once

#include <camera/CICamera.h>

namespace engine
{

    class CFixedCamera : public CICamera
    {
    public :

        CFixedCamera( const std::string& name,
                      const CVec3& position,
                      const CVec3& targetPoint,
                      const eAxis& upAxis,
                      const CCameraProjData& projData );

        static eCameraType GetStaticType() { return eCameraType::FIXED; }

    protected :

        void _positionChangedInternal() override;
        void _updateInternal() override;
        std::string _toStringInternal() override;
    };


}