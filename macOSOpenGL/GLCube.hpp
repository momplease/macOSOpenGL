//
//  GLCube.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/25/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLCube_hpp
#define GLCube_hpp

#include <string>
#include <array>
#include <OpenGL/gl.h>
#include "glm/glm.hpp"

using std::array;
using std::string;
using glm::mat4;

class GLCube {
public:
    GLCube();
    virtual ~GLCube() = default;
    
    virtual array<GLfloat, 123> vertices();
    virtual array<GLfloat, 123> colors();

    virtual mat4 model();
    
private:
    
    array<GLfloat, 123> _verticesData;
    array<GLfloat, 123> _colorData;
    
    mat4 _model;
};

#endif /* GLCube_hpp */
