//
//  FBXLoader.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/4/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "fbxsdk.h"

#include <vector>
#include <string>
#include <unordered_map>
#import "glm/glm.hpp"

using std::vector;
using std::string;
using std::unordered_map;
using glm::vec3;
using glm::vec2;

@interface FBXLoader : NSObject

-(void)loadWithFilename:(NSString *)filename;

-(vector<vec3>)vertices;
-(vector<unsigned int>)indices;
-(vector<vec3>)indexedVertices;
-(vector<vec3>)normals;

-(unordered_map<string, vector<vec2>>)UVs;
-(vector<string>)UVKeys;

-(vector<vec3>)secondMesh;
@end
