#!/usr/bin/env python

import tinyrenderer as tr
import numpy as np
from OpenGL.GL import *

def createGeometryVer1NoIndices() :
    bufferData = np.array( [ 
    #|     pos     |    color    |#
        -0.5, -0.5, 1.0, 0.0, 0.0,
         0.5, -0.5, 0.0, 1.0, 0.0,
         0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5, -0.5, 1.0, 0.0, 0.0,
         0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5,  0.5, 1.0, 1.0, 1.0
    ], dtype = np.float32 )

    layout = tr.core.VertexBufferLayout( [ [ 'pos', tr.core.ElementType.Float2, False ],
                                      [ 'color', tr.core.ElementType.Float3, False ] ] )

    vbuffer = tr.core.VertexBuffer( layout,
                               tr.core.BufferUsage.STATIC,
                               bufferData.size * bufferData.itemsize,
                               bufferData )

    varray = tr.core.VertexArray()
    varray.addVertexBuffer( vbuffer )
    return varray

def createGeometryVer2NoIndices() :
    bufferPosData = np.array( [
    #|     pos     |#
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        -0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5 
    ], dtype = np.float32 )

    bufferColData = np.array( [
    #|      color      |#
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0
    ], dtype = np.float32 )

    layoutPos = tr.core.VertexBufferLayout( [ [ 'pos', tr.core.ElementType.Float2, False ] ] )
    layoutCol = tr.core.VertexBufferLayout( [ [ 'color', tr.core.ElementType.Float3, False ] ] )

    vbufferPos = tr.core.VertexBuffer( layoutPos,
                                  tr.core.BufferUsage.STATIC,
                                  bufferPosData.size * bufferPosData.itemsize,
                                  bufferPosData )

    vbufferCol = tr.core.VertexBuffer( layoutCol,
                                  tr.core.BufferUsage.STATIC,
                                  bufferColData.size * bufferColData.itemsize,
                                  bufferColData )

    varray = tr.core.VertexArray()
    varray.addVertexBuffer( vbufferPos )
    varray.addVertexBuffer( vbufferCol )

    return varray

def createGeometryVer1WithIndices() :
    bufferData = np.array( [
    #|     pos     |     color      |#
        -0.5, -0.5, 1.0, 0.0, 0.0,
         0.5, -0.5, 0.0, 1.0, 0.0,
         0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5,  0.5, 1.0, 1.0, 1.0
    ], dtype = np.float32 )

    indices = np.array( [
        0, 1, 2, # first triangle
        0, 2, 3  # second triangle
    ], dtype = np.uint32 )

    layout = tr.core.VertexBufferLayout( [ [ 'pos', tr.core.ElementType.Float2, False ],
                                      [ 'color', tr.core.ElementType.Float3, False ] ] )

    vbuffer = tr.core.VertexBuffer( layout,
                               tr.core.BufferUsage.STATIC,
                               bufferData.size * bufferData.itemsize,
                               bufferData )

    ibuffer = tr.core.IndexBuffer( tr.core.BufferUsage.STATIC, 6, indices )

    varray = tr.core.VertexArray()
    varray.addVertexBuffer( vbuffer )
    varray.setIndexBuffer( ibuffer )

    return varray

def createGeometryVer2WithIndices() :
    bufferPosData = np.array( [
    #|     pos     |#
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5 
    ], dtype = np.float32 )

    bufferColData = np.array( [
    #|      color      |#
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0
    ], dtype = np.float32 )

    indices = np.array( [
        0, 1, 2, # first triangle
        0, 2, 3  # second triangle
    ], dtype = np.uint32 )

    layoutPos = tr.core.VertexBufferLayout( [ [ 'pos', tr.core.ElementType.Float2, False ] ] )
    layoutCol = tr.core.VertexBufferLayout( [ [ 'color', tr.core.ElementType.Float3, False ] ] )

    vbufferPos = tr.core.VertexBuffer( layoutPos,
                                  tr.core.BufferUsage.STATIC,
                                  bufferPosData.size * bufferPosData.itemsize,
                                  bufferPosData )

    vbufferCol = tr.core.VertexBuffer( layoutCol,
                                  tr.core.BufferUsage.STATIC,
                                  bufferColData.size * bufferColData.itemsize,
                                  bufferColData )

    ibuffer = tr.core.IndexBuffer( tr.core.BufferUsage.STATIC, 6, indices )

    varray = tr.core.VertexArray()
    varray.addVertexBuffer( vbufferPos )
    varray.addVertexBuffer( vbufferCol )
    varray.setIndexBuffer( ibuffer )

    return varray

if __name__ == '__main__' :
    app = tr.core.Application()

    shader = tr.core.ShaderManager.GetCachedShader( 'basic2d_no_textures' )

    geometryVAO = createGeometryVer1NoIndices()
    ## geometryVAO = createGeometryVer2NoIndices()
    ## geometryVAO = createGeometryVer1WithIndices()
    ## geometryVAO = createGeometryVer2WithIndices()

    while app.active() :
        app.begin()

        if ( tr.core.InputManager.IsKeyDown( tr.core.Keys.KEY_ESCAPE ) ) :
            break

        shader.bind()
        geometryVAO.bind()

        if ( geometryVAO.indexBuffer() ) :
            glDrawElements( GL_TRIANGLES, geometryVAO.indexBuffer().count(), GL_UNSIGNED_INT, None )
        else :
            glDrawArrays( GL_TRIANGLES, 0, 6 )

        geometryVAO.unbind()
        shader.unbind()
        app.end()

####     # check the addresses and str-representation of the objects
####     print( 'vao-py-address: {} \n\robj: {}'.format( hex( id( geometryVAO ) ), geometryVAO ) )
####     for vbo in geometryVAO.vertexBuffers() :
####         print( 'vbo-py-address: {}\n\robj: {}'.format( hex( id( vbo ) ), vbo ) )
####     print( 'ebo-py-address: {} \n\robj: {}'.format( hex( id( geometryVAO.indexBuffer() ) ), geometryVAO.indexBuffer() ) )