//==============================================================================//
// Name : SpriteRenderer.h														// 
// Describe :	スプライトレンダラー											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2022/08/15														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Camera.h"
#include "Engine/Renderer/BatchRenderer.h"

namespace Fluoresce {

	//	スプライトレンダラー
	class SpriteRenderer : public BatchRenderer
	{
	public:
		void Init(const std::string& shaderPath);
		void ShutDown();

		virtual void Submit() override;
		virtual void StartBatch() override;
		virtual void NextBatch() override;

		void Begin(const Camera& camera, const Mat4& transform);
		void End();

		void DrawQuad(const Vec3& position, const Vec2& size, const Vec4& color);
		void DrawQuad(const Mat4& transform, const Vec4& color);
	private:
		struct RendererData;
		RendererData* m_Data;
	};
};