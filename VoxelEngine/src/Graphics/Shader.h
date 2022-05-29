#pragma once

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>

#include <string>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Voxel
{

	class Shader : public Utils::Resource
	{
	public:
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void Destroy() override;
		void Use() const;

		Int32 GetUniformLocation(const std::string& name);
		void UniformFloat(const std::string& name, float value);
		void UniformFloatArray(const std::string& name, size_t size, const float data[]);
		void UniformVec2(const std::string& name, const glm::vec2& vec);
		void UniformVec3(const std::string& name, const glm::vec3& vec);
		void UniformVec4(const std::string& name, const glm::vec4& vec);
		void UniformMat4(const std::string& name, const glm::mat4& mat);
		void UniformInt(const std::string& name, int value);
		void UniformIntArray(const std::string& name, size_t size, const int data[]);

		static std::shared_ptr<Shader> Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	private:
		Uint32 id;
		std::unordered_map<std::string, Int32> uniformLocationsCache;
	};

} // namespace Voxel
