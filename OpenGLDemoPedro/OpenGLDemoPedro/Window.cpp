#include <SDL.h>
#include "Window.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

Window::Window(std::string title, int windowPosX, int windowPosY, int windowWidth, int windowHeight)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	window = SDL_CreateWindow("OpenGL", windowPosX, windowPosY, windowWidth, windowHeight, SDL_WINDOW_OPENGL);


	if (window == nullptr)
	{

	}
	else if (window)
	{
		std::cout << "Window Created" << std::endl;
	}

}

SDL_Window* Window::GetWindow()
{
	return window;
}

Window::~Window()
{
	SDL_DestroyWindow(window);
}
