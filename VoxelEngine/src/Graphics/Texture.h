#pragma once

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>

#include <string>
#include <memory>

namespace Voxel
{

	class Texture : public Utils::Resource
	{
	public:
		Texture(const std::string& path);
		Texture(Uint32 width, Uint32 height);

		void SetData(void* data, Uint32 size);

		void Destroy() override;

		void Bind(int unit) const;
		void Bind() const;

		Uint32 GetWidth() const { return width; }
		Uint32 GetHeight() const { return height; }

		bool operator==(const Texture& other) const
		{
			return id == other.id;
		}

		static std::shared_ptr<Texture> Create(const std::string& path);
		static std::shared_ptr<Texture> Create(Uint32 width, Uint32 height);
	private:
		Uint32 id;
		Uint32 width;
		Uint32 height;
	};

} // namespace Voxel