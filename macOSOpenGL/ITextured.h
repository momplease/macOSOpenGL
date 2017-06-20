//
//  ITextured.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/16/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef ITextured_h
#define ITextured_h

#include <OpenGL/gl.h>
#include <cmath>

struct TexturedConfig {
    GLint offsetInTextureCoords = NAN;
    GLenum textureCoordsType = NAN;
    GLboolean textureCoordsNormilized = NAN;
    GLsizei textureCoordsStride = NAN;
    GLuint textureId = NAN;
    GLuint normalMapId = NAN;
};

struct ITextured {
    ITextured() = default;
    ~ITextured() = default;
    
    ITextured(const ITextured&) = delete;
    ITextured& operator=(const ITextured&) = delete;
    ITextured(ITextured&&) = delete;
    ITextured& operator=(ITextured&&) = delete;
    
    virtual GLint getOffsetInTextureCoords() const = 0;
    virtual GLenum getTextureCoordsType() const = 0;
    virtual GLboolean getTextureCoordsNormilized() const = 0;
    virtual GLsizei getTextureCoordsStride() const = 0;
    virtual GLuint getUVBufferObject() const = 0;
    virtual GLuint getNormalBufferObject() const = 0;
    virtual GLint getNormalDataOffset() const = 0;
    virtual GLuint getTextureId() const = 0;
    virtual GLuint getTangentBufferObject() const = 0;
    virtual GLuint getBinormalBufferObject() const = 0;

};

#endif /* ITextured_h */
