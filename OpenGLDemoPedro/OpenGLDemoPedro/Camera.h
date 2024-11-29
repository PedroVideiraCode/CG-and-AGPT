#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

struct SDL_Window;

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

class Camera
{
public:
	Camera(glm::vec3 cameraPosition, glm::vec3 up, float yaw, float pitch);

	glm::mat4 getViewMatrix() const;

	void processKeyboardInput(const Uint8* keyState, float deltaTime);
	void processMouseInput(float xpos, float ypos);
	float getFov() const { return fov; }

	void setWindow(SDL_Window* window)const;

	glm::vec3 getPosition() const;

	glm::vec3 getRotation() const;

private:
	void updateCameraVectors();
	void move(CameraMovement direction, float velocity);
	int width, height;

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float fov;

	float lastX, lastY;
	bool firstMouse;
};
