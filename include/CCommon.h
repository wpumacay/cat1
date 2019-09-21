#pragma once

#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <memory>
#include <csignal>
#include <functional>

#define ENGINE_PI 3.141592653589793
#define ENGINE_EPS 0.000001

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

namespace engine
{

    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned long uint64;
    typedef int int32;
    typedef long int64;
    typedef float float32;
    typedef double float64;

    enum class eAxis
    {
        X = 0, Y, Z
    };

    std::string toString( const eAxis& axis );

    enum class ePixelFormat
    {
        NONE = 0, 
        RGB, 
        RGBA
    };

    std::string toString( const ePixelFormat& format );
    uint32 toOpenGLEnum( const ePixelFormat& format );

}