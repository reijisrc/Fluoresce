//==============================================================================//
// Name : LineRenderer.cpp														// 
// Describe :	ラインレンダラー												// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2023/02/20														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/LineRenderer.h"

#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/UniformBuffer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/RenderPipeline.h"

namespace Fluoresce {

	// バッチ上限
	constexpr uint32 maxLine = 10000;
	constexpr uint32 maxVectices = maxLine * 2;
	constexpr size_t lineVertexCount = 2;
	constexpr size_t rectVertexCount = 4;

	// ライン矩形デフォルト頂点座標
	static Vec4 s_DefaultVertexPositions[rectVertexCount] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	struct LineVertex
	{
		Vec3 Position = Vec3(0.0f);
		Vec4 Color = Vec4(1.0f);
	};

	struct CameraData
	{
		Mat4 ViewProjection;
	};

	struct LineRenderer::RendererData
	{
		Ref<VertexArray>	VertexArray;
		Ref<VertexBuffer>	VertexBuffer;
		Ref<EffectShader>			Shader;

		uint32 LineVertexCount = 0;

		float32 LineWidth = 4.0f;
	};

	void LineRenderer::Init(const std::string& shaderPath)
	{
		m_Data = new LineRenderer::RendererData();
		m_Data->Shader = EffectShader::Create(shaderPath);
		m_Data->VertexArray = VertexArray::Create();

		m_Data->VertexBuffer = VertexBuffer::Create(maxVectices * sizeof(LineVertex));
		m_Data->VertexBuffer->SetLayout({
			{ GPUDataType::Float3, "a_Position" },
			{ GPUDataType::Float4, "a_Color"    }
			});

		m_Data->VertexArray->AddVertexBuffer(m_Data->VertexBuffer);
		m_Buffer.Allocate(maxVectices * sizeof(LineVertex));
	}

	void LineRenderer::ShutDown()
	{
		m_Buffer.Free();
		delete m_Data;
	}

	void LineRenderer::Submit()
	{
		if (m_Data->LineVertexCount)
		{
			uint32 dataSize = m_Buffer.GetValidOffset();
			m_Data->VertexBuffer->SetData(&m_Buffer[0], dataSize);

			m_Data->Shader->Bind();
			RenderCommand::SetLineWidth(m_Data->LineWidth);
			RenderCommand::DrawLines(m_Data->VertexArray, m_Data->LineVertexCount);
			m_Stats.DrawCalls++;
		}
	}

	void LineRenderer::StartBatch()
	{
		m_Data->LineVertexCount = 0;
		m_Buffer.Reset(false);
	}

	void LineRenderer::NextBatch()
	{
		Submit();
		StartBatch();
	}

	void LineRenderer::Begin(const Camera& camera, const Mat4& transform)
	{
		Mat4 view = glm::inverse(transform);
		Mat4 proj = camera.GetProjection();
		Mat4 vp = proj * view;

		if (auto ubo = RenderPipeline::GetConstBuffers().GetUniformBuffer(ConstBuffer::UniformBufferIndex::Camera); ubo)
		{
			ubo->SetData(&vp, sizeof(RenderPipeline::CameraData));
		}

		StartBatch();
	}

	void LineRenderer::Begin(const EditorCamera& camera)
	{
		if (auto ubo = RenderPipeline::GetConstBuffers().GetUniformBuffer(ConstBuffer::UniformBufferIndex::Camera); ubo)
		{
			ubo->SetData(&camera.GetViewProjection(), sizeof(RenderPipeline::CameraData));
		}

		StartBatch();
	}

	void LineRenderer::End()
	{
		Submit();
	}

	void LineRenderer::DrawLine(const Vec3& p0, const Vec3& p1, const Vec4& color)
	{
		if (m_Data->LineVertexCount >= maxVectices)
			NextBatch();

		Vec3 pos[2] = { p0, p1 };

		for (size_t i = 0; i < lineVertexCount; i++)
		{
			LineVertex point;
			point.Position = pos[i];
			point.Color = color;
			m_Buffer.PushData(&point, sizeof(LineVertex));
		}

		m_Data->LineVertexCount += 2;

		m_Stats.VertexCount += lineVertexCount;
		m_Stats.IndexCount += lineVertexCount;
	}

	void LineRenderer::DrawRect(const Vec3& position, const Vec2& size, const Vec4& color)
	{
		Vec3 p0 = Vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		Vec3 p1 = Vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		Vec3 p2 = Vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		Vec3 p3 = Vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void LineRenderer::DrawRect(const Mat4& transform, const Vec4& color)
	{
		Vec3 lineVertices[rectVertexCount];
		for (size_t i = 0; i < rectVertexCount; i++)
		{
			lineVertices[i] = transform * s_DefaultVertexPositions[i];
		}

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	float32 LineRenderer::GetLineWidth()
	{
		return m_Data->LineWidth;
	}

	void LineRenderer::SetLineWidth(float32 width)
	{
		m_Data->LineWidth = width;
	}
}