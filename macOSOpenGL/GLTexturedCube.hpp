//
//  GLTexturedCube.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/27/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLTexturedCube_hpp
#define GLTexturedCube_hpp

#include "GLCube.hpp"

class GLTexturedCube : public GLCube {
public:
    GLTexturedCube();
    virtual ~GLTexturedCube() = default;
    
    virtual array<GLfloat, 72> uv();
    
private:
    array<GLfloat, 72> _uv;
};

#endif /* GLTexturedCube_hpp */
