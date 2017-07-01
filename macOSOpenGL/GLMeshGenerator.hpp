//
//  GLMeshGenerator.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/21/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLMeshGenerator_hpp
#define GLMeshGenerator_hpp

#include <vector>
#include <string>

class FbxLoader;
class GLMesh;

class GLMeshGenerator final {
public:
    GLMeshGenerator(FbxLoader *);
    ~GLMeshGenerator() = default;
    
    GLMeshGenerator(const GLMeshGenerator&) = delete;
    GLMeshGenerator(GLMeshGenerator&&) = delete;
    GLMeshGenerator& operator=(const GLMeshGenerator&) = delete;
    GLMeshGenerator& operator=(GLMeshGenerator&&) = delete;
    
public:
    const std::vector<GLMesh *>& getMeshes();
    
    template<typename T>
    std::vector<T> index(const std::vector<T>& what,
                         const std::vector<unsigned int>& indexes);
    
private:
    void load();
    
    std::vector<GLMesh *> createMeshUsing(const std::string& key);
    std::vector<GLMesh *> splitMeshesWith(const std::string& key, const std::vector<std::string>& UVSetNames);
    
    std::vector<GLMesh *> readyMeshes;
    FbxLoader *loader;
};

#endif /* GLMeshGenerator_hpp */
