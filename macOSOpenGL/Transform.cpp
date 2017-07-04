//
//  Transform.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "Transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Transform::~Transform(){
}

glm::vec3 Transform::getPosition() {
    return position;
}

glm::quat Transform::getRotation() {
    return rotation;
}

glm::vec3 Transform::getScale() {
    return scale;
}

glm::mat4 Transform::asMatrix() {
    glm::mat4 translateAsMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotateAsMatrix = glm::mat4_cast(rotation);
    glm::mat4 scaleAsMatrix = glm::scale(glm::mat4(1.0f), scale);
    
    return translateAsMatrix * rotateAsMatrix * scaleAsMatrix;
}

void Transform::setPosition(glm::vec3 position) {
    this->position = position;
}

void Transform::translateBy(glm::vec3 vector) {
    position += vector;
}
