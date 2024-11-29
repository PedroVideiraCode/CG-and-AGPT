#include "objLoader.h"
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;
#define _CRT_SECURE_NO_WARNINGS

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    FILE* file;
    errno_t err = fopen_s(&file, path, "r");
    if (err != 0) {
        printf("Impossible to open the file !\n");
        return false;
    }

    while (1) {
        char lineHeader[128];
        int res = fscanf_s(file, "%s", lineHeader, (unsigned)_countof(lineHeader));
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }

        else if (strcmp(lineHeader, "f") == 0) {

            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
            long posicao_armazenada = ftell(file);
            if (int matches = fscanf_s(file, "%u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2],
                &vertexIndex[3], &uvIndex[3], &normalIndex[3]))
            {
                if (matches == 12)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[3]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[2]);
                    uvIndices.push_back(uvIndex[3]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[2]);
                    normalIndices.push_back(normalIndex[3]);
                }
                if (matches == 9)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }
                if (matches == 3)
                {
                    fseek(file, posicao_armazenada, SEEK_SET);
                    if (int matches = fscanf_s(file, "%u/%u/%u\n",
                        &vertexIndex[0],
                        &vertexIndex[1],
                        &vertexIndex[2]))
                    {
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                    }
                }
            }
            fseek(file, posicao_armazenada, SEEK_SET);
            if (int matches = fscanf_s(file, "%u/%u %u/%u %u/%u %u/%u\n",
                &vertexIndex[0], &uvIndex[0],
                &vertexIndex[1], &uvIndex[1],
                &vertexIndex[2], &uvIndex[2],
                &vertexIndex[3], &uvIndex[3]))
            {
                if (matches == 8)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[3]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[2]);
                    uvIndices.push_back(uvIndex[3]);
                }
                if (matches == 6)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                }
            }
            fseek(file, posicao_armazenada, SEEK_SET);
            if (int matches = fscanf_s(file, "%u %u %u %u %u %u %u %u %u %u %u %u\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2],
                &vertexIndex[3], &uvIndex[3], &normalIndex[3]))
            {
                if (matches == 12)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[3]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[2]);
                    uvIndices.push_back(uvIndex[3]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[2]);
                    normalIndices.push_back(normalIndex[3]);
                }
                if (matches == 9)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }
                if (matches == 3)
                {
                    fseek(file, posicao_armazenada, SEEK_SET);
                    if (int matches = fscanf_s(file, "%u %u %u\n",
                        &vertexIndex[0],
                        &vertexIndex[1],
                        &vertexIndex[2]))
                    {
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);

                    }
                }
            }
        }

    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++)
    {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[normalIndex - 1];
        out_normals.push_back(normal);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++)
    {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 u = temp_uvs[uvIndex - 1];
        out_uvs.push_back(u);
    }
}

bool loadOBJS(std::vector<std::string>& out_file_names, std::vector<std::vector<glm::vec3>>& out_vertices, std::vector<std::vector<glm::vec2>>& out_uvs, std::vector<std::vector<glm::vec3>>& out_normals)
{
    std::string path = "Obj";
    std::vector<std::string> file_names;

    std::vector < std::vector< glm::vec3 >> vertices;
    std::vector < std::vector< glm::vec2 >> uvs;
    std::vector < std::vector< glm::vec3 >> normals;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string str = entry.path().filename().string();
            file_names.push_back("Obj/" + str);
            out_file_names.push_back("Obj/" + str);
        }
    }

    for (int i = 0; i < file_names.size(); i++) {
        // Initialize new vectors for each file
        std::vector<glm::vec3> file_vertices;
        std::vector<glm::vec2> file_uvs;
        std::vector<glm::vec3> file_normals;

        // Load the OBJ file into the corresponding vectors
        bool obj = loadOBJ(file_names[i].c_str(), file_vertices, file_uvs, file_normals);

        if (obj) {
            // Store the loaded data into the main vectors
            out_vertices.push_back(file_vertices);
            out_uvs.push_back(file_uvs);
            out_normals.push_back(file_normals);
        }
        else
        {
            return false;
        }
    }
}