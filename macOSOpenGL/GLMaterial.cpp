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
#include "glm/glm.hpp"

GLMaterial::GLMaterial(const std::vector<glm::vec2>& UVs,
                       const std::vector<glm::vec3>& normals,
                       const std::vector<glm::vec3>& tangents,
                       GLMaterialConfig config) : GLObject(), UVs(UVs), normals(normals), tangents(tangents), config(config) {
}

GLMaterial::~GLMaterial() {
}

void GLMaterial::prepareOpenGL() {
    // TODO
}

void GLMaterial::clearOpenGL() {
    // TODO
}

void GLMaterial::bind(GLShaderProgram *shaderProgram) {
    // TODO
}

void GLMaterial::unbind(GLShaderProgram *shaderProgram) {
    // TODO
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
