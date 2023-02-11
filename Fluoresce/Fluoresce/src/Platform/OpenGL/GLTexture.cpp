//==============================================================================//
// Name : GLTexture.cpp															// 
// Describe : 	GLテクスチャ													// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/10														// 
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
		case Fluoresce::TextureFormat::RGBA16f:    return GL_RGBA16F;
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
		case Fluoresce::TextureFormat::RGBA16f:    return GL_RGBA;
		}
		FR_CORE_ASSERT(false, "Unknown texture format!");
		return 0;
	}


	GLTexture2D::GLTexture2D(TextureFormat format, uint32 width, uint32 height, TextureWrap wrap)
		: m_Format(format), m_Width(width), m_Height(height), m_Wrap(wrap)

	{
		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum glwrap = m_Wrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, glwrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, glwrap);
	}

	GLTexture2D::GLTexture2D(const std::string& path, TextureWrap wrap)
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

		GLenum glwrap = m_Wrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, glwrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, glwrap);

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

	GLTextureCube::GLTextureCube(TextureFormat format, uint32 width, uint32 height)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	GLTextureCube::GLTextureCube(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);

		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);

		m_Width = width;
		m_Height = height;
		m_Format = TextureFormat::RGB;

		uint32_t faceWidth = m_Width / 4;
		uint32_t faceHeight = m_Height / 3;
		FR_CORE_ASSERT(faceWidth == faceHeight, "Non-square faces!");

		std::array<unsigned char*, 6> faces;
		for (size_t i = 0; i < faces.size(); i++)
			faces[i] = new unsigned char[faceWidth * faceHeight * 3]; // 3 BPP

		int faceIndex = 0;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + i * faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = data[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = data[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = data[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		for (size_t i = 0; i < 3; i++)
		{
			// Skip the middle one
			if (i == 1)
				continue;

			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + i * faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = data[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = data[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = data[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);
		auto dataFormat = ConvertOpenGLTextureDataFormat(m_Format);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[0]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[3]);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_2D, 0);

		for (size_t i = 0; i < faces.size(); i++)
			delete[] faces[i];

		stbi_image_free(data);
	}


	GLTextureCube::~GLTextureCube()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void GLTextureCube::SetData(void* data, uint32 size)
	{
		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);
		auto dataFormat = ConvertOpenGLTextureDataFormat(m_Format);

		uint32_t faceWidth = m_Width / 4;
		uint32_t faceHeight = m_Height / 3;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	void GLTextureCube::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}