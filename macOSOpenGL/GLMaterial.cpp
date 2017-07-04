//
//  Material.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLMaterial.hpp"
#include "GLShaderProgram.hpp"
#include "GLTexture.hpp"
#include "GLVertexShader.hpp"
#include "GLFragmentShader.hpp"
#include "glm/glm.hpp"

namespace {
const std::string kDiffuseTextureInShaderName = "uniDiffuseTexture";
const std::string kNormalMapTextureInShaderName = "uniNormalMap";
const std::string kUVInShaderName = "UV";
const std::string kNormalAttributeInShaderName = "vertexNormal_modelSpace";
const std::string kTangentAttributeInShaderName = "vertexTangent_modelSpace";
}

GLMaterial::GLMaterial(const std::vector<glm::vec2>& UVs,
                       const std::vector<glm::vec3>& normals,
                       const std::vector<glm::vec3>& tangents,
                       GLMaterialConfig config) : GLObject(), UVs(UVs), normals(normals), tangents(tangents), config(config) {    
}

GLMaterial::~GLMaterial() {
}

void GLMaterial::prepareOpenGL() {
    glGenBuffers(1, &UVBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, UVBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec2) * UVs.size(),
                 UVs.data(),
                 GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * normals.size(),
                 normals.data(),
                 GL_STATIC_DRAW);
    
    glGenBuffers(1, &tangentBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * tangents.size(),
                 tangents.data(),
                 GL_STATIC_DRAW);
    
    for (int i = 0; i < textures.size(); ++i) {
        textures[i]->prepareOpenGL();
    }
    
}

void GLMaterial::clearOpenGL() {
    // TODO
}

void GLMaterial::bind(GLShaderProgram *shaderProgram) {
    
    bindTextures(shaderProgram);
    
    glBindBuffer(GL_ARRAY_BUFFER, UVBufferObject);
    glEnableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName(kUVInShaderName));
    glVertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName(kUVInShaderName),
                          config.offsetInUVCoords,
                          config.UVCoordsType,
                          config.UVCoordsNormilized,
                          config.UVCoordsStride,
                          (void*)0);
    
    glEnableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName(kNormalAttributeInShaderName));
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glVertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName(kNormalAttributeInShaderName),
                          config.normalsOffset,
                          config.UVCoordsType,
                          config.UVCoordsNormilized,
                          config.UVCoordsStride,
                          (void*)0);
    
    glEnableVertexAttribArray(shaderProgram->getVertexShader()->getAttributeLocationByName(kTangentAttributeInShaderName));
    glBindBuffer(GL_ARRAY_BUFFER, tangentBufferObject);
    glVertexAttribPointer(shaderProgram->getVertexShader()->getAttributeLocationByName(kTangentAttributeInShaderName),
                          config.normalsOffset,
                          config.UVCoordsType,
                          config.UVCoordsNormilized,
                          config.UVCoordsStride,
                          (void*)0);
    
}

void GLMaterial::unbind(GLShaderProgram *shaderProgram) {
    // TODO
}

void GLMaterial::bindTextures(GLShaderProgram *shaderProgram) {
    if (textures.empty())
        return;
    
    GLint diffuseTextureLocation = shaderProgram->getFragmentShader()->getUniformLocationByName(kDiffuseTextureInShaderName);
    glUniform1i(diffuseTextureLocation, 0);
    
    GLint normalMapTextureLocation = shaderProgram->getFragmentShader()->getUniformLocationByName(kNormalMapTextureInShaderName);
    glUniform1i(normalMapTextureLocation, 1);
    
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]->getGLId());
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]->getGLId());
}

const std::vector<std::unique_ptr<GLTexture>>& GLMaterial::getTextures() const {
    return textures;
}

void GLMaterial::addTexture(GLTexture *texture) {
    textures.push_back(std::unique_ptr<GLTexture>(texture));
}

void GLMaterial::setConfig(GLMaterialConfig config) {
    this->config = config;
}

GLMaterialConfig* GLMaterial::getConfig() {
    return &config;
}
