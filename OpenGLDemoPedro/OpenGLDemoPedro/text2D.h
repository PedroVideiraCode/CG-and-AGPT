#pragma once
#include "shader.h"


	void initText2D(const char* texturePath);
	void printText2D(glm::vec4 srcRect, glm::vec4 dstRect, int sourtingLayer, glm::vec3 keyColor);
	void cleanupText2D();

	
