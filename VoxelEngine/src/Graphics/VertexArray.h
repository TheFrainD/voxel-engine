#pragma once

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>
#include <Graphics/Buffer.h>

#include <vector>
#include <memory>

namespace Voxel
{

	class VertexArray : public Utils::Resource
	{
	public:
		VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return vertexBuffers; }
		const std::shared_ptr<ElementBuffer>& GetElementBuffer() const { return elementBuffer; }

		void Destroy() override;

		static std::shared_ptr<VertexArray> Create();
	private:
		Uint32 varray;
		Uint32 vertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<ElementBuffer> elementBuffer;
	};

} // namespace Voxel