//
//  OpenGLShaderLoader.m
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/23/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "OpenGLShaderLoader.hpp"
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "ArrayDeleter.hpp"

using std::string;
using std::ifstream;
using std::getline;
using std::tuple;
using std::make_tuple;
using std::get;
using std::shared_ptr;
using std::map;

// OpenGLShaderLoader
@implementation OpenGLShaderLoader

/*- (GLuint)loadShadersVertex:(NSString *)vertexFilePath fragment:(NSString *)fragmentFilePath {
    
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    
    string vertexShaderCode = [self loadRawShaderCode:[vertexFilePath UTF8String]];
    string fragmentShaderCode = [self loadRawShaderCode:[fragmentFilePath UTF8String]];
    
    
    assert(vertexShaderCode != "" && fragmentShaderCode != "");
    
    
    [self compileShader:vertexShaderId withSourceCode:vertexShaderCode];
    [self compileShader:fragmentShaderId withSourceCode:fragmentShaderCode];
    
    ShaderIdsContainer shaderIds = {{kVertexShaderKey,   vertexShaderId},
                                    {kFragmentShaderKey, fragmentShaderId}};
    
    
    return [self createShaderProgram:shaderIds
                      withCompletion:^(void){
                          glDeleteShader(vertexShaderId);
                          glDeleteShader(fragmentShaderId);
                      }];
    
    
}*/


- (GLuint)createShaderProgramWith:(GLuint)vertexShaderId
                              _:(GLuint)fragmentShaderId
                       completion:(dispatch_block_t)completion{
    GLuint result = [self createShaderProgramWith:vertexShaderId
                                                _:fragmentShaderId];
    
    completion();
    return result;
}

- (GLuint)createShaderProgramWith:(GLuint)vertexShaderId
                                _:(GLuint)fragmentShaderId {
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    
    tuple<bool, string> compileResult = [self checkIfShaderProgramLinked:programId];
    bool compiled = get<0>(compileResult);
    if (!compiled) {
        NSLog(@"Shader program is not linked\nLink log: %s", get<1>(compileResult).c_str());
    }
    return programId;
}


- (tuple<bool, string>)checkIfShaderProgramLinked:(GLuint)programId {
    GLint result = GL_FALSE;
    GLint logLength = 0;
    
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        // using ArrayDeleter to delete memory properly
        GLint length = logLength + 1;
        std::vector<GLchar> infoLog(length);
        glGetProgramInfoLog(programId, length, 0, infoLog.data());
        return make_tuple(false, string(infoLog.begin(), infoLog.end()));
    }
    return make_tuple(true, "");
}


- (void)compileShader:(GLuint)shaderId withSourceCode:(string)shaderSourceCode {
    char const *shaderSourcePointer = shaderSourceCode.c_str();
    glShaderSource(shaderId, 1, &shaderSourcePointer, NULL);
    glCompileShader(shaderId);
    
    tuple<bool, string> compileResult = [self checkIfShaderCompiled:shaderId];
    bool compiled = std::get<0>(compileResult);
    if (!compiled) {
        NSLog(@"%s is not compiled\nCompile log: %s", shaderSourceCode.c_str(), std::get<1>(compileResult).c_str());
        return;
    }
    
}

- (tuple<bool, string>)checkIfShaderCompiled:(GLuint)shaderId {
    GLint result = GL_FALSE;
    GLint logLength;
    
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0) {
        // using ArrayDeleter to properly delete memory
        shared_ptr<GLchar> infoLog(new GLchar[logLength + 1], ArrayDeleter<GLchar>());
        glGetShaderInfoLog(shaderId, logLength, NULL, infoLog.get());
        return make_tuple(false, string(infoLog.get()));
    }
    return make_tuple(true, "");
    
}

- (string)loadRawShaderCode:(string)filePath {
    string shaderCode = "";
    ifstream shaderStream;
    
    shaderStream.open(filePath);
    if (shaderStream.is_open()) {
        
        string buffer;
        
        while(getline(shaderStream, buffer)) {
            shaderCode += buffer + "\n";
        }
        
        shaderStream.close();
        
    }
    return shaderCode;
}



@end
