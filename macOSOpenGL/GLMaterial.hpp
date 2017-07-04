//
//  GLMaterial.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLMaterial_hpp
#define GLMaterial_hpp

#include <OpenGL/gl.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "glm/fwd.hpp"
#include "GLObject.hpp"

class GLShaderProgram;
class GLTexture;

struct GLMaterialConfig {
    GLint offsetInUVCoords = 2;
    GLenum UVCoordsType = GL_FLOAT;
    GLboolean UVCoordsNormilized = GL_FALSE;
    GLsizei UVCoordsStride = 0;
    GLint normalsOffset = 3;
};

class GLMaterial : public GLObject {
public:
    GLMaterial(const std::vector<glm::vec2>& UVs,
               const std::vector<glm::vec3>& normals,
               const std::vector<glm::vec3>& tangents,
               GLMaterialConfig config = GLMaterialConfig());
    
    virtual ~GLMaterial();
    
    GLMaterial(const GLMaterial&) = delete;
    GLMaterial& operator=(const GLMaterial&) = delete;
    GLMaterial(GLMaterial&&) = delete;
    GLMaterial& operator=(GLMaterial&&) = delete;
public:
    virtual void prepareOpenGL() override;
    virtual void clearOpenGL() override;
    
    virtual void bind(GLShaderProgram *) override;
    virtual void unbind(GLShaderProgram *) override;
    
    const std::vector<std::unique_ptr<GLTexture>>& getTextures() const;
    void addTexture(GLTexture *);
    
    void setConfig(GLMaterialConfig config);
    GLMaterialConfig* getConfig();
    
protected:
    std::vector<glm::vec2> UVs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    
    std::vector<std::unique_ptr<GLTexture>> textures;
    GLMaterialConfig config;
    
    GLuint UVBufferObject = NAN;
    GLuint normalBufferObject = NAN;
    GLuint tangentBufferObject = NAN;
    
};

#endif /* GLMaterial_hpp */
