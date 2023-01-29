//==============================================================================//
// Name : SpriteRenderer.cpp													// 
// Describe :	スプライトレンダラー											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2022/08/15														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/SpriteRenderer.h"

#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/UniformBuffer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/RenderPipeline.h"

namespace Fluoresce {

	// バッチ上限
	constexpr uint32 maxQuad = 10000;
	constexpr uint32 maxVectices = maxQuad * 4;
	constexpr uint32 maxIndices = maxQuad * 6;
	constexpr size_t quadVertexCount = 4;

	// スプライトデフォルト頂点座標とUV座標
	static Vec4 s_DefaultVertexPositions[quadVertexCount] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	static Vec2 s_DefaultTexCoord[quadVertexCount] =
	{ 
		{ 0.0f, 0.0f }, 
		{ 1.0f, 0.0f }, 
		{ 1.0f, 1.0f }, 
		{ 0.0f, 1.0f } 
	};

	struct QuadVertex
	{
		Vec3 Position = Vec3(0.0f);
		Vec4 Color = Vec4(1.0f);
		Vec2 TexCoord = Vec2(0.0f);
		float32 TexIndex = 0.0f;
		float32 TilingFactor = 1.0f;
	};

	struct SpriteRenderer::RendererData
	{
		Ref<VertexArray>	VertexArray;
		Ref<VertexBuffer>	VertexBuffer;
		Ref<Shader>			Shader;

		uint32 QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

    void SpriteRenderer::Init(const std::string& shaderPath)
    {
		m_Data = new SpriteRenderer::RendererData();
		m_Data->Shader = Shader::Create(shaderPath);
		m_Data->VertexArray = VertexArray::Create();

		m_Data->VertexBuffer = VertexBuffer::Create(maxVectices * sizeof(QuadVertex));
		m_Data->VertexBuffer->SetLayout({
			{ GPUDataType::Float3, "a_Postion"},
			{ GPUDataType::Float4, "a_Color"},
			{ GPUDataType::Float2, "a_TexCoord"},
			{ GPUDataType::Float, "a_TexIndex" },
			{ GPUDataType::Float, "a_TilingFactor" },
			});

		m_Data->VertexArray->AddVertexBuffer(m_Data->VertexBuffer);
		m_Data->QuadVertexBufferBase = new QuadVertex[maxVectices];

		uint32* quadIndices = new uint32[maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, maxIndices);
		m_Data->VertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;
    }

	void SpriteRenderer::ShutDown()
	{
		delete[] m_Data->QuadVertexBufferBase;
		delete m_Data;
	}

	void SpriteRenderer::Submit()
	{
		if (m_Data->QuadIndexCount > 0)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)m_Data->QuadVertexBufferPtr - (uint8_t*)m_Data->QuadVertexBufferBase);
			m_Data->VertexBuffer->SetData(m_Data->QuadVertexBufferBase, dataSize);

			m_Data->Shader->Bind();
			RenderCommand::DrawIndexed(m_Data->VertexArray, m_Data->QuadIndexCount);
			m_Stats.DrawCalls++;
		}
	}

	void SpriteRenderer::StartBatch()
	{
		m_Data->QuadIndexCount = 0;
		m_Data->QuadVertexBufferPtr = m_Data->QuadVertexBufferBase;
	}

	void SpriteRenderer::NextBatch()
	{
		Submit();
		StartBatch();
	}

	void SpriteRenderer::Begin(const Camera& camera, const Mat4& transform)
	{
		Mat4 view = glm::inverse(transform);
		Mat4 proj = camera.GetProjection();
		Mat4 vp = proj * view;

		if (auto ubo = RenderPipeline::GetUniformBuffer(RenderPipeline::UniformBufferIndex::Camera); ubo)
		{
			ubo->SetData(&vp, sizeof(RenderPipeline::CameraData));
		}

		StartBatch();
	}

	void SpriteRenderer::End()
	{
		Submit();
	}

	void SpriteRenderer::DrawQuad(const Vec3& position, const Vec2& size, const Vec4& color)
	{
		Mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void SpriteRenderer::DrawQuad(const Mat4& transform, const Vec4& color)
	{
		if (m_Data->QuadIndexCount >= maxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Vec3 pos = transform * s_DefaultVertexPositions[i];
			m_Data->QuadVertexBufferPtr->Position = pos;
			m_Data->QuadVertexBufferPtr->Color = color;
			m_Data->QuadVertexBufferPtr->TexCoord = s_DefaultTexCoord[i];
			m_Data->QuadVertexBufferPtr->TexIndex = 0.0f;
			m_Data->QuadVertexBufferPtr->TilingFactor = 1.0f;
			m_Data->QuadVertexBufferPtr++;
		}

		m_Data->QuadIndexCount += 6;

		m_Stats.VertexCount += quadVertexCount;
		m_Stats.IndexCount += 6;
	}
}