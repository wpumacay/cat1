
#include <debug/LDebugDrawer.h>

using namespace std;



namespace engine
{

    LDebugDrawer* LDebugDrawer::INSTANCE = NULL;

    LDebugDrawer::LDebugDrawer()
    {
        m_shaderLinesRef = ( engine::LShaderDebug3d* ) LShaderManager::getShader( "debug3d" );

        m_linesRenderBufferPositions = vector< LDLinePositions >( DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE );
        m_linesRenderBufferColors = vector< LDLineColors >( DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE );

        m_linesVAO = new LVertexArray();

        m_linesPositionsVBO = new LVertexBuffer( GL_STREAM_DRAW );
        m_linesPositionsVBO->setData( sizeof( LDLinePositions ) * DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE,
                                      3, ( GLfloat* ) m_linesRenderBufferPositions.data() );

        m_linesColorsVBO = new LVertexBuffer( GL_STREAM_DRAW );
        m_linesColorsVBO->setData( sizeof( LDLineColors ) * DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE,
                                   3, (GLfloat*)m_linesRenderBufferColors.data() );

        m_linesVAO->addBuffer( m_linesPositionsVBO, 0 );
        m_linesVAO->addBuffer( m_linesColorsVBO, 1 );
    }

    void LDebugDrawer::create()
    {
        if ( LDebugDrawer::INSTANCE != NULL )
        {
            cout << "LDebugDrawer::create> already created" << endl;
            return;
        }

        LDebugDrawer::INSTANCE = new LDebugDrawer();
    }

    void LDebugDrawer::release()
    {
        if ( LDebugDrawer::INSTANCE == NULL )
        {
            cout << "LDebugDrawer::release> already released" << endl;
            return;
        }

        delete LDebugDrawer::INSTANCE;
        LDebugDrawer::INSTANCE = NULL;
    }

    LDebugDrawer::~LDebugDrawer()
    {
        m_linesPositions.clear();
        m_linesColors.clear();

        delete m_linesVAO;
        m_linesPositionsVBO = NULL;
        m_linesColorsVBO = NULL;
    }

    void LDebugDrawer::setupMatrices( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix )
    {
        m_viewMat = viewMatrix;
        m_projMat = projectionMatrix;
    }

    void LDebugDrawer::render()
    {
        _renderLines();
    }

    void LDebugDrawer::_renderLines()
    {

        for ( int q = 0; q < m_linesPositions.size(); q++ )
        {
            m_linesRenderBufferPositions[ q % 1024 ] = m_linesPositions[q];
            m_linesRenderBufferColors[ q % 1024 ] = m_linesColors[q];

            if ( ( q + 1 ) % DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE == 0 )
            {
                _renderLinesBatch( DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE );
            }
        }

        int _remainingCountLines = m_linesPositions.size() % DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE;

        if ( _remainingCountLines != 0 )
        {
            // If not exactly the buffer size, then there is still something ...
            // to be drawn in the lines buffers

            _renderLinesBatch( _remainingCountLines );
        }

        m_linesPositions.clear();
        m_linesColors.clear();
    }

    void LDebugDrawer::_renderLinesBatch( int count )
    {
        m_linesVAO->bind();

        m_linesPositionsVBO->updateData( count * sizeof( LDLinePositions ), ( GLfloat* ) m_linesRenderBufferPositions.data() );
        m_linesColorsVBO->updateData( count * sizeof( LDLineColors ), ( GLfloat* ) m_linesRenderBufferColors.data() );

        //m_linesPositionsVBO->setData( count * sizeof( LDLinePositions ), 3, ( GLfloat* ) m_linesRenderBufferPositions.data() );
        //m_linesColorsVBO->setData( count * sizeof( LDLineColors ), 3, ( GLfloat* ) m_linesRenderBufferColors.data() );

        m_shaderLinesRef->bind();
        m_shaderLinesRef->setViewMatrix( m_viewMat );
        m_shaderLinesRef->setProjectionMatrix( m_projMat );

        glDrawArrays( GL_LINES, 0, count * 2 );

        m_shaderLinesRef->unbind();

        m_linesVAO->unbind();
    }

    void LDebugDrawer::drawLine( const LVec3& start, const LVec3& end, const LVec3& color )
    {
        LDLinePositions _linePos;
        _linePos.vStart = start;
        _linePos.vEnd = end;

        m_linesPositions.push_back( _linePos );

        LDLineColors _lineCol;
        _lineCol.cStart = color;
        _lineCol.cEnd = color;

        m_linesColors.push_back( _lineCol );
    }

    void LDebugDrawer::drawArrow( const LVec3& start, const LVec3& end, const LVec3& color )
    {
        // Draw core of the arrow
        drawLine( start, end, color );

        // Draw side parts of the arrow
        LVec3 _arrowVec = LVec3::minus( end, start );
        float _length = _arrowVec.length();

        LVec3 _uf = LVec3::normalize( _arrowVec );
        LVec3 _ur = LVec3::cross( _uf, LVec3( 0, 1, 0 ) );
        LVec3 _uu = LVec3::cross( _ur, _uf );

        float _sidesLength = _length / 10.0f;

        _ur.scale( _sidesLength, _sidesLength, _sidesLength );
        _uu.scale( _sidesLength, _sidesLength, _sidesLength );

        LVec3 _p0 = LVec3::plus( end, _ur ) + _uu;
        LVec3 _p1 = LVec3::plus( end, _ur ) - _uu;
        LVec3 _p2 = LVec3::minus( end, _ur ) + _uu;
        LVec3 _p3 = LVec3::minus( end, _ur ) - _uu;

        drawLine( end, _p0, color );
        drawLine( end, _p1, color );
        drawLine( end, _p2, color );
        drawLine( end, _p3, color );
    }

    void LDebugDrawer::drawClipVolume( const glm::mat4& clipMatrix, const LVec3& color )
    {
        glm::mat4 _invClipMatrix = glm::inverse( clipMatrix );

        glm::vec3 _frustumPointsClipSpace[8] = {
            // near plane
            { -1.0f, -1.0f, -1.0f },{ 1.0f, -1.0f, -1.0f },
            { 1.0f,  1.0f, -1.0f },{ -1.0f,  1.0f, -1.0f },
            // far plane
            { -1.0f, -1.0f,  1.0f },{ 1.0f, -1.0f,  1.0f },
            { 1.0f,  1.0f,  1.0f },{ -1.0f,  1.0f,  1.0f }
        };

        vector< engine::LVec3 > _points3d;
        for ( int q = 0; q < 8; q++ )
        {
            glm::vec4 _pointFrustum = _invClipMatrix * glm::vec4( _frustumPointsClipSpace[q], 1.0f );
            glm::vec3 _pointFrustumNormalized = glm::vec3( _pointFrustum.x / _pointFrustum.w,
                                                           _pointFrustum.y / _pointFrustum.w,
                                                           _pointFrustum.z / _pointFrustum.w );

            _points3d.push_back( engine::LVec3( _pointFrustumNormalized.x, 
                                                _pointFrustumNormalized.y, 
                                                _pointFrustumNormalized.z ) );
        }

        // near face
        drawLine( _points3d[0], _points3d[1] );
        drawLine( _points3d[1], _points3d[2] );
        drawLine( _points3d[2], _points3d[3] );
        drawLine( _points3d[3], _points3d[0] );

        // far face
        drawLine( _points3d[4], _points3d[5] );
        drawLine( _points3d[5], _points3d[6] );
        drawLine( _points3d[6], _points3d[7] );
        drawLine( _points3d[7], _points3d[4] );

        // side lines
        drawLine( _points3d[0], _points3d[4] );
        drawLine( _points3d[1], _points3d[5] );
        drawLine( _points3d[2], _points3d[6] );
        drawLine( _points3d[3], _points3d[7] );
    }

    void LDebugDrawer::drawTrailPoints( const vector< LVec3 >& trailpoints, const LVec3& color )
    {
        if ( trailpoints.size() < 1 )
        {
            return;
        }

        for ( int q = 0; q < trailpoints.size() - 1; q++ )
        {
            drawLine( trailpoints[q], trailpoints[ q + 1 ], color );
        }
    }
}