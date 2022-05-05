#include <vepch.h>
#include "Shader.h"

// Headers
#include <Config.h>
#include <Core/Log.h>
#include <Utils/Files.h>
#include <Utils/ResourceManager.h>

#include <string>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CompileType : GLenum
{
	COMPILE_TYPE_PROGRAM,
	COMPILE_TYPE_VERTEX = GL_VERTEX_SHADER,
	COMPILE_TYPE_FRAGMENT = GL_FRAGMENT_SHADER
};

static void OpenGLError()
{
	int error = glGetError();
	while (error != GL_NO_ERROR)
	{
		VE_LOG_ERROR("OpenGL error: {}", error);
		error = glGetError();
	}
}

static void LogError(GLuint handle, GLenum type)
{
	GLint length;
	if (type == COMPILE_TYPE_PROGRAM)
	{
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
	}
	else 
	{
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);
	}

	if (length > 0)
	{
		GLint ch;
		GLchar* log = new GLchar[length];
		if (type == COMPILE_TYPE_PROGRAM)
		{
			glGetProgramInfoLog(handle, length, &ch, log);
			VE_LOG_ERROR("Shader program error: {}", log);
		}
		else
		{
			glGetShaderInfoLog(handle, length, &ch, log);
			VE_LOG_ERROR("{} shader error: {}", (type == COMPILE_TYPE_VERTEX ? "Vertex" : "Fragment"), log);
		}
		delete[] log;
	}
}

static GLuint CreateShader(const GLchar* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);

	glCompileShader(shader);
	OpenGLError();
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		LogError(shader, type);
	}

	return shader;
}

static GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	OpenGLError();
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		LogError(program, COMPILE_TYPE_PROGRAM);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

namespace Voxel
{

	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		std::string vertexShaderSource_str = Utils::ReadFile(vertexShaderPath);
		std::string fragmentShaderSource_str = Utils::ReadFile(fragmentShaderPath);
		const char* vertexShaderSource = vertexShaderSource_str.c_str();
		const char* fragmentShaderSource = fragmentShaderSource_str.c_str();

		GLuint vertexShader = CreateShader(vertexShaderSource, COMPILE_TYPE_VERTEX);
		GLuint fragmentShader = CreateShader(fragmentShaderSource, COMPILE_TYPE_FRAGMENT);

		id = CreateProgram(vertexShader, fragmentShader);

		Utils::ResourceManager::Add(this);
	}

	void Shader::Destroy()
	{
		glDeleteProgram(id);
	}

	void Shader::Use() const {
		glUseProgram(id);
	}

	void Shader::UniformFloat(const std::string& name, float value) const {
		Use();
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}

	void Shader::UniformFloatArray(const std::string& name, size_t size, const float data[]) const {
		Use();
		glUniform1fv(glGetUniformLocation(id, name.c_str()), size, data);
	}

	void Shader::UniformVec2(const std::string& name, const glm::vec2& vec) const {
		Use();
		glUniform2f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y);
	}

	void Shader::UniformVec3(const std::string& name, const glm::vec3& vec) const {
		Use();
		glUniform3f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z);
	}

	void Shader::UniformVec4(const std::string& name, const glm::vec4& vec) const {
		Use();
		glUniform4f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::UniformMat4(const std::string& name, const glm::mat4& mat) const {
		Use();
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, false, glm::value_ptr(mat));
	}

	void Shader::UniformInt(const std::string& name, int value) const {
		Use();
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	void Shader::UniformIntArray(const std::string& name, size_t size, const int data[]) const {
		Use();
		glUniform1iv(glGetUniformLocation(id, name.c_str()), size, data);
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		return std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
	}

} // namespace Voxel