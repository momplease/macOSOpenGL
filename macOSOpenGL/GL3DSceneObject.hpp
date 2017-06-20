//
//  GL3DSceneObject.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GL3DSceneObject_hpp
#define GL3DSceneObject_hpp

#include <vector>
#include <memory>
#include "GL3DObject.hpp"
#include "ISceneBehaviour.hpp"

class GLScene;
class Mesh;

class GL3DSceneObject final: public GL3DObject, public ISceneBehaviour {
public:
    GL3DSceneObject(Mesh*, macOSOpenGL::Transform*, const GLScene *);
    GL3DSceneObject(const std::vector<Mesh*>&, macOSOpenGL::Transform*, const GLScene *);
    
    virtual ~GL3DSceneObject();
    
    GL3DSceneObject(const GL3DSceneObject&) = delete;
    GL3DSceneObject& operator=(const GL3DSceneObject&) = delete;
    GL3DSceneObject(GL3DSceneObject&&) = delete;
    GL3DSceneObject& operator=(GL3DSceneObject&&) = delete;
    
    virtual const GLScene* getScene();
    
    void addMesh(Mesh*);
    Mesh* getMeshAt(int);
    std::vector<Mesh*> getMeshes();
    int getMeshesCount() const;
    
    // ISceneBehaviour
    virtual void prepareOpenGL() override;
    virtual void updateAnimations(double deltaTime) override;
    
protected:
    const GLScene *scene;
    
    std::vector<std::unique_ptr<Mesh>> meshes;
    
    //GLDrawableComponent* drawable;
    //GLVertexedComponent* vertexed;
    //
    
};

#endif /* GL3DSceneObject_hpp */
