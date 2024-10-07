
#include <iostream>
#include "SDL.h"
#include <glad/glad.h>

//#undef main
//#ifdef __APPLE__
//#include <SDL2/SDL.h>
//#elif _WIN32
//#include <SDL.h>
//#endif

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	float vertices[] = {
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  
		0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f,  0.0f, 0.0f, 0.0f
	};

	//float -> 32 bit = 4 bytes
	//double -> 64 bit = 8 bytes


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//Vertex Shader--------------------------------------------------------------------------------------------------------------------------------------------------------------------
	const char* vertexShaderSource = R"glsl(
	#version 330 core
	layout (location = 0) in vec2 position;
	in vec3 color;

	out vec3 Color;
	void main()
	{
		Color = color;
		gl_Position = vec4(position, 0.0, 1.0);
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

	const char* fragmentShaderSource = R"glsl(
	#version 330 core
	in vec3 Color;

	out vec4 outColor;

	void main()
	{
		outColor = vec4(Color, 1.0);
	}
	)glsl";

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

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);  // Attach the vertex shader
	glAttachShader(shaderProgram, fragmentShader);  // Attach the fragment shader

	glLinkProgram(shaderProgram);  // Link the shader program

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader); // Delete the vertex shader
	glDeleteShader(fragmentShader); // Delete the fragment shader

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)(2 * sizeof(float)));

//Window-----------------------------------------------------------------------------------------------------

	SDL_Event windowEvent;

	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 1, 4);

		//float time = SDL_GetTicks()/1000.f;

		//GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
		//glUniform3f(uniColor, (sin(time+180) + 1)/2, (cos(time) + 1) / 2, ((cos(time) + 1))* ((sin(time) + 1)));

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}