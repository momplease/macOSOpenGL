//
//  GL3DSceneObject.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GL3DSceneObject.hpp"
#include "GLScene.hpp"
#include "GLMesh.hpp"
#include "Transform.hpp"

GL3DSceneObject::GL3DSceneObject(GLMesh *mesh, Transform *transform, const GLScene *scene) : GL3DObject(transform), scene(scene) {
    addMesh(mesh);
}

GL3DSceneObject::GL3DSceneObject(const std::vector<GLMesh*>& meshes, Transform *transform, const GLScene *scene) : GL3DObject(transform), scene(scene) {
    for (auto&& mesh: meshes) {
        addMesh(mesh);
    }
}

GL3DSceneObject::~GL3DSceneObject() {
}

void GL3DSceneObject::prepareOpenGL() {
    auto rawPointersToMeshes = getMeshes();
    std::for_each(rawPointersToMeshes.begin(),
                  rawPointersToMeshes.end(),
                  [this](GLMesh *mesh){
                      mesh->prepareOpenGL();
                  });
}

void GL3DSceneObject::updateAnimations(double deltaTime) {
    auto rawPointersToMeshes = getMeshes();
    std::for_each(rawPointersToMeshes.begin(),
                  rawPointersToMeshes.end(),
                  [this, deltaTime](GLMesh *mesh){
                      mesh->updateAnimations(deltaTime);
                  });
}

const GLScene* GL3DSceneObject::getScene() {
    return scene;
}


void GL3DSceneObject::addMesh(GLMesh *mesh) {
    meshes.push_back(std::unique_ptr<GLMesh>(mesh));
}

GLMesh* GL3DSceneObject::getMeshAt(int index) {
    return meshes.at(index).get();
}

// RVO
std::vector<GLMesh*> GL3DSceneObject::getMeshes() {
    std::vector<GLMesh*> rawPointersCollection;
    auto copyPointersInto = [this](std::vector<GLMesh*>& collection) {
        for(auto& mesh: meshes) {
            collection.push_back(mesh.get());
        }
    };
    copyPointersInto(rawPointersCollection);
    return rawPointersCollection; // RVO
}

int GL3DSceneObject::getMeshesCount() const {
    return static_cast<int>(meshes.size());
}

void GL3DSceneObject::bind(GLShaderProgram *shaderProgram) {
    auto meshes = getMeshes();
    std::for_each(meshes.begin(),
                  meshes.end(),
                  [this, shaderProgram](GLMesh *mesh){
                      mesh->bind(shaderProgram);
                  });
}

//void GL3DSceneObject::updateAnimations(double deltaTime) {
    // update animations
    
    //vector<vec2> empty;
    //_scene->setRenderOrderUsingComparator(_mesh->_vertices,
    //                                      empty,
    //                                      [this](macOSOpenGL::SortPolygon p1, macOSOpenGL::SortPolygon p2) {
    //                                          return macOSOpenGL::operator<(p2, p1);
    //                                      });
//}
