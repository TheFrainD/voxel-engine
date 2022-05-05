#pragma once

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>

#include <string>
#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	class Shader : public Utils::Resource
	{
	public:
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void Destroy() override;
		void Use() const;

		void UniformFloat(const std::string& name, float value) const;
		void UniformFloatArray(const std::string& name, size_t size, const float data[]) const;
		void UniformVec2(const std::string& name, const glm::vec2& vec) const;
		void UniformVec3(const std::string& name, const glm::vec3& vec) const;
		void UniformVec4(const std::string& name, const glm::vec4& vec) const;
		void UniformMat4(const std::string& name, const glm::mat4& mat) const;
		void UniformInt(const std::string& name, int value) const;
		void UniformIntArray(const std::string& name, size_t size, const int data[]) const;

		static std::shared_ptr<Shader> Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	private:
		Uint32 id;
	};

} // namespace Voxel
