//==============================================================================//
// Name : GLTexture.cpp															// 
// Describe : 	GLテクスチャ													// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLTexture.h"

#include "stb_image.h"

namespace Fluoresce {

	static GLenum ConvertOpenGLTextureInternalFormat(TextureFormat format)
	{
		switch (format)
		{
		case Fluoresce::TextureFormat::RGB:     return GL_RGB8;
		case Fluoresce::TextureFormat::RGBA:    return GL_RGBA8;
		}
		FR_CORE_ASSERT(false, "Unknown texture format!");
		return 0;
	}

	static GLenum ConvertOpenGLTextureDataFormat(TextureFormat format)
	{
		switch (format)
		{
		case Fluoresce::TextureFormat::RGB:     return GL_RGB;
		case Fluoresce::TextureFormat::RGBA:    return GL_RGBA;
		}
		FR_CORE_ASSERT(false, "Unknown texture format!");
		return 0;
	}


	GLTexture2D::GLTexture2D(TextureFormat format, uint32 width, uint32 height)
		: m_Format(format), m_Width(width), m_Height(height)

	{
		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	GLTexture2D::GLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		FR_CORE_ASSERT(data, "Failed to load image");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			m_Format = TextureFormat::RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			m_Format = TextureFormat::RGB;
		}
		else
		{
			FR_CORE_ASSERT(false, "Format not supported!")
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void GLTexture2D::SetData(void* data, uint32 size)
	{
		uint32 bpc = (m_Format == TextureFormat::RGBA) ? 4 : 3;
		auto dataFormat = ConvertOpenGLTextureDataFormat(m_Format);
		FR_CORE_ASSERT(size == m_Width * m_Height * bpc, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture2D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}