
#include <gl/COpenGLWindow.h>

using namespace std;

namespace engine
{

    COpenGLWindow::COpenGLWindow( const CWindowProps& properties )
    {
        m_properties = properties;

        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, properties.gl_api_version_major );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, properties.gl_api_version_minor );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    #ifdef __APPLE__
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
        glfwWindowHint( GLFW_RESIZABLE, ( properties.resizable ) ? GL_TRUE : GL_FALSE );

        m_glfwWindowPtr = glfwCreateWindow( m_properties.width,
                                            m_properties.height,
                                            m_properties.title.c_str(),
                                            NULL, NULL );

        if ( !m_glfwWindowPtr )
        {
            glfwTerminate();
            return;
        }

        m_glContext = new COpenGLContext( m_glfwWindowPtr );
        m_glContext->init();

        glfwSetWindowUserPointer( m_glfwWindowPtr, &m_properties );

        glfwSetKeyCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, int key, int scancode, int action, int mode )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops->callbackKey )
                    return;

                _wprops->callbackKey( key, action );
            } );

        glfwSetMouseButtonCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, int button, int action, int mode )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops->callbackMouse )
                    return;

                double _x,_y;
                glfwGetCursorPos( pWindow, &_x, &_y );

                _wprops->callbackMouse( button, action, _x, _y );
            } );

        glfwSetCursorPosCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, double x, double y )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops->callbackMouse )
                    return;

                _wprops->callbackMouseMove( x, y );
            } );

        glfwSetScrollCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, double xOff, double yOff )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops->callbackScroll )
                    return;

                _wprops->callbackScroll( xOff, yOff );
            } );

        glfwSetWindowSizeCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, int width, int height )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );
                _wprops->width = width;
                _wprops->height = height;

                if ( !_wprops->callbackResize )
                    return;

                _wprops->callbackResize( width, height );
            } );

        glfwSetInputMode( m_glfwWindowPtr, GLFW_STICKY_KEYS, 1 );
        glfwGetFramebufferSize( m_glfwWindowPtr, &m_properties.width, &m_properties.height );
        glViewport( 0, 0, m_properties.width, m_properties.height );

        glEnable( GL_DEPTH_TEST );
        glClearColor( m_properties.clearColor.x(), 
                      m_properties.clearColor.y(),
                      m_properties.clearColor.z(),
                      m_properties.clearColor.w() );
    }

    COpenGLWindow::~COpenGLWindow()
    {
        if ( m_glContext )
            delete m_glContext;

        m_glfwWindowPtr = NULL;
        m_glContext = NULL;

        glfwDestroyWindow( m_glfwWindowPtr );
        glfwTerminate();
    }

    void COpenGLWindow::enableCursor()
    {
        glfwSetInputMode( m_glfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }

    void COpenGLWindow::disableCursor()
    {
        glfwSetInputMode( m_glfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    void COpenGLWindow::begin()
    {
        glClearColor( m_properties.clearColor.x(), 
                      m_properties.clearColor.y(),
                      m_properties.clearColor.z(),
                      m_properties.clearColor.w() );

        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        glfwPollEvents();
    }

    void COpenGLWindow::end()
    {
        m_glContext->swapBuffers();
    }

    bool COpenGLWindow::active()
    { 
        return glfwWindowShouldClose( m_glfwWindowPtr ) == 0;
    }

    void COpenGLWindow::requestClose() 
    { 
        glfwSetWindowShouldClose( m_glfwWindowPtr, 1 ); 
    }

    void COpenGLWindow::registerKeyCallback( FnPtr_keyboard_callback callback )
    {
        m_properties.callbackKey = callback;
    }

    void COpenGLWindow::registerMouseCallback( FnPtr_mouse_callback callback )
    {
        m_properties.callbackMouse = callback;
    }

    void COpenGLWindow::registerMouseMoveCallback( FnPtr_mousemove_callback callback )
    {
        m_properties.callbackMouseMove = callback;
    }

    void COpenGLWindow::registerScrollCallback( FnPtr_scroll_callback callback )
    {
        m_properties.callbackScroll = callback;
    }

    void COpenGLWindow::registerResizeCallback( FnPtr_resize_callback callback )
    {
        m_properties.callbackResize = callback;
    }

}
