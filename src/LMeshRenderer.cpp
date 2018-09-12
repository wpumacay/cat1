
#include <LMeshRenderer.h>
#include <shaders/LShaderEntitiesLighting.h>

namespace engine
{

    LMeshRenderer::LMeshRenderer()
    {
        
    }

    LMeshRenderer::~LMeshRenderer()
    {
        m_renderList.clear();
    }



    void LMeshRenderer::begin( LScene* pScene )
    {
        // Collect mesh-renderables
        auto _renderables = pScene->getRenderables();
        for ( LIRenderable* _renderable : _renderables )
        {
            if ( _renderable->getType() == RENDERABLE_TYPE_MESH )
            {
                m_renderList.push_back( ( LMesh* )_renderable );
            }
        }
    }

    void LMeshRenderer::renderScene( LScene* pScene )
    {
        // Use simple lighting shader
        auto _shader = ( LShaderEntitiesLighting* ) LShaderManager::getShader( "lighting_entities" );
        // Get current camera for uniforms loading
        auto _camera = pScene->getCurrentCamera();
        if ( _camera == NULL )
        {
            std::cout << "ERROR> There is no current camera in the scene. No render executed" << std::endl;
            return;
        }
        // Get current light for uniforms loading
        auto _lights = pScene->getLights< LLightDirectional >();
        if ( _lights.size() < 1 )
        {
            std::cout << "ERROR> There is no light in the scene. No render executed" << std::endl;
            return;
        }

        auto _light =  _lights[0];

        // start render pass
        _shader->bind();

        // Load uniforms
        _shader->setLightDirectional( _light );
        _shader->setViewMatrix( _camera->getViewMatrix() );
        _shader->setProjectionMatrix( _camera->getProjectionMatrix() );
        _shader->setViewPosition( _camera->getPosition() );
        _shader->setGlobalAmbientLight( LVec3( 0.25f, 0.25f, 0.25f ) );

        // remder all meshes
        for ( LMesh* _mesh : m_renderList )
        {
            _shader->setModelMatrix( _mesh->getModelMatrix() );
            _shader->setMaterial( _mesh->getMaterial() );

            _mesh->render();
        }

        _shader->unbind();
        // end render pass
    }

    void LMeshRenderer::end( LScene* pScene )
    {
        m_renderList.clear();
    }
}