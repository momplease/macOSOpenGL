//
//  Property.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 5/31/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef Property_hpp
#define Property_hpp

template <class Type>
class Property {
public:
    Type target;
    
    Property(const Property<Type>& value) {
        this->operator=(value);
    }
    
    Property& operator=(const Property<Type>& value) {
        if (*this == value) {
            return;
        }
        
        target = value.target;
        return *this;
    }
    
    bool operator ==(const Property<Type>& value) {
        return target == value.target;
    }
    
};

#endif /* Property_hpp */
