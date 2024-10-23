#include <iostream>
#include <SDL.h>

SDL_Texture* LoadTexture(std::string filePath, SDL_Renderer* renderTarget)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
	if (surface == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surface);

	return texture;
}
SDL_Surface* OptimizedSurface(std::string filePath, SDL_Surface* windowSurface)
{
	SDL_Surface* optimizedSurface = nullptr;
	SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
	if (surface == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		optimizedSurface = SDL_ConvertSurface(surface, windowSurface->format, 0);
		if (optimizedSurface == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surface);

	return optimizedSurface;
}

int main(int argc, char* argv[])
{

	SDL_Window* window = nullptr;
	SDL_Texture* currentImage = nullptr;
	SDL_Texture* background = nullptr;
	SDL_Renderer* renderTarget = nullptr;
	SDL_Rect playerRect;
	SDL_Rect playerPosition;
	playerPosition.x = playerPosition.y = 0;
	playerPosition.w = playerPosition.h = 32;
	int frameWidth, frameHeight;
	int textureWidth, textureHeight;
	float frameTime = 0;
	int prevTime = 0;
	int currentTime = 0;
	float deltaTime = 0;

	float moveSpeed = 200.0f;
	const Uint8* keyState;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC);
	currentImage = LoadTexture("graphics/drone.bmp", renderTarget);
	background = LoadTexture("graphics/galaxy2.bmp", renderTarget);

	SDL_QueryTexture(currentImage, NULL, NULL, &textureWidth, &textureHeight);

	frameWidth = textureWidth / 8;
	frameHeight = textureHeight / 2;

	playerRect.x = playerRect.y = 0;
	playerRect.w = frameWidth;
	playerRect.h = frameHeight;

	SDL_SetRenderDrawColor(renderTarget, 0xFF, 0, 0, 0xFF);

	bool isRunning = true;
	SDL_Event ev;

	while (isRunning)
	{
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - prevTime) / 1000.0f;

		while (SDL_PollEvent(&ev) != 0)
		{
			// Getting the events
			if (ev.type == SDL_QUIT)
				isRunning = false;
		}

		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_RIGHT])
			playerPosition.x += moveSpeed * deltaTime;
		else if (keyState[SDL_SCANCODE_LEFT])
			playerPosition.x -= moveSpeed * deltaTime;

		frameTime += deltaTime;

		if (frameTime >= 0.1f)
		{
			frameTime = 0;
			playerRect.x += frameWidth;
			if (playerRect.x >= textureWidth) {
				playerRect.x = 0;
				playerRect.y += frameHeight;
				if (playerRect.y >= textureHeight) {
					playerRect.y = 0;
				}
			}
		}

		SDL_RenderClear(renderTarget);
		SDL_RenderCopy(renderTarget, background, NULL, NULL);
		SDL_RenderCopy(renderTarget, currentImage, &playerRect, &playerPosition);
		SDL_RenderPresent(renderTarget);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(currentImage);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(renderTarget);
	window = nullptr;
	currentImage = nullptr;
	renderTarget = nullptr;

	SDL_Quit();

	return 0;
}


