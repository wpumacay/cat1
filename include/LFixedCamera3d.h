
#pragma once

#include "LICamera.h"

using namespace std;

namespace engine
{


    class LFixedCamera3d : public LICamera
    {
        public :

        LFixedCamera3d( const string& name,
                        const LVec3& pos,
                        const LVec3& targetPoint,
                        int worldUpId,
                        float fov = 45.0f,
                        float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                        float zNear = 1.0f, float zFar = 20.0f );

        static string GetStaticType() { return string( "fixed3d" ); }

        glm::mat4 getViewMatrix() override;
    };




}