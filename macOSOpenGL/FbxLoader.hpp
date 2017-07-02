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
#include <future>
#include <unordered_map>
#include "glm/fwd.hpp"

namespace fbxsdk_2015_1 {
class FbxManager;
class FbxMesh;
class FbxScene;
}

class FbxLoader final {
public:
    FbxLoader();
    ~FbxLoader() = default;
    
    FbxLoader(const FbxLoader &) = delete;
    FbxLoader& operator=(const FbxLoader &) = delete;
    FbxLoader(FbxLoader &&) = delete;
    FbxLoader& operator=(FbxLoader &&) = delete;
 
public:
    
    void loadWithFilename(const std::string& filename);
    
    const std::vector<std::string>& getKeys();
    const std::vector<std::string>& getUVSetNamesForKey(const std::string &key);
    
    // Vertices
    const std::vector<glm::vec3>& getVerticesForKey(const std::string &key);
    // Indices
    const std::vector<unsigned int>& getIndicesForKey(const std::string &key);
    // Normals
    const std::vector<glm::vec3>& getNormalsForKey(const std::string &key);
    // Tangents
    const std::vector<glm::vec3>& getTangentsForKey(const std::string &key);
    // UVs
    const std::vector<glm::vec2>& getUVsForKeyWithSetName(const std::string &key, const std::string &UVSetName);
    
    std::vector<glm::vec3> computeTangentBasis(const std::vector<glm::vec3>&,
                                               const std::vector<glm::vec2>&,
                                               const std::vector<glm::vec3>&);
    
private:
    std::string filePath;
    
    fbxsdk_2015_1::FbxManager *fbxManager = nullptr;
    
    // Extracting
    void startVerticesExtractionOf(fbxsdk_2015_1::FbxMesh *);
    void startIndicesExtractionOf(fbxsdk_2015_1::FbxMesh *);
    void startUVsExtractionOf(fbxsdk_2015_1::FbxMesh *);
    void startNormalsExtractionOf(fbxsdk_2015_1::FbxMesh *);
    void startTangentsExtractionOf(fbxsdk_2015_1::FbxMesh *);
    
    std::vector<glm::vec3> extractVerticesOf(fbxsdk_2015_1::FbxMesh *);
    std::vector<unsigned int> extractIndicesOf(fbxsdk_2015_1::FbxMesh *);
    std::vector<glm::vec3> extractNormalsOf(fbxsdk_2015_1::FbxMesh *);
    std::vector<glm::vec3> extractTangentsOf(fbxsdk_2015_1::FbxMesh *);
    // key: UVSetName, value: UVs
    std::unordered_map<std::string, std::vector<glm::vec2>> extractUVsOf(fbxsdk_2015_1::FbxMesh *);
    
    
    // Containers
    std::vector<std::string> keys;
    // key: key, value: UVSetNames collection for key
    std::unordered_map<std::string, std::vector<std::string>> UVSetNames;
    
    std::unordered_map<std::string, std::future<std::vector<glm::vec3>>> processingVertices;
    std::unordered_map<std::string, std::vector<glm::vec3>> readyVertices;
    
    std::unordered_map<std::string, std::future<std::vector<unsigned int>>> processingIndices;
    std::unordered_map<std::string, std::vector<unsigned int>> readyIndices;
    
    std::unordered_map<std::string, std::future<std::vector<glm::vec3>>> processingNormals;
    std::unordered_map<std::string, std::vector<glm::vec3>> readyNormals;
    
    std::unordered_map<std::string, std::future<std::vector<glm::vec3>>> processingTangents;
    std::unordered_map<std::string, std::vector<glm::vec3>> readyTangents;
    // key: key, value: {key: UVSetName, value: UV collection for UVSetName}
    std::unordered_map<std::string, std::future<std::unordered_map<std::string, std::vector<glm::vec2>>>> processingUVs;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<glm::vec2>>> readyUVs;
    
    void forceLoadProcessingVertices();
    void forceLoadProcessingUVs();
    void forceLoadProcessingNormals();
    void forceLoadProcessingIndices();
    void forceLoadProcessingTangents();
    
    fbxsdk_2015_1::FbxScene* loadScene(const char *);
    
    // TODO:
    //std::vector<glm::vec2> extractUVsByControlPoints(fbxsdk_2015_1::FbxMesh *mesh, const std::string& setName);
    //std::vector<glm::vec2> extractUVsByPolygonVertex(fbxsdk_2015_1::FbxMesh *mesh, const std::string& setName);
};

#endif /* FbxLoader_hpp */
