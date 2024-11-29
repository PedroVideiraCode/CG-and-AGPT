#include "Camera.h"
#include "Window.h"
#include <iostream>
using namespace std;
SDL_Window* windowH;

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 up, float cameraYaw, float cameraPitch)
	: front(glm::vec3(0.f, 0.f, -1.f)), movementSpeed(10.f), mouseSensitivity(0.01f), fov(45.f)
{
	position = cameraPosition;
	worldUp = up;
	yaw = cameraYaw;
	pitch = cameraPitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardInput(const Uint8* keyState, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (keyState[SDL_SCANCODE_W]) move(FORWARD, velocity);
	if (keyState[SDL_SCANCODE_S]) move(BACKWARD, velocity);
	if (keyState[SDL_SCANCODE_A]) move(LEFT, velocity);
	if (keyState[SDL_SCANCODE_D]) move(RIGHT, velocity);
}

void Camera::processMouseInput(float xpos, float ypos) {



	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		SDL_GetWindowSize(windowH, &width, &height);
		std::cout << "Window width: " << width << ", height: " << height << std::endl;
		firstMouse = false;

	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
	SDL_GetWindowSize(windowH, &width, &height);
	SDL_WarpMouseInWindow(windowH, width / 2, height / 2);

	lastX = width / 2;
	lastY = height / 2;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraVectors();

}

void Camera::updateCameraVectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::move(CameraMovement direction, float velocity)
{
	if (direction == FORWARD) position += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
	if (direction == BACKWARD) position -= glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
	if (direction == LEFT) position -= right * velocity;
	if (direction == RIGHT) position += right * velocity;
}

void Camera::setWindow(SDL_Window* window)const
{
	windowH = window;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getRotation() const
{
	return front;
}
