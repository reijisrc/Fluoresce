//==============================================================================//
// Name : SpriteRenderer.h														// 
// Describe :	スプライトレンダラー											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2023/01/07														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Renderer/BatchRenderer.h"
#include "Engine/Scene/Components.h"

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

		void DrawQuad(const Vec3& position, const Vec2& size, const Vec4& color, sint32 entityID = -1);
		void DrawQuad(const Mat4& transform, const Vec4& color, sint32 entityID = -1);

		void DrawSprite(const Vec3& positon, const Vec2& size, const Vec4& color, const Ref<Texture2D>& texture, float32 tilingFactor = 1.0f, sint32 entityID = -1);
		void DrawSprite(const Mat4& transform, const Vec4& color, const Ref<Texture2D>& texture, float32 tilingFactor = 1.0f, sint32 entityID = -1);

		void DrawSpriteEntity(const Mat4& transform, SpriteRendererComponent& src, sint32 entityID);

	private:
		struct RendererData;
		RendererData* m_Data;
	};
};