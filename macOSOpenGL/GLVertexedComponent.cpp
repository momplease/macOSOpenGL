//
//  GLVertexedComponent.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/3/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLVertexedComponent.hpp"
#include "GL3DRenderMesh.hpp"
#include "GLShaderProgram.hpp"
#include "GLContext.hpp"
#include "GLVertexShader.hpp"
#include "GLFragmentShader.hpp"
#include "glm/glm.hpp"

GLVertexedComponent::GLVertexedComponent(VertexedConfig config, GL3DRenderMesh *holder) : GLComponent() {
    setConfig(config);
    this->holder = holder;
}

void GLVertexedComponent::setConfig(VertexedConfig config) {
    verticesType = config.verticesType;
    dataOffset = config.dataOffset;
    verticesNormilized = config.verticesNormilized;
    stride = config.stride;
    useIndices = config.useIndices;
}

void GLVertexedComponent::prepareOpenGL() {
    glGenBuffers(1, &vertexBufferObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * holder->getVertices().size(),
                 holder->getVertices().data(),
                 GL_STATIC_DRAW);
    
    if (useIndices) {
        glGenBuffers(1, &indicesBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * holder->getIndices().size(),
                     holder->getIndices().data(),
                     GL_STATIC_DRAW);
    }
}

void GLVertexedComponent::bind(GLShaderProgram *shaderProgram) {
    GLContext::mainContext()->bindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    GLContext::mainContext()->enableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexPosition_modelSpace"));
    GLContext::mainContext()->vertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexPosition_modelSpace"),
                                                  dataOffset,
                                                  verticesType,
                                                  verticesNormilized,
                                                  stride,
                                                  (void*)0);
}

GLenum GLVertexedComponent::getVerticesType() const {
    return verticesType;
}

GLint GLVertexedComponent::getDataOffset() const {
    return dataOffset;
}

GLboolean GLVertexedComponent::getVerticesNormilized() const {
    return verticesNormilized;
}

GLsizei GLVertexedComponent::getStride() const {
    return stride;
}

bool GLVertexedComponent::getUseIndices() const {
    return useIndices;
}

GLuint GLVertexedComponent::getVertexBufferObject() const {
    return vertexBufferObject;
}

GLuint GLVertexedComponent::getIndicesBufferObject() const {
    return indicesBufferObject;
}


