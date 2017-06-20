//
//  ArrayDeleter.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/24/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef ArrayDeleter_hpp
#define ArrayDeleter_hpp

// Array deleter for proper usage of shared_ptr with array
template <typename Type>
class ArrayDeleter {
public:
    void operator()(Type const *pointer) {
        delete [] pointer;
    }
};

#endif /* ArrayDeleter_hpp */
