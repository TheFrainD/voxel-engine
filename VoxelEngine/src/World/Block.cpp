#include <vepch.h>
#include "Block.h"

// Headers
#include <Config.h>

#include <array>
#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	std::array<BlockData, 256> Block::block_data;

	void Block::LoadData() {
		BlockData bd;

		// Air
		bd.transparent = true;
		block_data[BlockType::Air] = bd;

		// Grass
		bd.transparent = false;
		bd.faces[Side::Top].vertices = base_vertices[Side::Top];
		bd.faces[Side::Top].uv = CalculateUV(1);
		bd.faces[Side::Bottom].vertices = base_vertices[Side::Bottom];
		bd.faces[Side::Bottom].uv = CalculateUV(3);
		bd.faces[Side::Front].vertices = base_vertices[Side::Front];
		bd.faces[Side::Front].uv = CalculateUV(5);
		bd.faces[Side::Back].vertices = base_vertices[Side::Back];
		bd.faces[Side::Back].uv = CalculateUV(5);
		bd.faces[Side::Right].vertices = base_vertices[Side::Right];
		bd.faces[Side::Right].uv = CalculateUV(5);
		bd.faces[Side::Left].vertices = base_vertices[Side::Left];
		bd.faces[Side::Left].uv = CalculateUV(5);
		block_data[BlockType::Grass] = bd;

		// Stone
		bd.transparent = false;
		bd.faces[Side::Top].vertices = base_vertices[Side::Top];
		bd.faces[Side::Top].uv = CalculateUV(2);
		bd.faces[Side::Bottom].vertices = base_vertices[Side::Bottom];
		bd.faces[Side::Bottom].uv = CalculateUV(2);
		bd.faces[Side::Front].vertices = base_vertices[Side::Front];
		bd.faces[Side::Front].uv = CalculateUV(2);
		bd.faces[Side::Back].vertices = base_vertices[Side::Back];
		bd.faces[Side::Back].uv = CalculateUV(2);
		bd.faces[Side::Right].vertices = base_vertices[Side::Right];
		bd.faces[Side::Right].uv = CalculateUV(2);
		bd.faces[Side::Left].vertices = base_vertices[Side::Left];
		bd.faces[Side::Left].uv = CalculateUV(2);
		block_data[BlockType::Stone] = bd;

		// Dirt
		bd.transparent = false;
		bd.faces[Side::Top].vertices = base_vertices[Side::Top];
		bd.faces[Side::Top].uv = CalculateUV(3);
		bd.faces[Side::Bottom].vertices = base_vertices[Side::Bottom];
		bd.faces[Side::Bottom].uv = CalculateUV(3);
		bd.faces[Side::Front].vertices = base_vertices[Side::Front];
		bd.faces[Side::Front].uv = CalculateUV(3);
		bd.faces[Side::Back].vertices = base_vertices[Side::Back];
		bd.faces[Side::Back].uv = CalculateUV(3);
		bd.faces[Side::Right].vertices = base_vertices[Side::Right];
		bd.faces[Side::Right].uv = CalculateUV(3);
		bd.faces[Side::Left].vertices = base_vertices[Side::Left];
		bd.faces[Side::Left].uv = CalculateUV(3);
		block_data[BlockType::Dirt] = bd;
	}

	std::array<glm::vec2, 4> Block::CalculateUV(Uint32 id)
	{
		float uv_size = 1.0f / 16.0f;
		float u = ((id - 1) % 16) * uv_size;
		float v = ((id - 1) / 16) * uv_size;
		float uu = u + uv_size;
		float vv = v + uv_size;

		return std::array<glm::vec2, 4> {
			glm::vec2(uu, vv),
			glm::vec2(uu, v ),
			glm::vec2(u,  v ),
			glm::vec2(u,  vv),
		};
	}

} // namespace Voxel