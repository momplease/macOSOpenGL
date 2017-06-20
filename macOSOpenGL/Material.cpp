//
//  Material.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "Material.hpp"
#include "GLShader.hpp"

Material::Material(GLShader *shader) : GLObject() {
    this->shader.reset(shader);
}

Material::~Material() {

}

void Material::prepareOpenGL() {
    //shader->prepareOpenGL();
}
