#pragma once

// Headers
#include <Config.h>
#include <Core/Assert.h>
#include <Utils/ResourceManager.h>

#include <string>
#include <vector>
#include <memory>

namespace Voxel
{

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		VE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		Uint32 size;
		Uint32 offset;
		bool normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) :
			name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized)
		{
		}

		Uint32 GetComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3* float3
			case ShaderDataType::Mat4:    return 4; // 4* float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			VE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements) : elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		Uint32 GetStride() const { return stride; }
		const std::vector<BufferElement>& GetElements() const { return elements; }

		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }
	private:
		std::vector<BufferElement> elements;
		Uint32 stride = 0;

		void CalculateOffsetsAndStride()
		{
			Uint32 offset = 0;
			stride = 0;
			for (auto& element : elements)
			{
				element.offset = offset;
				offset += element.size;
				stride += element.size;
			}
		}
	};

	class VertexBuffer : public Utils::Resource
	{
	public:
		VertexBuffer(Uint32 size);
		VertexBuffer(const float* data, Uint32 size);

		const BufferLayout& GetLayout() const { return layout; }
		void SetLayout(const BufferLayout& layout) { this->layout = layout; }

		void SetData(const void* data, Uint32 size);

		void Bind() const;
		void Unbind() const;

		void Destroy() override;

		static std::shared_ptr<VertexBuffer> Create(Uint32 size);
		static std::shared_ptr<VertexBuffer> Create(const float* data, Uint32 size);
	private:
		Uint32 buffer;
		BufferLayout layout;
	};

	class ElementBuffer : public Utils::Resource
	{
	public:
		ElementBuffer(Uint32* elements, Uint32 count);

		void Bind() const;
		void Unbind() const;

		Uint32 GetCount() const { return count; }

		void Destroy() override;

		static std::shared_ptr<ElementBuffer> Create(Uint32* elements, Uint32 count);
	private:
		Uint32 buffer;
		Uint32 count;
	};

} // namespace Voxel