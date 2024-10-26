
#include <iostream>
#include "SDL.h"
#include <glad/glad.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>	// get the the standard-library math function interface including sqrt()
#include "objLoader.h"

using namespace std;
//#undef main
//#ifdef __APPLE__
//#include <SDL2/SDL.h>
//#elif _WIN32
//#include <SDL.h>
//#endif

float screenWidth = 800;
float screenHeight = 600;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);



	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cout << "Failed to create SDL Window" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}


	std::vector< glm::vec3 > vertices_;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("1.obj", vertices_, uvs, normals);

	GLuint vbo; // vertex buffer object
	glGenBuffers(1, &vbo); // Generate 1 buffer

	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint vao;
	glGenVertexArrays(1, &vao);

	// 1. bind Vertex Array Object
	glBindVertexArray(vao);

	// 2. copy our vertices array in a buffer for OpenGL to use



	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &vertices_[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

//Vertex Shader--------------------------------------------------------------------------------------------------------------------------------------------------------------------
	const char* vertexShaderSource = R"glsl(
	in vec3 position;
	in vec3 color;
	in vec2 texCoord;

	out vec3 Color;
	out vec2 TexCoord;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		Color = color;
		TexCoord = texCoord;
		gl_Position = projection * view * model * vec4(position, 1.0);
	}
		)glsl";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach the shader source code
	glCompileShader(vertexShader); // Compile the shader

	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

//Fragment Shader-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	//outColor = mix(colTex1, colTex2, 0.5);

	const char* fragmentShaderSource = R"glsl(
		#version 330 core
		in vec3 Color;
		in vec2 TexCoord;

		out vec4 outColor;

		uniform sampler2D ourTexture;
		uniform sampler2D ourTexture2;

		void main()
		{
			vec4 colTex1 = texture(ourTexture, TexCoord);
			vec4 colTex2 = texture(ourTexture2, TexCoord);

			outColor = colTex1;

		})glsl";

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create a fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Attach the shader source code
	glCompileShader(fragmentShader); // Compile the shader

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

//Shader Program---------------------------------------------------------------------------------------------------------------------------------------------------------------

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 3. then set our vertex attributes pointers
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	if (uvs.size()>0)
	{
		GLuint uvBuffer;
		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

		GLint texCoordAttribObj = glGetAttribLocation(shaderProgram, "texCoord");
		glEnableVertexAttribArray(texCoordAttribObj);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(texCoordAttribObj, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)(0 * sizeof(float)));
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("PenguinDiffuseColor.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);


	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glUseProgram(shaderProgram);

	GLuint textureLocation;
	GLuint textureLocation2;

	textureLocation = glGetUniformLocation(shaderProgram, "ourTexture");
	textureLocation2 = glGetUniformLocation(shaderProgram, "ourTexture2");

	glUniform1i(textureLocation, 0);
	glUniform1i(textureLocation2, 1);

	//Window-----------------------------------------------------------------------------------------------------
	int start = SDL_GetTicks();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	SDL_Event windowEvent;

	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}

		int now = SDL_GetTicks();
		float time = (now - start) / 1000.0f;

		//-----------------------------------------------------------------------------------------------------

		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));

		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5));
		view = glm::rotate(view, time, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

		GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		GLuint viewLocation = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		GLuint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		//-----------------------------------------------------------------------------------------------------
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw .obj ---------------------------------------------------------------------------------------------------------------------
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

//GLuint normalbuffer;
//glGenBuffers(1, &normalbuffer);
//glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

//-----------------------------------------------------------------------------------------------------

//GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
//glEnableVertexAttribArray(colorAttrib);
//glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// 
//for (unsigned int i = 0; i < 10; ++i)
//{
//	glm::mat4 model(1.0f);
//	model = glm::translate(model, cubePositions[i]);
//	model = glm::rotate(model, time, glm::vec3(0.5f, 1.0f, 0.0f));
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

//	glDrawArrays(GL_TRIANGLES, 0, 36);
//}

//glm::mat4 trans(1.0f);
////trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
////trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));
////trans = glm::scale(trans, glm::vec3(glm::cos(time), glm::cos(time), 1.0f));
//unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//float vertices[] = {
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//};
//
//glm::vec3 cubePositions[] = {
//glm::vec3(0.0f,  0.0f,  0.0f),
//glm::vec3(2.0f,  5.0f, -15.0f),
//glm::vec3(-1.5f, -2.2f, -2.5f),
//glm::vec3(-3.8f, -2.0f, -12.3f),
//glm::vec3(2.4f, -0.4f, -3.5f),
//glm::vec3(-1.7f,  3.0f, -7.5f),
//glm::vec3(1.3f, -2.0f, -2.5f),
//glm::vec3(1.5f,  2.0f, -2.5f),
//glm::vec3(1.5f,  0.2f, -1.5f),
//glm::vec3(-1.3f,  1.0f, -1.5f)
//};
//
////float -> 32 bit = 4 bytes
////double -> 64 bit = 8 bytes