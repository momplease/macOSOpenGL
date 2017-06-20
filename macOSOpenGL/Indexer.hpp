//
//  Indexer.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/18/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef Indexer_hpp
#define Indexer_hpp

#include "glm/glm.hpp"
#include <vector>

void indexVBO(
              std::vector<glm::vec3> & in_vertices,
              std::vector<glm::vec2> & in_uvs,
              std::vector<glm::vec3> & in_normals,
              
              std::vector<unsigned int> & out_indices,
              std::vector<glm::vec3> & out_vertices,
              std::vector<glm::vec2> & out_uvs,
              std::vector<glm::vec3> & out_normals
              );


void indexVBO_TBN(
                  std::vector<glm::vec3> & in_vertices,
                  std::vector<glm::vec2> & in_uvs,
                  std::vector<glm::vec3> & in_normals,
                  std::vector<glm::vec3> & in_tangents,
                  std::vector<glm::vec3> & in_bitangents,
                  
                  std::vector<unsigned short> & out_indices,
                  std::vector<glm::vec3> & out_vertices,
                  std::vector<glm::vec2> & out_uvs,
                  std::vector<glm::vec3> & out_normals,
                  std::vector<glm::vec3> & out_tangents,
                  std::vector<glm::vec3> & out_bitangents
                  );

#endif /* Indexer_hpp */
