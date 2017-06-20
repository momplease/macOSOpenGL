//
//  GLShaderProgram.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLShaderProgram.hpp"
#include "GLVertexShader.hpp"
#include "GLFragmentShader.hpp"
#include "GLContext.hpp"
#include "GLShaderLoader.hpp"

GLShaderProgram::GLShaderProgram() {
    shaderLoader.reset(new GLShaderLoader());
}


GLShaderProgram::~GLShaderProgram() {

}

void GLShaderProgram::addFragmentShader(GLFragmentShader *fragment) {
    fragmentShader.reset(fragment);
}

void GLShaderProgram::addVertexShader(GLVertexShader *vertex) {
    vertexShader.reset(vertex);
}

void GLShaderProgram::prepareOpenGL() {
    vertexShader->buildShader(shaderLoader.get());
    fragmentShader->buildShader(shaderLoader.get());
    
    programId = shaderLoader->createShaderProgram(vertexShader->getId(),
                                                  fragmentShader->getId(),
                                                  [this](){
                                                      glDeleteShader(vertexShader->getId());
                                                      glDeleteShader(fragmentShader->getId());
                                                  });
    
    vertexShader->prepareOpenGL();
    fragmentShader->prepareOpenGL();
}

void GLShaderProgram::use() const {
    GLContext::mainContext()->useShaderProgram(getProgramId());
}

GLuint GLShaderProgram::getProgramId() const {
    return programId;
}

GLFragmentShader* GLShaderProgram::getFragmentShader() const {
    return fragmentShader.get();
}

GLVertexShader* GLShaderProgram::getVertexShader() const {
    return vertexShader.get();
}
