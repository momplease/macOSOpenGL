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

namespace {
bool equalvec2(const glm::vec2& v1, const glm::vec2& v2) {
    return equalf(v1.x, v2.x) && equalf(v1.y, v2.y);
}
}

FbxLoader::FbxLoader() {
}

void FbxLoader::loadWithFilename(string filename) {
    const char *ccfilename = filename.c_str();
    
    _fbxManager = FbxManager::Create();
    
    FbxIOSettings *ioSettings = FbxIOSettings::Create(_fbxManager, IOSROOT);
    _fbxManager->SetIOSettings(ioSettings);
    
    FbxImporter *fbxImporter = FbxImporter::Create(_fbxManager, "");
    
    bool importerIsInitialized = fbxImporter->Initialize(ccfilename, -1, _fbxManager->GetIOSettings());
    
    if (!importerIsInitialized) {
        fbxImporter->Destroy();
        throw std::runtime_error("Cannot initialize importer");
    }
    
    FbxScene *scene = FbxScene::Create(_fbxManager, "");
        
    fbxImporter->Import(scene);
    fbxImporter->Destroy();
        
    FbxNode *rootNode = scene->GetRootNode();
        
    FbxNode *childNode = nullptr;
        
    vector<vec3> verticesOfNode;
        
    int childCount = rootNode->GetChildCount();
        
    for (int i = 0; i < childCount; ++i) {
        childNode = rootNode->GetChild(i);
        FbxMesh *mesh = childNode->GetMesh();
        if (mesh != NULL) {
                
            extractVerticesOf(mesh);
                
            extractIndicesOf(mesh);
                
            extractUVsOf(mesh);
                
            extractNormalsOf(mesh);
            
            extractTangentsOf(mesh);
            
            loadIndexedVertices();
            
            computeTangentBasis(_indexedVertices, _UVs.at(_UVKeys.at(0)), _normals, _tangents, _bitangents);
                
            break; // extracting only 1 mesh now
        }
    }
}


// Getters
const vector<vec3>& FbxLoader::vertices() {
    return _vertices;
}
const vector<unsigned int>& FbxLoader::indices() {
    return _indices;
}

const vector<vec3>& FbxLoader::indexedVertices() {
    return _indexedVertices;
}

const vector<vec3>& FbxLoader::normals() {
    return _normals;
}

const unordered_map<string, vector<vec2>>& FbxLoader::UVs() {
    return _UVs;
}
const vector<string>& FbxLoader::UVKeys() {
    return _UVKeys;
}


const std::vector<glm::vec3>& FbxLoader::getTangents() {
    return _tangents;
}

const std::vector<glm::vec3>& FbxLoader::getBitangents() {
    return _bitangents;
}

// Test
const vector<vec3>& FbxLoader::secondMesh() {
    //--------------------------- Kostyl ---------------------------//
    
    auto& uvset1 = _UVs.at(_UVKeys.at(0));
    auto& uvset2 = _UVs.at(_UVKeys.at(1));
    
    
    vector<vec2> newuvs;
    
    vector<vec3> verts;
    vector<vec2> uvs;
    vector<vec3> norms;
    vector<vec3> tangs;
    
    int counter = 0;
    for (size_t i = 0; i < uvset1.size(); ++i) {
        if (!equalvec2(uvset1[i], uvset2[i])) {
            uvset2[i] = vec2(-1, -1);
            uvs.push_back(uvset1[i]);
            verts.push_back(_indexedVertices.at(i));
            norms.push_back(_normals.at(i));
            tangs.push_back(_tangents.at(i));
        } else {
            _secondMeshVertices.push_back(_indexedVertices.at(i));
            _secondNormals.push_back(_normals[i]);
            _secondTangents.push_back(_tangents[i]);
            _secondBitangents.push_back(_bitangents[i]);
            newuvs.push_back(uvset2[i]);
        }
    }
    
    
    
    _indexedVertices = std::move(verts);
    _normals = std::move(norms);
    _tangents = std::move(tangs);
    uvset1 = std::move(uvs);
    
    uvset2 = std::move(newuvs);
    
    //--------------------------- Kostyl ---------------------------//
    
    return _secondMeshVertices;
}

const std::vector<glm::vec3>& FbxLoader::secondNormals() {
    return _secondNormals;
}

const std::vector<glm::vec3>& FbxLoader::secondTangents() {
    return _secondTangents;
}

const std::vector<glm::vec3>& FbxLoader::secondBitangents() {
    return _secondBitangents;
}

void FbxLoader::loadIndexedVertices() {
    if (_indexedVertices.empty()) {
        _indexedVertices.reserve(_indices.size());
        for (int i = 0; i < _indices.size(); ++i) {
            _indexedVertices.push_back(_vertices[_indices[i]]);
        }
    }
}

void FbxLoader::extractVerticesOf(FbxMesh *mesh) {
    
    int verticesCount = mesh->GetControlPointsCount();
    
    _vertices.clear();
    _vertices.reserve(verticesCount);
    
    for (int i = 0; i < verticesCount; ++i) {
        FbxVector4 fbxVertex = mesh->GetControlPointAt(i);
        vec3 vertex;
        vertex.x = fbxVertex.mData[0];
        vertex.y = fbxVertex.mData[1];
        vertex.z = fbxVertex.mData[2];
        
        _vertices.push_back(vertex);
    }
    
    testMesh = mesh; // for testing
}

void FbxLoader::extractIndicesOf(FbxMesh *mesh) {
    int* directArray = mesh->GetPolygonVertices();
    _indices.clear();
    _indices.assign(directArray, directArray + mesh->GetPolygonVertexCount());
}

void FbxLoader::extractUVsOf(FbxMesh *mesh) {
    
    _UVs.clear();
    
    FbxStringList lUVSetNameList;
    mesh->GetUVSetNames(lUVSetNameList);
    
    string key;
    vector<vec2> resultUV;
    
    for(int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); ++lUVSetIndex) {
        resultUV.clear();
        
        key.clear();
        const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
        key = string(lUVSetName);
        
        _UVKeys.push_back(key);
        
        FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
        
        if(!lUVElement)
            continue;
        
        if(lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
           lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
            throw std::runtime_error("FBXLoader - UV extracting: unsupported mapping mode");
        }
        
        
        //index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eIndex;
        const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
        
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
                    
                    resultUV.push_back(vec2((float)lUVValue[0], (float)lUVValue[1]));
                    
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
        
        _UVs.insert(std::pair<string, vector<vec2>>(key, resultUV));
    }
}

void FbxLoader::extractNormalsOf(FbxMesh *mesh) {
    FbxGeometryElementNormal *normalElement = mesh->GetElementNormal();
    
    if (normalElement)
    {
        _normals.clear();
        int numNormals = mesh->GetPolygonCount();
        int vertexCounter = 0;
        
        _normals.reserve(numNormals * kNumberOfPointsInVertex);
        
        for(int polyCounter = 0 ; polyCounter < numNormals; polyCounter++)
        {
            int polygonSize = mesh->GetPolygonSize(polyCounter);
            if (polygonSize != kNumberOfPointsInVertex)
                throw std::runtime_error("Cannot extract normals, a size of polygon != " + kNumberOfPointsInVertex);
            
            for (int i = 0; i < polygonSize; ++i)
            {
                FbxVector4 fbxNormal = normalElement->GetDirectArray().GetAt(vertexCounter);
                vec3 normal(fbxNormal[0], fbxNormal[1], fbxNormal[2]);
                _normals.push_back(std::move(normal));
                
                vertexCounter++;
            }
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
}

void FbxLoader::computeTangentBasis(const std::vector<glm::vec3>& verticesIn,
                                    const std::vector<glm::vec2>& uvsIn,
                                    const std::vector<glm::vec3>& normalsIn,
                                    std::vector<glm::vec3>& tangentsOut,
                                    std::vector<glm::vec3>& bitangentsOut) {
    
    for (unsigned int i = 0; i < verticesIn.size(); i+=3) {
        const glm::vec3& v0 = verticesIn[i + 0];
        const glm::vec3& v1 = verticesIn[i + 1];
        const glm::vec3& v2 = verticesIn[i + 2];
        
        glm::vec2 uv0 = uvsIn[i + 0];
        uv0.y = 1.0 - uv0.y;
        
        glm::vec2 uv1 = uvsIn[i + 1];
        uv1.y = 1.0 - uv1.y;
        
        glm::vec2 uv2 = uvsIn[i + 2];
        uv2.y = 1.0 - uv2.y;
        
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
        glm::vec3 bitangent;
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        //bitangent = glm::normalize(bitangent);
        
        
        tangentsOut.push_back(tangent);
        tangentsOut.push_back(tangent);
        tangentsOut.push_back(tangent);
        
        
        bitangentsOut.push_back(bitangent);
        bitangentsOut.push_back(bitangent);
        bitangentsOut.push_back(bitangent);
    }
    
    
    /*for (unsigned int i=0; i < verticesIn.size(); i+=1 )
    {
        const glm::vec3 & n = normalsIn[i];
        glm::vec3 & t = tangentsOut[i];
        glm::vec3 & b = bitangentsOut[i];
        
        // Gram-Schmidt orthogonalize
        t = glm::normalize(t - n * glm::dot(n, t));
        
        // Calculate handedness
        if (glm::dot(glm::cross(n, t), b) < 0.0f){
            t = t * -1.0f;
        }
        
    }*/
    
}

