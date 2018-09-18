
#include <iostream>

#include <LApp.h>
#include <LFpsCamera.h>
#include <LLightDirectional.h>
#include <LMeshBuilder.h>


int main()
{
    auto _app = engine::LApp::GetInstance();
    auto _scene = _app->scene();
    
    // make a sample mesh just for testing
    auto _mesh = engine::LMeshBuilder::createBox( 0.5f, 0.5f, 0.5f );
    // make a sample camera
    auto _camera = new engine::LFpsCamera( "fixed",
                                           engine::LVec3( 1.0f, 2.0f, -1.0f ),
                                           engine::LVec3( -2.0f, -4.0f, -2.0f ),
                                           engine::LICamera::UP_Z );
    // make a sample light source
    auto _light = new engine::LLightDirectional( engine::LVec3( 0.2, 0.2, 0.2 ), engine::LVec3( 0.8, 0.8, 0.8 ),
                                                 engine::LVec3( 0.05, 0.05, 0.05 ), 0, engine::LVec3( -1, -1, 0 ) );

    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );
    _scene->addRenderable( _mesh );

    while( _app->isActive() )
    {
        _app->update();
    }

    return 0;
}
