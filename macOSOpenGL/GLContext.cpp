//
//  GLContext.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/3/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLContext.hpp"

void GLContext::drawArrays(GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
}

void GLContext::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
    glDrawElements(mode, count, type, indices);
}

void GLContext::bindBuffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void GLContext::enableVertexAttribArray(GLuint index) {
    glEnableVertexAttribArray(index);
}

void GLContext::vertexAttribPointer(GLuint index,
                                    GLint size,
                                    GLenum type,
                                    GLboolean normalized,
                                    GLsizei stride,
                                    const GLvoid *pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void GLContext::uniformMatrix4fv(GLint location,
                                 GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat *value) {
    glUniformMatrix4fv(location,
                       count,
                       transpose,
                       value);
}

void GLContext::uniformMatrix3fv(GLint location,
                                 GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat *value) {
    glUniformMatrix3fv(location,
                       count,
                       transpose,
                       value);
}

void GLContext::uniform3fv(GLint location,
                           GLsizei count,
                           const GLfloat *value) {
    glUniform3fv(location, count, value);
}

GLuint GLContext::createShader(GLenum type) {
    return glCreateShader(type);
}

void GLContext::deleteShader(GLuint shaderId) {
    glDeleteShader(shaderId);
}

void GLContext::useShaderProgram(GLuint programId) {
    glUseProgram(programId);
}

GLint GLContext::getUniformLocation(GLuint program, const GLchar *name) {
    return glGetUniformLocation(program, name);
}

GLint GLContext::getAttributeLocation(GLuint program, const GLchar *name) {
    return glGetAttribLocation(program, name);
}
