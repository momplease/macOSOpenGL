//
//  Polygon.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 5/17/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef Polygon_hpp
#define Polygon_hpp

#include "glm/glm.hpp"

namespace macOSOpenGL {
    class Polygon {
    public:
        Polygon(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
        ~Polygon() = default;
        
        Polygon(const Polygon&);
        Polygon& operator=(const Polygon&);
        bool operator==(const Polygon&);
        
        glm::vec3 vertex1;
        glm::vec3 vertex2;
        glm::vec3 vertex3;
        
        glm::vec3 center() const;
        glm::vec3 barycentricPointWithWeights(float weightForVertex1,
                                              float weightForVertex2,
                                              float weightForVertex3) const;
    };
}


#endif /* Polygon_hpp */
