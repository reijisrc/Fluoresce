//==============================================================================//
// Name : Texture.h																// 
// Describe : 	�e�N�X�`��														// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/11														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/TextureDefine.h"

namespace Fluoresce {

	// �e�N�X�`���I�v�V����
	struct TexturetOption
	{
		TextureFormat foramt = TextureFormat::RGBA;
		TextureWrap   wrap = TextureWrap::Clamp;
		bool          sRGB = false;
	};

	// �e�N�X�`��
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual TextureFormat GetFormat() const = 0;
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual uint32 GetRendererID() const = 0;

		virtual void SetData(void* data, uint32 size) = 0;

		virtual void Bind(uint32 slot) const = 0;

		virtual bool IsHDR() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	// 2D�e�N�X�`��
	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32 width, uint32 height, const TexturetOption& option = TexturetOption());
		static Ref<Texture2D> Create(const std::string& path, const TexturetOption& option = TexturetOption());
	};

	// �L���[�u�e�N�X�`��
	class TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(uint32 width, uint32 height, const TexturetOption& option = TexturetOption());
		static Ref<TextureCube> Create(const std::string& path, const TexturetOption& option = TexturetOption());
	};

}
