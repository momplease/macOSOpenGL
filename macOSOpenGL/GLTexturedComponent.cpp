//
//  GLTexturedComponent.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/4/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLTexturedComponent.hpp"
#include "glm/glm.hpp"
#include "GL3DRenderMesh.hpp"
#include "GLContext.hpp"
#include "GLShaderProgram.hpp"
#include "GLVertexShader.hpp"
#include "GLFragmentShader.hpp"

GLTexturedComponent::GLTexturedComponent(TexturedConfig config, GL3DRenderMesh *holder) :
GLComponent() {
    setConfig(config);
    this->holder = holder;
}

void GLTexturedComponent::setConfig(TexturedConfig config) {
    offsetInTextureCoords = config.offsetInTextureCoords;
    textureCoordsType = config.textureCoordsType;
    textureCoordsNormilized = config.textureCoordsNormilized;
    textureCoordsStride = config.textureCoordsStride;
    textureId = config.textureId;
    normalMapId = config.normalMapId;
}

void GLTexturedComponent::prepareOpenGL() {
    glGenBuffers(1, &UVBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, UVBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec2) * holder->getUVs().size(),
                 holder->getUVs().data(),
                 GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * holder->getNormals().size(),
                 holder->getNormals().data(),
                 GL_STATIC_DRAW);
    
    glGenBuffers(1, &tangentBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * holder->getTangents().size(),
                 holder->getTangents().data(),
                 GL_STATIC_DRAW);
    
    glGenBuffers(1, &binormalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, binormalBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * holder->getBitangents().size(),
                 holder->getBitangents().data(),
                 GL_STATIC_DRAW);

}

void GLTexturedComponent::bind(GLShaderProgram *shaderProgram) {
    GLint diffuseTextureLocation = shaderProgram->getFragmentShader()->getUniformLocationByName("uniDiffuseTexture");
    glUniform1i(diffuseTextureLocation, 0);
    
    GLint normalMapTextureLocation = shaderProgram->getFragmentShader()->getUniformLocationByName("uniNormalMap");
    glUniform1i(normalMapTextureLocation, 1);
    
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMapId);
    
    glBindBuffer(GL_ARRAY_BUFFER, UVBufferObject);
    glEnableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName("UV"));
    glVertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName("UV"),
                          offsetInTextureCoords,
                          textureCoordsType,
                          textureCoordsNormilized,
                          textureCoordsStride,
                          (void*)0);
    
    glEnableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexNormal_modelSpace"));
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glVertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexNormal_modelSpace"),
                          holder->getVertexed()->getDataOffset(),
                          textureCoordsType,
                          textureCoordsNormilized,
                          textureCoordsStride,
                          (void*)0);
    
    glEnableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexTangent_modelSpace"));
    glBindBuffer(GL_ARRAY_BUFFER, tangentBufferObject);
    glVertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexTangent_modelSpace"),
                          holder->getVertexed()->getDataOffset(),
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0);
    
    
    glEnableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexBinormal_modelSpace"));
    glBindBuffer(GL_ARRAY_BUFFER, binormalBufferObject);
    glVertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName("vertexBinormal_modelSpace"),
                          holder->getVertexed()->getDataOffset(),
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0);
    
}

GLint GLTexturedComponent::getOffsetInTextureCoords() const {
    return offsetInTextureCoords;
}

GLenum GLTexturedComponent::getTextureCoordsType() const {
    return textureCoordsType;
}

GLboolean GLTexturedComponent::getTextureCoordsNormilized() const {
    return textureCoordsNormilized;
}

GLsizei GLTexturedComponent::getTextureCoordsStride() const {
    return textureCoordsStride;
}

GLuint GLTexturedComponent::getUVBufferObject() const {
    return UVBufferObject;
}

GLuint GLTexturedComponent::getNormalBufferObject() const {
    return normalBufferObject;
}

GLuint GLTexturedComponent::getTextureId() const {
    return textureId;
}

GLsizei GLTexturedComponent::getNormalDataOffset() const {
    return holder->getVertexed()->getDataOffset();
}

GLuint GLTexturedComponent::getTangentBufferObject() const {
    return tangentBufferObject;
}

GLuint GLTexturedComponent::getBinormalBufferObject() const {
    return binormalBufferObject;
}

GLTexturedComponent::~GLTexturedComponent() {
}
