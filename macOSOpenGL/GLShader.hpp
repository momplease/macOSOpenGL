//
//  GLShader.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLShader_hpp
#define GLShader_hpp

#include "GLObject.hpp"
#include <unordered_map>
#include <OpenGL/gl.h>
#include <string>

class GLShaderLoader;

class GLShaderProgram;

class GLShader : public GLObject {
public:
    GLShader(std::string, GLShaderProgram *);
    virtual ~GLShader();
    
    GLShader(const GLShader&) = delete;
    GLShader& operator=(const GLShader&) = delete;
    GLShader(GLShader&&) = delete;
    GLShader& operator=(GLShader&&) = delete;
public:
    virtual void prepareOpenGL() override;
    virtual void clearOpenGL() override {}
    
    virtual void bind(GLShaderProgram *) override {}
    virtual void unbind(GLShaderProgram *) override {}
    
    std::string getPathWhereLocated() const;
    
    void addUniform(std::string name, std::string type);
    
    GLint getUniformLocationByName(std::string name);
    
    template<typename T>
    void setUniformValueByName(std::string, T *);
    
    virtual void buildShader(GLShaderLoader *);
    GLuint getId() const;
    
    bool hasUniformWithName(std::string name);
    
    bool isBuild() const;
    
protected:
    virtual GLenum getType() const = 0;
    
    std::string path;
    GLuint shaderId = NAN;
    GLShaderProgram *program;
    
    std::unordered_map<std::string, GLint> uniformIds;
    std::unordered_map<std::size_t, std::function<void(std::string key, void *value)>> uniformValueSetters;
    
};

#endif /* GLShader_hpp */
