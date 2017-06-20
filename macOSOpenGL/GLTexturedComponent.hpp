//
//  GLTexturedComponent.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/4/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLTexturedComponent_hpp
#define GLTexturedComponent_hpp

#include "GLComponent.hpp"
#include "ITextured.h"

class GL3DRenderMesh;

class GLTexturedComponent final : public GLComponent, public ITextured {
public:
    GLTexturedComponent(TexturedConfig, GL3DRenderMesh*);
    virtual ~GLTexturedComponent();
    
    GLTexturedComponent(const GLTexturedComponent&) = delete;
    GLTexturedComponent& operator=(const GLTexturedComponent&) = delete;
    GLTexturedComponent(GLTexturedComponent&&) = delete;
    GLTexturedComponent& operator=(GLTexturedComponent&&) = delete;
    
    void setConfig(TexturedConfig);
    
    virtual void prepareOpenGL() override;
    
    // ITextured
    virtual GLint getOffsetInTextureCoords() const override;
    virtual GLenum getTextureCoordsType() const override;
    virtual GLboolean getTextureCoordsNormilized() const override;
    virtual GLsizei getTextureCoordsStride() const override;
    virtual GLuint getUVBufferObject() const override;
    virtual GLuint getNormalBufferObject() const override;
    virtual GLuint getTextureId() const override;
    virtual GLint getNormalDataOffset() const override;
    virtual GLuint getTangentBufferObject() const override;
    virtual GLuint getBinormalBufferObject() const override;
    
    virtual void bind(GLShaderProgram *) override;
    
private:
    GLint offsetInTextureCoords;
    GLenum textureCoordsType;
    GLboolean textureCoordsNormilized;
    GLsizei textureCoordsStride;
    GLuint UVBufferObject = NAN;
    GLuint normalBufferObject = NAN;
    GLuint textureId;
    GLuint normalMapId;
    
    GLuint tangentBufferObject;
    GLuint binormalBufferObject;
    
    GL3DRenderMesh *holder;
};

#endif /* GLTexturedComponent_hpp */
