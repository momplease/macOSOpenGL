//
//  GL3DObjectRenderable.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GL3DRenderMesh.hpp"
#include "GLDrawableComponent.hpp"
#include "GLVertexedComponent.hpp"
#include "GLTexturedComponent.hpp"
#include "GLShaderProgram.hpp"

using namespace macOSOpenGL;

GL3DRenderMesh::GL3DRenderMesh(const std::vector<glm::vec3>& vertices,
                               const std::vector<glm::vec2>& uvs,
                               const std::vector<glm::vec3>& normals,
                               const std::vector<unsigned int>& indices,
                               macOSOpenGL::Transform* transform,
                               RenderableConfig config) : Mesh(vertices,
                                                               uvs,
                                                               normals,
                                                               indices,
                                                               transform) {
    drawable.reset(new GLDrawableComponent(config.drawablePart, this));
    vertexed.reset(new GLVertexedComponent(config.vertexedPart, this));
    textured.reset(new GLTexturedComponent(config.texturedPart, this));
}

GL3DRenderMesh::GL3DRenderMesh(const std::vector<glm::vec3>& vertices,
                               const std::vector<glm::vec2>& uvs,
                               const std::vector<glm::vec3>& normals,
                               const std::vector<unsigned int>& indices,
                               RenderableConfig config) : Mesh(vertices,
                                                               uvs,
                                                               normals,
                                                               indices) {
    drawable.reset(new GLDrawableComponent(config.drawablePart, this));
    vertexed.reset(new GLVertexedComponent(config.vertexedPart, this));
    textured.reset(new GLTexturedComponent(config.texturedPart, this));
}

IGLDrawable* GL3DRenderMesh::getDrawable() const {
    return drawable.get();
}

IVertexed* GL3DRenderMesh::getVertexed() const {
    return vertexed.get();
}

ITextured* GL3DRenderMesh::getTextured() const {
    return textured.get();
}

void GL3DRenderMesh::updateAnimations(double deltaTime) {
    // reserved
}

void GL3DRenderMesh::bind(GLShaderProgram *program) {
    vertexed->bind(program);
    textured->bind(program);
}

void GL3DRenderMesh::prepareOpenGL() {
    vertexed->prepareOpenGL();
    textured->prepareOpenGL();
    drawable->prepareOpenGL();
}

// Constructors / Destructor

//GL3DObjectRenderable::GL3DObjectRenderable() {
//    _primitiveType = GL_TRIANGLES;
//    _verticesType = GL_FLOAT;
//    _dataOffset = 3; // 3D object
//    _verticesNormilized = GL_FALSE;
//    _stride = 0;
//}

//GL3DObjectRenderable::GL3DObjectRenderable(Mesh *mesh, Transform *transform)
//{
    
    
    
    //setUseIndices(mesh->indices().size() == 0 ? false : true);
//}

/*// Render call
GLuint GL3DObjectRenderable::primitiveType() const {
    return _primitiveType;
}

void GL3DObjectRenderable::setPrimitiveType(GLuint type) {
    _primitiveType = type;
}

GLint GL3DObjectRenderable::offset() const {
    return 0;
}

GLsizei GL3DObjectRenderable::verticesCount(int meshIndex) const {
    return static_cast<GLsizei>(_meshTransforms[meshIndex]->mesh->vertices().size());
}

// Attributes
GLenum GL3DObjectRenderable::verticesType() const {
    return _verticesType;
}

void GL3DObjectRenderable::setVerticesType(GLenum type) {
    _verticesType = type;
}

GLint GL3DObjectRenderable::dataOffset() const {
    // // 3D object
    return _dataOffset;
}

void GL3DObjectRenderable::setDataOffset(GLint offset) {
    _dataOffset = offset;
}

GLboolean GL3DObjectRenderable::verticesNormilized() const {
    return _verticesNormilized;
}

void GL3DObjectRenderable::setVerticesNormilized(GLboolean normilized) {
    _verticesNormilized = normilized;
}

GLsizei GL3DObjectRenderable::stride() const {
    return _stride;
}

void GL3DObjectRenderable::setStride(GLsizei stride) {
    _stride = stride;
}


mat4 GL3DObjectRenderable::model(int transformIndex) const {
    return _meshTransforms[transformIndex]->transform->asMatrix();
}


bool GL3DObjectRenderable::useIndices() const {
    return _useIndices;
}

void GL3DObjectRenderable::setUseIndices(bool use) {
    _useIndices = use;
}

void GL3DObjectRenderable::prepareOpenGL() {
    //for (int i = 0; i < _meshTransforms.size(); ++i) {
    int i = 0;
    for (int j = 0; j < _meshTransforms.size(); ++j) {
            GLuint buffer;
        
            glGenBuffers(1, &buffer);
            _verticesBufferObject.push_back(buffer);
        
            glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject[i]);
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(glm::vec3) * _meshTransforms[i]->mesh->vertices().size(),
                         _meshTransforms[i]->mesh->vertices().data(),
                         GL_STATIC_DRAW);
    
        
            glGenBuffers(1, &buffer);
            _UVBufferObjects.push_back(buffer);
        
            glBindBuffer(GL_ARRAY_BUFFER, _UVBufferObjects[i]);
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(glm::vec2) * std::next(_meshTransforms[i]->mesh->uvs().begin(), j)->second.size(),
                         std::next(_meshTransforms[i]->mesh->uvs().begin(), j)->second.data(),
                         GL_STATIC_DRAW);
        }
    
    
        glGenBuffers(1, &_normalBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBufferObject);
        glBufferData(GL_ARRAY_BUFFER, _meshTransforms[i]->mesh->normals().size() * sizeof(glm::vec3), _meshTransforms[i]->mesh->normals().data(), GL_STATIC_DRAW);
    
        if (_useIndices) {
            glGenBuffers(1, &_indicesBufferObject);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _meshTransforms[i]->mesh->indices().size(), _meshTransforms[i]->mesh->indices().data(), GL_STATIC_DRAW);
        }
    //}
}

GLuint GL3DObjectRenderable::normalBufferObject() const {
    return _normalBufferObject;
}

std::vector<GLuint> GL3DObjectRenderable::vertexBufferObjects() const {
    return _verticesBufferObject;
}

std::vector<GLuint> GL3DObjectRenderable::UVBufferObjects() const {
    return _UVBufferObjects;
}


GLint GL3DObjectRenderable::offsetInTexCoords() const {
    return 2;
}

GLenum GL3DObjectRenderable::texCoordType() const {
    return GL_FLOAT;
}

GLboolean GL3DObjectRenderable::texCoordsNormilized() const {
    return GL_FALSE;
}

GLsizei GL3DObjectRenderable::texCoordsStride() const {
    return 0;
}

std::vector<GLuint> GL3DObjectRenderable::diffuseTextureIds() const {
    return _diffuseTextureIds;
}

void GL3DObjectRenderable::addDiffuseTextureId(GLuint id) {
    _diffuseTextureIds.push_back(id);
}

GLsizei GL3DObjectRenderable::indicesCount(int index) const {
    return static_cast<GLsizei>(_meshTransforms[index]->mesh->indices().size());
}

GLenum GL3DObjectRenderable::indicesType() const {
    return GL_UNSIGNED_INT;
}

GLuint GL3DObjectRenderable::indicesBufferObject() const {
    return _indicesBufferObject;
}*/
