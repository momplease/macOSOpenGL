//
//  GLPointLight.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLPointLight_hpp
#define GLPointLight_hpp

#include "GLLight.hpp"

class GLPointLight : public GLLight {
public:
    GLPointLight(macOSOpenGL::Transform*);
    virtual ~GLPointLight();
    
    GLPointLight(const GLPointLight&) = delete;
    GLPointLight& operator=(const GLPointLight&) = delete;
    GLPointLight(GLPointLight&&) = delete;
    GLPointLight& operator=(GLPointLight&&) = delete;
    
    virtual void prepareOpenGL() override;
    
private:
    
};

#endif /* GLPointLight_hpp */
