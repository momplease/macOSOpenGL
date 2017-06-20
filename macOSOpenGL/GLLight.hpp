//
//  GLLight.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLLight_hpp
#define GLLight_hpp

#include "GL3DObject.hpp"

class GLLight : public GL3DObject {
public:
    GLLight(macOSOpenGL::Transform*);
    virtual ~GLLight();
    
    GLLight(const GLLight&) = delete;
    GLLight& operator=(const GLLight&) = delete;
    GLLight(GLLight&&) = delete;
    GLLight& operator=(GLLight&&) = delete;
    
    virtual float getPower() const;
    virtual void setPower(float);
    
protected:
    float lightPower;
};

#endif /* GLLight_hpp */
