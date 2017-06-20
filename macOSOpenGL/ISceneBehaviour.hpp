//
//  ISceneBehaviour.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef ISceneBehaviour_hpp
#define ISceneBehaviour_hpp

struct ISceneBehaviour {
    virtual void updateAnimations(double deltaTime) = 0;
};

#endif /* ISceneBehaviour_hpp */
