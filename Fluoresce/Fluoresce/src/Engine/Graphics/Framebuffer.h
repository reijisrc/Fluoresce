//==============================================================================//
// Name : Framebuffer.h															// 
// Describe : 	フレームバッファ												// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/TextureDefine.h"

namespace Fluoresce {

	// フレームバッファフォーマット
	enum class FramebufferTextureFormat
	{
		UndefineFormat = 0,

		// カラーバッファ
		RGBA8,
		RGBA16F,
		RED_INTEGER,

		// デプスバッファ
		DEPTH24STENCIL8,

		Depth = DEPTH24STENCIL8
	};

	// フレームバッファテクスチャスペック
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format, 
			TextureFilter filter = TextureFilter::Linear,
			TextureWrap   wrap = TextureWrap::Clamp,
			uint32 mipmap = 1)
			: Format(format),
			Filter(filter),
			Wrap(wrap),
			MipmapLevel(mipmap)
		{
		
		}

		FramebufferTextureFormat Format =	FramebufferTextureFormat::UndefineFormat;
		TextureFilter			 Filter =	TextureFilter::Linear;
		TextureWrap				 Wrap =		TextureWrap::Clamp;
		uint32					 MipmapLevel =		1;
	};

	// フレームバッファ-カラーバッファ情報(Attachment0 - Attachment3)
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	// フレームバッファ情報
	struct FramebufferSpecification
	{
		uint32 Width, Height;
		uint32 Samples = 1;
		Vec4 ClearColor = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
		FramebufferAttachmentSpecification Attachments;
	};

	// フレームバッファ
	class Framebuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual sint32 ReadPixel(uint32 attachmentIndex, sint32 x, sint32 y) = 0;

		virtual void BlitMultisampledBuffer(const Ref<Framebuffer>& intermediateBuffer) = 0;

		virtual uint32 GetColorAttachmentRendererID(uint32 index = 0) const = 0;
		virtual uint32 GetDepthAttachmentRendererID() const = 0;
		virtual void ClearAttachment(uint32 attachmentIndex, sint32 value) = 0;
		virtual void BindAttachmentToTextureSlot(uint32 slot, uint32 index = 0) const = 0;

		virtual uint32 GetRendererID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}