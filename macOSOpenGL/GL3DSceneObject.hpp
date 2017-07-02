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
class GLMesh;
class GLShaderProgram;

class GL3DSceneObject : public GL3DObject, public ISceneBehaviour {
public:
    GL3DSceneObject(GLMesh *, Transform *, const GLScene *);
    GL3DSceneObject(const std::vector<GLMesh*>&, Transform *, const GLScene *);
    
    virtual ~GL3DSceneObject();
    
    GL3DSceneObject(const GL3DSceneObject&) = delete;
    GL3DSceneObject& operator=(const GL3DSceneObject&) = delete;
    GL3DSceneObject(GL3DSceneObject&&) = delete;
    GL3DSceneObject& operator=(GL3DSceneObject&&) = delete;
    
    virtual const GLScene* getScene();
    
    void addMesh(GLMesh*);
    GLMesh* getMeshAt(int);
    std::vector<GLMesh*> getMeshes();
    int getMeshesCount() const;
    
    // ISceneBehaviour
    virtual void prepareOpenGL() override;
    virtual void updateAnimations(double deltaTime) override;
    
    virtual void clearOpenGL() override {}
    
    virtual void bind(GLShaderProgram *) override {}
    virtual void unbind(GLShaderProgram *) override {}
    
protected:
    const GLScene *scene;
    
    std::vector<std::unique_ptr<GLMesh>> meshes;
    
    //GLDrawableComponent* drawable;
    //GLVertexedComponent* vertexed;
    //
    
};

#endif /* GL3DSceneObject_hpp */
