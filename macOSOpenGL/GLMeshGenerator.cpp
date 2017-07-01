//
//  MeshGenerator.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/21/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLMeshGenerator.hpp"
#include "GLScene.hpp"
#include "FbxLoader.hpp"
#include "Helpers.hpp"

#include "GLMesh.hpp"
#include "GLGrid.hpp"
#include "GLMaterial.hpp"
#include "Transform.hpp"

#include "glm/gtc/matrix_transform.hpp"


GLMeshGenerator::GLMeshGenerator(FbxLoader *loader) : loader(loader) {
}

void GLMeshGenerator::load() {
    const auto& keys = loader->getKeys();
    std::for_each(keys.begin(),
                  keys.end(),
                  [this](const std::string& key){
                      auto meshes = createMeshUsing(key);
                      std::for_each(meshes.begin(),
                                    meshes.end(), [this](GLMesh *mesh){
                                        readyMeshes.push_back(mesh);
                                    });
                  });
}

// TODO: generate mesh with textures from fbx
std::vector<GLMesh *> GLMeshGenerator::createMeshUsing(const std::string& key) {
    const std::vector<std::string>& UVSetNames = loader->getUVSetNamesForKey(key);
    std::vector<GLMesh *> resultMeshes;
    resultMeshes.reserve(UVSetNames.size());
    
    // Caution: workaround below
    if (UVSetNames.empty())
        throw std::runtime_error("createMeshUsing: UVSetNames is empty, check UVs extraction");
    if (UVSetNames.size() > 2)
        throw std::runtime_error("createMeshUsing: UVSetNames > 2, can't split more than 2 meshes, check .fbx file");
    
    
    if (UVSetNames.size() > 1 && UVSetNames.size() < 3) {
        resultMeshes = splitMeshesWith(key, UVSetNames);
    } else {
        
                                                                //loader->getIndicesForKey(key));
        GLGrid *grid = new GLGrid(loader->getVerticesForKey(key), std::vector<unsigned int>());
        
        GLMaterial *material = new GLMaterial(loader->getUVsForKeyWithSetName(key, UVSetNames[0]),
                                              loader->getNormalsForKey(key),
                                              loader->getTangentsForKey(key));
        
        
        Transform *transform = new Transform(GLScene::getCenter(),
                                             //glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
                                             glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)),
                                             glm::vec3(1.0f));
        
        
        GLMesh *mesh = new GLMesh(grid, material, transform);
        resultMeshes.push_back(mesh);
    }
    
    return resultMeshes;
}

// RVO
std::vector<GLMesh *> GLMeshGenerator::splitMeshesWith(const std::string& key, const std::vector<std::string>& UVSetNames) {
    std::vector<GLMesh *> result;
    std::vector<glm::vec3> indexedVertices = this->index(loader->getVerticesForKey(key),
                                                         loader->getIndicesForKey(key));
    
    const std::vector<glm::vec3>& vertices = loader->getVerticesForKey(key);
    const std::vector<glm::vec3>& normals = loader->getNormalsForKey(key);
    const std::vector<glm::vec3>& tangents = loader->getTangentsForKey(key);
    
    std::vector<std::vector<glm::vec2>> UVsets;
    std::for_each(UVSetNames.begin(),
                  UVSetNames.end(),
                  [this, &UVsets, &key](std::string setName){
                      UVsets.push_back(loader->getUVsForKeyWithSetName(key, setName));
                  });
    
    // Caution: workaround below
    std::vector<glm::vec2> newUVset1;
    std::vector<glm::vec3> newVertices1;
    std::vector<glm::vec3> newNormals1;
    std::vector<glm::vec3> newTangents1;
    
    std::vector<glm::vec2> newUVset2;
    std::vector<glm::vec3> newVertices2;
    std::vector<glm::vec3> newNormals2;
    std::vector<glm::vec3> newTangents2;
    
    for (size_t i = 0; i < UVsets.at(0).size(); ++i) {
        if (!equalvec2(UVsets.at(0)[i], UVsets.at(1)[i])) {
            newUVset1.push_back(UVsets.at(0)[i]);
            newVertices1.push_back(vertices[i]);
            newNormals1.push_back(normals[i]);
            newTangents1.push_back(tangents[i]);
        } else {
            newUVset2.push_back(UVsets.at(1)[i]);
            newVertices2.push_back(vertices[i]);
            newNormals2.push_back(normals[i]);
            newTangents2.push_back(tangents[i]);
        }
    }
    
    GLGrid *grid1 = new GLGrid(loader->getVerticesForKey(key), std::vector<unsigned int>());
    
    GLMaterial *material1 = new GLMaterial(loader->getUVsForKeyWithSetName(key, UVSetNames[0]),
                                           loader->getNormalsForKey(key),
                                           loader->getTangentsForKey(key));
    
    
    Transform *transform1 = new Transform(GLScene::getCenter(),
                                         //glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
                                          glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)),
                                          glm::vec3(1.0f));
    
    
    GLMesh *mesh1 = new GLMesh(grid1, material1, transform1);
    
    GLGrid *grid2 = new GLGrid(loader->getVerticesForKey(key), std::vector<unsigned int>());
    
    GLMaterial *material2 = new GLMaterial(loader->getUVsForKeyWithSetName(key, UVSetNames[0]),
                                           loader->getNormalsForKey(key),
                                           loader->getTangentsForKey(key));
    
    
    Transform *transform2 = new Transform(GLScene::getCenter(),
                                         //glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
                                          glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)),
                                          glm::vec3(1.0f));
    
    
    GLMesh *mesh2 = new GLMesh(grid2, material2, transform2);
    
    result.push_back(mesh1);
    result.push_back(mesh2);
    
    return result; // RVO
}

const std::vector<GLMesh *>& GLMeshGenerator::getMeshes() {
    if (readyMeshes.empty()) {
        load();
    }
    
    return readyMeshes;
}

/*
 //auto& uvset1 = _UVs.at(_UVKeys.at(0));
 //auto& uvset2 = _UVs.at(_UVKeys.at(1));
 
 
 /*vector<vec2> newuvs;
 
 vector<vec3> verts;
 vector<vec2> uvs;
 vector<vec3> norms;
 vector<vec3> tangs;
 
 int counter = 0;
 for (size_t i = 0; i < uvset1.size(); ++i) {
 if (!equalvec2(uvset1[i], uvset2[i])) {
 uvset2[i] = vec2(-1, -1);
 uvs.push_back(uvset1[i]);
 verts.push_back(_indexedVertices.at(i));
 norms.push_back(_normals.at(i));
 tangs.push_back(_tangents.at(i));
 } else {
 _secondMeshVertices.push_back(_indexedVertices.at(i));
 _secondNormals.push_back(_normals[i]);
 _secondTangents.push_back(_tangents[i]);
 newuvs.push_back(uvset2[i]);
 }
 }
 
 
 
 _indexedVertices = std::move(verts);
 _normals = std::move(norms);
 _tangents = std::move(tangs);
 uvset1 = std::move(uvs);
 
 uvset2 = std::move(newuvs);*/
 
