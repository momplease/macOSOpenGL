//
//  GLCamera.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/27/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLCamera.hpp"
#include "glm/gtc/matrix_transform.hpp"

GLCamera::GLCamera() :
_fov(45.0f),
_pitch(glm::radians(0.0f)),
_yaw(glm::radians(180.0f)),
_roll(glm::radians(0.0f)),
//_position(-37.809700, -4.458747, 17.164913),
_position(0.0f, 0.0f, 60.0f),
_direction(0.0f, 0.0f, -1.0f),
_right(-1.0f, 0.0f, 0.0f),
_up(glm::cross(_direction, _right))
{
}

glm::mat4 GLCamera::view() {
    
    _direction = glm::vec3(
                           glm::cos(_pitch) * glm::sin(_yaw),
                           glm::sin(_pitch),
                           glm::cos(_pitch) * glm::cos(_yaw)
    );
    
    
    _right = glm::vec3(
                       glm::sin(_yaw - 3.14f/2.0f),
                       0.0f,
                       glm::cos(_yaw - 3.14f/2.0f)
                    );
    
    
    _up = glm::cross(_direction, _right);
    
    
    return glm::lookAt(_position,
                       _position + _direction,
                       _up);
    
}

glm::mat4 GLCamera::projection() {
    CGRect viewRect = delegate->viewRect();
    return glm::perspective(glm::radians(_fov), float(viewRect.size.width / viewRect.size.height), 0.1f, 100.0f);
}

glm::mat4 GLCamera::viewProjection() {
    return projection() * view();
}


glm::vec3 GLCamera::direction() {
    return _direction;
}

glm::vec3 GLCamera::right() {
    return _right;
}

glm::vec3 GLCamera::position() {
    return _position;
}

void GLCamera::setPosition(glm::vec3 newPosition) {
    _position = newPosition;
}

void GLCamera::translate(glm::vec3 vector) {
    _position += vector;
}

glm::vec3 GLCamera::up() {
    return _up;
}


void GLCamera::updateYawBy(float offset) {
    _yaw += offset;
}

void GLCamera::updatePitchBy(float offset) {
    _pitch += offset;
}

void GLCamera::updateFov(float value) {
    _fov += value;
}
