//
//  GLGrid.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/28/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLGrid_hpp
#define GLGrid_hpp

#include <vector>
#include <OpenGL/gl.h>
#include <math.h>

#include "glm/fwd.hpp"
#include "GLObject.hpp"

struct GLGridConfig {
    GLenum topology = GL_FLOAT;
    GLint dataOffset = 3;
    GLboolean verticesNormilized = GL_FALSE;
    GLsizei stride = 0;
};

class GLGrid : public GLObject {
public:
    GLGrid(const std::vector<glm::vec3>& vertices,
           const std::vector<unsigned int>& indices,
           GLGridConfig config = GLGridConfig());
    
    virtual ~GLGrid();
    
    GLGrid(const GLGrid&) = delete;
    GLGrid(GLGrid&&) = delete;
    GLGrid& operator=(const GLGrid&) = delete;
    GLGrid& operator=(GLGrid&&) = delete;
    
public:
    
    virtual void prepareOpenGL() override;
    virtual void clearOpenGL() override;
    
    virtual void bind(GLShaderProgram *) override;
    virtual void unbind(GLShaderProgram *) override;
    
    void setConfig(GLGridConfig config);
    GLGridConfig* getConfig();
    
private:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    GLGridConfig config;
    
    GLuint vertexBufferObject = NAN;
    GLuint indicesBufferObject = NAN;
};

#endif /* GLGrid_hpp */
