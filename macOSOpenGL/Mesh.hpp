//
//  Mesh.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <memory>
#include <string>
#include <vector>
#include "glm/fwd.hpp"
#include "ISceneBehaviour.hpp"
#include "IGLRenderable.h"
#include "GL3DObject.hpp"

namespace macOSOpenGL {
class Transform;
}

class Mesh : public GL3DObject, public IGLRenderable, public ISceneBehaviour {
public:
    Mesh(const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec2>& uvs,
         const std::vector<glm::vec3>& normals,
         const std::vector<unsigned int>& indices,
         macOSOpenGL::Transform *transform);
    
    Mesh(const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec2>& uvs,
         const std::vector<glm::vec3>& normals,
         const std::vector<unsigned int>& indices);
    
    virtual ~Mesh();
    
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;
    Mesh& operator=(Mesh&&) = delete;
    
    const void addTangents(const std::vector<glm::vec3>& tangents);
    const void addBitangents(const std::vector<glm::vec3>& bitangents);
    
    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec2>& getUVs() const;
    const std::vector<glm::vec3>& getNormals() const;
    const std::vector<unsigned int>& getIndices() const;
    const std::vector<glm::vec3>& getTangents() const;
    const std::vector<glm::vec3>& getBitangents() const;
    
    virtual glm::mat4 getModel() const override;
    
protected:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
};

#endif /* Mesh_hpp */
