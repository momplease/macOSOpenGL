//
//  GLLight.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLLight_hpp
#define GLLight_hpp

#include "GL3DObject.hpp"
#include "ISceneBehaviour.hpp"

class GLLight : public GL3DObject, public ISceneBehaviour {
public:
    GLLight(Transform*);
    virtual ~GLLight();
    
    GLLight(const GLLight&) = delete;
    GLLight& operator=(const GLLight&) = delete;
    GLLight(GLLight&&) = delete;
    GLLight& operator=(GLLight&&) = delete;
    
    virtual float getPower() const;
    virtual void setPower(float);
    
    virtual void updateAnimations(double deltaTime) override;
    
protected:
    float lightPower;
    
    double timePast = 0.0;
};

#endif /* GLLight_hpp */
