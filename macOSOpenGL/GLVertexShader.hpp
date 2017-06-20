//
//  GLVertexShader.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLVertexShader_hpp
#define GLVertexShader_hpp

#include "GLShader.hpp"

class GLVertexShader : public GLShader {
public:
    GLVertexShader(std::string path, GLShaderProgram *);
    virtual ~GLVertexShader();
    
    GLVertexShader(const GLVertexShader&) = delete;
    GLVertexShader& operator=(const GLVertexShader&) = delete;
    GLVertexShader(GLVertexShader&&) = delete;
    GLVertexShader& operator=(GLVertexShader&&) = delete;
    
    virtual void buildShader(GLShaderLoader *) override;
    
    void addAttribute(std::string name, std::string type);
    GLint getAttributeLocationByName(std::string name);
    
    bool hasAttributeWithName(std::string name);
    
protected:
    virtual GLenum getType() const override;
    GLenum shaderType = GL_VERTEX_SHADER;
    
    std::unordered_map<std::string, GLint> attributeLocations;
};

#endif /* GLVertexShader_hpp */
