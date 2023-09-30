#pragma once

#include "Shader.h"
#include "Texture.h"

#include <map>
#include <string>

class ResourceManager {
private:
	ResourceManager() { }
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static Shader LoadShader(const char *vShaderPath, const char *fShaderPath, const char *gShaderPath, const char *name);
	static Shader& GetShader(const char * name);
	static Texture2D LoadTexture(const char* file, bool alpha, const char* name);
	static Texture2D& GetTexture(const char* name);
	static void Delete();
};