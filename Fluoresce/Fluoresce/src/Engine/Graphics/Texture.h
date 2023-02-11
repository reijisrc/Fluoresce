//==============================================================================//
// Name : Texture.h																// 
// Describe : 	テクスチャ														// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/10														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
		RGBA16f = 3
	};

	enum class TextureWrap
	{
		None = 0,
		Clamp = 1,
		Repeat = 2
	};

	// テクスチャ
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual TextureFormat GetFormat() const = 0;
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
		static Ref<Texture2D> Create(TextureFormat format, uint32 width, uint32 height, TextureWrap wrap = TextureWrap::Clamp);
		static Ref<Texture2D> Create(const std::string& path, TextureWrap wrap = TextureWrap::Clamp);
	};

	// キューブテクスチャ
	class TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(TextureFormat format, uint32 width, uint32 height);
		static Ref<TextureCube> Create(const std::string& path);
	};

}
