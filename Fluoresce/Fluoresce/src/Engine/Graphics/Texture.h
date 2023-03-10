//==============================================================================//
// Name : Texture.h																// 
// Describe : 	テクスチャ														// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/TextureDefine.h"

namespace Fluoresce {

	class Framebuffer;

	// テクスチャスペック
	struct TexturetSpecification
	{
		TextureFormat Format = TextureFormat::RGBA;
		TextureFilter Filter = TextureFilter::Linear;
		TextureWrap   Wrap = TextureWrap::Clamp;
		uint32		  MipmapLevel = 1;
		bool          sRGB = false;
	};

	// テクスチャ
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual TexturetSpecification GetSpecification() const = 0;
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual uint32 GetRendererID() const = 0;

		virtual void SetData(void* data, uint32 size) = 0;

		virtual void Bind(uint32 slot) const = 0;

		virtual bool IsHDR() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	// 2Dテクスチャ
	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32 width, uint32 height, const TexturetSpecification& spec = TexturetSpecification());
		static Ref<Texture2D> Create(const std::string& path, const TexturetSpecification& spec = TexturetSpecification());
	};

	// キューブテクスチャ
	class TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(uint32 width, uint32 height, const TexturetSpecification& spec = TexturetSpecification());
		static Ref<TextureCube> Create(const std::string& path, const TexturetSpecification& spec = TexturetSpecification());
	};

}
