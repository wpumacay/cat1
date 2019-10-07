
#include <CEngine.h>

class ApplicationUi : public engine::CImguiUi
{

public :

    ApplicationUi( engine::COpenGLContext* context ) 
        : engine::CImguiUi( context ) {}

    ~ApplicationUi() {}

protected :

    void _initInternal() override
    {
        // nothing for now
    }

    void _renderInternal() override
    {
        // nothing for now
    }
};

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::LIRenderable* floor,
                        std::vector< engine::LIRenderable* > cubes );

void renderScene( engine::CILight* lightPtr, 
                  engine::CICamera* cameraPtr, 
                  engine::CShader* shaderPtr,
                  engine::CPhongMaterial* floorMaterialPtr,
                  engine::CPhongMaterial* cubeMaterialPtr,
                  engine::LIRenderable* floor,
                  std::vector< engine::LIRenderable* > cubes );

void renderShadowMapVisualization( engine::CVertexArray* quadVAO,
                                   engine::CShader* shaderPtr,
                                   engine::CShadowMap* shadowMapPtr );

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _ui = new ApplicationUi( engine::COpenGLApp::GetWindow()->context() );
    _ui->init();

    _app->setUi( _ui );

    /* load the shader used to render the scene normally (single-light for now) */
    std::string _baseNamePhong = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong_lightcasters";
    auto _shaderPhongLighting = engine::CShaderManager::CreateShaderFromFiles( "phong_lightcasters_shader",
                                                                       _baseNamePhong + "_vs.glsl",
                                                                       _baseNamePhong + "_fs.glsl" );
    ENGINE_ASSERT( _shaderPhongLighting, "Could not load phong-lightcasters shader to render the scene normally T_T" );

    /* load the shader used for shadow mapping */
    std::string _baseNameShadowMapProjection = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_projection";
    auto _shaderShadowMapProj = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_projection_shader",
                                                                               _baseNameShadowMapProjection + "_vs.glsl",
                                                                               _baseNameShadowMapProjection + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapProj, "Couldn't load shadow-mapping shader use to create the depth-map T_T" );

    /* load the shader in charge of depth-map visualization */
    std::string _baseNameShadowMapViz = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_visualization";
    auto _shaderShadowMapViz = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_visualization_shader",
                                                                             _baseNameShadowMapViz + "_vs.glsl",
                                                                             _baseNameShadowMapViz + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapViz, "Couldn't load the visualization shader to check the depth-map T_T" );

    /* Create a simple scene for testing **********************************************************/

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

    auto _floor = engine::CMeshBuilder::createPlane( 50.0f, 50.0f, engine::eAxis::Y );
    _floor->pos = { 0.0f, 0.0f, 0.0f };

    auto _cube1 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube1->pos = { 0.0f, 1.5f, 0.0f };

    auto _cube2 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube2->pos = { 2.0f, 0.5f, 1.0f };

    auto _cube3 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube3->pos = { -1.0f, 0.25f, 2.0f };
    _cube3->rotation = engine::CMat4::rotation( engine::toRadians( 60.0f ), { 1.0f, 0.0f, 1.0f } );
    _cube3->scale = { 0.5f, 0.5f, 0.5f };

    auto _floorTexture = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );
    auto _cubeTexture = engine::CTextureManager::GetCachedTexture( "img_smiley" );

    ENGINE_ASSERT( _floorTexture, "Could not retrieve valid texture for the sample - floor" );
    ENGINE_ASSERT( _cubeTexture, "Could not retrieve valid texture for the sample - cube" );

    auto _floorMaterial = new engine::CPhongMaterial( "floor_material",
                                                      { 1.0f, 1.0f, 1.0f },
                                                      { 1.0f, 1.0f, 1.0f },
                                                      { 1.0f, 1.0f, 1.0f },
                                                      64.0f,
                                                      _floorTexture,
                                                      _floorTexture );

    auto _cubeMaterial = new engine::CPhongMaterial( "cube_material",
                                                     { 1.0f, 1.0f, 1.0f },
                                                     { 1.0f, 1.0f, 1.0f },
                                                     { 1.0f, 1.0f, 1.0f },
                                                     64.0f,
                                                     _cubeTexture,
                                                     _cubeTexture );

    /* define the position (point-light-> actual position | directional-light-> virtual position) */
    engine::CVec3 _lightPosition = { -2.0f, 4.0f, -1.0f };

    auto _dirlight = new engine::CDirectionalLight( "directional",
                                                    { 0.3f, 0.3f, 0.3f },
                                                    { 0.3f, 0.3f, 0.3f },
                                                    { 0.3f, 0.3f, 0.3f },
                                                    { -1.0f, -1.0f, -1.0f } );

    auto _pointlight = new engine::CPointLight( "point",
                                                { 0.3f, 0.3f, 0.3f },
                                                { 0.3f, 0.3f, 0.3f },
                                                { 0.3f, 0.3f, 0.3f },
                                                _lightPosition,
                                                1.0f, 0.09f, 0.032f );

    /**********************************************************************************************/

    auto _currentLight = _dirlight;
    auto _shadowmap = new engine::CShadowMap( 1024, 1024 );

    engine::float32 _quad_buffData[] = {
     /*|  positions |     uvs    |*/
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    engine::uint32 _quad_indices[] = {
        0, 1, 2, 
        0, 2, 3
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false },
                                            { "uv", engine::eElementType::Float2, false } };

    auto _quad_vbuffer = new engine::CVertexBuffer( _layout,
                                                    engine::eBufferUsage::STATIC,
                                                    sizeof( _quad_buffData ),
                                                    _quad_buffData );;

    auto _quad_ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                                   6, _quad_indices );

    auto _quad_varray = new engine::CVertexArray();
    _quad_varray->addVertexBuffer( _quad_vbuffer );
    _quad_varray->setIndexBuffer( _quad_ibuffer );

    while( _app->isActive() )
    {
        if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
            break;
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_SPACE ) )
            _camera->setActiveMode( false );
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ENTER ) )
            _camera->setActiveMode( true );

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->begin();
        _camera->update();

        /* do our thing here ************************/

        // render to shadow map first
        renderToShadowMap( _currentLight, _shadowmap, _shaderShadowMapProj, _floor, { _cube1, _cube2, _cube3 } );

        // render the scene normally
        renderScene( _currentLight, _camera, _shaderPhongLighting, _floorMaterial, _cubeMaterial, _floor, { _cube1, _cube2, _cube3 } );

        // render the shadowmap to a quad
        renderShadowMapVisualization( _quad_varray, _shaderShadowMapViz, _shadowmap );

        /********************************************/

        engine::CDebugDrawer::Render( _camera );

        if ( !_camera->active() )
            _app->renderUi();

        _app->end();
    }

    return 0;
}

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::LIRenderable* floor,
                        std::vector< engine::LIRenderable* > cubes )
{
    shaderPtr->bind();
    shadowMapPtr->bind();

    const float _znear = 1.0f;
    const float _zfar = 7.5f;
    auto _lightViewMat = engine::CMat4::lookAt( { -2.0f, 4.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
    auto _lightProjMat = engine::CMat4::ortho( 20, 20, _znear, _zfar );

    shaderPtr->setMat4( "u_lightSpaceViewProjMatrix", _lightProjMat * _lightViewMat );

    {
        shaderPtr->setMat4( "u_modelMatrix", floor->getModelMatrix() );
        floor->render();
    }

    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        shaderPtr->setMat4( "u_modelMatrix", cubes[i]->getModelMatrix() );
        cubes[i]->render();
    }

    shadowMapPtr->unbind();
    shaderPtr->unbind();
}

void renderScene( engine::CILight* lightPtr, 
                  engine::CICamera* cameraPtr, 
                  engine::CShader* shaderPtr,
                  engine::CPhongMaterial* floorMaterialPtr,
                  engine::CPhongMaterial* cubeMaterialPtr,
                  engine::LIRenderable* floor,
                  std::vector< engine::LIRenderable* > cubes )
{
    if ( !lightPtr || !cameraPtr || !shaderPtr )
        return;

    shaderPtr->bind();
    shaderPtr->setInt( "u_spotLight.enabled", 0 );
    shaderPtr->setInt( "u_pointLight.enabled", 0 );
    shaderPtr->setInt( "u_directionalLight.enabled", 0 );

    /* setup the light according to its type */
    if ( lightPtr->type() == engine::eLightType::DIRECTIONAL )
    {
        shaderPtr->setInt( "u_directionalLight.enabled", 1 );
        shaderPtr->setVec3( "u_directionalLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_directionalLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_directionalLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_directionalLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_directionalLight.direction", reinterpret_cast< engine::CDirectionalLight* >( lightPtr )->direction );
    }
    else if ( lightPtr->type() == engine::eLightType::POINT )
    {
        shaderPtr->setInt( "u_pointLight.enabled", 1 );
        shaderPtr->setVec3( "u_pointLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_pointLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_pointLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_pointLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_pointLight.position", reinterpret_cast< engine::CPointLight* >( lightPtr )->position );
        shaderPtr->setFloat( "u_pointLight.attnk0", reinterpret_cast< engine::CPointLight* >( lightPtr )->atnConstant );
        shaderPtr->setFloat( "u_pointLight.attnk1", reinterpret_cast< engine::CPointLight* >( lightPtr )->atnLinear );
        shaderPtr->setFloat( "u_pointLight.attnk2", reinterpret_cast< engine::CPointLight* >( lightPtr )->atnQuadratic );
    }
    else if ( lightPtr->type() == engine::eLightType::SPOT )
    {
        shaderPtr->setInt( "u_spotLight.enabled", 1 );
        shaderPtr->setVec3( "u_spotLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_spotLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_spotLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_spotLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_spotLight.position", reinterpret_cast< engine::CSpotLight* >( lightPtr )->position );
        shaderPtr->setFloat( "u_spotLight.attnk0", reinterpret_cast< engine::CSpotLight* >( lightPtr )->atnConstant );
        shaderPtr->setFloat( "u_spotLight.attnk1", reinterpret_cast< engine::CSpotLight* >( lightPtr )->atnLinear );
        shaderPtr->setFloat( "u_spotLight.attnk2", reinterpret_cast< engine::CSpotLight* >( lightPtr )->atnQuadratic );
        shaderPtr->setVec3( "u_spotLight.direction", reinterpret_cast< engine::CSpotLight* >( lightPtr )->direction );
        shaderPtr->setFloat( "u_spotLight.innerCutoffCos", std::cos( reinterpret_cast< engine::CSpotLight* >( lightPtr )->innerCutoff ) );
        shaderPtr->setFloat( "u_spotLight.outerCutoffCos", std::cos( reinterpret_cast< engine::CSpotLight* >( lightPtr )->outerCutoff ) );
    }

    /* setup the view and proj matrices */
    shaderPtr->setMat4( "u_viewProjMatrix", cameraPtr->matProj() * cameraPtr->matView() );
    shaderPtr->setVec3( "u_viewerPosition", cameraPtr->position() );

    /* render the floor */
    floorMaterialPtr->bind( shaderPtr );
    {
        shaderPtr->setMat4( "u_modelMatrix", floor->getModelMatrix() );
        floor->render();
    }
    floorMaterialPtr->unbind();

    /* render the cubes (all use the same material) */
    cubeMaterialPtr->bind( shaderPtr );
    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        auto _modelMat = cubes[i]->getModelMatrix();
        shaderPtr->setMat4( "u_modelMatrix", _modelMat );
        shaderPtr->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        cubes[i]->render();
    }
    cubeMaterialPtr->unbind();

    shaderPtr->unbind();
}

void renderShadowMapVisualization( engine::CVertexArray* quadVAO,
                                   engine::CShader* shaderPtr,
                                   engine::CShadowMap* shadowMapPtr )
{
    glDisable( GL_DEPTH_TEST );
    glViewport( 0, 0, 256, 256 );
    shaderPtr->bind();
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->bind();
    quadVAO->bind();

    glDrawElements( GL_TRIANGLES, quadVAO->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

    quadVAO->unbind();
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->unbind();
    shaderPtr->unbind();
    glEnable( GL_DEPTH_TEST );
    glViewport( 0, 0, engine::COpenGLApp::GetWindow()->width(), engine::COpenGLApp::GetWindow()->height() );
}