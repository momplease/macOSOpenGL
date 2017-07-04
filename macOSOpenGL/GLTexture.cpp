//
//  GLImage.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/23/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "OpenGLTextureLoader.hpp"

#include "GLShaderProgram.hpp"
#include "GLTexture.hpp"

GLTexture::GLTexture(const std::string& path, GLTextureType aType) : path(path), aType(aType) {
}


void GLTexture::prepareOpenGL() {
    if (not ready) {
        OpenGLTextureLoader *loader = [[OpenGLTextureLoader alloc] init];
        glId = [loader loadTexture2D:[NSString stringWithUTF8String:path.c_str()]];
        ready = true;
    }
}

void GLTexture::clearOpenGL() {
    // TODO
}

void GLTexture::bind(GLShaderProgram *shaderProgram) {
    // TODO
}

void GLTexture::unbind(GLShaderProgram *shaderProgram) {
    // TODO
}

const std::string& GLTexture::getPathWhereLocated() const {
    return path;
}

GLuint GLTexture::getGLId() const {
    return glId;
}

GLTextureType GLTexture::getType() const {
    return aType;
}
