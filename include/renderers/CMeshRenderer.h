#pragma once

#include <renderers/CRenderersCommon.h>

namespace engine
{

    const int MESH_RENDERER_ALBEDO_MAP_SLOT = 0;
    const int MESH_RENDERER_SPECULAR_MAP_SLOT = 1;
    const int MESH_RENDERER_DEPTH_MAP_SLOT = 2;

    struct CMeshComparatorFarthestFromPoint
    {
        CVec3 point;

        bool operator() ( CMesh* mesh1, CMesh* mesh2 )
        {
            return CVec3::length( mesh1->position - point ) > CVec3::length( mesh2->position - point );
        }
    };

    struct CMeshComparatorClosesToPoint
    {
        CVec3 point;

        bool operator() ( CMesh* mesh1, CMesh* mesh2 )
        {
            return CVec3::length( mesh1->position - point ) < CVec3::length( mesh2->position - point );
        }
    };

    struct CMeshRenderContext
    {
        /* some render options */
        bool    useFog;
        bool    useFaceCulling;
        /* camera information */
        CMat4   viewMatrix;
        CMat4   projMatrix;
        CVec3   viewPosition;
        /* light information */
        eLightType  lightType;
        CVec3       lightAmbient;
        CVec3       lightDiffuse;
        CVec3       lightSpecular;
        float32     lightIntensity;
        CVec3       lightPosition;
        CVec3       lightDirection;
        float32     lightInnerCutoffCos;
        float32     lightOuterCutoffCos;
        float32     lightAttnK0;
        float32     lightAttnK1;
        float32     lightAttnK2;
        /* when using shadow mapping */
        CMat4   shadowMappingViewMatrix;
        CMat4   shadowMappingProjMatrix;
        int32   shadowMappingPCFcount;
        int32   shadowMappingSize;
        uint32  shadowMappingTextureId;
        /* when using fog */
        int32   fogType;
        CVec3   fogColor;
        float32 fogDensity;
        float32 fogGradient;
        float32 fogDistStart;
        float32 fogDistEnd;
        /* when rendering in depth-only mode */
        float32 depthViewZnear; // from camera frustum
        float32 depthViewZfar; // from camera frustum
        float32 depthViewZmin;
        float32 depthViewZmax;
        CVec3   depthViewZminColor;
        CVec3   depthViewZmaxColor;
        /* when rendering in semantic-only mode */
        std::unordered_map< int32, CVec3 > semanticViewIdMap;
    };

    class CMeshRenderer
    {

    public :

        CMeshRenderer();
        ~CMeshRenderer();

        void submit( const std::vector< CMesh* >& meshesVisible,
                     const std::vector< CMesh* >& meshesInView, 
                     const CRenderOptions& renderOptions );

        void renderToShadowMap();
        void render( bool useShadowMapping );
        void renderDepthOnly();
        void renderSemanticOnly();

        template< class T >
        void _collectMeshesByMaterial( const std::vector< CMesh* >& meshes,
                                       std::vector< CMesh* >& meshesOfGivenMaterial )
        {
            for ( auto _mesh : meshes )
                if ( _mesh->material()->type() == T::GetStaticType() )
                    meshesOfGivenMaterial.push_back( _mesh );
        }

        std::string status() const { return m_status; }

    private :

        void _render_Lambert( const std::vector< CMesh* >& meshesWithFaceCulling, 
                              const std::vector< CMesh* >& meshesWithNoFaceCulling,
                              bool renderWithShadows );
        void _render_Phong( const std::vector< CMesh* >& meshesWithFaceCulling,
                            const std::vector< CMesh* >& meshesWithNoFaceCulling, 
                            bool renderWithShadows );

        void _renderMesh_Lambert( CMesh* meshPtr, CShader* shaderPtr );
        void _renderMesh_Phong( CMesh* meshPtr, CShader* shaderPtr );

        void _setupRenderState_camera( CShader* shaderPtr );
        void _setupRenderState_light( CShader* shaderPtr );
        void _setupRenderState_shadows( CShader* shaderPtr );
        void _setupRenderState_fog( CShader* shaderPtr );

    private :

        std::string m_status;
        CMeshRenderContext m_context;

        std::vector< CMesh* > m_meshesCastShadows;
        std::vector< CMesh* > m_meshesDontCastShadows;

        std::vector< CMesh* > m_meshesOpaque;
        std::vector< CMesh* > m_meshesTransparent;

        std::default_random_engine m_randGen;
        std::uniform_real_distribution< float32 > m_randDist;

        std::unordered_map< int32, CVec3 > m_cachedRandomColors;

        CShader* m_shaderLambertNoShadowsNoFog;
        CShader* m_shaderLambertNoShadowsFog;
        CShader* m_shaderLambertShadowsNoFog;
        CShader* m_shaderLambertShadowsFog;

        CShader* m_shaderPhongNoShadowsNoFog;
        CShader* m_shaderPhongNoShadowsFog;
        CShader* m_shaderPhongShadowsNoFog;
        CShader* m_shaderPhongShadowsFog;

        CShader* m_shaderDepthView;
        CShader* m_shaderSemanticView;
        CShader* m_shaderShadowMapping;
    };

}