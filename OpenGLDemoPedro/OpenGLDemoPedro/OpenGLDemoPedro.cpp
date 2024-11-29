#include <iostream>
#include "SDL.h"
#include "Texture.h"
#include <glad/glad.h>
#include <string>
#include "objLoader.h"
#include "Shader.h"
#include "Camera.h"
#include <filesystem>
#include "Window.h"
#include <glm/gtc/matrix_access.hpp>

using namespace std;
namespace fs = std::filesystem;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lightPower = 1.0f;
glm::vec3 materialDiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 materialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition ;
float lightAmbientPower = 3;
glm::vec3 materialAmbientColor = glm::vec3(0.1* lightAmbientPower, 0.1* lightAmbientPower, 0.1* lightAmbientPower);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

bool gameIsRunning = true;
float timeElapsed = 0.0f;

glm::mat4 projection;
glm::mat4 model = glm::mat4(1.f);
glm::mat4 view;

Window* windowH;
SDL_Window* window;

int main(int argc, char** argv)
{

	float screenWidth = 800;
	float screenHeight = 600	;
	windowH = new Window("SDL window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight);
	window = windowH->GetWindow();
	SDL_GLContext context = SDL_GL_CreateContext(window);
	camera.setWindow(window);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);


	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}

	// Load Objs_________________________________________________________________________________________________________________________

	std::vector<std::string> file_names;
	std::vector < std::vector< glm::vec3 >> vertices;
	std::vector < std::vector< glm::vec2 >> uvs;
	std::vector < std::vector< glm::vec3 >> normals;

	bool objs = loadOBJS(file_names, vertices, uvs, normals);

	//_________________________________________________________________________________________________________________________

	GLuint vbo;
	glGenBuffers(1, &vbo);
	GLuint ebo;
	glGenBuffers(1, &ebo);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Shader shader( "Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");

	shader.use();

	Texture myTexture;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	myTexture.CreateTextureJPG("Textures/10064_colosseum_diffuse.jpg", texture, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

	GLuint WeaponTex1;
	glGenTextures(1, &WeaponTex1);
	glBindTexture(GL_TEXTURE_2D, WeaponTex1);
	myTexture.CreateTextureJPG("Textures/ak-47.jpg", WeaponTex1, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

	GLuint WeaponTex2;
	glGenTextures(1, &WeaponTex2);
	glBindTexture(GL_TEXTURE_2D, WeaponTex2);
	myTexture.CreateTextureJPG("Textures/ak-47-round.jpg", WeaponTex2, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);


	std::vector<GLuint> uvBuffer(file_names.size());
	std::vector<GLuint> normalbuffer(file_names.size());

	for (int i = 0; i < file_names.size(); i++) {

		if (uvs[i].size() > 0)
		{

			glGenBuffers(1, &uvBuffer[i]);
			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, uvs[i].size() * sizeof(glm::vec2), &uvs[i][0], GL_STATIC_DRAW);

		}

		if (normals[i].size() > 0)
		{

			glGenBuffers(1, &normalbuffer[i]);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, normals[i].size() * sizeof(glm::vec3), &normals[i][0], GL_STATIC_DRAW);
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	shader.setInt("ourTexture", 0);
	view = camera.getViewMatrix();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.set3Float("LightPosition_worldspace", lightPosition.x, lightPosition.y, lightPosition.y);
	shader.set3Float("LightColor", lightColor.x , lightColor.y, lightColor.y);
	shader.setFloat("LightPower", lightPower);
	shader.set3Float("MaterialDiffuseColor", materialDiffuseColor.x, materialDiffuseColor.y, materialDiffuseColor.y);
	shader.set3Float("MaterialSpecularColor", materialSpecularColor.x, materialSpecularColor.y, materialSpecularColor.y);
	shader.set3Float("MaterialAmbientColor", materialAmbientColor.x, materialAmbientColor.y, materialAmbientColor.y);
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
	shader.setMat4("normalMatrix", normalMatrix);

	//Window-----------------------------------------------------------------------------------------------------

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int start = SDL_GetTicks();
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	SDL_Event event;

	while (gameIsRunning)
	{
		int now = SDL_GetTicks();
		float deltaTime = (now - lastFrameTime) / 1000.f;
		lastFrameTime = now;
		timeElapsed += deltaTime;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);

		while (SDL_PollEvent(&event) != 0) // Event Loop
		{
			if (event.type == SDL_MOUSEMOTION) {
				camera.processMouseInput(event.motion.x, event.motion.y);
			}

			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				gameIsRunning = false;
			}
		}

		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		camera.processKeyboardInput(keyState, deltaTime);

		//-----------------------------------------------------------------------------------------------------
		
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 200.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glm::vec3 centerPosition = glm::vec3 (0.f,0.f,0.f);
		float radius = 20.0f;
		float speed = 1.0f;
		float x = centerPosition.x + radius * cos(timeElapsed * speed);
		float z = centerPosition.z + radius * sin(timeElapsed * speed);

		//lightPosition = glm::vec3(x, centerPosition.y, z);
		lightPosition = glm::vec3(x, 20 ,z);
		shader.set3Float("LightPosition_worldspace", lightPosition.x, lightPosition.y, lightPosition.z);

		//-----------------------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();

		for (int i = 0; i < file_names.size(); i++) {

				glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(glm::vec3), &vertices[i][0], GL_STATIC_DRAW);
				shader.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				//shader.setVertexAttribPointer("color", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

				if (uvs[i].size() > 0)
				{
					glBindBuffer(GL_ARRAY_BUFFER, uvBuffer[i]);
					glBufferData(GL_ARRAY_BUFFER, uvs[i].size() * sizeof(glm::vec2), &uvs[i][0], GL_STATIC_DRAW);
					shader.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
				}

				if (normals[i].size() > 0)
				{
					glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[i]);
					glBufferData(GL_ARRAY_BUFFER, normals[i].size() * sizeof(glm::vec3), &normals[i][0], GL_STATIC_DRAW);
					shader.setVertexAttribPointer("normal", 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				}

			model = glm::mat4(1.f);
			if (i == 1)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);

				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));			

				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
				model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));

				shader.setMat4("model", model);
			}
			else if (i == 2 || i == 3)
			{
				if (i == 3)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, WeaponTex1);
				}
				if (i == 2)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, WeaponTex2);
				}

				glm::vec3 cameraPos = camera.getPosition();
				glm::vec3 cameraFront = camera.getRotation();
				glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
				glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront)); 

				model = glm::mat4(1.0f);

				glm::vec3 weaponOffset = cameraPos
					+ cameraFront * 0.5f 
					+ cameraRight * 0.2f 
					- cameraUp * 0.3f;   

				model = glm::translate(model, weaponOffset);

				glm::mat4 rotation = glm::lookAt(glm::vec3(0.0f), cameraFront, cameraUp);
				rotation = glm::transpose(rotation);
				model *= rotation;

				float angle = glm::radians(90.0f);
				glm::mat4 extraRotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, - 1.0f, 0.0f));
				model *= extraRotation;

				model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));

				shader.setMat4("model", model);

			}
			else if (i == 0)
			{
				model = glm::translate(model, lightPosition);

				shader.setMat4("model", model);
			}

			glDrawArrays(GL_TRIANGLES, 0, vertices[i].size());
		}
		glBindVertexArray(vao);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};


