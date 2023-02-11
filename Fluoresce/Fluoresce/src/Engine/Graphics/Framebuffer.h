//==============================================================================//
// Name : Framebuffer.h															// 
// Describe : 	�t���[���o�b�t�@												// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/11														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// �t���[���o�b�t�@�t�H�[�}�b�g
	enum class FramebufferTextureFormat
	{
		None = 0,

		// �J���[�o�b�t�@
		RGBA8,
		RGBA16F,
		RED_INTEGER,

		// �f�v�X�o�b�t�@
		DEPTH24STENCIL8,

		Depth = DEPTH24STENCIL8
	};

	// �t���[���o�b�t�@�e�N�X�`���X�y�b�N
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	// �t���[���o�b�t�@-�J���[�o�b�t�@���(Attachment0 - Attachment3)
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	// �t���[���o�b�t�@���
	struct FramebufferSpecification
	{
		uint32 Width, Height;
		uint32 Samples = 1;
		//bool SwapChainTarget = false;
		FramebufferAttachmentSpecification Attachments;
	};

	// �t���[���o�b�t�@
	class Framebuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual sint32 ReadPixel(uint32 attachmentIndex, sint32 x, sint32 y) = 0;

		virtual void BlitMultisampledBuffer(const Ref<Framebuffer>& intermediateBuffer) = 0;

		virtual uint32 GetColorAttachmentRendererID(uint32 index = 0) const = 0;
		virtual void ClearAttachment(uint32 attachmentIndex, sint32 value) = 0;
		virtual void BindAttachmentToTextureSlot(uint32 slot, uint32 index = 0) const = 0;

		virtual uint32 GetRendererID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}