

#pragma once

#include "../Common.h"
#include <shaders/LShader.h>

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

namespace engine
{

    struct LShaderManager
    {

        private :

        LShaderManager();
        static LShaderManager* _INSTANCE;

        map< string, GLuint > m_programs;
        map< string, LShader* > m_programObjs;

        public :


        static void create();
        static void release();

        ~LShaderManager();

        GLuint createShader( const char* filename, int shaderType );
        GLuint createProgram( GLuint vShader, GLuint fShader );

        static LShader* getShader( const string& shaderName );
    };

}