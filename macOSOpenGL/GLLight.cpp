//
//  GLLight.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLLight.hpp"
#include "Transform.hpp"
#include <chrono>

GLLight::GLLight(Transform *transform) : GL3DObject(transform), lightPower(50.0f) {
}

GLLight::~GLLight() {
}

float GLLight::getPower() const {
    return lightPower;
}

void GLLight::setPower(float power) {
    lightPower = power;
}

void GLLight::updateAnimations(double deltaTime) {
    timePast += deltaTime;
    
    const float radius = 60.f;
    const float height = 0.0f;
    
    glm::vec3 translation;
    translation.x = radius * sin(timePast);
    translation.y = height;
    translation.z = radius * cos(timePast);
    
    transform->setPosition(translation);
    
}
