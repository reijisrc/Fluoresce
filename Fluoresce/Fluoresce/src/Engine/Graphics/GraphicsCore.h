//==============================================================================//
// Name : GraphicsCore.h														// 
// Describe : 	グラフィックスコア												// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/23														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// グラフィックコア
	class GraphicsCore
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL4 = 1,
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		virtual void SetClearColor(const FrVec4& color) = 0;
		virtual void SetClearColor(const Vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetLineWidth(float32 width) = 0;

		static API GetAPI() { return s_API; }

		static Scope<GraphicsCore> Create();
	private:
		static API s_API;
	};

	// レンダーコマンド
	class RenderCommand
	{
	public:
		static void Init()
		{
			s_GraphicsAPI->Init();
		}

		static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
		{
			s_GraphicsAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const FrVec4& color)
		{
			s_GraphicsAPI->SetClearColor(color);
		}

		static void SetClearColor(const Vec4& color)
		{
			s_GraphicsAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_GraphicsAPI->Clear();
		}

		static void SetLineWidth(float width)
		{
			s_GraphicsAPI->SetLineWidth(width);
		}

	private:
		static Scope<GraphicsCore> s_GraphicsAPI;
	};
}