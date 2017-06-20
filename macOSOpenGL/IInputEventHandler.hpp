//
//  IInputEventHandler.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/5/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef IInputEventHandler_hpp
#define IInputEventHandler_hpp

struct IInputEventHandler {
    virtual void keyUp(NSEvent *event) = 0;
    virtual void keyDown(NSEvent *event) = 0;
};

#endif /* IInputEventHandler_hpp */
