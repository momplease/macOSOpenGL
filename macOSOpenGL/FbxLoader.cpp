//
//  FbxLoader.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 5/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "FbxLoader.hpp"
#include "fbxsdk.h"
#include "glm/glm.hpp"
#include "Helpers.hpp"

using namespace std;
using namespace glm;

FbxLoader::FbxLoader() {
}

FbxScene* FbxLoader::loadScene(const char *ccfilename) {
    if (!fbxManager)
        fbxManager = FbxManager::Create();
    
    FbxIOSettings *ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ioSettings);
    
    FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "");
    
    bool importerIsInitialized = fbxImporter->Initialize(ccfilename, -1, fbxManager->GetIOSettings());
    
    if (!importerIsInitialized) {
        fbxImporter->Destroy();
        throw std::runtime_error("Cannot initialize importer");
    }
    
    FbxScene *scene = FbxScene::Create(fbxManager, "");
    
    fbxImporter->Import(scene);
    fbxImporter->Destroy();
    
    return scene;
}

void FbxLoader::loadWithFilename(const string& filename) {
    filePath = filename;
    
    FbxScene *scene = loadScene(filePath.c_str());
        
    FbxNode *rootNode = scene->GetRootNode();
        
    FbxNode *childNode = nullptr;
    
    int childCount = rootNode->GetChildCount();
    
    for (int i = 0; i < childCount; ++i) {
        childNode = rootNode->GetChild(i);
        FbxMesh *mesh = childNode->GetMesh();
        
        if (mesh != NULL) {
            keys.push_back(mesh->GetName());
            // Extraction
            startVerticesExtractionOf(mesh);
            startIndicesExtractionOf(mesh);
            startUVsExtractionOf(mesh);
            startNormalsExtractionOf(mesh);
            startTangentsExtractionOf(mesh);
        }
    }
}

#pragma mark - Keys

const std::vector<std::string>& FbxLoader::getKeys() const {
    return keys;
}

const std::vector<std::string>& FbxLoader::getUVSetNamesForKey(const std::string &key) const {
    return UVSetNames.at(key);
}

#pragma mark - Vertices

void FbxLoader::startVerticesExtractionOf(FbxMesh *mesh) {
    if (processingVertices.find(mesh->GetName()) != processingVertices.end())
        throw std::runtime_error("startVerticesExtractionOf: such mesh already extracting");
    
    std::pair<std::string, std::future<std::vector<glm::vec3>>> result;
    result.first = mesh->GetName();
    result.second = std::async(&FbxLoader::extractVerticesOf, this, mesh);
    
    processingVertices.insert(std::move(result));
}

// RVO
std::vector<glm::vec3> FbxLoader::extractVerticesOf(fbxsdk_2015_1::FbxMesh *mesh) {
    std::vector<glm::vec3> resultVertices;

    int verticesCount = mesh->GetControlPointsCount();
    
    resultVertices.reserve(verticesCount);
    
    for (int i = 0; i < verticesCount; ++i) {
        FbxVector4 fbxVertex = mesh->GetControlPointAt(i);
        glm::vec3 vertex;
        vertex.x = fbxVertex.mData[0];
        vertex.y = fbxVertex.mData[1];
        vertex.z = fbxVertex.mData[2];
        
        resultVertices.push_back(std::move(vertex));
    }

    return resultVertices; // RVO should work here
}


const std::vector<glm::vec3>& FbxLoader::getVerticesForKey(const std::string &key) {
    if (readyVertices.at(key).empty()) {
        forceLoadProcessingVertices();
    }
    return readyVertices.at(key);
}

void FbxLoader::forceLoadProcessingVertices() {
    if (!readyVertices.empty())
        throw std::runtime_error("forceLoadProcessingVertices: readyVertices are not empty, no need to load");
    
    std::for_each(keys.begin(),
                  keys.end(),
                  [this](const std::string& key){
                      readyVertices.insert({key, processingVertices.at(key).get()});
                  });
}

#pragma mark - UVs

void FbxLoader::startUVsExtractionOf(FbxMesh *mesh) {
    if (processingUVs.find(mesh->GetName()) != processingUVs.end())
        throw std::runtime_error("startUVsExtractionOf: such mesh already extracting");
    
    std::pair<std::string, std::future<std::unordered_map<std::string, std::vector<glm::vec2>>>> result;
    result.first = mesh->GetName();
    result.second = std::async(&FbxLoader::extractUVsOf, this, mesh);
    
    processingUVs.insert(std::move(result));
}

std::unordered_map<std::string, std::vector<glm::vec2>> FbxLoader::extractUVsOf(fbxsdk_2015_1::FbxMesh *mesh) {
    std::unordered_map<std::string, std::vector<glm::vec2>> result;
    
    FbxStringList lUVSetNameList;
    mesh->GetUVSetNames(lUVSetNameList);
        
    std::string key;
    std::vector<vec2> resultUV;
    std::vector<std::string> UVSetNames;
    
    for(int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); ++lUVSetIndex) {

        key = lUVSetNameList.GetStringAt(lUVSetIndex);
        
        UVSetNames.push_back(key);
            
        FbxGeometryElementUV* lUVElement = mesh->GetElementUV(key.c_str());
            
        if(!lUVElement)
            continue;
            
        if(lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
            lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
            throw std::runtime_error("FBXLoader - UV extracting: unsupported mapping mode");
        }
            
            
        //index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eIndex;
            
        //iterating through the data by polygon
        const int lPolyCount = mesh->GetPolygonCount();
            
        //iterating through the data by polygon
        if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
                
            for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
                for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
                {
                    FbxVector2 lUVValue;
                        
                    //get the index of the current vertex in control points array
                    int lPolyVertIndex = mesh->GetPolygonVertex(lPolyIndex,lVertIndex);
                        
                    //the UV index depends on the reference mode
                    int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
                        
                    lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                        
                    resultUV.push_back(vec2(static_cast<float>(lUVValue[0]), static_cast<float>(lUVValue[1])));
                        
                }
            }
                
        } else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
            int lPolyIndexCounter = 0;
                
            for( int lPolyIndex = 0; lPolyIndex < mesh->GetPolygonCount(); ++lPolyIndex )
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
                    
                for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
                {
                    //if (lPolyIndexCounter < lIndexCount)
                    {
                        FbxVector2 lUVValue;
                            
                        //the UV index depends on the reference mode
                            
                        int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
                            
                        lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                            
                        resultUV.push_back(vec2(static_cast<float>(lUVValue[0]), static_cast<float>(lUVValue[1])));
                            
                        lPolyIndexCounter++;
                    }
                }
            }
        }
            
    }
    this->UVSetNames.insert({mesh->GetName(), std::move(UVSetNames)});
    return result;
    
    /*std::unordered_map<std::string, std::vector<glm::vec2>> result;
    
    FbxStringList lUVSetNameList;
    mesh->GetUVSetNames(lUVSetNameList);
    
    for(int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); ++lUVSetIndex) {

        std::string key = lUVSetNameList.GetStringAt(lUVSetIndex);
        std::vector<glm::vec2> UVs;
        
        FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
        
        if(!lUVElement)
            continue;
        
        if(lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
           lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
            throw std::runtime_error("FBXLoader - UV extracting: unsupported mapping mode");
        }
        
        if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
            UVs = extractUVsByControlPoints(mesh, key);
        } else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
            UVs = extractUVsByPolygonVertex(mesh, key);
        }
        result.insert({std::move(key), std::move(UVs)});
    }
    
    return result;*/
}

// RVO
/*std::vector<glm::vec2> FbxLoader::extractUVsByControlPoints(fbxsdk_2015_1::FbxMesh *mesh, const std::string& setName) {
    FbxGeometryElement *geometryElement = mesh->GetElementUV(lUVSet)
    
    if (geometryElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
        throw std::runtime_error("extractUVsByControlPoints: wrong mapping mode");
    
    FbxGeometryElementUV *lUVElement = mesh->GetElementUV(setName.c_str());
    
    std::vector<glm::vec2> resultUVs;
    
    const int lPolyCount = mesh->GetPolygonCount();
    //iterating through the data by polygon
    for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
    {
        // build the max index array that we need to pass into MakePoly
        const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
        for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
        {
            FbxVector2 lUVValue;
                
            //get the index of the current vertex in control points array
            int lPolyVertIndex = mesh->GetPolygonVertex(lPolyIndex, lVertIndex);
                
            //the UV index depends on the reference mode
            int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
                
            lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                
            resultUVs.push_back(vec2(static_cast<float>(lUVValue[0]), static_cast<float>(lUVValue[1])));
            
        }
    }
    
    return resultUVs;
}

// RVO
std::vector<glm::vec2> FbxLoader::extractUVsByPolygonVertex(fbxsdk_2015_1::FbxMesh *mesh, const std::string& setName) {
    if (geometryElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
        throw std::runtime_error("extractUVsByPolygonVertex: wrong mapping mode");
    
    FbxGeometryElementUV *lUVElement = mesh->GetElementUV(setName.c_str());
    
    std::vector<glm::vec2> resultUVs;
    
    int lPolyIndexCounter = 0;
    
    for( int lPolyIndex = 0; lPolyIndex < mesh->GetPolygonCount(); ++lPolyIndex )
    {
        // build the max index array that we need to pass into MakePoly
        const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
        
        for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
        {
            //if (lPolyIndexCounter < lIndexCount)
            {
                FbxVector2 lUVValue;
                
                //the UV index depends on the reference mode
                
                int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
                
                lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                
                resultUV.push_back(vec2(static_cast<float>(lUVValue[0]), static_cast<float>(lUVValue[1])));
                
                lPolyIndexCounter++;
            }
        }
    }
    
    return resultUVs;
}*/

const std::vector<glm::vec2>& FbxLoader::getUVsForKeyWithSetName(const std::string &key, const std::string &UVSetName) {
    if (readyUVs.empty()) {
        forceLoadProcessingUVs();
    }
    
    return readyUVs.at(key).at(UVSetName);
}

void FbxLoader::forceLoadProcessingUVs() {
    if (!readyUVs.empty())
        throw std::runtime_error("forceLoadProcessingUVs: readyUVs are not empty, no need to load");
    
    std::for_each(keys.begin(),
                  keys.end(),
                  [this](const std::string& key){
                      readyUVs.insert({key, processingUVs.at(key).get()});
                  });
}

#pragma mark - Indices

void FbxLoader::startIndicesExtractionOf(fbxsdk_2015_1::FbxMesh *mesh) {
    if (processingIndices.find(mesh->GetName()) != processingIndices.end())
        throw std::runtime_error("startIndicesExtractionOf: such mesh already extracting");
    
    std::pair<std::string, std::future<std::vector<unsigned int>>> result;
    result.first = mesh->GetName();
    result.second = std::async(&FbxLoader::extractIndicesOf, this, mesh);
    
    processingIndices.insert(std::move(result));
}

std::vector<unsigned int> FbxLoader::extractIndicesOf(fbxsdk_2015_1::FbxMesh *mesh) {
    std::vector<unsigned int> result;
    
    int* directArray = mesh->GetPolygonVertices();
    result.assign(directArray, directArray + mesh->GetPolygonVertexCount());
    
    return result;
}

const std::vector<unsigned int>& FbxLoader::getIndicesForKey(const std::string &key) {
    if (readyIndices.empty()) {
        forceLoadProcessingIndices();
    }
    
    return readyIndices.at(key);
}

void FbxLoader::forceLoadProcessingIndices() {
    if (!readyIndices.empty())
        throw std::runtime_error("forceLoadProcessingVertices: readyIndices are not empty, no need to load");
    
    std::for_each(keys.begin(),
                  keys.end(),
                  [this](const std::string& key){
                      readyIndices.insert({key, processingIndices.at(key).get()});
                  });
}

#pragma mark - Normals

void FbxLoader::startNormalsExtractionOf(fbxsdk_2015_1::FbxMesh *mesh) {
    if (processingNormals.find(mesh->GetName()) != processingNormals.end())
        throw std::runtime_error("startNormalsExtractionOf: such mesh already extracting");
    
    std::pair<std::string, std::future<std::vector<glm::vec3>>> result;
    result.first = mesh->GetName();
    result.second = std::async(&FbxLoader::extractNormalsOf, this, mesh);
    
    processingNormals.insert(std::move(result));
}

std::vector<glm::vec3> FbxLoader::extractNormalsOf(fbxsdk_2015_1::FbxMesh *mesh) {
    FbxGeometryElementNormal *normalElement = mesh->GetElementNormal();
    if (!normalElement)
        throw std::runtime_error("extractNormalsOf: FbxGeometryElementNormal == NULL");
    
    std::vector<glm::vec3> result;
    
    int numNormals = mesh->GetPolygonCount();
    int vertexCounter = 0;
        
    result.reserve(numNormals * kNumberOfPointsInVertex);
        
    for(int polyCounter = 0 ; polyCounter < numNormals; polyCounter++)
    {
        int polygonSize = mesh->GetPolygonSize(polyCounter);
        if (polygonSize != kNumberOfPointsInVertex)
            throw std::runtime_error("Cannot extract normals, a size of polygon != " + kNumberOfPointsInVertex);
            
        for (int i = 0; i < polygonSize; ++i)
        {
            FbxVector4 fbxNormal = normalElement->GetDirectArray().GetAt(vertexCounter);
            vec3 normal(fbxNormal[0], fbxNormal[1], fbxNormal[2]);
            result.push_back(std::move(normal));
                
            vertexCounter++;
        }
    }
    
    return result;
}

const std::vector<glm::vec3>& FbxLoader::getNormalsForKey(const std::string &key) {
    if (readyNormals.empty()) {
        forceLoadProcessingNormals();
    }
    
    return readyNormals.at(key);
}

void FbxLoader::forceLoadProcessingNormals() {
    if (!readyNormals.empty())
        throw std::runtime_error("forceLoadProcessingNormals: readyNormals are not empty, no need to load");
    
    std::for_each(keys.begin(),
                  keys.end(),
                  [this](const std::string& key){
                      readyNormals.insert({key, processingNormals.at(key).get()});
                  });
}

#pragma mark - Tangents

void FbxLoader::startTangentsExtractionOf(fbxsdk_2015_1::FbxMesh *mesh) {
    if (processingTangents.find(mesh->GetName()) != processingTangents.end())
        throw std::runtime_error("startTangentsExtractionOf: such mesh already extracting");
    
    std::pair<std::string, std::future<std::vector<glm::vec3>>> result;
    result.first = mesh->GetName();
    result.second = std::async(&FbxLoader::extractTangentsOf, this, mesh);
    
    processingTangents.insert(std::move(result));
}

std::vector<glm::vec3> FbxLoader::extractTangentsOf(fbxsdk_2015_1::FbxMesh *mesh) {
    FbxGeometryElementTangent *tangentElement = mesh->GetElementTangent();
    std::vector<glm::vec3> result;
    
    if (not tangentElement) {
        mesh->GenerateTangentsDataForAllUVSets();
    }
    
    if (not tangentElement) {
        result = computeTangentBasis(readyVertices.at(mesh->GetName()),
                                     readyUVs.at(mesh->GetName()).at(UVSetNames.at(mesh->GetName()).at(0)), // for first uv set only
                                     readyNormals.at(mesh->GetName()));
    } else {
        int numTangents = mesh->GetPolygonCount();
        int vertexCounter = 0;
        result.reserve(numTangents * kNumberOfPointsInVertex);
    
        for (int polyCounter = 0; polyCounter < numTangents; ++polyCounter) {
            int polygonSize = mesh->GetPolygonSize(polyCounter);
            if (polygonSize > kPolygonSize)
                throw std::runtime_error("Cannot extract tangents, a size of polygon != " + kNumberOfPointsInVertex);
            
            for (int i = 0; i < polygonSize; ++i) {
                FbxVector4 fbxTangent = tangentElement->GetDirectArray().GetAt(vertexCounter);
                vec3 tangent(fbxTangent[0], fbxTangent[1], fbxTangent[2]);
                result.push_back(std::move(tangent));
                
                vertexCounter++;
            }
        }
    }
    return result;
}

const std::vector<glm::vec3>& FbxLoader::getTangentsForKey(const std::string &key) {
    if (readyTangents.empty()) {
        forceLoadProcessingTangents();
    }
    
    return readyTangents.at(key);
}

void FbxLoader::forceLoadProcessingTangents() {
    if (readyTangents.empty())
        throw std::runtime_error("forceLoadProcessingTangents: readyTangents are not empty, no need to load");
    
    std::for_each(keys.begin(),
                  keys.end(),
                  [this](const std::string& key){
                      readyTangents.insert({key, processingTangents.at(key).get()});
                  });
}


#pragma mark - Utility

/*void FbxLoader::loadIndexedVertices() {
    if (_indexedVertices.empty()) {
        _indexedVertices.reserve(_indices.size());
        for (int i = 0; i < _indices.size(); ++i) {
            _indexedVertices.push_back(_vertices[_indices[i]]);
        }
    }
}


void FbxLoader::extractTangentsOf(fbxsdk_2015_1::FbxMesh *mesh) {
    
    FbxGeometryElementTangent *tangentElement = mesh->GetElementTangent();

    if (tangentElement) {
        _tangents.clear();
        
        int numTangents = mesh->GetPolygonCount();
        int vertexCounter = 0;
        _tangents.reserve(numTangents * kNumberOfPointsInVertex);
        for (int polyCounter = 0; polyCounter < numTangents; ++polyCounter) {
            int polygonSize = mesh->GetPolygonSize(polyCounter);
            if (polygonSize > kPolygonSize)
                throw std::runtime_error("Cannot extract tangents, a size of polygon != " + kNumberOfPointsInVertex);
            
            for (int i = 0; i < polygonSize; ++i) {
                FbxVector4 fbxTangent = tangentElement->GetDirectArray().GetAt(vertexCounter);
                vec3 tangent(fbxTangent[0], fbxTangent[1], fbxTangent[2]);
                _tangents.push_back(std::move(tangent));
                
                vertexCounter++;
            }
        }
        
    }
}*/

std::vector<glm::vec3> FbxLoader::computeTangentBasis(const std::vector<glm::vec3>& verticesIn,
                                                      const std::vector<glm::vec2>& uvsIn,
                                                      const std::vector<glm::vec3>& normalsIn) {
    
    std::vector<glm::vec3> tangentsOut;
    
    for (unsigned int i = 0; i < verticesIn.size(); i+=3) {
        const glm::vec3& v0 = verticesIn[i + 0];
        const glm::vec3& v1 = verticesIn[i + 1];
        const glm::vec3& v2 = verticesIn[i + 2];
        
        glm::vec2 uv0 = uvsIn[i + 0];
        //uv0.y = 1.0 - uv0.y;
        
        glm::vec2 uv1 = uvsIn[i + 1];
        //uv1.y = 1.0 - uv1.y;
        
        glm::vec2 uv2 = uvsIn[i + 2];
        //uv2.y = 1.0 - uv2.y;
        
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;
        
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        //tangent = glm::normalize(tangent);
        
        //glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * f;
        //glm::vec3 bitangent = (edge1 * deltaUV1.x - edge1 * deltaUV2.x) * f;
        
        tangentsOut.push_back(tangent);
        tangentsOut.push_back(tangent);
        tangentsOut.push_back(tangent);
    }
    
    
    for (unsigned int i=0; i < verticesIn.size(); i+=1 )
    {
        const glm::vec3 & n = normalsIn[i];
        glm::vec3 & t = tangentsOut[i];
        glm::vec3 b = glm::cross(n, t);
        
        // Gram-Schmidt orthogonalize
        t = glm::normalize(t - n * glm::dot(n, t));
        
        // Calculate handedness
        if (glm::dot(glm::cross(n, t), b) < 0.0f){
            t = t * -1.0f;
        }
        
    }
    
    return tangentsOut;
}

