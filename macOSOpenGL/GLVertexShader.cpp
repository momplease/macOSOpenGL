//
//  GLVertexShader.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLVertexShader.hpp"
#include "GLShaderProgram.hpp"
#include "GLShaderLoader.hpp"
#include "GLContext.hpp"

GLVertexShader::GLVertexShader(std::string path, GLShaderProgram *program) : GLShader(path, program) {
}

GLVertexShader::~GLVertexShader() {
}


void GLVertexShader::buildShader(GLShaderLoader *loader) {
    GLShader::buildShader(loader); // create id
    
    std::string rawShaderCode = loader->loadRawShaderCode(path);
    loader->compileShader(getId(), rawShaderCode);
}

void GLVertexShader::addAttribute(std::string name, std::string type) {
    if (!hasAttributeWithName(name)) {
        GLint location = NAN;
        location = GLContext::mainContext()->getAttributeLocation(program->getProgramId(), name.c_str());
        if (location == NAN) // GLShaderWriter
            throw std::runtime_error("Can't find location in shader for attribute named " + name);
        attributeLocations.insert({name, location});
    }
}

GLint GLVertexShader::getAttributeLocationByName(std::string name) {
    return attributeLocations.at(name);
}

bool GLVertexShader::hasAttributeWithName(std::string name) {
    return attributeLocations.find(name) != attributeLocations.end();
}

GLenum GLVertexShader::getType() const {
    return shaderType;
}
