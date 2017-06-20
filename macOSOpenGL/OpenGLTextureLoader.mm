//
//  OpenGLTextureLoader.m
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/25/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "OpenGLTextureLoader.hpp"
#import <Cocoa/Cocoa.h>

@implementation OpenGLTextureLoader

- (GLuint)loadTexture2D:(NSString *)filePath {
    
    NSImage* image = [[NSImage alloc] initWithContentsOfFile:filePath];
    
    NSBitmapImageRep* rawImage = [NSBitmapImageRep imageRepWithData:image.TIFFRepresentation];
    
    // OpenGL part
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    GLuint type = rawImage.hasAlpha ? GL_RGBA : GL_RGB;
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 type,
                 (GLsizei)rawImage.pixelsWide,
                 (GLsizei)rawImage.pixelsHigh,
                 0,
                 type,
                 GL_UNSIGNED_BYTE,
                 rawImage.bitmapData);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        
    glGenerateMipmap(GL_TEXTURE_2D);
    
    
    return textureId;
    
}

@end
