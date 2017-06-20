//
//  IRenderingEngine.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/1/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef IRenderingEngine_hpp
#define IRenderingEngine_hpp


class GLScene;

// Rendering Engine Interface
class IRenderingEngine {
public:
    IRenderingEngine() = default;
    virtual ~IRenderingEngine() = default;
    
    IRenderingEngine(const IRenderingEngine &engine) = delete;
    IRenderingEngine& operator=(const IRenderingEngine&) = delete;
    IRenderingEngine(IRenderingEngine &&engine) = delete;
    IRenderingEngine& operator=(IRenderingEngine&&) = delete;
    
    
    virtual void initialize(GLScene *scene) = 0;
    virtual void render(double deltaTime) = 0;
};

#endif /* IRenderingEngine_hpp */
