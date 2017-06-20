//
//  IVertexed.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/16/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef IVertexed_h
#define IVertexed_h

#include <OpenGL/gl.h>
#include <cmath>

struct VertexedConfig {
    GLenum verticesType = NAN;
    GLint dataOffset = NAN;
    GLboolean verticesNormilized = NAN;
    GLsizei stride = NAN;
    bool useIndices = false;
};

// Main vertex binding interface
struct IVertexed {
    IVertexed() = default;
    ~IVertexed() = default;
    
    IVertexed(const IVertexed&) = delete;
    IVertexed& operator=(const IVertexed&) = delete;
    IVertexed(IVertexed&&) = delete;
    IVertexed& operator=(IVertexed&&) = delete;
    
    virtual GLenum getVerticesType() const = 0;
    virtual GLint getDataOffset() const = 0;
    virtual GLboolean getVerticesNormilized() const = 0;
    virtual GLsizei getStride() const = 0;
    virtual bool getUseIndices() const = 0;
    virtual GLuint getVertexBufferObject() const = 0;
    virtual GLuint getIndicesBufferObject() const = 0;
    
};

#endif /* IVertexed_h */
