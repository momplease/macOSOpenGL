//
//  Polygon.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 5/17/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "Polygon.hpp"

using namespace macOSOpenGL;
using namespace glm;

Polygon::Polygon(vec3 v1, vec3 v2, vec3 v3) {
    vertex1 = v1;
    vertex2 = v2;
    vertex3 = v3;
}

Polygon::Polygon(const Polygon& polygon) {
    operator=(polygon);
}

bool Polygon::operator==(const Polygon& polygon) {
    return vertex1 == polygon.vertex1 && vertex2 == polygon.vertex2 && vertex3 == polygon.vertex2;
}

Polygon& Polygon::operator=(const Polygon& polygon) {
    if (*this == polygon) {
        return *this;
    }
    
    vertex1 = polygon.vertex1;
    vertex2 = polygon.vertex2;
    vertex3 = polygon.vertex3;
    
    return *this;
}


glm::vec3 Polygon::center() const {
    return barycentricPointWithWeights(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f);
}

glm::vec3 Polygon::barycentricPointWithWeights(float weightForVertex1,
                                               float weightForVertex2,
                                               float weightForVertex3) const {
    
    // P = (1 - u - v)A + uB + vC
    vec3 result;//-37.809700 -4.458747 17.164913
    
    vec3 barycentricVertex1 = vertex1 * weightForVertex1;
    vec3 barycentricVertex2 = vertex2 * weightForVertex2;
    vec3 barycentricVertex3 = vertex3 * weightForVertex3;
    
    result.x = barycentricVertex1.x + barycentricVertex2.x + barycentricVertex3.x;
    result.y = barycentricVertex1.y + barycentricVertex2.y + barycentricVertex3.y;
    result.z = barycentricVertex1.z + barycentricVertex2.z + barycentricVertex3.z;
    
    return result;
}
