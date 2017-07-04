//
//  OpenGLScene.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/31/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OpenGLTextureLoader.hpp"

#include "GLScene.hpp"
#include "FbxLoader.hpp"
#include "GL3DSceneObject.hpp"
#include "Transform.hpp"
#include "GLPointLight.hpp"

// Concept
#include "GLMeshGenerator.hpp"
#include "GLMaterial.hpp"
#include "GLTexture.hpp"
#include "GLMesh.hpp"


GLScene::GLScene() {
    GLCamera *newCamera = new GLCamera();
    cameraController.reset(new CameraController(newCamera));
    cameraController->delegate = this;
    newCamera->delegate = cameraController.get();
    
    this->handlers.push_back(cameraController.get());
    
}

GLScene::~GLScene() {

}

void GLScene::prepareOpenGL() {
    NSBundle *mainBundle = [NSBundle mainBundle];
    if (not fbxLoader)
        fbxLoader.reset(new FbxLoader());
    fbxLoader->loadWithFilename([[mainBundle pathForResource:@"ttcogl/cat" ofType:@"FBX"] UTF8String]);
    
    
    
    GLMeshGenerator *generator = new GLMeshGenerator(fbxLoader.get());
        
    std::vector<GLMesh *> generatedMeshes = generator->getMeshes();
        
    loadSceneObjectsWith(std::move(generatedMeshes));
        
    delete generator;

    
    std::shared_ptr<GLLight> light(new GLPointLight(new Transform(glm::vec3(0.0, 0.0, 60.0),
                                                                  glm::quat(0.0, 0.0, 0.0, 1.0f),
                                                                  glm::vec3(1.0f, 1.0f, 1.0f))));
    
    addLight(std::move(light));
    
    
    for(int i = 0; i < objects.size(); ++i) {
        objects[i]->prepareOpenGL();
    }
    
}

void GLScene::updateAnimations(double deltaTime) {
    this->updateObject(cameraController.get(), deltaTime);
    for (auto &object: objects) {
        updateObject(object.get(), deltaTime);
    }
    
    for (auto &light: lights) {
        updateObject(light.get(), deltaTime);
    }
}


void GLScene::updateObject(ISceneBehaviour *object, double deltaTime) {
    object->updateAnimations(deltaTime);
}


CGRect GLScene::viewRect() {
    return delegate->viewRect();
}


std::shared_ptr<GLCamera> GLScene::getCamera() const {
    return cameraController->getTargetCamera();
}


const std::vector<std::shared_ptr<GL3DSceneObject>>& GLScene::getObjects() const {
    return objects;
}

const std::vector<std::shared_ptr<GLLight>>& GLScene::getLights() const {
    return lights;
}

void GLScene::addSceneObject(std::shared_ptr<GL3DSceneObject> object) {
    objects.push_back(object);
}

void GLScene::addLight(std::shared_ptr<GLLight> light) {
    lights.push_back(light);
}

void GLScene::loadSceneObjectsWith(std::vector<GLMesh *> meshes) {
    std::shared_ptr<GL3DSceneObject> sceneObject(new GL3DSceneObject(
                                                    meshes.at(0),
                                                    Transform::defaultTransform(),
                                                    this
                                                 ));
    sceneObject->addMesh(meshes.at(1));
    
    // Temporary
    //delete meshes.at(0);
    
    addSceneObject(std::move(sceneObject));
    
}

#pragma mark - IInputEventHandler

void GLScene::keyUp(NSEvent *event) {
    for(auto handler : handlers) {
        handler->keyUp(event);
    }
}

void GLScene::keyDown(NSEvent *event) {
    for(auto handler : handlers) {
        handler->keyDown(event);
    }
}
