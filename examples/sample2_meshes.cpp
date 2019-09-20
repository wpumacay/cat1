
#include <iostream>

#include <core/COpenGLApp.h>
#include <graphics/CMeshBuilder.h>

#include <camera/CFpsCamera.h>
#include <camera/CFixedCamera.h>
#include <camera/COrbitCamera.h>
#include <LLightDirectional.h>

#include <utils/CImguiUiDemo.h>

class Application : public engine::COpenGLApp
{

public :

    Application() : engine::COpenGLApp() {}
    ~Application() {}

protected :

    void _initUser() override
    {
        ENGINE_TRACE( "Initializing custom ui" );
        m_uiPtr = new engine::CImguiUiDemo( m_windowPtr->context() );
        m_uiPtr->init();
    }

};

void checkPointClip( engine::CMat4 proj, engine::CMat4 view, engine::CVec4 worldpoint )
{
    auto _clipv = proj * view * worldpoint;
    std::cout << "world-point: " << engine::toString( worldpoint ) << " mapped to clip point: " << engine::toString( _clipv ) << std::endl;
}

int main()
{
    auto _app = new Application();
    _app->init();

    auto _scene = _app->scene();
    
    auto _plane = engine::CMeshBuilder::createPlane( 10.0f, 10.0f );
    _plane->getMaterial()->setColor( { 0.2f, 0.3f, 0.4f } );
    _scene->addRenderable( _plane );

    auto _chessboardTex = engine::LAssetsManager::getBuiltInTexture( "chessboard" );
    auto _containerTex = engine::LAssetsManager::getTexture( "wood_container" );
    auto _gridTex = engine::LAssetsManager::getTexture( "grid_deepmimic" );
    // make a sample mesh just for testing
    auto _boxy      = engine::CMeshBuilder::createBox( 0.25f, 0.5f, 1.0f );
    auto _sphery    = engine::CMeshBuilder::createSphere( 0.5f );
    auto _ellipsy   = engine::CMeshBuilder::createEllipsoid( 0.2f, 0.4f, 0.6f );
    auto _cylindyX  = engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::X );
    auto _cylindyY  = engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Y );
    auto _cylindyZ  = engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Z );
    auto _capsulyX  = engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::X );
    auto _capsulyY  = engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Y );
    auto _capsulyZ  = engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Z );
    auto _arrowyX   = engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::X );
    auto _arrowyY   = engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Y );
    auto _arrowyZ   = engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Z );
    auto _axy       = engine::CMeshBuilder::createAxes( 0.5f );

    std::vector< engine::LIRenderable* > _renderables;
    _renderables.push_back( _boxy );
    _renderables.push_back( _sphery );
    _renderables.push_back( _ellipsy );
    _renderables.push_back( _cylindyX );
    _renderables.push_back( _cylindyY );
    _renderables.push_back( _cylindyZ );
    _renderables.push_back( _capsulyX );
    _renderables.push_back( _capsulyY );
    _renderables.push_back( _capsulyZ );
    _renderables.push_back( _arrowyX );
    _renderables.push_back( _arrowyY );
    _renderables.push_back( _arrowyZ );
    _renderables.push_back( _axy );

    _boxy->pos      = { -1.0f, -1.0f, 1.0f };
    _sphery->pos    = { -1.0f,  1.0f, 1.0f };
    _ellipsy->pos   = {  1.0f,  1.0f, 1.0f };
    _cylindyX->pos  = {  1.0f, -1.0f, 1.0f };

    _cylindyY->pos  = { -1.0f, -1.0f, 2.0f };
    _cylindyZ->pos  = { -1.0f,  1.0f, 2.0f };
    _capsulyX->pos  = {  1.0f,  1.0f, 2.0f };
    _capsulyY->pos  = {  1.0f, -1.0f, 2.0f };

    _capsulyZ->pos  = { -1.0f, -1.0f, 3.0f };
    _arrowyX->pos   = { -1.0f,  1.0f, 3.0f };
    _arrowyY->pos   = {  1.0f,  1.0f, 3.0f };
    _arrowyZ->pos   = {  1.0f, -1.0f, 3.0f };

    _axy->pos       = {  0.0f,  0.0f, 1.0f };

    for ( size_t i = 0; i < _renderables.size(); i++ )
    {
        if ( _renderables[i]->getType() == engine::eRenderableType::MESH )
            _renderables[i]->getMaterial()->setColor( { 0.7f, 0.5f, 0.3f } );
    }

    for ( size_t i = 0; i < _renderables.size(); i++ )
    {
        if ( _renderables[i]->getType() == engine::eRenderableType::MESH )
        {
            _renderables[i]->addTexture( _chessboardTex );
        }
        else
        {
            auto _submeshes = reinterpret_cast< engine::LModel* >( _renderables[i] )->getMeshes();
            for ( size_t j = 0; j < _submeshes.size(); j++ )
                _submeshes[j]->addTexture( _gridTex );

            // @TODO: fix issue of requiring to add texture to model as well, even though is not used
            _renderables[i]->addTexture( _gridTex );
        }
    }

    _scene->addRenderable( _boxy );
    _scene->addRenderable( _sphery );
    _scene->addRenderable( _ellipsy );
    _scene->addRenderable( _cylindyX );
    _scene->addRenderable( _cylindyY );
    _scene->addRenderable( _cylindyZ );
    _scene->addRenderable( _capsulyX );
    _scene->addRenderable( _capsulyY );
    _scene->addRenderable( _capsulyZ );
    _scene->addRenderable( _arrowyX );
    _scene->addRenderable( _arrowyY );
    _scene->addRenderable( _arrowyZ );
    _scene->addRenderable( _axy );

    // make a sample camera
    auto _cameraProjData = engine::CCameraProjData();

    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    // // @TODO: Still presents some weird behaviour. It should be width - height, but that 
    // //        clips everything in the wrong way.
    // _cameraProjData.projection  = engine::eCameraProjection::ORTHOGRAPHIC;
    // _cameraProjData.width       = 4.0f * engine::COpenGLApp::GetWindow()->aspect();
    // _cameraProjData.height      = 4.0f;
    // _cameraProjData.zNear       = 0.1f;
    // _cameraProjData.zFar        = 100.0f;

    // const float _cameraSensitivity  = 0.25f;
    // const float _cameraSpeed        = 5.0f;
    // const float _cameraMaxDelta     = 10.0f;
    // 
    // auto _camera = new engine::CFpsCamera( "fps",
    //                                        engine::CVec3( 2.0f, 2.0f, 2.0f ),
    //                                        engine::CVec3( 0.0f, 0.0f, 0.0f ),
    //                                        engine::eAxis::Z,
    //                                        _cameraProjData,
    //                                        _cameraSensitivity,
    //                                        _cameraSpeed,
    //                                        _cameraMaxDelta );

    // auto _camera = new engine::CFixedCamera( "fixed",
    //                                          engine::CVec3( 2.0f, 2.0f, 2.0f ),
    //                                          engine::CVec3( 0.0f, 0.0f, 0.0f ),
    //                                          engine::eAxis::Z,
    //                                          _cameraProjData );

    auto _camera = new engine::COrbitCamera( "orbit",
                                             engine::CVec3( 2.0f, 2.0f, 2.0f ),
                                             engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                             engine::eAxis::Z,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

    // make a sample light source
    auto _light = new engine::LLightDirectional( engine::CVec3( 0.8, 0.8, 0.8 ), engine::CVec3( 0.8, 0.8, 0.8 ),
                                                 engine::CVec3( 0.3, 0.3, 0.3 ), 0, engine::CVec3( 0, 0, -1 ) );
    _light->setVirtualPosition( engine::CVec3( 5.0f, 0.0f, 5.0f ) );

    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );

    float _t = 0.0;

    while( _app->isActive() )
    {
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_SPACE ) )
            _scene->getCurrentCamera()->setActiveMode( false );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ENTER ) )
            _scene->getCurrentCamera()->setActiveMode( true );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_L ) )
            for ( size_t i = 0; i < _renderables.size(); i++ )
                _renderables[i]->setWireframeMode( true );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_N ) )
            for ( size_t i = 0; i < _renderables.size(); i++ )
                _renderables[i]->setWireframeMode( false );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

//         std::cout << "cam-info *****************" << std::endl;
//         std::cout << _camera->toString() << std::endl;
//         std::cout << "proj-mat: " << std::endl;
//         std::cout << engine::toString( _camera->matProj() ) << std::endl;
//         std::cout << "view-mat: " << std::endl;
//         std::cout << engine::toString( _camera->matView() ) << std::endl;

//         std::cout << "scroll-off-x          : " << std::to_string( engine::InputSystem::getScrollOffX() ) << std::endl;
//         std::cout << "scroll-off-y          : " << std::to_string( engine::InputSystem::getScrollOffY() ) << std::endl;
//         std::cout << "scroll-accum-x        : " << std::to_string( engine::InputSystem::getScrollAccumValueX() ) << std::endl;
//         std::cout << "scroll-accum-y        : " << std::to_string( engine::InputSystem::getScrollAccumValueY() ) << std::endl;

        _app->begin();
        _app->update();
        _app->end();
    }

    return 0;
}
