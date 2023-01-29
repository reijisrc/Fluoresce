//==============================================================================//
// Name : Texture.h																// 
// Describe : 	テクスチャ														// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2022/10/15														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// テクスチャ
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32 size) = 0;

		virtual void Bind(uint32 slot) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	// 2Dテクスチャ
	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32 width, uint32 height);
		static Ref<Texture2D> Create(const std::string& path);
	};

}
