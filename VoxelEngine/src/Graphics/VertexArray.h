#pragma once

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>
#include <Graphics/Buffer.h>

#include <vector>

namespace Voxel
{

	class VertexArray : public Utils::Resource
	{
	public:
		void Create();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const VertexBuffer& vertexBuffer);
		void SetElementBuffer(const ElementBuffer& elementBuffer);

		const std::vector<VertexBuffer>& GetVertexBuffer() const { return vertexBuffers; }
		const ElementBuffer& GetElementBuffer() const { return elementBuffer; }

		void Destroy() override;
	private:
		Uint32 varray;
		Uint32 vertexBufferIndex = 0;
		std::vector<VertexBuffer> vertexBuffers;
		ElementBuffer elementBuffer;
	};

} // namespace Voxel