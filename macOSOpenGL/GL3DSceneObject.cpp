//
//  GL3DSceneObject.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GL3DSceneObject.hpp"
#include "GLScene.hpp"
#include "GL3DRenderMesh.hpp"

using namespace macOSOpenGL;

GL3DSceneObject::GL3DSceneObject(Mesh *mesh, Transform *transform, const GLScene *scene) : GL3DObject(transform), scene(scene) {
    addMesh(mesh);
}

GL3DSceneObject::GL3DSceneObject(const std::vector<Mesh*>& meshes, Transform *transform, const GLScene *scene) : GL3DObject(transform), scene(scene) {
    for (auto&& mesh: meshes) {
        addMesh(mesh);
    }
}

void GL3DSceneObject::prepareOpenGL() {
    auto rawPointersToMeshes = getMeshes();
    std::for_each(rawPointersToMeshes.begin(),
                  rawPointersToMeshes.end(),
                  [this](Mesh *mesh){
                      mesh->prepareOpenGL();
                  });
}

void GL3DSceneObject::updateAnimations(double deltaTime) {
    auto rawPointersToMeshes = getMeshes();
    std::for_each(rawPointersToMeshes.begin(),
                  rawPointersToMeshes.end(),
                  [this, deltaTime](Mesh *mesh){
                      mesh->updateAnimations(deltaTime);
                  });
}

const GLScene* GL3DSceneObject::getScene() {
    return scene;
}


void GL3DSceneObject::addMesh(Mesh *mesh) {
    meshes.push_back(std::unique_ptr<Mesh>(mesh));
}

Mesh* GL3DSceneObject::getMeshAt(int index) {
    return meshes.at(index).get();
}

// RVO
std::vector<Mesh*> GL3DSceneObject::getMeshes() {
    std::vector<Mesh*> rawPointersCollection;
    auto copyPointersInto = [this](std::vector<Mesh*>& collection) {
        for(auto& mesh: meshes) {
            collection.push_back(mesh.get());
        }
    };
    copyPointersInto(rawPointersCollection);
    return rawPointersCollection; // RVO, no need in std::move()
}

int GL3DSceneObject::getMeshesCount() const {
    return static_cast<int>(meshes.size());
}

GL3DSceneObject::~GL3DSceneObject() {
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
