//
//  OpenGLShaderLoader.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/23/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGL/gl.h>

#include <string>

@interface OpenGLShaderLoader : NSObject

// Deprecated
//- (GLuint)loadShadersVertex:(NSString *)vertexFilePath fragment:(NSString *)fragmentFilePath;

- (std::string)loadRawShaderCode:(std::string)filePath;

- (void)compileShader:(GLuint)shaderId
       withSourceCode:(std::string)shaderSourceCode;

- (GLuint)createShaderProgramWith:(GLuint)vertexShaderId
                                _:(GLuint)fragmentShaderId;

- (GLuint)createShaderProgramWith:(GLuint)vertexShaderId
                                _:(GLuint)fragmentShaderId
                       completion:(dispatch_block_t)completion;

@end
