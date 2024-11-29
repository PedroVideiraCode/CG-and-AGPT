#include <string>

struct SDL_Window;

class Window
{
public:
	Window(std::string title, int windowPosX, int windowPosY, int windowWidth, int windowHeight);

	SDL_Window* GetWindow();

	~Window();
private:
	SDL_Window* window;
};

