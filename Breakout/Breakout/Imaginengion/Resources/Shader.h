#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
	unsigned int ProgramID = -1;
	const char *Name = "";
public:
	Shader() { }

	void Init(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode, const char* name) {
		Name = name;

		//compile and link shaders to program
		unsigned int vertex, fragment, geometry;
		int success;
		char infoLog[512];

		//vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 5121, NULL, infoLog);
			std::cout << "ERROR::SHADER:FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//g shader if it exists
		if (gShaderCode) {
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragment, 5121, NULL, infoLog);
				std::cout << "ERROR::SHADER:FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		//create program and attach shaders
		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vertex);
		glAttachShader(ProgramID, fragment);
		if (gShaderCode) {
			glAttachShader(ProgramID, geometry);
		}
		glLinkProgram(ProgramID);
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ProgramID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		//clean up shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (gShaderCode) {
			glDeleteShader(geometry);
		}
	}

	void use() {
		glUseProgram(ProgramID);
	}

	//utility uniform functions
	template <class T>
	void setUniform(const std::string& name, T value) {}
	template <>
	void setUniform<bool>(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
	}
	template <>
	void setUniform<int>(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
	}
	template <>
	void setUniform<float>(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
	};
	template <>
	void setUniform<glm::vec2>(const std::string& name, glm::vec2 value) {
		glUniform2f(glGetUniformLocation(ProgramID, name.c_str()), value.x, value.y);
	}
	template <>
	void setUniform<glm::vec3>(const std::string& name, glm::vec3 value) {
		glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), value.x, value.y, value.z);
	}
	template <>
	void setUniform<glm::vec4>(const std::string& name, glm::vec4 value) {
		glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), value.x, value.y, value.z, value.w);
	}
	template <>
	void setUniform<glm::mat4>(const std::string& name, glm::mat4 value) {
		glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	//for cleaning up after window should close
	void Delete() {
		glDeleteProgram(ProgramID);
	}
};