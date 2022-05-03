#pragma once

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>

#include <string>

namespace Voxel
{

	class Texture : public Utils::Resource
	{
	public:
		void Load(const std::string& path);

		void Destroy() override;

		void Bind(int unit) const;
		void Bind() const;

		Uint32 GetWidth() const { return width; }
		Uint32 GetHeight() const { return height; }
	private:
		Uint32 id;
		Uint32 width;
		Uint32 height;
	};

} // namespace Voxel