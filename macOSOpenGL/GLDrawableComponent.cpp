//
//  GLDrawableComponent.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/2/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLDrawableComponent.hpp"
#include "GL3DRenderMesh.hpp"
#include "glm/glm.hpp"

GLDrawableComponent::GLDrawableComponent(DrawableConfig config, GL3DRenderMesh *holder) : GLComponent() {
    setConfig(config);
    this->holder = holder;
}

void GLDrawableComponent::setConfig(DrawableConfig config) {
    primitiveType = config.primitiveType;
    offset = config.offset;
    indicesType = config.indicesType;
    useIndices = config.useIndices;
    useDepthBuffer = config.useDepthBuffer;
}

void GLDrawableComponent::prepareOpenGL() {
    // reserved
}

GLuint GLDrawableComponent::getPrimitiveType() const {
    return primitiveType;
}

GLint GLDrawableComponent::getOffset() const {
    return offset;
}

GLsizei GLDrawableComponent::getVerticesCount() const {
    return static_cast<GLsizei>(holder->getVertices().size());
}

GLsizei GLDrawableComponent::getIndicesCount() const {
    return static_cast<GLsizei>(holder->getIndices().size());
}

GLenum GLDrawableComponent::getIndicesType() const {
    return indicesType;
}

bool GLDrawableComponent::isUsingIndices() const {
    return holder->getVertexed()->getUseIndices();
}

bool GLDrawableComponent::isUsingDepthBuffer() const {
    return useDepthBuffer;
}

GLuint GLDrawableComponent::getIndicesBufferObject() const {
    return holder->getVertexed()->getIndicesBufferObject();
}
