#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <camera/CICamera.h>
#include <core/CFrameBuffer.h>
#include <graphics/CFog.h>
#include <graphics/CSkybox.h>
#include <graphics/CShadowMap.h>
#include <graphics/CIRenderable.h>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>
#include <graphics/CScene.h>
#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>
#include <shaders/CShaderManager.h>
#include <shaders/CShader.h>

namespace engine
{

    enum class eRenderMode
    {
        NONE = 0,
        NORMAL,
        DEPTH_ONLY,
        SEMANTIC_ONLY,
        NO_SUBMIT
    };

    std::string toString( const eRenderMode& mode );

    enum class eCullingGeom
    {
        NONE = 0,
        BOUNDING_BOX,
        BOUNDING_SPHERE
    };

    std::string toString( const eCullingGeom& geom );

    struct CRenderOptions
    {
        eRenderMode             mode;
        bool                    useFrustumCulling;
        eCullingGeom            cullingGeom;
        bool                    useFaceCulling;
        bool                    useShadowMapping;
        int                     pcfCount;
        bool                    useBlending;
        bool                    useFog;
        bool                    renderGizmos;
        int32                   viewportWidth;
        int32                   viewportHeight;
        CShadowMapRangeConfig   shadowMapRangeConfig;
        CICamera*               cameraPtr;
        CILight*                lightPtr;
        CShadowMap*             shadowMapPtr;
        CFrameBuffer*           renderTargetPtr;
        CFog*                   fogPtr;

        CRenderOptions()
        {
            mode                    = eRenderMode::NORMAL;
            useFrustumCulling       = false;
            cullingGeom             = eCullingGeom::BOUNDING_SPHERE;
            useFaceCulling          = false;
            useShadowMapping        = false;
            pcfCount                = 2;
            useBlending             = false;
            useFog                  = false;
            renderGizmos            = true;
            viewportWidth           = -1;// default (-1) means use current viewport
            viewportHeight          = -1;// default (-1) means use current viewport
            shadowMapRangeConfig    = CShadowMapRangeConfig();
            cameraPtr               = nullptr;
            lightPtr                = nullptr;
            shadowMapPtr            = nullptr;
            renderTargetPtr         = nullptr;
            fogPtr                  = nullptr;
        }
    };

    std::string toString( const CRenderOptions& renderOptions );

}