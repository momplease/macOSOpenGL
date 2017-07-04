//
//  GLGrid.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/28/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLGrid.hpp"
#include "glm/glm.hpp"
#include "GLShaderProgram.hpp"
#include "GLVertexShader.hpp"
#include "GLFragmentShader.hpp"
#include "GLContext.hpp"
#include <OpenGL/gl.h>

namespace {
const std::string kVertexAttributeInShaderName = "vertexPosition_modelSpace";
}

GLGrid::GLGrid(const std::vector<glm::vec3>& vertices,
               const std::vector<unsigned int>& indices,
               GLGridConfig config) : GLObject(), vertices(vertices), indices(indices), config(config) {
}


GLGrid::~GLGrid() {
}

void GLGrid::prepareOpenGL() {
    if (wasInitialized)
        return;
    
    
    glGenBuffers(1, &vertexBufferObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);
    
    if (isUsingIndices()) {
        glGenBuffers(1, &indicesBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * indices.size(),
                     indices.data(),
                     GL_STATIC_DRAW);
    }
    
    wasInitialized = true;
}

void GLGrid::clearOpenGL() {
    // TODO
}

void GLGrid::bind(GLShaderProgram *shaderProgram) {
    GLContext::mainContext()->bindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    
    auto vertexLocation = shaderProgram->getVertexShader()->getAttributeLocationByName(kVertexAttributeInShaderName);
    
    GLContext::mainContext()->enableVertexAttribArray(vertexLocation);
    GLContext::mainContext()->vertexAttribPointer(vertexLocation,
                                                  config.dataOffset,
                                                  config.verticesType,
                                                  config.verticesNormilized,
                                                  config.stride,
                                                  (void*)0);
}

void GLGrid::unbind(GLShaderProgram *shaderProgram) {
    // TODO
}

void GLGrid::setConfig(GLGridConfig config) {
    this->config = config;
}

GLGridConfig* GLGrid::getConfig() {
    return &config;
}

bool GLGrid::isUsingIndices() const {
    return not indices.empty();
}

GLuint GLGrid::getIBO() const {
    return indicesBufferObject;
}

GLsizei GLGrid::getIndicesCount() const {
    return static_cast<GLsizei>(indices.size());
}

GLenum GLGrid::getIndicesType() const {
    return GL_UNSIGNED_INT;
}

GLsizei GLGrid::getVerticesCount() const {
    return static_cast<GLsizei>(vertices.size());
}
