//
//  GLLight.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLLight.hpp"
#include "Transform.hpp"

GLLight::GLLight(macOSOpenGL::Transform *transform) : GL3DObject(transform), lightPower(50.0f) {
}

GLLight::~GLLight() {
}

float GLLight::getPower() const {
    return lightPower;
}

void GLLight::setPower(float power) {
    lightPower = power;
}
