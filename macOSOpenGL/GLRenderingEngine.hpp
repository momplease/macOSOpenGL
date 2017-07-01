//
//  GLRenderingEngine.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/1/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLRenderingEngine_hpp
#define GLRenderingEngine_hpp

#include <memory>
#include "GLScene.hpp"
#include "IRenderingEngine.hpp"
#include "glm/fwd.hpp"
#include "RenderingEngineDelegate.mm"

class GLContext;
class GLShaderProgram;

class GLRenderingEngine : public IRenderingEngine, public GLSceneDelegate {
public:
    GLRenderingEngine();
    virtual ~GLRenderingEngine();
    
    GLRenderingEngine(const GLRenderingEngine &) = delete;
    GLRenderingEngine& operator=(const GLRenderingEngine&) = delete;
    GLRenderingEngine(GLRenderingEngine &&) = delete;
    GLRenderingEngine& operator=(GLRenderingEngine&&) = delete;
public:
    // IRenderingEngine
    void initialize(GLScene *scene) override;
    void render(double deltaTime) override;
    
    // Own delegate
    struct RenderingEngineDelegate *delegate;
    
    // GLSceneDelegate override
    CGRect viewRect() const override;
    GLShaderProgram* getShaderProgram() const override;
    
protected:
    virtual void prepareOpenGL();
    virtual void clearOpenGL();
    virtual void prepareShaders();
    // Render
    virtual void renderObject(GL3DSceneObject *objectToRender);
    virtual void render(GLMesh *mesh, glm::mat4 parentsModel);
    // Native draw call
    virtual void draw(GLMesh *mesh);
    
private:
    GLScene *scene;
    
    std::unique_ptr<GLShaderProgram> shaderProgram;
    GLuint vao;
};

#endif /* GLRenderingEngine_hpp */
