

#include <shaders/LShaderManager.h>

// General purpose shaders
#include <shaders/LShaderBasic3d.h>
#include <shaders/LShaderDebug3d.h>
#include <shaders/LShaderSkybox.h>
#include <shaders/LShaderShadowMap.h>

// Special purpose shaders
#include <shaders/LShaderEntitiesLighting.h>
#include <shaders/LShaderEntitiesLightingShadows.h>

using namespace std;

namespace engine
{

    LShaderManager* LShaderManager::_INSTANCE = NULL;

    LShaderManager::LShaderManager()
    {
        // Load all shaders available

        GLuint _vShader, _fShader, _program;

        // cout << "LShaderManager-> using glsl 330 core shaders" << endl;

        // Common shaders **************************************************

        std::string _basepath = ENGINE_RESOURCES_PATH;
        _basepath += "shaders/";

        _vShader = createShader( _basepath + "basic_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "basic_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["basic2d"] = _program;
        m_programObjs["basic2d"] = new LShader( _program );

        _vShader = createShader( _basepath + "basic2d_tex_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "basic2d_tex_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["basic2d_tex"] = _program;
        m_programObjs["basic2d_tex"] = new LShader( _program );

        _vShader = createShader( _basepath + "basic3d_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "basic3d_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["basic3d"] = _program;
        m_programObjs["basic3d"] = new LShaderBasic3d( _program );

        _vShader = createShader( _basepath + "debug/debug_3d_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "debug/debug_3d_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["debug3d"] = _program;
        m_programObjs["debug3d"] = new LShaderDebug3d( _program );

        _vShader = createShader( _basepath + "shadows/shadow_mapping_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "shadows/shadow_mapping_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["shadow_mapping"] = _program;
        m_programObjs["shadow_mapping"] = new LShaderShadowMap( _program );

        _vShader = createShader( _basepath + "basic3d_skybox_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "basic3d_skybox_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["skybox"] = _program;
        m_programObjs["skybox"] = new LShaderSkybox( _program );

        // *****************************************************************
        // Specific rendering shaders **************************************

        // We have separate entities shaders in case there is the need ...
        // to do something different in for each case

        _vShader = createShader( _basepath + "entities/lighting_entities_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "entities/lighting_entities_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["lighting_entities"] = _program;
        m_programObjs["lighting_entities"] = new LShaderEntitiesLighting( _program );

        _vShader = createShader( _basepath + "entities/lighting_entities_textured_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "entities/lighting_entities_textured_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["lighting_entities_textured"] = _program;
        m_programObjs["lighting_entities_textured"] = new LShaderEntitiesLighting( _program );

        _vShader = createShader( _basepath + "entities/lighting_entities_shadows_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "entities/lighting_entities_shadows_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["lighting_entities_shadows"] = _program;
        m_programObjs["lighting_entities_shadows"] = new LShaderEntitiesLightingShadows( _program );

        _vShader = createShader( _basepath + "entities/lighting_entities_textured_shadows_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( _basepath + "entities/lighting_entities_textured_shadows_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        m_programs["lighting_entities_textured_shadows"] = _program;
        m_programObjs["lighting_entities_textured_shadows"] = new LShaderEntitiesLightingShadows( _program );

        // *****************************************************************
    }

    LShaderManager::~LShaderManager()
    {
        map< string, GLuint >::iterator _it;
        for ( _it = m_programs.begin(); _it != m_programs.end(); ++_it )
        {
            glDeleteProgram( _it->second );
        }
    }

    void LShaderManager::release()
    {
        if ( LShaderManager::_INSTANCE != NULL )
        {
            delete LShaderManager::_INSTANCE;
            LShaderManager::_INSTANCE = NULL;
        }
    }

    void LShaderManager::create()
    {
        if ( LShaderManager::_INSTANCE != NULL )
        {
            delete LShaderManager::_INSTANCE;
        }

        LShaderManager::_INSTANCE = new LShaderManager();
    }


    GLuint LShaderManager::createShader( const std::string& filename, int shaderType )
    {
        // Load the shader code into a string ********************************************
        string _shaderStrCode;
        ifstream _shaderFile;

        string _fullFilePath = filename;

        // cout << "file: " << _fullFilePath << endl;

        _shaderFile.exceptions( ifstream::badbit );

        try 
        {
            _shaderFile.open( _fullFilePath.c_str() );
            stringstream _shaderStream;
            _shaderStream << _shaderFile.rdbuf();

            _shaderFile.close();

            _shaderStrCode = _shaderStream.str();
        }
        catch ( ... )
        {
            cout << "LShader::createFromFile> failed opening the resource file" << endl;
            return 0;
        }

        const char* _shaderCode_cstr = _shaderStrCode.c_str();
        // *******************************************************************************

        GLint _success;
        GLchar _infoLog[1024];

        GLuint _shaderId = glCreateShader( shaderType );
        glShaderSource( _shaderId, 1, &_shaderCode_cstr, NULL );
        glCompileShader( _shaderId );
        glGetShaderiv( _shaderId, GL_COMPILE_STATUS,
                       &_success );

        if ( !_success )
        {
            glGetShaderInfoLog( _shaderId, 1024, 
                                NULL, _infoLog );
            cout << "Shader: " << _fullFilePath << endl;
            cout << "Failed to compile shader" << endl;
            cout << "Error: " << _infoLog << endl;

            return 0;
        }

        return _shaderId;
    }

    GLuint LShaderManager::createProgram( GLuint vShaderId, GLuint fShaderId )
    {
        GLint _success;
        GLchar _infoLog[1024];

        GLuint _programId = glCreateProgram();
        glAttachShader( _programId, vShaderId );
        glAttachShader( _programId, fShaderId );
        glLinkProgram( _programId );

        glDetachShader( _programId, vShaderId );
        glDetachShader( _programId, fShaderId );
        glDeleteShader( vShaderId );
        glDeleteShader( fShaderId );

        glGetProgramiv( _programId, GL_LINK_STATUS, &_success );
        if ( !_success )
        {
            glGetProgramInfoLog( _programId, 1024, NULL, _infoLog );
            cout << "LINKING ERROR: " << _infoLog << endl;

            return 0;
        }

        return _programId;
    }

    LShader* LShaderManager::getShader( const string& shaderName )
    {
        if ( LShaderManager::_INSTANCE->m_programObjs.find( shaderName ) == 
             LShaderManager::_INSTANCE->m_programObjs.end() )
        {
            std::cout << "ERROR> The requested shader with name " 
                      << shaderName << " is not part of the loaded shaders" 
                      << std::endl;
            return NULL;
        }

        return LShaderManager::_INSTANCE->m_programObjs[ shaderName ];
    }

}