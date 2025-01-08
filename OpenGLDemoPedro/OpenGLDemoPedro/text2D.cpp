#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "Shader.h"
#include "texture.h"
#include "text2D.h"

#include "SDL.h"
#include <glad/glad.h>
#include "Window.h"

using namespace glm;
#include <iostream>

#include <string>
#include "objLoader.h"

#include "Camera.h"
#include <filesystem>

using namespace std;
GLuint Text2DTextureID;
GLuint Text2DVertexBufferID;
GLuint Text2DUVBufferID;
GLuint Text2DUniformID;
GLuint Text2DShaderID;
const char* texturePathId;


void initText2D(const char* texturePath)
{

	texturePathId = texturePath;

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);

	glGenBuffers(1, &Text2DUVBufferID);
	Texture myTexture;
	glGenTextures(1, &Text2DTextureID);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	myTexture.CreateTextureBMP(texturePathId, Text2DTextureID, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

}

void printText2D(glm::vec4 srcRect, glm::vec4 dstRect, int sourtingLayer, glm::vec3 keyColor)
	{

	static Shader shaders("Shaders/Text2DVertexShader.glsl", "Shaders/Text2DFragmentShader.glsl");

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

	for (unsigned int i = 0; i < 1; i++) {

		glm::vec2 vertex_up_left = glm::vec2(dstRect.x, dstRect.y);
		glm::vec2 vertex_up_right = glm::vec2(dstRect.x + dstRect.z, dstRect.y);
		glm::vec2 vertex_down_right = glm::vec2(dstRect.x + dstRect.z, dstRect.y + dstRect.w);
		glm::vec2 vertex_down_left = glm::vec2(dstRect.x, dstRect.y + dstRect.w);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		int textureWidth;
		int textureHeight;
		int nrChannels;

		stbi_load(texturePathId, &textureWidth, &textureHeight, &nrChannels, 0);

		int reckX = 0, reckY = 0, Reckh = textureHeight, Reckw = textureWidth;

		float uv_x = reckX / textureWidth;
		float uv_y = reckY / textureHeight;
		float uv_w = Reckw / textureWidth;
		float uv_h = Reckh / textureHeight;

		// Define as novas coordenadas UV
		glm::vec2 uv_up_left = glm::vec2(0, 0);
		glm::vec2 uv_up_right = glm::vec2(1, 0);
		glm::vec2 uv_down_right = glm::vec2(1, 1);
		glm::vec2 uv_down_left = glm::vec2(0, 1);

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}


	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);


	// Usar o shader
	shaders.use();

	std::cout << "Binding Texture ID: " << Text2DTextureID << std::endl;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error in glBindTexture: " << error << std::endl;
	}

	glUniform1i(Text2DTextureID, 0);
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error in glUniform1i: " << error << std::endl;
	}

	std::cout << "Binding Vertex Buffer ID: " << Text2DVertexBufferID << std::endl;
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);

	if (error != GL_NO_ERROR) {
		std::cerr << "Error in glBindBuffer (Vertex): " << error << std::endl;
	}
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error in glVertexAttribPointer (Vertex): " << error << std::endl;
	}

	// Configurar atributo 1
	std::cout << "Binding UV Buffer ID: " << Text2DUVBufferID << std::endl;
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error in glBindBuffer (UV): " << error << std::endl;
	}
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error in glVertexAttribPointer (UV): " << error << std::endl;
	}


	// Desenhar
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}
void cleanupText2D()
{

	// Delete buffers
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &Text2DTextureID);

	//// Delete shader
	//shaders.~Shader();
}