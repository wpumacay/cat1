message( "TINYRENDERER::bindings >>> configuring python-bindings" )

file( GLOB_RECURSE BindingsFilesSources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
set( BindingsFilesSources
     "${CMAKE_CURRENT_SOURCE_DIR}/common_py.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/math_py.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_py.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/window/window_py.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/bindings_py.cpp" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )
include_directories( "${TINYRENDERER_INCLUDES_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_math/python/include" )

pybind11_add_module( tinyrenderer_core MODULE "${BindingsFilesSources}" )
target_link_libraries( tinyrenderer_core PRIVATE tinyrenderer_cpp_lib tinymath_python_lib )

if ( ENGINE_GLFW_ENABLED )
    target_compile_definitions( tinyrenderer_core PRIVATE ENGINE_GLFW_ENABLED=1 )
endif()

if ( ENGINE_EGL_ENABLED )
    target_compile_definitions( tinyrenderer_core PRIVATE ENGINE_EGL_ENABLED=1 )
endif()
