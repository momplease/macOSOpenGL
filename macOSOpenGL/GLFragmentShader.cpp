//
//  GLFragmentShader.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLFragmentShader.hpp"
#include "GLShaderProgram.hpp"
#include "GLContext.hpp"
#include "GLShaderLoader.hpp"
#include <string>

GLFragmentShader::GLFragmentShader(std::string path, GLShaderProgram *program) : GLShader(path, program) {
}

GLFragmentShader::~GLFragmentShader() {
}

void GLFragmentShader::buildShader(GLShaderLoader *loader) {
    GLShader::buildShader(loader); // create id
    
    std::string rawShaderCode = loader->loadRawShaderCode(path);
    loader->compileShader(getId(), rawShaderCode);
}

GLenum GLFragmentShader::getType() const {
    return shaderType;
}
