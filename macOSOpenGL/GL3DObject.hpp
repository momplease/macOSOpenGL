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

namespace macOSOpenGL {
class Transform;
}

class GL3DObject : public GLObject {
public:
    GL3DObject(macOSOpenGL::Transform*);
    virtual ~GL3DObject();
    
    GL3DObject(const GL3DObject&) = delete;
    GL3DObject& operator=(const GL3DObject&) = delete;
    GL3DObject(GL3DObject&&) = delete;
    GL3DObject& operator=(GL3DObject&&) = delete;
    
    virtual macOSOpenGL::Transform* getTransform() const;
    virtual void setTransform(macOSOpenGL::Transform*);
    
protected:
    std::unique_ptr<macOSOpenGL::Transform> transform;
    
};

#endif /* GL3DObject_hpp */
