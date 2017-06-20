//
//  GLShaderLoader.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLShaderLoader_hpp
#define GLShaderLoader_hpp

#include <string>
#include <functional>
#include <OpenGL/gl.h>

class GLShaderLoader final {
public:
    GLShaderLoader();
    virtual ~GLShaderLoader();
    
    GLShaderLoader(const GLShaderLoader&) = delete;
    GLShaderLoader& operator=(const GLShaderLoader&) = delete;
    GLShaderLoader(GLShaderLoader&&) = delete;
    GLShaderLoader& operator=(GLShaderLoader&&) = delete;
    
    std::string loadRawShaderCode(std::string filePath);
    void compileShader(GLuint shaderId, std::string shaderSourceCode);
    GLuint createShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
    GLuint createShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId, std::function<void()> completion);
    
private:
    std::tuple<bool, std::string> checkIfShaderCompiled(GLuint shaderId);
    std::tuple<bool, std::string> checkIfShaderProgramLinked(GLuint programId);
};
#endif /* GLShaderLoader_hpp */
