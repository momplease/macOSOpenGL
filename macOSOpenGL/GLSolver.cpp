//
//  GLSolver.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/13/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "GLSolver.hpp"
#include "glm/glm.hpp"

void GLSolver::computeTangentBasis(const std::vector<glm::vec3>& vertices,
                                   const std::vector<glm::vec2>& uvs,
                                   const std::vector<glm::vec3>& normals,
                                   std::vector<glm::vec3>& tangentsOut,
                                   std::vector<glm::vec3>& bitangentOut) {
    
    for (unsigned int i = 0; i < vertices.size(); i+=3) {
        const glm::vec3& v0 = vertices[i + 0];
        const glm::vec3& v1 = vertices[i + 1];
        const glm::vec3& v2 = vertices[i + 2];
        
        const glm::vec2 & uv0 = uvs[i + 0];
        const glm::vec2 & uv1 = uvs[i + 1];
        const glm::vec2 & uv2 = uvs[i + 2];
        
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;
        
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);
        
        //glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * f;
        //glm::vec3 bitangent = (edge1 * deltaUV1.x - edge1 * deltaUV2.x) * f;
        glm::vec3 bitangent;
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        
        
        tangentsOut.push_back(tangent);
        tangentsOut.push_back(tangent);
        tangentsOut.push_back(tangent);
        
        
        bitangentOut.push_back(bitangent);
        bitangentOut.push_back(bitangent);
        bitangentOut.push_back(bitangent);
    }
    
    for (unsigned int i=0; i<vertices.size(); i+=1 )
    {
        const glm::vec3 & n = normals[i];
        glm::vec3 & t = tangentsOut[i];
        glm::vec3 & b = bitangentOut[i];
        
        // Gram-Schmidt orthogonalize
        t = glm::normalize(t - n * glm::dot(n, t));
        
        // Calculate handedness
        if (glm::dot(glm::cross(n, t), b) < 0.0f){
            t = t * -1.0f;
        }
        
    }

}
