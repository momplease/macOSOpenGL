//
//  GLPointLight.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLPointLight.hpp"
#include "Transform.hpp"

GLPointLight::GLPointLight(Transform *transform) : GLLight(transform) {
}

GLPointLight::~GLPointLight() {
}

void GLPointLight::prepareOpenGL() {
}
