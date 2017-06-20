//
//  GLObject.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLObject_hpp
#define GLObject_hpp


// May become a super class for 2D / 3D objects

class GLObject {
public:
    GLObject() = default;
    virtual ~GLObject() = default;
    
    GLObject(const GLObject&) = delete;
    GLObject& operator=(const GLObject&) = delete;
    GLObject(GLObject&&) = delete;
    GLObject& operator=(GLObject&&) = delete;
    
    virtual void prepareOpenGL() = 0;
    //virtual void clearOpenGL() = 0; // will do to clear OpenGL resources
    
protected:
};

#endif /* GLObject_hpp */
