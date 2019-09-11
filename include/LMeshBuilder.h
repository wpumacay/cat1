
#pragma once

#include "LCommon.h"

#include "LMesh.h"
#include "LModel.h"

#include <fstream>
#include <string>
#include <vector>
#include <cmath>

// Assimp helper functionality
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

#define OBJ_COMMENT string( "#" )
#define OBJ_VERTEX string( "v" )
#define OBJ_NORMAL string( "vn" )
#define OBJ_TEXTURE string( "vt" )
#define OBJ_FACE string( "f" )
#define OBJ_MATERIAL_LIB string( "mtllib" )
#define OBJ_OBJECT_NAME string( "o" )
#define OBJ_GROUP_NAME string( "g" )
#define OBJ_MATERIAL_ID string( "usemtl" )
#define OBJ_SMOOTH_SHADING string( "s" )

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

#define DEFAULT_AXIS AXIS_Z

namespace engine
{

    struct LObjInfo
    {
        vector<LVec3> vertices;
        vector<LVec3> normals;
        vector<LVec2> texCoords;
        vector<LInd3> indices;

        string materialFile;
    };

    struct LGeometryInfo
    {
        vector<LVec3> vertices;
        vector<LVec3> normals;
        vector<LVec2> texCoords;
        vector<LInd3> indices;
    };

    struct LMeshBuilder
    {

        private :

        static vector<string> _split( const string &txt, char separator = ';' );
        static LVec3 _computeFaceNormal( LVec3 v1, LVec3 v2, LVec3 v3, bool normalize = false );
        static LVec3 _createVec3WithAxis( float x, float y, float z, const std::string& axis );

        static LMesh* _processAssimpMesh( aiMesh* assimpMeshPtr );
        static void _processAssimpNode( LModel* modelPtr, 
                                        aiNode* assimpNodePtr, 
                                        const aiScene* assimpScenePtr );

        public :

        static LMesh* createSphere( GLfloat radius, int levelDivision = 10, int numLevels = 10 );
        static LMesh* createBox( GLfloat width, GLfloat height, GLfloat depth );
        static LMesh* createCylinder( GLfloat radius, GLfloat height, int axis = AXIS_Z, int sectionDivision = 10 );
        static LMesh* createCapsule( GLfloat radius, GLfloat height, int axis = AXIS_Z, int sectionDivision = 10, int capLevels = 10 );
        static LMesh* createPlane( GLfloat width, GLfloat depth, float texRangeWidth = 10.0f, float texRangeDepth = 10.0f );
        static LMesh* createArrow( GLfloat length, const std::string& axis );
        static LModel* createAxes( GLfloat length );

        // Use assimp importer to load meshes
        static LModel* createModelFromFile( const std::string& filename,
                                            const std::string& modelName );

        static LMesh* createPerlinPatch( GLfloat width, GLfloat depth, int cellDivision );
        // static LMesh* createHeightField();
    };



}