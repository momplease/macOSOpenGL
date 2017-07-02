//
//  GLShader.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLShader.hpp"
#include "GLContext.hpp"
#include "GLShaderProgram.hpp"
#include "GLShaderLoader.hpp"
#include "glm/glm.hpp"

GLShader::GLShader(std::string path, GLShaderProgram *program) : path(path), program(program) {
}

GLShader::~GLShader() {
    
}

std::string GLShader::getPathWhereLocated() const {
    return path;
}

void GLShader::addUniform(std::string name, std::string type) {
    if (!hasUniformWithName(name)) {
        GLint location = NAN;
        location = GLContext::mainContext()->getUniformLocation(program->getProgramId(), name.c_str());
        if (location == NAN) // Write uniform type (GLUniformWriter ???) "uniform type key;\n"
            throw std::runtime_error("Can't find uniform with name " + name + " in shader");
        uniformIds.insert({name, location});
    }
}

template<typename T>
void GLShader::setUniformValueByName(std::string name, T *value) {
    if (!hasUniformWithName(name)) {
        throw std::runtime_error("Can't find uniform with name " + name);
    }
    // Get value type
    // Calling mapped function to set uniform value
    const auto& typeInfo = typeid(*value);
    uniformValueSetters.at(typeInfo.hash_code())(name, value);
}
template void GLShader::setUniformValueByName<glm::mat4>(std::string name, glm::mat4 *value);
template void GLShader::setUniformValueByName<glm::vec3>(std::string name, glm::vec3 *value);
template void GLShader::setUniformValueByName<glm::mat3>(std::string name, glm::mat3 *value);

bool GLShader::hasUniformWithName(std::string name) {
    return uniformIds.find(name) != uniformIds.end();
}

GLint GLShader::getUniformLocationByName(std::string name) {
    return GLContext::mainContext()->getUniformLocation(program->getProgramId(), name.c_str());
}

void GLShader::prepareOpenGL() {
    // mat4 uniform setter
    uniformValueSetters.insert({typeid(glm::mat4).hash_code(), [this](std::string key, void *value){
        glm::mat4 *mat = (glm::mat4*)value;
        GLContext::mainContext()->uniformMatrix4fv(uniformIds.at(key),
                                                   1,
                                                   GL_FALSE,
                                                   &(*mat)[0][0]);
    }});
    // mat3 uniform setter
    uniformValueSetters.insert({typeid(glm::mat3).hash_code(), [this](std::string key, void *value){
        glm::mat3 *mat = (glm::mat3*)value;
        GLContext::mainContext()->uniformMatrix3fv(uniformIds.at(key),
                                                   1,
                                                   GL_FALSE,
                                                   &(*mat)[0][0]);
    }});
    // vec3 uniform setter
    uniformValueSetters.insert({typeid(glm::vec3).hash_code(), [this](std::string key, void *value){
        glm::vec3 *vec = (glm::vec3*)value;
        GLContext::mainContext()->uniform3fv(uniformIds.at(key),
                                             1,
                                             &(*vec)[0]);
    }});
    
    
    
}

GLuint GLShader::getId() const {
    return shaderId;
}

bool GLShader::isBuild() const {
    return shaderId == NAN;
}

void GLShader::buildShader(GLShaderLoader *loader) {
    this->shaderId = GLContext::mainContext()->createShader(getType());
}
