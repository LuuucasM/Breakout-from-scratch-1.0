#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "ResourceManager.h"

#include <fstream>

Shader ResourceManager::LoadShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath, const char *name) {
	//retrive code from files
	std::string vShaderSource;
	std::string fShaderSource;
	std::string gShaderSource;

	std::ifstream vShaderFile, fShaderFile, gShaderFile;

	std::stringstream vShaderStream, fShaderStream, gShaderStream;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vShaderPath);
		fShaderFile.open(fShaderPath);
		

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderSource = vShaderStream.str();
		fShaderSource = fShaderStream.str();

		if (gShaderPath)
		{
			gShaderFile.open(gShaderPath);
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			gShaderSource = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULY READ" << e.what() << std::endl;
	}
	const char *vShaderCode = vShaderSource.c_str();
	const char *fShaderCode = fShaderSource.c_str();
	const char* gShaderCode = gShaderSource.c_str();
	Shader shader;
	shader.Init(vShaderCode, fShaderCode, gShaderPath != nullptr ? gShaderCode : nullptr, name);
	Shaders[name] = shader;
	return shader;
}
Shader& ResourceManager::GetShader(const char *name) {
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, const char* name) {
	Texture2D texture;
	if (alpha) {
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	texture.Generate(width, height, data);
	stbi_image_free(data);
	Textures[name] = texture;
	return texture;
}
Texture2D& ResourceManager::GetTexture(const char* name) {
	return Textures[name];
}

void ResourceManager::Delete() {
	for (auto i : Shaders) {
		i.second.Delete();
	}
	for (auto i : Textures) {
		glDeleteTextures(1, &i.second.ID);
	}
}