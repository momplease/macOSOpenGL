//
//  GLMesh.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/28/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLMesh.hpp"
#include "GLGrid.hpp"
#include "GLMaterial.hpp"
#include "Transform.hpp"

GLMesh::GLMesh(GLGrid *grid,
               GLMaterial *material,
               Transform *transform,
               GLMeshConfig config) : GL3DObject(transform), grid(grid), material(material), config(config) {
}

GLMesh::~GLMesh() {
}

GLGrid* GLMesh::getGrid() const {
    return grid.get();
}

GLMaterial* GLMesh::getMaterial() const {
    return material.get();
}

void GLMesh::setConfig(GLMeshConfig config) {
    this->config = config;
}

GLMeshConfig* GLMesh::getConfig() {
    return &config;
}

void GLMesh::prepareOpenGL() {
    grid->prepareOpenGL();
    material->prepareOpenGL();
}

void GLMesh::clearOpenGL() {
    grid->clearOpenGL();
    material->clearOpenGL();
}

void GLMesh::bind(GLShaderProgram *shaderProgram) {
    grid->bind(shaderProgram);
    material->bind(shaderProgram);
}

void GLMesh::unbind(GLShaderProgram *shaderProgram) {
    grid->unbind(shaderProgram);
    material->unbind(shaderProgram);
}
