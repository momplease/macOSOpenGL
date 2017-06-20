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
#include "Polygon.hpp"

@class OpenGLTextureLoader;
class FbxLoader;
class IRenderingEngine;
class GL3DSceneObject;
class GLLight;
class GLShaderProgram;

namespace macOSOpenGL {
    struct SortPolygon {
        SortPolygon(const Polygon& p, const int& d) : polygon(p), distanceToCamera(d) {}
        
        Polygon polygon;
        int distanceToCamera = INT_MAX;
    };
    
    inline bool operator<(const SortPolygon& p1, const SortPolygon& p2) {
        return p1.distanceToCamera < p2.distanceToCamera;
    }
}

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
    
    //void setRenderOrderUsingComparator(std::vector<glm::vec3>&,
    //                                   std::vector<glm::vec2>&,
    //                                   function<bool(macOSOpenGL::SortPolygon, macOSOpenGL::SortPolygon)>);
    
    static glm::vec3 getCenter() {
        return glm::vec3(0.0, 0.0, 0.0);
    }
    
private:
    
    std::unique_ptr<CameraController> cameraController;
    
    void updateObject(ISceneBehaviour *object, double deltaTime);
    
    // Handle events in another objects
    std::vector<IInputEventHandler*> handlers;
    std::vector<std::shared_ptr<GLLight>> lights;
    std::vector<std::shared_ptr<GL3DSceneObject>> objects;

    std::unique_ptr<FbxLoader> fbxLoader;
    OpenGLTextureLoader *textureLoader;
};



#endif /* GLScene_hpp */
