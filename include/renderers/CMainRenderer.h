#pragma once

#include <renderers/CRenderersCommon.h>
#include <renderers/CMeshRenderer.h>

namespace engine
{

    const int32 RENDERER_SHADOWMAP_WIDTH    = 4096;
    const int32 RENDERER_SHADOWMAP_HEIGHT   = 4096;

    class CMainRenderer
    {

    public :

        CMainRenderer();
        ~CMainRenderer();

        void render( CScene* scenePtr, CRenderOptions renderOptions );

        std::string status() const { return m_status; }

    private :

        void _collectRenderablesInView( const CFrustum& frustum,
                                        const eCullingGeom& cullGeometryToUse,
                                        const std::vector< CIRenderable* >& renderablesToCheck,
                                        std::vector< CIRenderable* >& renderablesInView );

        void _collectRenderablesInView_bbox( const CFrustum& frustum,
                                             const std::vector< CIRenderable* >& renderablesToCheck,
                                             std::vector< CIRenderable* >& renderablesInView );

        void _collectRenderablesInView_bsphere( const CFrustum& frustum,
                                                const std::vector< CIRenderable* >& renderablesToCheck,
                                                std::vector< CIRenderable* >& renderablesInView );

        template< class T >
        void _collectRenderablesByType( const std::vector< CIRenderable* >& renderables,
                                        std::vector< T* >& renderablesOfGivenType )
        {
            for ( auto _renderable : renderables )
                if ( _renderable->type() == T::GetStaticType() )
                    renderablesOfGivenType.push_back( dynamic_cast<T*>( _renderable ) );
        }

    private :

        std::string m_status;
        std::unique_ptr< CShadowMap > m_shadowMap;

        std::unique_ptr< CMeshRenderer > m_rendererMeshes;

    };

}