//
//  GL3DRenderMesh.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GL3DRenderMesh_hpp
#define GL3DRenderMesh_hpp

#include "Mesh.hpp"
#include "glm/fwd.hpp"

class GLDrawableComponent;
class GLVertexedComponent;
class GLTexturedComponent;
class GLNormalMappedComponent;
class GLShaderProgram;

class GL3DRenderMesh final : public Mesh {
public:
    GL3DRenderMesh(const std::vector<glm::vec3>& vertices,
                   const std::vector<glm::vec2>& uvs,
                   const std::vector<glm::vec3>& normals,
                   const std::vector<unsigned int>& indices,
                   macOSOpenGL::Transform *transform,
                   RenderableConfig config);
    
    GL3DRenderMesh(const std::vector<glm::vec3>& vertices,
                   const std::vector<glm::vec2>& uvs,
                   const std::vector<glm::vec3>& normals,
                   const std::vector<unsigned int>& indices,
                   RenderableConfig config);
    
    
    virtual ~GL3DRenderMesh() = default;
    
    GL3DRenderMesh(const GL3DRenderMesh&) = delete;
    GL3DRenderMesh& operator=(const GL3DRenderMesh&) = delete;
    GL3DRenderMesh(GL3DRenderMesh&&) = delete;
    GL3DRenderMesh& operator=(GL3DRenderMesh&&) = delete;
    
    virtual IGLDrawable* getDrawable() const override;
    virtual IVertexed* getVertexed() const override;
    virtual ITextured* getTextured() const override;
    
    virtual void updateAnimations(double) override;
    virtual void prepareOpenGL() override;
    virtual void bind(GLShaderProgram *) override;
    
private:
    std::unique_ptr<GLDrawableComponent> drawable;
    std::unique_ptr<GLVertexedComponent> vertexed;
    std::unique_ptr<GLTexturedComponent> textured;
    
    friend GLTexturedComponent;
    friend GLVertexedComponent;
    friend GLDrawableComponent;
};

#endif /* GL3DRenderMesh_hpp */
