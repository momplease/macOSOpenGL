//
//  OpenGLScene.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/31/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "GLScene.hpp"
#include "FbxLoader.hpp"
#include "GL3DSceneObject.hpp"
#include "OpenGLShaderLoader.hpp"
#include "OpenGLTextureLoader.hpp"
#include "Polygon.hpp"
#include "Transform.hpp"
#include "GL3DRenderMesh.hpp"
#include "GLPointLight.hpp"

using namespace macOSOpenGL;

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
    
    textureLoader = [[OpenGLTextureLoader alloc] init];
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    { // Create cat
        
        // Configure object
        RenderableConfig furConfig;
        
        // Vertices config
        furConfig.vertexedPart.verticesType = GL_FLOAT;
        furConfig.vertexedPart.dataOffset = 3;
        furConfig.vertexedPart.verticesNormilized = GL_FALSE;
        furConfig.vertexedPart.stride = 0;
        furConfig.vertexedPart.useIndices = false;
        
        // Texture config
        furConfig.texturedPart.textureId = [textureLoader loadTexture2D:[mainBundle pathForResource:@"ttcogl/fur_diff"
                                                                                             ofType:@"png"]];
        furConfig.texturedPart.offsetInTextureCoords = 2;
        furConfig.texturedPart.textureCoordsType = GL_FLOAT;
        furConfig.texturedPart.textureCoordsNormilized = GL_FALSE;
        furConfig.texturedPart.textureCoordsStride = 0;
        
        // Draw config
        furConfig.drawablePart.primitiveType = GL_TRIANGLES;
        furConfig.drawablePart.offset = 0;
        furConfig.drawablePart.indicesType = GL_UNSIGNED_INT; //???
        furConfig.drawablePart.useIndices = false;
        furConfig.drawablePart.useDepthBuffer = true;
        
        
        Transform *furMeshTransform = new Transform(GLScene::getCenter(),
                                                    glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)),
                                                    glm::vec3(1.0f));
        
        Mesh *furMesh = new GL3DRenderMesh(fbxLoader->secondMesh(),
                                           fbxLoader->UVs().at(fbxLoader->UVKeys().at(1)),
                                           fbxLoader->secondNormals(),
                                           std::vector<unsigned int>(),
                                           furMeshTransform,
                                           furConfig);
        
        furMesh->addTangents(fbxLoader->secondTangents());
        furMesh->addBitangents(fbxLoader->secondBitangents());
        
        
        
        RenderableConfig bodyConfig;
        // Vertices config
        bodyConfig.vertexedPart.verticesType = GL_FLOAT;
        bodyConfig.vertexedPart.dataOffset = 3;
        bodyConfig.vertexedPart.verticesNormilized = GL_FALSE;
        bodyConfig.vertexedPart.stride = 0;
        bodyConfig.vertexedPart.useIndices = false;
        
        // Texture config
        bodyConfig.texturedPart.textureId = [textureLoader loadTexture2D:[mainBundle pathForResource:@"ttcogl/body_diff"
                                                                                              ofType:@"png"]];
        bodyConfig.texturedPart.normalMapId = [textureLoader loadTexture2D:[mainBundle pathForResource:@"ttcogl/body_norm"
                                                                                                ofType:@"png"]];
        bodyConfig.texturedPart.offsetInTextureCoords = 2;
        bodyConfig.texturedPart.textureCoordsType = GL_FLOAT;
        bodyConfig.texturedPart.textureCoordsNormilized = GL_FALSE;
        bodyConfig.texturedPart.textureCoordsStride = 0;
        
        // Draw config
        bodyConfig.drawablePart.primitiveType = GL_TRIANGLES;
        bodyConfig.drawablePart.offset = 0;
        bodyConfig.drawablePart.indicesType = GL_UNSIGNED_INT; //???
        bodyConfig.drawablePart.useIndices = false;
        bodyConfig.drawablePart.useDepthBuffer = true;
        
        
        
        Transform* bodyMeshTransform = new Transform(GLScene::getCenter(),
                                                     //glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
                                                     glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)),
                                                     glm::vec3(1.0f));
        
        Mesh *bodyMesh = new GL3DRenderMesh(fbxLoader->indexedVertices(),
                                            fbxLoader->UVs().at(fbxLoader->UVKeys().at(0)),
                                            fbxLoader->normals(),
                                            std::vector<unsigned int>(),
                                            bodyMeshTransform,
                                            bodyConfig);
        
        bodyMesh->addTangents(fbxLoader->getTangents());
        bodyMesh->addBitangents(fbxLoader->getBitangents());
        
        // Cat
        std::shared_ptr<GL3DSceneObject> cat(new GL3DSceneObject(bodyMesh,
                                                                 Transform::defaultTransform3D(),
                                                                 this));
        //cat->addMesh(furMesh);
        
        addSceneObject(std::move(cat)); // Move into collection
        
        /*std::vector<glm::vec3> verts;
        verts.push_back(glm::vec3(-10.0,  10.0, 0.0));
        verts.push_back(glm::vec3(-10.0, -10.0, 0.0));
        verts.push_back(glm::vec3( 10.0, -10.0, 0.0));
        verts.push_back(glm::vec3(-10.0,  10.0, 0.0));
        verts.push_back(glm::vec3( 10.0,  10.0, 0.0));
        verts.push_back(glm::vec3( 10.0, -10.0, 0.0));
        
        std::vector<glm::vec2> uvss;
        uvss.push_back(glm::vec2(0.0, 1.0));
        uvss.push_back(glm::vec2(0.0, 0.0));
        uvss.push_back(glm::vec2(1.0, 0.0));
        uvss.push_back(glm::vec2(0.0, 1.0));
        uvss.push_back(glm::vec2(1.0, 1.0));
        uvss.push_back(glm::vec2(1.0, 0.0));
        
        std::vector<glm::vec3> nms;
        nms.push_back(glm::vec3(0.0, 0.0, 1.0));
        nms.push_back(glm::vec3(0.0, 0.0, 1.0));
        nms.push_back(glm::vec3(0.0, 0.0, 1.0));
        nms.push_back(glm::vec3(0.0, 0.0, 1.0));
        nms.push_back(glm::vec3(0.0, 0.0, 1.0));
        nms.push_back(glm::vec3(0.0, 0.0, 1.0));
        
        std::vector<glm::vec3> tangs;
        std::vector<glm::vec3> bitangs;
        
        Mesh *mesh = new GL3DRenderMesh(verts,
                                        uvss,
                                        nms,
                                        std::vector<unsigned int>(),
                                        Transform::defaultTransform3D(),
                                        bodyConfig);
        
        fbxLoader->computeTangentBasis(verts,
                                       uvss,
                                       nms,
                                       tangs,
                                       bitangs);
        
        mesh->addTangents(tangs);
        mesh->addBitangents(bitangs);
        
        
        std::shared_ptr<GL3DSceneObject> b(new GL3DSceneObject(mesh,
                                                               Transform::defaultTransform3D(),
                                                               this));
        
        addSceneObject(std::move(b));*/
        
    } // Create cat
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    std::shared_ptr<GLLight> light(new GLPointLight(new Transform(glm::vec3(0.0, 0.0, 60.0),
                                                                  glm::quat(0.0, 0.0, 0.0, 1.0f),
                                                                  glm::vec3(1.0f, 1.0f, 1.0f))));
    
    addLight(std::move(light));
    
}

void GLScene::updateAnimations(double deltaTime) {
    this->updateObject(cameraController.get(), deltaTime);
    for (auto object: objects) {
        updateObject(object.get(), deltaTime);
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

/*const vector<IRenderable const *> GLScene::renderingObjects() const {
    vector<IRenderable const *> retValue;
    for(auto obj: _objects) {
        retValue.push_back(obj.get());
    }
    return retValue;
}*/


/*void GLScene::setRenderOrderUsingComparator(vector<glm::vec3>& vertices,
                                            vector<glm::vec2>& UVs,
                                            function<bool(macOSOpenGL::SortPolygon, macOSOpenGL::SortPolygon)> comparator) {
    vector<macOSOpenGL::SortPolygon> polygonsWithDistanceToCamera;
    
    glm::vec3 cameraPosition = _cameraController->targetCamera()->position();

    for (int polygonIndex = 0; polygonIndex < vertices.size(); polygonIndex += 3) {
        glm::vec3 v1 = vertices.at(polygonIndex);
        glm::vec3 v2 = vertices.at(polygonIndex + 1);
        glm::vec3 v3 = vertices.at(polygonIndex + 2);
        macOSOpenGL::Polygon polygon(v1, v2, v3);
        
        int dist = distanceBetween(cameraPosition, polygon.center());
        
        polygonsWithDistanceToCamera.push_back({polygon, dist});
    }
    
    sort(polygonsWithDistanceToCamera.begin(), polygonsWithDistanceToCamera.end(), comparator);
    
    int verticesSize = static_cast<int>(vertices.size());
    vertices.clear();
    vertices.resize(verticesSize);

    int polygonsCounter = 0;
    for (int i = 0; i < verticesSize; i += 3) {
        auto polygon = polygonsWithDistanceToCamera.at(polygonsCounter++).polygon;
        vertices.at(i) = polygon.vertex1;
        vertices.at(i + 1) = polygon.vertex2;
        vertices.at(i + 2) = polygon.vertex3;
    }
    
}*/


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
