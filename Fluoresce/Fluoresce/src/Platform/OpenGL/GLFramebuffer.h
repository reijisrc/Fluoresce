//==============================================================================//
// Name : GLFramebuffer.h														// 
// Describe : 	GLフレームバッファ												// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2022/10/15														// 
//==============================================================================//
#include "Engine/Graphics/Framebuffer.h"

namespace Fluoresce {

	// GLフレームバッファ
	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer(const FramebufferSpecification& spec);
		virtual ~GLFramebuffer();

		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32 width, uint32 height) override;
		virtual sint32 ReadPixel(uint32 attachmentIndex, sint32 x, sint32 y) override;

		virtual uint32 GetColorAttachmentRendererID(uint32 index = 0) const override
		{
			FR_CORE_ASSERT(index < m_ColorAttachments.size(), "Invalid colorattachment RendererID!"); return m_ColorAttachments[index];
		}
		virtual void ClearAttachment(uint32 attachmentIndex, sint32 value) override;

		virtual void BindAttachmentToTextureSlot(uint32 slot, uint32 index = 0) const override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };
	private:
		uint32 m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32> m_ColorAttachments;
		uint32 m_DepthAttachment = 0;
	};
}