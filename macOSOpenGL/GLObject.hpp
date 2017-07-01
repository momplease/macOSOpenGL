//
//  GLObject.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLObject_hpp
#define GLObject_hpp

class GLShaderProgram;

class GLObject {
public:
    GLObject() = default;
    virtual ~GLObject() = default;
    
    GLObject(const GLObject&) = delete;
    GLObject& operator=(const GLObject&) = delete;
    GLObject(GLObject&&) = delete;
    GLObject& operator=(GLObject&&) = delete;
    
    virtual void prepareOpenGL() = 0;
    virtual void clearOpenGL() = 0;
    
    virtual void bind(GLShaderProgram *) = 0;
    virtual void unbind(GLShaderProgram *) = 0;
    
protected:
};

#endif /* GLObject_hpp */
