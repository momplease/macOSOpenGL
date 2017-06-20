//
//  GLComponent.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/2/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLComponent_hpp
#define GLComponent_hpp

#include "GLObject.hpp"
#include <OpenGL/gl.h>
#include <cmath>

class GLShaderProgram;

class GLComponent : public GLObject {
public:
    GLComponent();
    virtual ~GLComponent();
    
    GLComponent(const GLComponent&) = delete;
    GLComponent& operator=(const GLComponent&) = delete;
    GLComponent(GLComponent&&) = delete;
    GLComponent& operator=(GLComponent&&) = delete;
    
    // TODO
    virtual void bind(GLShaderProgram *) = 0;
    //virtual void unbind() = 0;
protected:
};

#endif /* GLComponent_hpp */
