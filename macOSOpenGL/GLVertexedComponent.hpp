//
//  GLVertexedComponent.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/3/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLVertexedComponent_hpp
#define GLVertexedComponent_hpp

#include "GLComponent.hpp"
#include "IVertexed.h"

class GL3DRenderMesh;

class GLVertexedComponent final: public GLComponent, public IVertexed {
public:
    GLVertexedComponent(VertexedConfig, GL3DRenderMesh *);
    virtual ~GLVertexedComponent() = default;
    
    GLVertexedComponent(const GLVertexedComponent&) = delete;
    GLVertexedComponent& operator=(const GLVertexedComponent&) = delete;
    GLVertexedComponent(GLVertexedComponent&&) = delete;
    GLVertexedComponent& operator=(GLVertexedComponent&&) = delete;
    
    void setConfig(VertexedConfig);
    
    virtual void prepareOpenGL() override;
    
    // IVertexed
    virtual GLenum getVerticesType() const override;
    virtual GLint getDataOffset() const override;
    virtual GLboolean getVerticesNormilized() const override;
    virtual GLsizei getStride() const override;
    virtual bool getUseIndices() const override;
    virtual GLuint getVertexBufferObject() const override;
    virtual GLuint getIndicesBufferObject() const override;
    
    virtual void bind(GLShaderProgram *) override;
    
private:
    GLenum verticesType;
    GLint dataOffset;
    GLboolean verticesNormilized;
    GLsizei stride;
    bool useIndices;
    GLuint vertexBufferObject = NAN;
    GLuint indicesBufferObject = NAN;
    
    GL3DRenderMesh *holder;
};

#endif /* GLVertexedComponent_hpp */
