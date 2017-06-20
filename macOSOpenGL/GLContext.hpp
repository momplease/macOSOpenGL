//
//  GLContext.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/3/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLContext_hpp
#define GLContext_hpp

#include <OpenGL/gl.h>
#include <mutex>

class GLContext final {
public:
    static GLContext* mainContext() {
        static std::once_flag flag;
        static GLContext* instance;
        std::call_once(flag, [] {
            instance = new GLContext();
        });
        return instance;
    }
    
    GLContext(const GLContext&) = delete;
    GLContext& operator=(const GLContext&) = delete;
    GLContext(GLContext&&) = delete;
    GLContext& operator=(GLContext&&) = delete;
    
    void drawArrays(GLenum mode, GLint first, GLsizei count);
    void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);

    void bindBuffer(GLenum target, GLuint buffer);
    
    void enableVertexAttribArray(GLuint index);
    void vertexAttribPointer(GLuint index,
                             GLint size,
                             GLenum type,
                             GLboolean normalized,
                             GLsizei stride,
                             const GLvoid *pointer);
    
    void uniformMatrix4fv(GLint location,
                          GLsizei count,
                          GLboolean transpose,
                          const GLfloat *value);
    
    void uniformMatrix3fv(GLint location,
                          GLsizei count,
                          GLboolean transpose,
                          const GLfloat *value);
    
    void uniform3fv(GLint location,
                    GLsizei count,
                    const GLfloat *value);
    
    GLuint createShader(GLenum type);
    void deleteShader(GLuint shaderId);
    
    void useShaderProgram(GLuint programId);
    
    GLint getUniformLocation(GLuint program, const GLchar *name);
    GLint getAttributeLocation(GLuint program, const GLchar *name);
    
private:
    GLContext() = default;
    ~GLContext() = default;
};

#endif /* GLContext_hpp */
