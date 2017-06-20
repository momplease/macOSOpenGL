//
//  GLDrawableComponent.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/2/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLDrawableComponent_hpp
#define GLDrawableComponent_hpp

#include "GLComponent.hpp"
#include "IGLDrawable.h"

class GL3DRenderMesh;

class GLDrawableComponent final: public GLComponent, public IGLDrawable {
public:
    GLDrawableComponent(DrawableConfig, GL3DRenderMesh*);
    virtual ~GLDrawableComponent() = default;
    
    GLDrawableComponent(const GLDrawableComponent&) = delete;
    GLDrawableComponent& operator=(const GLDrawableComponent&) = delete;
    GLDrawableComponent(GLDrawableComponent&&) = delete;
    GLDrawableComponent& operator=(GLDrawableComponent&&) = delete;
    
    void setConfig(DrawableConfig);
    
    virtual void prepareOpenGL() override;
    
    // IGLDrawable
    virtual GLuint getPrimitiveType() const override;
    virtual GLint getOffset() const override;
    virtual GLsizei getVerticesCount() const override;
    virtual bool isUsingIndices() const override;
    virtual bool isUsingDepthBuffer() const override;
    virtual GLsizei getIndicesCount() const override;
    virtual GLenum getIndicesType() const override;
    virtual GLuint getIndicesBufferObject() const override;
    
    virtual void bind(GLShaderProgram *) override {}
    
private:
    GLuint primitiveType;
    GLint offset;
    GLenum indicesType;
    bool useIndices;
    bool useDepthBuffer;
    GLuint indicesBufferObject;
    
    GL3DRenderMesh *holder;
};


#endif /* GLDrawableComponent_hpp */
