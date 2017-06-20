//
//  IGLRenderable.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/3/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef IGLRenderable_h
#define IGLRenderable_h

#include "IGLDrawable.h"
#include "IVertexed.h"
#include "ITextured.h"

class GLShaderProgram;

struct RenderableConfig {
    DrawableConfig drawablePart;
    VertexedConfig vertexedPart;
    TexturedConfig texturedPart;
};

struct IGLRenderable {
    IGLRenderable() = default;
    virtual ~IGLRenderable() = default;
    
    IGLRenderable(const IGLRenderable&) = delete;
    IGLRenderable(IGLRenderable&&) = delete;
    IGLRenderable& operator=(const IGLRenderable&) = delete;
    IGLRenderable& operator=(IGLRenderable&&) = delete;
    
    virtual glm::mat4 getModel() const = 0;
    
    virtual IGLDrawable* getDrawable() const = 0;
    virtual IVertexed* getVertexed() const = 0;
    virtual ITextured* getTextured() const = 0;
    
    virtual void bind(GLShaderProgram *) = 0;
    
};

#endif /* IGLRenderable_h */
