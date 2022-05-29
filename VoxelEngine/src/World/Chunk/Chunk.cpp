#include <vepch.h>
#include "Chunk.h"

// Headers
#include <Config.h>
#include <World/Block.h>
#include <World/Chunk/ChunkMesh.h>

#include <memory>
#include <array>

#include <glm/glm.hpp>

namespace Voxel
{

	Chunk::Chunk(const glm::ivec3& position) :
		position(position), modified(true)
	{
		mesh = ChunkMesh::Create(volume);
		mesh->position = position * size;
	}

	void Chunk::GenerateMesh()
	{
		if (!modified)
			return;

		modified = false;

		std::vector<ChunkMesh::Vertex> vertices;
		std::vector<Uint32> elements;

		for (Uint32 y = 0; y < size.y; y++)
		{
			for (Uint32 z = 0; z < size.z; z++)
			{
				for (Uint32 x = 0; x < size.x; x++)
				{
					Block* block = GetBlock(x, y, z);
					const glm::vec3 position = glm::vec3(x, y, z);

					if (block == nullptr || block->id == BlockType::Air)
						continue;

					int faceCount = 0;


					// top face
					if (GetBlock(x, y + 1, z) != nullptr && Block::block_data[GetBlock(x, y + 1, z)->id].transparent)
					{
						BlockFace face = Block::block_data[block->id].faces[Side::Top];
						vertices.push_back(ChunkMesh::Vertex(position + face.vertices[0], face.uv[0], 1.0f));
						vertices.push_back(ChunkMesh::Vertex(position + face.vertices[1], face.uv[1], 1.0f));
						vertices.push_back(ChunkMesh::Vertex(position + face.vertices[2], face.uv[2], 1.0f));
						vertices.push_back(ChunkMesh::Vertex(position + face.vertices[3], face.uv[3], 1.0f));
						faceCount++;
					}

					// bottom face
					if (GetBlock(x, y - 1, z) != nullptr && Block::block_data[GetBlock(x, y - 1, z)->id].transparent)
					{
						BlockFace face = Block::block_data[block->id].faces[Side::Bottom];
						if (face.render)
						{
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[0], face.uv[0], 0.5f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[1], face.uv[1], 0.5f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[2], face.uv[2], 0.5f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[3], face.uv[3], 0.5f));
							faceCount++;
						}
					}

					// front face
					if (GetBlock(x, y, z + 1) != nullptr && Block::block_data[GetBlock(x, y, z + 1)->id].transparent)
					{
						BlockFace face = Block::block_data[block->id].faces[Side::Front];
						if (face.render)
						{
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[0], face.uv[0], 0.6f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[1], face.uv[1], 0.6f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[2], face.uv[2], 0.6f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[3], face.uv[3], 0.6f));
							faceCount++;
						}
					}

					// back face
					if (GetBlock(x, y, z - 1) != nullptr && Block::block_data[GetBlock(x, y, z - 1)->id].transparent)
					{
						BlockFace face = Block::block_data[block->id].faces[Side::Back];
						if (face.render)
						{
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[0], face.uv[0], 0.6f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[1], face.uv[1], 0.6f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[2], face.uv[2], 0.6f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[3], face.uv[3], 0.6f));
							faceCount++;
						}
					}

					// right face
					if (GetBlock(x + 1, y, z) != nullptr && Block::block_data[GetBlock(x + 1, y, z)->id].transparent)
					{
						BlockFace face = Block::block_data[block->id].faces[Side::Right];
						if (face.render)
						{
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[0], face.uv[0], 0.8f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[1], face.uv[1], 0.8f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[2], face.uv[2], 0.8f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[3], face.uv[3], 0.8f));
							faceCount++;
						}
					}

					// left face
					if (GetBlock(x - 1, y, z) != nullptr && Block::block_data[GetBlock(x - 1, y, z)->id].transparent)
					{
						BlockFace face = Block::block_data[block->id].faces[Side::Left];
						if (face.render)
						{
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[0], face.uv[0], 0.8f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[1], face.uv[1], 0.8f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[2], face.uv[2], 0.8f));
							vertices.push_back(ChunkMesh::Vertex(position + face.vertices[3], face.uv[3], 0.8f));
							faceCount++;
						}
					}

					int t = vertices.size() - 4 * faceCount;
					for (int i = 0; i < faceCount; i++) {
						elements.push_back(t + i * 4 + 0);
						elements.push_back(t + i * 4 + 1);
						elements.push_back(t + i * 4 + 2);
						elements.push_back(t + i * 4 + 2);
						elements.push_back(t + i * 4 + 3);
						elements.push_back(t + i * 4 + 0);
					}
				}
			}
		}

		mesh->SetData(vertices, elements);
	}

	Block* Chunk::GetBlock(Int32 x, Int32 y, Int32 z) {
		if (x >= 0 && y >= 0 && z >= 0 && x < size.x && y < size.y && z < size.z) {
			return &blocks[(z * size.x * size.y) + (y * size.x) + x];
		}
		else if (x < 0 && z >= 0 && z < size.z && y >= 0 && y < size.y) 
		{
			if (neighbouring[Side::Left] != nullptr)
				return neighbouring[Side::Left]->GetBlock(size.x - 1, y, z);
		}
		else if (x >= size.x && z >= 0 && z < size.z && y >= 0 && y < size.y) 
		{
			if (neighbouring[Side::Right] != nullptr)
				return neighbouring[Side::Right]->GetBlock(0, y, z);
		}
		else if (z < 0 && x >= 0 && x < size.x && y >= 0 && y < size.y) 
		{
			if (neighbouring[Side::Back] != nullptr)
				return neighbouring[Side::Back]->GetBlock(x, y, size.x - 1);
		}
		else if (z >= size.z && x >= 0 && x < size.x && y >= 0 && y < size.y) 
		{
			if (neighbouring[Side::Front] != nullptr)
				return neighbouring[Side::Front]->GetBlock(x, y, 0);
		}
		else if (y < 0 && x >= 0 && x < size.x && z >= 0 && z < size.z) 
		{
			if (neighbouring[Side::Bottom] != nullptr)
				return neighbouring[Side::Bottom]->GetBlock(x, size.z - 1, z);
		}
		else if (y >= size.y && x >= 0 && x < size.x && z >= 0 && z < size.z) 
		{
			if (neighbouring[Side::Top] != nullptr)
				return neighbouring[Side::Top]->GetBlock(x, 0, z);
		}

		return nullptr;
	}

	void Chunk::SetBlock(Int32 x, Int32 y, Int32 z, const Block& block)
	{
		if (x >= 0 && y >= 0 && z >= 0 && x < size.x && y < size.y && z < size.z)
		{
			blocks[(z * size.x * size.y) + (y * size.x) + x] = block;
			modified = true;

			if (x == 0 && (neighbouring[Side::Left] != nullptr))
				neighbouring[Side::Left]->SetModified();

			if (y == 0 && (neighbouring[Side::Bottom] != nullptr))
				neighbouring[Side::Bottom]->SetModified();

			if (z == 0 && (neighbouring[Side::Back] != nullptr))
				neighbouring[Side::Back]->SetModified();

			if (x == size.x - 1 && (neighbouring[Side::Right] != nullptr))
				neighbouring[Side::Right]->SetModified();

			if (y == size.y - 1 && (neighbouring[Side::Top] != nullptr))
				neighbouring[Side::Top]->SetModified();

			if (z == size.z - 1 && (neighbouring[Side::Front] != nullptr))
				neighbouring[Side::Front]->SetModified();
		}
	}

	void Chunk::SetNeighbour(int side, const std::shared_ptr<Chunk>& chunk)
	{
		if (side < 6)
		{
			neighbouring[side] = chunk;
		}
	}

	std::shared_ptr<Chunk> Chunk::Create(const glm::ivec3& position)
	{
		return std::make_shared<Chunk>(position);
	}

} // namespace Voxel