//
//  FbxLoader.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 5/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef FbxLoader_hpp
#define FbxLoader_hpp

#include <vector>
#include <string>
#include <unordered_map>
#include "glm/fwd.hpp"

namespace fbxsdk_2015_1 {
class FbxManager;
class FbxMesh;
}

class FbxLoader {
public:
    
    FbxLoader();
    virtual ~FbxLoader() = default;
    
    FbxLoader(const FbxLoader &) = delete;
    FbxLoader& operator=(const FbxLoader &) = delete;
    
    
    void loadWithFilename(std::string filename);
    
    const std::vector<glm::vec3>& vertices();
    const std::vector<unsigned int>& indices();
    const std::vector<glm::vec3>& indexedVertices();
    const std::vector<glm::vec3>& normals();
    
    const std::unordered_map<std::string, std::vector<glm::vec2>>& UVs();
    const std::vector<std::string>& UVKeys();
    
    const std::vector<glm::vec3>& getTangents();
    const std::vector<glm::vec3>& getBitangents();
    
    // Test
    const std::vector<glm::vec3>& secondMesh();
    const std::vector<glm::vec3>& secondNormals();
    const std::vector<glm::vec3>& secondTangents();
    const std::vector<glm::vec3>& secondBitangents();
    
    void computeTangentBasis(const std::vector<glm::vec3>& verticesIn,
                             const std::vector<glm::vec2>& uvsIn,
                             const std::vector<glm::vec3>& normalsIn,
                             std::vector<glm::vec3>& tangentsOut,
                             std::vector<glm::vec3>& bitangentsOut);
    
    const int kNumberOfPointsInVertex = 3;
    const int kPolygonSize = 3;
    
private:
    
    fbxsdk_2015_1::FbxManager *_fbxManager;
    
    // Vertices
    void extractVerticesOf(fbxsdk_2015_1::FbxMesh *);
    std::vector<glm::vec3> _vertices;
    
    // Indices
    void extractIndicesOf(fbxsdk_2015_1::FbxMesh *);
    std::vector<unsigned int> _indices;
    
    // Indexed vertices
    void loadIndexedVertices();
    std::vector<glm::vec3> _indexedVertices;
    
    // UVs
    void extractUVsOf(fbxsdk_2015_1::FbxMesh *);
    std::unordered_map<std::string, std::vector<glm::vec2>> _UVs;
    std::vector<std::string> _UVKeys;
    
    // Normals
    void extractNormalsOf(fbxsdk_2015_1::FbxMesh *);
    std::vector<glm::vec3> _normals;
    
    // Tangents
    void extractTangentsOf(fbxsdk_2015_1::FbxMesh *);
    std::vector<glm::vec3> _tangents;
    std::vector<glm::vec3> _bitangents;
    
    // Test
    fbxsdk_2015_1::FbxMesh* testMesh;
    std::vector<glm::vec3> _secondMeshVertices;
    std::vector<glm::vec3> _secondNormals;
    std::vector<glm::vec3> _secondTangents;
    std::vector<glm::vec3> _secondBitangents;
};

#endif /* FbxLoader_hpp */
