//
//  GLFragmentShader.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLFragmentShader_hpp
#define GLFragmentShader_hpp

#include "GLShader.hpp"

class GLShaderLoader;

class GLFragmentShader : public GLShader {
public:
    GLFragmentShader(std::string path, GLShaderProgram *);
    virtual ~GLFragmentShader();
    
    GLFragmentShader(const GLFragmentShader&) = delete;
    GLFragmentShader& operator=(const GLFragmentShader&) = delete;
    GLFragmentShader(GLFragmentShader&&) = delete;
    GLFragmentShader& operator=(GLFragmentShader&&) = delete;
    
    virtual void buildShader(GLShaderLoader *) override;
    
protected:
    virtual GLenum getType() const override;
    GLenum shaderType = GL_FRAGMENT_SHADER;
};

#endif /* GLFragmentShader_hpp */
