//
//  Transform.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/15/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"

namespace macOSOpenGL {
    
class Transform final {
public:
    Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
    virtual ~Transform();
    
    Transform(const Transform&) = delete;
    Transform& operator=(const Transform&) = delete;
    Transform(Transform&&) = delete;
    Transform& operator=(Transform&&) = delete;
    
    glm::vec3 getPosition();
    glm::quat getRotation();
    glm::vec3 getScale();
    
    glm::mat4 asMatrix();
    
    static Transform* defaultTransform3D() {
        return new Transform(glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::quat(0.0f, 0.0f, 0.0f, 0.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f));
    }

    
private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};
}


#endif /* Transform_hpp */
