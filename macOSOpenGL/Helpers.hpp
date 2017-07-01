//
//  Helpers.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/1/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef Helpers_h
#define Helpers_h

#include <iostream>
#include <memory>
#include <float.h>
#include "ArrayDeleter.hpp"

namespace {
    
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

template<typename T>
void print(T messege) {
    std::cout << messege << std::endl;
}

bool equalf(float value1, float value2) {
    return fabs(value1 - value2) < FLT_EPSILON;
}

bool equalvec2(const glm::vec2& v1, const glm::vec2& v2) {
    return equalf(v1.x, v2.x) && equalf(v1.y, v2.y);
}
}



#endif /* Helpers_h */
