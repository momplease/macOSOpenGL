//
//  GLMesh.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/28/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLMesh_hpp
#define GLMesh_hpp

#include <memory>
#include <OpenGL/gl.h>
#include "GL3DObject.hpp"
#include "ISceneBehaviour.hpp"

class GLGrid;
class GLMaterial;

class Transform;

struct GLMeshConfig {
    GLuint topology = GL_TRIANGLES;
    GLint offset = 0;
    GLenum indicesType = GL_UNSIGNED_INT;
    bool useDepthBuffer = true;
};

class GLMesh : public GL3DObject, public ISceneBehaviour {
public:
    GLMesh(GLGrid *grid,
           GLMaterial *material,
           Transform *transform,
           GLMeshConfig config = GLMeshConfig());
    
    virtual ~GLMesh();
    
    GLMesh(const GLMesh&) = delete;
    GLMesh(GLMesh&&) = delete;
    GLMesh& operator=(const GLMesh&) = delete;
    GLMesh& operator=(GLMesh&&) = delete;
    
    
    virtual void updateAnimations(double deltaTime) override {}
    
    
    virtual void prepareOpenGL() override {}
    virtual void clearOpenGL() override {}
    
    virtual void bind(GLShaderProgram *) override {}
    virtual void unbind(GLShaderProgram *) override {}
    
public:
    
    GLGrid* getGrid() const;
    GLMaterial* getMaterial() const;
    
    void setConfig(GLMeshConfig config);
    GLMeshConfig* getConfig();
    
private:
    std::unique_ptr<GLGrid> grid;
    std::unique_ptr<GLMaterial> material;
    GLMeshConfig config;
};

#endif /* GLMesh_hpp */
