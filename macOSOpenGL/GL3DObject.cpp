//
//  GL3DObject.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GL3DObject.hpp"
#include "Transform.hpp"
#include "glm/glm.hpp"

GL3DObject::GL3DObject(Transform *transform) : GLObject() {
    this->transform.reset(transform);
}

Transform* GL3DObject::getTransform() const {
    return transform.get();
}

void GL3DObject::setTransform(Transform *transform) {
    this->transform.reset(transform);
}

GL3DObject::~GL3DObject() {
}
