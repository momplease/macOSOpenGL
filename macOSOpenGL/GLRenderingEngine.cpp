//
//  OpenGLRenderingEngine.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/1/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLRenderingEngine.hpp"
#include "OpenGLViewController.h"

#include "GL3DSceneObject.hpp"
#include "IGLDrawable.h"

#include "GLContext.hpp"
#include "GLPointLight.hpp"

#include "GLShaderProgram.hpp"
#include "GLVertexShader.hpp"
#include "GLFragmentShader.hpp"

#include "Mesh.hpp"
#include "Transform.hpp"

#define thisDelegate ((__bridge  OpenGLViewController *)delegate)

GLRenderingEngine::GLRenderingEngine() {
}

GLRenderingEngine::~GLRenderingEngine() {
    clearOpenGL();
}


CGRect GLRenderingEngine::viewRect() const {
    return [thisDelegate viewRect];
}

GLShaderProgram* GLRenderingEngine::getShaderProgram() const {
    return shaderProgram.get();
}


/*****************************************************/
/*                  Rendering Engine                 */
/*****************************************************/
void GLRenderingEngine::initialize(GLScene *scene) {
    this->scene = scene;
    // May be additional set up here
    this->prepareOpenGL();
}

void GLRenderingEngine::prepareOpenGL() {
    GLint swapInt = 1;
    [[thisDelegate openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    [[thisDelegate openGLContext] makeCurrentContext];
    
    NSRect backingBounds = [thisDelegate viewRect];
    GLsizei backingPixelWidth = (GLsizei)(backingBounds.size.width);
    GLsizei backingPixelHeight = (GLsizei)(backingBounds.size.height);
    glViewport(0, 0, backingPixelWidth, backingPixelHeight);
    
    glEnable(GL_ALPHA_TEST);
    
    
    glEnable(GL_BLEND);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    
    glClearColor(0, 0, 0.4f, 0);
    
    
    // Generating VAO (Apple specific)
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    
    
    this->scene->prepareOpenGL();
    this->prepareShaders();

    for(auto& object: scene->getObjects()) {
        object->prepareOpenGL();
    }
    
}

// Main render
void GLRenderingEngine::render(double deltaTime) {
    scene->updateAnimations(deltaTime);
    // ... main rendering
    [[thisDelegate openGLContext] makeCurrentContext];
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glUseProgram(shaderProgramId);
    
    shaderProgram->use();
    
    
    auto objectsToRender = scene->getObjects();
    for (auto& objectToRender: objectsToRender) {
        renderObject(objectToRender.get());
    }
    
    //glDisableVertexAttribArray(attrVertexInShaderLocation);
    //glDisableVertexAttribArray(attrUVlocation);
    //glDisableVertexAttribArray(attrNormalInShaderLocation);
    
   
    //glSwapAPPLE();
    dispatch_async(dispatch_get_main_queue(), ^(void){
        glFlush();
    });
    
}

void GLRenderingEngine::renderObject(GL3DSceneObject *objectToRender) {
    auto meshesToRender = objectToRender->getMeshes();
    std::for_each(meshesToRender.begin(),
                  meshesToRender.end(),
                  [this, objectToRender](Mesh* mesh){
                      this->render(mesh, objectToRender->getTransform()->asMatrix());
                  });
}

void GLRenderingEngine::render(IGLRenderable* renderable, glm::mat4 parentsModel) {
    // Bind
    renderable->bind(shaderProgram.get());
    
    // ??? Material::prepare(OpenGL)
    
    glm::mat4 MVP = scene->getCamera()->viewProjection() * renderable->getModel() * parentsModel;
    shaderProgram->getVertexShader()->setUniformValueByName("uniMVP", &MVP);
    
    glm::mat4 viewMatrix = scene->getCamera()->view();
    shaderProgram->getVertexShader()->setUniformValueByName("uniViewMatrix", &viewMatrix);
    
    glm::mat4 modelMatrix = renderable->getModel() * parentsModel;
    shaderProgram->getVertexShader()->setUniformValueByName("uniModelMatrix", &modelMatrix);
    
    // Getting first light
    glm::vec3 lightPosition = scene->getLights()[0]->getTransform()->getPosition();
    shaderProgram->getVertexShader()->setUniformValueByName("uniLightPosition_worldSpace", &lightPosition);
    
    glm::mat4 modelView = viewMatrix * modelMatrix;
    glm::mat3 modelView3x3 = glm::mat3(
        modelView[0][0], modelView[0][1], modelView[0][2],
        modelView[1][0], modelView[1][1], modelView[1][2],
        modelView[2][0], modelView[2][1], modelView[2][2]
    );
    shaderProgram->getVertexShader()->setUniformValueByName("uniModelView3x3", &modelView3x3);
    
    draw(renderable->getDrawable());
    
    //renderable->unbind();
    
}


void GLRenderingEngine::draw(IGLDrawable* drawable) {
    if (drawable->isUsingDepthBuffer())
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    
    if (drawable->isUsingIndices()) {
        GLContext::mainContext()->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable->getIndicesBufferObject());
        GLContext::mainContext()->drawElements(drawable->getPrimitiveType(),
                                               drawable->getIndicesCount(),
                                               drawable->getIndicesType(),
                                               (void*)0);
    } else {
        GLContext::mainContext()->drawArrays(drawable->getPrimitiveType(),
                                             drawable->getOffset(),
                                             drawable->getVerticesCount());
    }
}


void GLRenderingEngine::clearOpenGL() {
    glDeleteVertexArraysAPPLE(1, &vao);
    //glDeleteProgram(shaderProgramId);
}


void GLRenderingEngine::prepareShaders() {
    NSBundle *mainBundle = [NSBundle mainBundle];
    
    shaderProgram.reset(new GLShaderProgram());
    
    GLVertexShader *vertexShader = new GLVertexShader([[mainBundle pathForResource:@"vertexshader"
                                                                            ofType:@"glsl"] UTF8String],
                                                      shaderProgram.get());
    GLFragmentShader *fragmentShader = new GLFragmentShader([[mainBundle pathForResource:@"fragmentshader"
                                                                                  ofType:@"glsl"] UTF8String],
                                                            shaderProgram.get());
    
    shaderProgram->addVertexShader(vertexShader);
    shaderProgram->addFragmentShader(fragmentShader);
    
    shaderProgram->prepareOpenGL();
    
    // Vertex
    shaderProgram->getVertexShader()->addAttribute("vertexPosition_modelSpace", "vec3");
    shaderProgram->getVertexShader()->addAttribute("UV", "vec2");
    shaderProgram->getVertexShader()->addAttribute("vertexNormal_modelSpace", "vec3");
    shaderProgram->getVertexShader()->addAttribute("vertexTangent_modelSpace", "vec3");
    shaderProgram->getVertexShader()->addAttribute("vertexBinormal_modelSpace", "vec3");
    
    shaderProgram->getVertexShader()->addUniform("uniMVP", "mat4");
    shaderProgram->getVertexShader()->addUniform("uniViewMatrix", "mat4");
    shaderProgram->getVertexShader()->addUniform("uniModelMatrix", "mat4");
    shaderProgram->getVertexShader()->addUniform("uniLightPosition_worldSpace", "vec3");
    shaderProgram->getVertexShader()->addUniform("uniModelView3x3", "mat3");
    // Fragment
    shaderProgram->getFragmentShader()->addUniform("uniDiffuseTexture", "sampler2D");
    shaderProgram->getFragmentShader()->addUniform("uniNormalMap", "sampler2D");
    shaderProgram->getFragmentShader()->addUniform("uniModelView", "mat4");
    
}
