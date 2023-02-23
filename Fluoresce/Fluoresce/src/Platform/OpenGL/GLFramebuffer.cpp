//==============================================================================//
// Name : GLFramebuffer.cpp														// 
// Describe : 	GLフレームバッファ												// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLFramebuffer.h"

#include "Platform/OpenGL/GLUtils.h"

namespace Fluoresce {

	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
	{
		glCreateTextures(TextureTarget(multisampled), count, outID);
	}

	static void BindTexture(bool multisampled, uint32_t id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
		}

		return false;
	}

	static GLenum ConvertOpenGLFramebufferInternalFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
		case FramebufferTextureFormat::RGBA16F:     return GL_RGBA16F;
		case FramebufferTextureFormat::RED_INTEGER: return GL_R32I;
		case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
		}

		FR_CORE_ASSERT(false, "Unknown FrameBuffer Format!");
		return 0;
	}

	static GLenum ConvertOpenGLFramebufferDataFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:       return GL_RGBA;
		case FramebufferTextureFormat::RGBA16F:     return GL_RGBA;
		case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL;
		}

		FR_CORE_ASSERT(false, "Unknown FrameBuffer Format!");
		return 0;
	}

	static void AttachColorTexture(uint32_t id, int samples, FramebufferTextureSpecification spec, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		auto texformat = ConvertOpenGLFramebufferInternalFormat(spec.Format);
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, texformat, width, height, GL_TRUE);
		}
		else
		{
			auto filter = GLUtils::ConvertOpenGLTextureFilter(spec.Filter);
			auto wrap = GLUtils::ConvertOpenGLTextureWrap(spec.Wrap);

			glTexStorage2D(GL_TEXTURE_2D, 1, texformat, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(uint32_t id, int samples, FramebufferTextureSpecification spec, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		auto texformat = ConvertOpenGLFramebufferInternalFormat(spec.Format);
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, texformat, width, height, GL_TRUE);
		}
		else
		{
			auto filter = GLUtils::ConvertOpenGLTextureFilter(spec.Filter);
			auto wrap = GLUtils::ConvertOpenGLTextureWrap(spec.Wrap);

			glTexStorage2D(GL_TEXTURE_2D, 1, texformat, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureTarget(multisampled), id, 0);
	}

	GLFramebuffer::GLFramebuffer(const FramebufferSpecification& spec) :
		m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!IsDepthFormat(spec.Format))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	GLFramebuffer::~GLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void GLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				BindTexture(multisample, m_ColorAttachments[i]);
				AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, m_ColorAttachmentSpecifications[i], m_Specification.Width, m_Specification.Height, i);
			}
		}

		if (m_DepthAttachmentSpecification.Format != FramebufferTextureFormat::UndefineFormat)
		{
			CreateTextures(multisample, &m_DepthAttachment, 1);
			BindTexture(multisample, m_DepthAttachment);
			AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, m_DepthAttachmentSpecification, m_Specification.Width, m_Specification.Height);
		}

		if (m_ColorAttachments.size() > 1)
		{
			FR_CORE_ASSERT(m_ColorAttachments.size() < 4, "Only support colorattachmeng 0 - 3!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		FR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		glClearColor(m_Specification.ClearColor.r, m_Specification.ClearColor.g, m_Specification.ClearColor.b, m_Specification.ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFramebuffer::Resize(uint32 width, uint32 height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	sint32 GLFramebuffer::ReadPixel(uint32 attachmentIndex, sint32 x, sint32 y)
	{
		FR_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Colorattachment does not exist!");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void GLFramebuffer::BlitMultisampledBuffer(const Ref<Framebuffer>& intermediateBuffer)
	{
		auto srcspec = GetSpecification();
		auto dstspec = intermediateBuffer->GetSpecification();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateBuffer->GetRendererID());
		glBlitFramebuffer(0, 0, srcspec.Width, srcspec.Height, 0, 0, dstspec.Width, dstspec.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void GLFramebuffer::ClearAttachment(uint32 attachmentIndex, sint32 value)
	{
		FR_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Could not ClearAttachment!");

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, ConvertOpenGLFramebufferDataFormat(spec.Format), GL_INT, &value);
	}

	void GLFramebuffer::BindAttachmentToTextureSlot(uint32 slot, uint32 index) const
	{
		uint32 rendererId = GetColorAttachmentRendererID(index);
		glBindTextureUnit(slot, rendererId);
	}
}