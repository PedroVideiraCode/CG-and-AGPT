#include "Texture.h"
#include "stb_image.h"
#include <iostream>
using namespace std;

void Texture::CreateTextureJPG(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const
{

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(nameOfTexture, &width, &height, &nrChannels, 0);
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
}

void Texture::CreateTexturePNG(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(nameOfTexture, &width, &height, &nrChannels, 0);
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
}

void Texture::CreateTextureTGA(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Inverte a textura verticalmente
	unsigned char* data = stbi_load(nameOfTexture, &width, &height, &nrChannels, 0);
	if (data) {
		// Determina o formato da imagem
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

		// Envia a textura para o OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << "Textura carregada com sucesso: " << nameOfTexture << std::endl;
	}
	else {
		std::cerr << "Erro ao carregar textura: " << nameOfTexture << std::endl;
	}

	stbi_image_free(data);
}

void Texture::CreateTextureBMP(const char* nameOfTexture, GLuint texture, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) const
{
	// Configura os parâmetros de textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // Inverte a textura verticalmente

	// Carrega o arquivo BMP
	unsigned char* data = stbi_load(nameOfTexture, &width, &height, &nrChannels, 0);
	if (data) {
		// Determina o formato da imagem com base nos canais
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

		// Envia a textura para o OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << "Textura BMP carregada com sucesso: " << nameOfTexture << std::endl;
	}
	else {
		std::cerr << "Erro ao carregar textura BMP: " << nameOfTexture << std::endl;
	}

	// Libera a memória da imagem
	stbi_image_free(data);
}