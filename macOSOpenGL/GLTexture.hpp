//
//  GLTexture.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/23/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLTexture_hpp
#define GLTexture_hpp

#include <OpenGL/gl.h>
#include <string>
#include "GLObject.hpp"

enum GLTextureType {
    Undefined = 0,
    Diffuse,
    NormalMap,
    Specular
};

class GLTexture : public GLObject {
public:
    GLTexture(const std::string& path, GLTextureType aType);
    virtual ~GLTexture() = default;
    
    GLTexture(const GLTexture&) = delete;
    GLTexture& operator=(const GLTexture&) = delete;
    GLTexture(GLTexture&&) = delete;
    GLTexture& operator=(GLTexture&&) = delete;
public:
    
    const std::string& getPathWhereLocated() const;
    GLuint getGLId() const;
    GLTextureType getType() const;
    
    virtual void prepareOpenGL() override;
    virtual void clearOpenGL() override;
    
    virtual void bind(GLShaderProgram *) override;
    virtual void unbind(GLShaderProgram *) override;
    
private:
    GLuint glId = NAN;
    std::string path;
    GLTextureType aType = GLTextureType::Undefined;
    
};

#endif /* GLTexture_hpp */
