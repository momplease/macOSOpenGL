//
//  GLShaderLoader.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLShaderLoader.hpp"
#include "ArrayDeleter.hpp"

#include <fstream>
#include <memory>
#include <vector>

GLShaderLoader::GLShaderLoader() {
}

GLShaderLoader::~GLShaderLoader() {
}

std::string GLShaderLoader::loadRawShaderCode(std::string filePath) {
    std::string shaderCode = "";
    std::ifstream shaderStream;
        
    shaderStream.open(filePath);
    if (shaderStream.is_open()) {
            
        std::string buffer;
            
        while(std::getline(shaderStream, buffer)) {
            shaderCode += buffer + "\n";
        }
            
        shaderStream.close();
            
    }
    return shaderCode;
}


void GLShaderLoader::compileShader(GLuint shaderId, std::string shaderSourceCode) {
    char const *shaderSourcePointer = shaderSourceCode.c_str();
    glShaderSource(shaderId, 1, &shaderSourcePointer, NULL);
    glCompileShader(shaderId);
    
    std::tuple<bool, std::string> compileResult = checkIfShaderCompiled(shaderId);
    bool compiled = std::get<0>(compileResult);
    if (!compiled) {
        throw std::runtime_error(shaderSourceCode + " is not compiled\nCompile log: " + std::get<1>(compileResult));
    }
}

GLuint GLShaderLoader::createShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId) {
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    
    std::tuple<bool, std::string> compileResult = checkIfShaderProgramLinked(programId);
    bool compiled = std::get<0>(compileResult);
    if (!compiled) {
        throw std::runtime_error("Shader program is not linked\nLink log: " + std::get<1>(compileResult));
    }
    return programId;
}

GLuint GLShaderLoader::createShaderProgram(GLuint vertexShaderId,
                                           GLuint fragmentShaderId,
                                           std::function<void()> completion) {
    GLuint result = createShaderProgram(vertexShaderId, fragmentShaderId);
    completion();
    return result;
}

std::tuple<bool, std::string> GLShaderLoader::checkIfShaderProgramLinked(GLuint programId) {
    GLint result = GL_FALSE;
    GLint logLength = 0;
    
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        // using ArrayDeleter to delete memory properly
        GLint length = logLength + 1;
        std::vector<GLchar> infoLog(length);
        glGetProgramInfoLog(programId, length, 0, infoLog.data());
        return make_tuple(false, std::string(infoLog.begin(), infoLog.end()));
    }
    return std::make_tuple(true, "");
}

std::tuple<bool, std::string> GLShaderLoader::checkIfShaderCompiled(GLuint shaderId) {
    GLint result = GL_FALSE;
    GLint logLength;
    
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0) {
        // using ArrayDeleter to properly delete memory
        std::shared_ptr<GLchar> infoLog(new GLchar[logLength + 1], ArrayDeleter<GLchar>());
        glGetShaderInfoLog(shaderId, logLength, NULL, infoLog.get());
        return make_tuple(false, std::string(infoLog.get()));
    }
    return std::make_tuple(true, "");
}
