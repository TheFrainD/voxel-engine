#pragma once

// Headers
#include <Config.h>

#include <array>
#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	namespace BlockType
	{

		enum : Uint32
		{
			Air,
			Grass,
			Stone,
			Dirt,
			Sand,
			Water = 15,
			Bedrock = 18
		};

	} // namespace BlockType

	namespace Side
	{

		enum : Uint32 {
			Top, Bottom,
			Front, Back,
			Right, Left
		};

	} // namespace Side

	struct BlockFace
	{
		bool render = true;
		std::array<glm::vec3, 4> vertices;
		std::array<glm::vec2, 4> uv;
	};

	struct BlockData
	{
		bool transparent;
		std::array<BlockFace, 6> faces;
	};

	struct Block
	{
		static constexpr std::array<std::array<glm::vec3, 4>, 6> base_vertices{
			// top
			std::array<glm::vec3, 4> {
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 1.0f)
			},

			// bottom
			std::array<glm::vec3, 4> {
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 1.0f)
			},

			// front
			std::array<glm::vec3, 4> {
				glm::vec3(1.0f, 0.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
			},

			// back
			std::array<glm::vec3, 4> {
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f)
			},

			// right
			std::array<glm::vec3, 4> {
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 0.0f, 1.0f)
			},

			// left
			std::array<glm::vec3, 4> {
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f)
			}
		};
		static std::array<BlockData, 256> block_data;
		static void LoadData();

		Block() : id(BlockType::Air) { }
		Block(Uint8 id) : id(id) { }
	public:
		Uint8 id;
	private:
		static std::array<glm::vec2, 4> CalculateUV(Uint8 id);
	};

} // namespace Voxel