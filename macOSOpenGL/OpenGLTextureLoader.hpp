//
//  OpenGLTextureLoader.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/25/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGL/gl.h>

@interface OpenGLTextureLoader : NSObject

- (GLuint)loadTexture2D:(NSString *)filePath;

@end
