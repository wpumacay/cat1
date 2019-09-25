
#include <LSkyboxRenderer.h>

namespace engine
{

    LSkyboxRenderer::LSkyboxRenderer()
    {
        // Do nothing
    }

    LSkyboxRenderer::~LSkyboxRenderer()
    {
        // Do nothing
    }

    void LSkyboxRenderer::begin( LScene* pScene )
    {
        // glDepthMask( GL_FALSE );
        glDepthFunc( GL_LEQUAL );
    }

    void LSkyboxRenderer::renderScene( LScene* pScene )
    {
        // use skybox shader
        auto _shader = CShaderManager::GetCachedShader( "skybox" );
        // get current camera for uniforms loading
        auto _camera = pScene->getCurrentCamera();
        if ( _camera == NULL )
        {
            std::cout << "ERROR> There is no current camera in the scene. No render executed" << std::endl;
            return;
        }
        // get skybox
        auto _skybox = pScene->getSkybox();
        if ( _skybox != NULL )
        {
            CMat4 _correctionMat;
            if ( _camera->upAxis() == engine::eAxis::X ) _correctionMat = engine::CMat4::rotationZ( -ENGINE_PI / 2.0f );
            if ( _camera->upAxis() == engine::eAxis::Y ) _correctionMat = engine::CMat4();
            if ( _camera->upAxis() == engine::eAxis::Z ) _correctionMat = engine::CMat4::rotationX( ENGINE_PI / 2.0f );

            _shader->bind();
            _shader->setMat4( "u_tView", _camera->matView().getRotation() * _correctionMat );
            _shader->setMat4( "u_tProj", _camera->matProj() );
            _skybox->render();
            _shader->unbind();
        }
    }

    void LSkyboxRenderer::renderToShadowMap( LScene* pScene, LShadowMap* shadowMap )
    {
        // Do nothing
    }

    void LSkyboxRenderer::renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap )
    {
        // Do nothing
    }

    void LSkyboxRenderer::end( LScene* pScene )
    {
        // glDepthMask( GL_TRUE );
        glDepthFunc( GL_LESS );
    }


}