//
//  FBXLoader.m
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/4/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "FBXLoader.h"
#include "Helpers.hpp"

#include <fbxsdk/core/base/fbxstringlist.h>




@interface FBXLoader(){
    
    FbxManager *_fbxManager;
    
    
    vector<vec3> _vertices;
    vector<unsigned int> _indices;
    vector<vec3> _indexedVertices;
    
    vector<string> _UVKeys;
    unordered_map<string, vector<vec2>> _UVs;
    
    vector<vec3> _normals;
    
    vector<vec3> _secondMeshVertices;
    
}

@end

@implementation FBXLoader

-(void)loadWithFilename:(NSString *)filename {
    const char *ccfilename = [filename UTF8String];
    
    _fbxManager = FbxManager::Create();
    
    FbxIOSettings *ioSettings = FbxIOSettings::Create(_fbxManager, IOSROOT);
    _fbxManager->SetIOSettings(ioSettings);
    
    FbxImporter *fbxImporter = FbxImporter::Create(_fbxManager, "");
    
    bool importerIsInitialized = fbxImporter->Initialize(ccfilename, -1, _fbxManager->GetIOSettings());
    
    if (importerIsInitialized) {
        
        FbxScene *scene = FbxScene::Create(_fbxManager, "");
        
        fbxImporter->Import(scene);
        fbxImporter->Destroy();
        
        FbxNode *rootNode = scene->GetRootNode();
        
        FbxNode *childNode = nullptr;
        
        std::vector<glm::vec3> verticesOfNode;
        
        int childCount = rootNode->GetChildCount();
        
        for (int i = 0; i < childCount; ++i) {
            childNode = rootNode->GetChild(i);
            FbxMesh *mesh = childNode->GetMesh();
            if (mesh != NULL) {
                
                _vertices = [self extractVerticesOfMesh:mesh];
                
                _indices = [self extractIndicesOfMesh:mesh];
                
                //_normals = [self extractMeshNormals:mesh];
                
                _UVs = [self extractUVsOfMesh:mesh];
                
                break;
            }
        }
        
        
    } else {
        //std::cout << "FBX loader is not initialized" << std::endl;
        
        fbxImporter->Destroy();
    }
    
    
}

- (void)loadIndexedVertices {
    if (_indexedVertices.empty()) {
        
        _indexedVertices.reserve(_indices.size());
        
        for (int i = 0; i < _indices.size(); ++i) {
            _indexedVertices.push_back(_vertices[_indices[i]]);
        }
        
    }
}


- (vector<vec3>)secondMesh {
    //--------------------------- Kostyl ---------------------------//
    
    auto& uvset1 = _UVs.at(_UVKeys.at(0));
    auto& uvset2 = _UVs.at(_UVKeys.at(1));
    
    std::vector<size_t> indices;
    vector<vec2> newuvs;
    
    [self loadIndexedVertices];
    
    /*int counter = 0;
    for (size_t i = 0; i < uvset1.size(); ++i) {
        if (!equalvec2(uvset1[i], uvset2[i])) {
            uvset2[i] = vec2(-1, -1);
        } else {
            _secondMeshVertices.push_back(_indexedVertices.at(i));
            newuvs.push_back(uvset2[i]);
        }
    }
    
    uvset2 = newuvs;*/
    
    //--------------------------- Kostyl ---------------------------//
    
    return _secondMeshVertices;
}

- (vector<vec3>)vertices {
    return _vertices;
}

- (vector<unsigned int>)indices {
    return _indices;
}

- (vector<vec3>)indexedVertices {
    [self loadIndexedVertices];
    return _indexedVertices;
}

- (vector<vec3>)normals {
    return _normals;
}


- (unordered_map<string, vector<vec2>>)UVs {
    return _UVs;
}

-(vector<string>)UVKeys {
    return _UVKeys;
}


// RVO should work
- (vector<glm::vec3>)extractVerticesOfMesh:(FbxMesh *)mesh {
    vector<glm::vec3> vertices;
    
    int verticesCount = mesh->GetControlPointsCount();
    
    vertices.reserve(verticesCount);
    
    for (int i = 0; i < verticesCount; ++i) {
        FbxVector4 fbxVertex = mesh->GetControlPointAt(i);
        vec3 vertex;
        vertex.x = fbxVertex.mData[0];
        vertex.y = fbxVertex.mData[1];
        vertex.z = fbxVertex.mData[2];
                
        vertices.push_back(vertex);
    }
    
    return vertices;
}

- (vector<unsigned int>)extractIndicesOfMesh:(FbxMesh *)mesh {
    vector<unsigned int> indices;
    
    int* directArray = mesh->GetPolygonVertices();
    indices.assign(directArray, directArray + mesh->GetPolygonVertexCount());
    
    return indices;
}


// RVO should work
/*- (vector<vec3>)extractMeshNormals:(FbxMesh *)mesh {
    FbxGeometryElementNormal* normalEl = mesh->GetElementNormal();
    
    vector<vec3> normals;
    
    if(normalEl)
    {
        int numNormals = mesh->GetPolygonCount();
        int vertexCounter = 0;
        
        normals.reserve(numNormals * kNumberOfPointsInVertex);
        
        for(int polyCounter = 0 ; polyCounter < mesh->GetPolygonCount(); polyCounter++)
        {
            int polygonSize = mesh->GetPolygonSize(polyCounter);
            if (polygonSize != kNumberOfPointsInVertex)
                throw std::runtime_error(string("Cannot extract normals, a size of polygon > " + kNumberOfPointsInVertex));
            
            for (int i = 0; i < polygonSize; ++i)
            {
                FbxVector4 fbxNormal = normalEl->GetDirectArray().GetAt(vertexCounter);
                vec3 normal(fbxNormal[0], fbxNormal[1], fbxNormal[2]);
                normals.push_back(normal);
                
                vertexCounter++;
            }
        }
    }

    return normals;
}*/

- (unordered_map<string, vector<vec2>>)extractUVsOfMesh:(FbxMesh *)mesh {
    unordered_map<string, vector<vec2>> UVmap;
    UVmap.clear();
    
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
        
        /*FbxVector2 element;
        for(int i = 0; i < lUVElement->GetDirectArray().GetCount(); ++i) {
            element = lUVElement->GetDirectArray().GetAt(i);
            resultUV.push_back(vec2(element[0], element[1]));
        }*/
        
        
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
                        //std::cout << "lUVElement: " << lUVElement->GetName() << std::endl;
                        //std::cout << lUVIndex << std::endl;
                        lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                        
                        //lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                        resultUV.push_back(vec2(static_cast<float>(lUVValue[0]), static_cast<float>(lUVValue[1])));
                        
                        lPolyIndexCounter++;
                    }
                }
            }
        }
        
        UVmap.insert(std::pair<string, vector<vec2>>(key, resultUV));
    }
    
    return UVmap;
}


@end
