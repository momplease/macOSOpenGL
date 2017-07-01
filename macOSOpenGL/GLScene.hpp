//
//  GLScene.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/31/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLScene_hpp
#define GLScene_hpp

#include <memory>
#include <vector>
#include "GLObject.hpp"
#include "CameraController.hpp"
#include "IInputEventHandler.hpp"
#include "ISceneBehaviour.hpp"

class FbxLoader;
class IRenderingEngine;
class GL3DSceneObject;
class GLLight;
class GLShaderProgram;
class GLMesh;

struct GLSceneDelegate {
    virtual CGRect viewRect() const = 0;
    virtual GLShaderProgram* getShaderProgram() const = 0;
};


class GLScene : public GLObject, public CameraControllerDelegate, public IInputEventHandler, public ISceneBehaviour {
public:
    GLScene();
    virtual ~GLScene();
    
    void updateAnimations(double deltaTime) override;
    
    std::shared_ptr<GLCamera> getCamera() const;
    
    void addSceneObject(std::shared_ptr<GL3DSceneObject>);
    const std::vector<std::shared_ptr<GL3DSceneObject>>& getObjects() const;
    
    void addLight(std::shared_ptr<GLLight>);
    const std::vector<std::shared_ptr<GLLight>>& getLights() const;
    
    // IInputEventHandler
    void keyUp(NSEvent *event) override;
    void keyDown(NSEvent *event) override;
    
    // CameraControllerDelegate
    CGRect viewRect() override;
    
    GLSceneDelegate *delegate;
    
    virtual void prepareOpenGL() override;
    virtual void clearOpenGL() override {}
    
    virtual void bind(GLShaderProgram *) override {}
    virtual void unbind(GLShaderProgram *) override {}
    
    //void setRenderOrderUsingComparator(std::vector<glm::vec3>&,
    //                                   std::vector<glm::vec2>&,
    //                                   function<bool(macOSOpenGL::SortPolygon, macOSOpenGL::SortPolygon)>);
    
    static glm::vec3 getCenter() {
        return glm::vec3(0.0, 0.0, 0.0);
    }
    
private:
    
    void updateObject(ISceneBehaviour *object, double deltaTime);
    void loadSceneObjectsWith(std::vector<GLMesh *> meshes);
    
    // Camera
    std::unique_ptr<CameraController> cameraController;
    
    // Handle events in another objects
    std::vector<IInputEventHandler*> handlers;
    std::vector<std::shared_ptr<GLLight>> lights;
    std::vector<std::shared_ptr<GL3DSceneObject>> objects;

    // Loaders
    std::unique_ptr<FbxLoader> fbxLoader;
};



#endif /* GLScene_hpp */
