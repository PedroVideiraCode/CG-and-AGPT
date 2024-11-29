#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool loadOBJ(
    const char* path,
    std::vector < glm::vec3 >& out_vertices,
    std::vector < glm::vec2 >& out_uvs,
    std::vector < glm::vec3 >& out_normals
);

bool loadOBJS(
    std::vector<std::string>& out_file_names,
    std::vector < std::vector< glm::vec3 >>& out_vertices,
    std::vector < std::vector< glm::vec2 >>& out_uvs,
    std::vector < std::vector< glm::vec3 >>& out_normals
);

#endif