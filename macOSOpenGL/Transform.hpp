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

class Transform final {
public:
    Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
    virtual ~Transform();
    
    Transform(const Transform&) = delete;
    Transform& operator=(const Transform&) = delete;
    Transform(Transform&&) = delete;
    Transform& operator=(Transform&&) = delete;
public:
    glm::vec3 getPosition();
    glm::quat getRotation();
    glm::vec3 getScale();
    
    void setPosition(glm::vec3);
    
    void translateBy(glm::vec3);
    
    void rotateBy(glm::quat);
    void rotateBy(float angle, glm::vec3 axis);
    
    void scaleBy(float);
    void scaleBy(glm::vec3);
    
    glm::mat4 asMatrix();
    
    static Transform* defaultTransform() {
        return new Transform(glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::quat(0.0f, 0.0f, 0.0f, 0.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f));
    }
    
private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};


#endif /* Transform_hpp */
