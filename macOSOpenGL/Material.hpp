//
//  Material.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/11/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <memory>
#include "GLObject.hpp"

class GLShader;

class Material : public GLObject {
public:
    Material(GLShader *);
    virtual ~Material();
    
    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;
    Material(Material&&) = delete;
    Material& operator=(Material&&) = delete;
    
    virtual void prepareOpenGL() override;
    
    GLShader* getShader() const;
    
protected:
    std::unique_ptr<GLShader> shader;
};

#endif /* Material_hpp */
