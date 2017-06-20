//
//  GLShaderProgram.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLShaderProgram_hpp
#define GLShaderProgram_hpp

#include <OpenGL/gl.h>
#include <memory>
#include "GLObject.hpp"

class GLShaderLoader;

class GLFragmentShader;
class GLVertexShader;

class GLShaderProgram : public GLObject {
public:
    GLShaderProgram();
    virtual ~GLShaderProgram();
    
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram& operator=(const GLShaderProgram&) = delete;
    GLShaderProgram(GLShaderProgram&&) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&) = delete;
    
    virtual void prepareOpenGL() override;
    
    void addFragmentShader(GLFragmentShader *);
    void addVertexShader(GLVertexShader *);
    
    GLFragmentShader* getFragmentShader() const;
    GLVertexShader* getVertexShader() const;
    
    GLuint getProgramId() const;
    
    void use() const;
    
protected:
    std::unique_ptr<GLFragmentShader> fragmentShader;
    std::unique_ptr<GLVertexShader> vertexShader;
    GLuint programId;
    
    std::unique_ptr<GLShaderLoader> shaderLoader;
};

#endif /* GLShaderProgram_hpp */
