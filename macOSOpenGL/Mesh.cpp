//
//  Mesh.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "Mesh.hpp"
#include "Transform.hpp"
#include "glm/glm.hpp"

using namespace macOSOpenGL;

Mesh::Mesh(const std::vector<glm::vec3>& vertices,
           const std::vector<glm::vec2>& uvs,
           const std::vector<glm::vec3>& normals,
           const std::vector<unsigned int>& indices,
           Transform* transform) : GL3DObject(transform) {
    this->vertices = vertices;
    this->uvs = uvs;
    this->normals = normals;
    this->indices = indices;
}

Mesh::Mesh(const std::vector<glm::vec3>& vertices,
           const std::vector<glm::vec2>& uvs,
           const std::vector<glm::vec3>& normals,
           const std::vector<unsigned int>& indices) : Mesh(vertices,
                                                            uvs,
                                                            normals,
                                                            indices,
                                                            Transform::defaultTransform3D()) {
}

const void Mesh::addTangents(const std::vector<glm::vec3>& tangents) {
    this->tangents = tangents;
}

const void Mesh::addBitangents(const std::vector<glm::vec3>& bitangents) {
    this->bitangents = bitangents;
}

const std::vector<glm::vec3>& Mesh::getVertices() const {
    return vertices;
}

const std::vector<glm::vec2>& Mesh::getUVs() const {
    return uvs;
}

const std::vector<glm::vec3>& Mesh::getNormals() const {
    return normals;
}

const std::vector<unsigned int>& Mesh::getIndices() const {
    return indices;
}

const std::vector<glm::vec3>& Mesh::getTangents() const {
    return tangents;
}

const std::vector<glm::vec3>& Mesh::getBitangents() const {
    return bitangents;
}

glm::mat4 Mesh::getModel() const {
    return transform->asMatrix();
}

Mesh::~Mesh() {
}

