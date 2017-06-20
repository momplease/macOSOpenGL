//
//  IGLDrawable.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef IGLDrawable_h
#define IGLDrawable_h

#include <OpenGL/gl.h>
#include <cmath>
#include "glm/fwd.hpp"

struct DrawableConfig {
    GLuint primitiveType = NAN;
    GLint offset = NAN;
    GLenum indicesType = NAN;
    bool useDepthBuffer = false;
    bool useIndices = false;
};

// Main draw call interface
struct IGLDrawable {
    IGLDrawable() = default;
    virtual ~IGLDrawable() = default;
    
    IGLDrawable(const IGLDrawable&) = delete;
    IGLDrawable& operator=(const IGLDrawable&) = delete;
    IGLDrawable(IGLDrawable&&) = delete;
    IGLDrawable& operator=(IGLDrawable&&) = delete;
    
    virtual GLuint getPrimitiveType() const = 0;
    virtual GLint getOffset() const = 0;
    virtual GLsizei getVerticesCount() const = 0;
    virtual bool isUsingIndices() const = 0;
    virtual bool isUsingDepthBuffer() const = 0;
    virtual GLsizei getIndicesCount() const = 0;
    virtual GLenum getIndicesType() const = 0;
    virtual GLuint getIndicesBufferObject() const = 0;
};

#endif /* IGLDrawable_h */
