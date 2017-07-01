//
//  GL3DObject.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GL3DObject_hpp
#define GL3DObject_hpp

#include <memory>
#include "GLObject.hpp"
#include "glm/fwd.hpp"

class Transform;

class GL3DObject : public GLObject {
public:
    GL3DObject(Transform*);
    virtual ~GL3DObject();
    
    GL3DObject(const GL3DObject&) = delete;
    GL3DObject& operator=(const GL3DObject&) = delete;
    GL3DObject(GL3DObject&&) = delete;
    GL3DObject& operator=(GL3DObject&&) = delete;
    
    virtual Transform* getTransform() const;
    virtual void setTransform(Transform*);
    
protected:
    std::unique_ptr<Transform> transform;
    
};

#endif /* GL3DObject_hpp */
