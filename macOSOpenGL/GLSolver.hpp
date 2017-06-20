//
//  GLSolver.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/13/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLSolver_hpp
#define GLSolver_hpp

#include <mutex>
#include <vector>
#include "glm/fwd.hpp"

class GLSolver {
public:
    GLSolver(const GLSolver&) = delete;
    GLSolver(GLSolver&&) = delete;
    GLSolver& operator=(const GLSolver&) = delete;
    GLSolver& operator=(GLSolver&&) = delete;
    
    static GLSolver* mainSolver() {
        static std::once_flag flag;
        static GLSolver* instance;
        std::call_once(flag, [] {
            instance = new GLSolver();
        });
        return instance;
    }
    
    void computeTangentBasis(const std::vector<glm::vec3>& vertices,
                             const std::vector<glm::vec2>& uvs,
                             const std::vector<glm::vec3>& normals,
                             std::vector<glm::vec3>& tangentsOut,
                             std::vector<glm::vec3>& binormalsOut);
    
    
private:
    GLSolver() = default;
    ~GLSolver() = default;
};

#endif /* GLSolver_hpp */
