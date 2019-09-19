
#include <iostream>

#include <core/COpenGLApp.h>
#include <graphics/CMeshBuilder.h>

#include <camera/CFpsCamera.h>
#include <camera/CFixedCamera.h>
#include <LLightDirectional.h>

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();
    
    auto _scene = _app->scene();
    
    // make a sample mesh just for testing
    std::string _modelpath;
    _modelpath += ENGINE_RESOURCES_PATH;
    _modelpath += "models/chassis.stl";

    auto _model = engine::CMeshBuilder::createModelFromFile( _modelpath, "chassis" );

    // make a sample camera
    auto _cameraProjData = engine::CCameraProjData();

    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    const float _cameraSensitivity  = 0.25f;
    const float _cameraSpeed        = 5.0f;
    const float _cameraMaxDelta     = 10.0f;

    auto _camera = new engine::CFpsCamera( "fps",
                                           engine::LVec3( 2.0f, 2.0f, 2.0f ),
                                           engine::LVec3( 0.0f, 0.0f, 0.0f ),
                                           engine::eAxis::Z,
                                           _cameraProjData,
                                           _cameraSensitivity,
                                           _cameraSpeed,
                                           _cameraMaxDelta );

    // auto _camera = new engine::CFixedCamera( "fixed",
    //                                          engine::LVec3( 1.0f, 2.0f, 1.0f ),
    //                                          engine::LVec3( 0.0f, 0.0f, 0.0f ),
    //                                          engine::eAxis::Z,
    //                                          _cameraProjData );

    // make a sample light source
    auto _light = new engine::LLightDirectional( engine::LVec3( 0.2, 0.2, 0.2 ), engine::LVec3( 0.8, 0.8, 0.8 ),
                                                 engine::LVec3( 0.05, 0.05, 0.05 ), 0, engine::LVec3( -1, -1, 0 ) );

    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );
    _scene->addRenderable( _model );

    while( _app->isActive() )
    {
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _model->rotation = _model->rotation * engine::LMat4::rotationX( 0.01f );

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_SPACE ) )
            _scene->getCurrentCamera()->setActiveMode( false );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ENTER ) )
            _scene->getCurrentCamera()->setActiveMode( true );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->begin();
        _app->update();
        _app->end();
    }

    return 0;
}
