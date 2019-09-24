
#include <utils/CDebugDrawer.h>

namespace engine
{

    CDebugDrawer* CDebugDrawer::s_instance = nullptr;

    CDebugDrawer::CDebugDrawer()
    {
        m_shader = ( engine::LShaderDebug3d* ) LShaderManager::getShader( "debug3d" );

        m_linesRenderBufferPositions = std::vector< LDLinePositions >( DEBUG_DRAWER_BATCH_SIZE );
        m_linesRenderBufferColors = std::vector< LDLinePositionsColors >( DEBUG_DRAWER_BATCH_SIZE );

        m_linesPositionsVBO = new CVertexBuffer( { { "position", eElementType::Float3, false } },
                                                 eBufferUsage::DYNAMIC,
                                                 sizeof( LDLinePositions ) * m_linesRenderBufferPositions.size(),
                                                 (float32*) m_linesRenderBufferPositions.data() );

        m_linesColorsVBO = new CVertexBuffer( { { "color", eElementType::Float3, false } },
                                              eBufferUsage::DYNAMIC,
                                              sizeof( LDLinePositionsColors ) * m_linesRenderBufferColors.size(),
                                              (float32*) m_linesRenderBufferColors.data() );

        m_linesVAO = new CVertexArray();
        m_linesVAO->addVertexBuffer( m_linesPositionsVBO );
        m_linesVAO->addVertexBuffer( m_linesColorsVBO );
    }

    void CDebugDrawer::Init()
    {
        if ( CDebugDrawer::s_instance )
        {
            ENGINE_CORE_WARN( "Attempting to initiliaze debug-drawer twice" );
            return;
        }

        CDebugDrawer::s_instance = new CDebugDrawer();
    }

    void CDebugDrawer::Release()
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Tried to release debug-drawer more than once" );

        delete CDebugDrawer::s_instance;
        CDebugDrawer::s_instance = nullptr;
    }

    void CDebugDrawer::Render( CICamera* camera )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_render( camera );
    }

    void CDebugDrawer::DrawLine( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawLine( start, end, color );
    }

    void CDebugDrawer::DrawArrow( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawArrow( start, end, color );
    }

    void CDebugDrawer::DrawClipVolume( const CMat4& clipMatrix, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawClipVolume( clipMatrix, color );
    }

    void CDebugDrawer::DrawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawTrailPoints( trailpoints, color );
    }

    void CDebugDrawer::DrawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawLinesBatch( linesBatch, color );
    }

    void CDebugDrawer::DrawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& worldTransform, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawAABB( aabbMin, aabbMax, worldTransform, color );
    }

    CDebugDrawer::~CDebugDrawer()
    {
        m_linesPositions.clear();
        m_linesColors.clear();

        delete m_linesVAO;

        m_linesVAO = nullptr;
        m_linesPositionsVBO = nullptr;
        m_linesColorsVBO = nullptr;
    }

    void CDebugDrawer::_render( CICamera* camera )
    {
        glLineWidth( 2.0f );

        for ( size_t q = 0; q < m_linesPositions.size(); q++ )
        {
            m_linesRenderBufferPositions[ q % DEBUG_DRAWER_BATCH_SIZE ] = m_linesPositions[q];
            m_linesRenderBufferColors[ q % DEBUG_DRAWER_BATCH_SIZE ] = m_linesColors[q];

            if ( ( q + 1 ) % DEBUG_DRAWER_BATCH_SIZE == 0 )
                _renderLinesBatch( camera, DEBUG_DRAWER_BATCH_SIZE );
        }

        int _remainingCountLines = m_linesPositions.size() % DEBUG_DRAWER_BATCH_SIZE;

        // Draw remaining lines (the ones that didn't get a batch)
        if ( _remainingCountLines != 0 )
            _renderLinesBatch( camera, _remainingCountLines );

        m_linesPositions.clear();
        m_linesColors.clear();

        glLineWidth( 1.0f );
    }

    void CDebugDrawer::_renderLinesBatch( CICamera* camera, int numLines )
    {
        m_linesVAO->bind();

        m_linesPositionsVBO->updateData( numLines * sizeof( LDLinePositions ), ( float32* ) m_linesRenderBufferPositions.data() );
        m_linesColorsVBO->updateData( numLines * sizeof( LDLinePositionsColors ), ( float32* ) m_linesRenderBufferColors.data() );

        m_shader->bind();
        m_shader->setViewMatrix( camera->matView() );
        m_shader->setProjectionMatrix( camera->matProj() );

        glDrawArrays( GL_LINES, 0, numLines * 2 );

        m_shader->unbind();

        m_linesVAO->unbind();
    }

    void CDebugDrawer::_drawLine( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        LDLinePositions _linePos;
        _linePos.vStart = start;
        _linePos.vEnd = end;

        m_linesPositions.push_back( _linePos );

        LDLinePositionsColors _lineCol;
        _lineCol.cStart = color;
        _lineCol.cEnd = color;

        m_linesColors.push_back( _lineCol );
    }

    void CDebugDrawer::_drawArrow( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        // Draw core of the arrow
        _drawLine( start, end, color );

        // Draw side parts of the arrow
        CVec3 _arrowVec = end - start;
        float _length = _arrowVec.length();

        CVec3 _uf = CVec3::normalize( _arrowVec );
        CVec3 _ur = CVec3::cross( _uf, CVec3( 0, 1, 0 ) );
        CVec3 _uu = CVec3::cross( _ur, _uf );

        float _sidesLength = _length / 10.0f;

        _uf.scale( _sidesLength, _sidesLength, _sidesLength );
        _ur.scale( _sidesLength, _sidesLength, _sidesLength );
        _uu.scale( _sidesLength, _sidesLength, _sidesLength );

        CVec3 _p0 = end + _ur + _uu - _uf;
        CVec3 _p1 = end + _ur - _uu - _uf;
        CVec3 _p2 = end + _ur + _uu - _uf;
        CVec3 _p3 = end + _ur - _uu - _uf;

        _drawLine( end, _p0, color );
        _drawLine( end, _p1, color );
        _drawLine( end, _p2, color );
        _drawLine( end, _p3, color );
    }

    void CDebugDrawer::_drawClipVolume( const CMat4& clipMatrix, const CVec3& color )
    {
        CMat4 _invClipMatrix = clipMatrix.inverse();

        CVec3 _frustumPointsClipSpace[8] = {
            /*      near plane      */
            { -1.0f, -1.0f, -1.0f }, 
            { 1.0f, -1.0f, -1.0f },
            { 1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
            /*      far plane       */
            { -1.0f, -1.0f, 1.0f }, 
            { 1.0f, -1.0f, 1.0f },
            { 1.0f,  1.0f, 1.0f },
            { -1.0f,  1.0f, 1.0f }
        };

        std::vector< engine::CVec3 > _points3d;
        for ( size_t q = 0; q < 8; q++ )
        {
            CVec4 _pointFrustum = _invClipMatrix * CVec4( _frustumPointsClipSpace[q], 1.0f );
            CVec3 _pointFrustumNormalized = CVec3( _pointFrustum.x / _pointFrustum.w,
                                                   _pointFrustum.y / _pointFrustum.w,
                                                   _pointFrustum.z / _pointFrustum.w );

            _points3d.push_back( _pointFrustumNormalized );
        }

        // near face
        _drawLine( _points3d[0], _points3d[1], color );
        _drawLine( _points3d[1], _points3d[2], color );
        _drawLine( _points3d[2], _points3d[3], color );
        _drawLine( _points3d[3], _points3d[0], color );

        // far face
        _drawLine( _points3d[4], _points3d[5], color );
        _drawLine( _points3d[5], _points3d[6], color );
        _drawLine( _points3d[6], _points3d[7], color );
        _drawLine( _points3d[7], _points3d[4], color );

        // side lines
        _drawLine( _points3d[0], _points3d[4], color );
        _drawLine( _points3d[1], _points3d[5], color );
        _drawLine( _points3d[2], _points3d[6], color );
        _drawLine( _points3d[3], _points3d[7], color );
    }

    void CDebugDrawer::_drawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color )
    {
        if ( trailpoints.size() < 1 )
            return;

        for ( size_t q = 0; q < trailpoints.size() - 1; q++ )
            _drawLine( trailpoints[q], trailpoints[ q + 1 ], color );
    }

    void CDebugDrawer::_drawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color )
    {
        if ( linesBatch.size() < 1 )
            return;

        for ( size_t q = 0; q < linesBatch.size(); q++ )
            _drawLine( linesBatch[q].start, linesBatch[q].end, color );
    }

    void CDebugDrawer::_drawAABB( const CVec3& aabbMin, 
                                  const CVec3& aabbMax, 
                                  const CMat4& worldTransform, 
                                  const CVec3& color )
    {
        auto _vmin2max = aabbMax - aabbMin;
        auto _dx = CVec3::dot( _vmin2max, worldTransform.getBasisVectorX() );
        auto _dy = CVec3::dot( _vmin2max, worldTransform.getBasisVectorY() );
        auto _dz = CVec3::dot( _vmin2max, worldTransform.getBasisVectorZ() );
        auto _origin = worldTransform.getPosition();
        auto _sidex = worldTransform.getBasisVectorX();
        auto _sidey = worldTransform.getBasisVectorY();
        auto _sidez = worldTransform.getBasisVectorZ();

        _sidex.scale( 0.5 * _dx, 0.5 * _dx, 0.5 * _dx );
        _sidey.scale( 0.5 * _dy, 0.5 * _dy, 0.5 * _dy );
        _sidez.scale( 0.5 * _dz, 0.5 * _dz, 0.5 * _dz );

        /*
        *      p8 ___________ p7 -> max
        *        /|         /|
        *       / |        / |      z    y
        *      /__|______ /  |      |  /
        *     |p4 |_ _ _ |p3_|      | /
        *     |  /p5     |  / p6    |/_ _ _ x
        *     | /        | / 
        *     |/_________|/ 
        *  p1 -> min      p2
        */

        auto _p1 = _origin - _sidex - _sidey - _sidez;
        auto _p2 = _origin + _sidex - _sidey - _sidez;
        auto _p3 = _origin + _sidex - _sidey + _sidez;
        auto _p4 = _origin - _sidex - _sidey + _sidez;

        auto _p5 = _origin - _sidex + _sidey - _sidez;
        auto _p6 = _origin + _sidex + _sidey - _sidez;
        auto _p7 = _origin + _sidex + _sidey + _sidez;
        auto _p8 = _origin - _sidex + _sidey + _sidez;

        _drawLine( _p1, _p2, color ); _drawLine( _p2, _p3, color );
        _drawLine( _p3, _p4, color ); _drawLine( _p4, _p1, color );

        _drawLine( _p2, _p6, color ); _drawLine( _p6, _p7, color );
        _drawLine( _p7, _p3, color ); _drawLine( _p3, _p2, color );

        _drawLine( _p4, _p3, color ); _drawLine( _p3, _p7, color );
        _drawLine( _p7, _p8, color ); _drawLine( _p8, _p4, color );

        _drawLine( _p4, _p8, color ); _drawLine( _p8, _p5, color );
        _drawLine( _p5, _p1, color ); _drawLine( _p1, _p4, color );

        _drawLine( _p1, _p2, color ); _drawLine( _p2, _p6, color );
        _drawLine( _p6, _p5, color ); _drawLine( _p5, _p1, color );

        _drawLine( _p5, _p6, color ); _drawLine( _p6, _p7, color );
        _drawLine( _p7, _p8, color ); _drawLine( _p8, _p5, color );
    }
}