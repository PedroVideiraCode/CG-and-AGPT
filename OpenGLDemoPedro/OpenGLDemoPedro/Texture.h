#pragma once
#include "glad/glad.h"
#include <string>
#include <glm/glm.hpp>

class Texture
{
public:
	void CreateTextureJPG(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const;
	void CreateTexturePNG(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const;
	void CreateTextureTGA(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const;
	void CreateTextureBMP(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const;

};
