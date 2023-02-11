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
		case Fluoresce::TextureFormat::RGBA16f:    return GL_RGB;
		}
		FR_CORE_ASSERT(false, "Unknown texture format!");
		return 0;
	}


	GLTexture2D::GLTexture2D(uint32 width, uint32 height, const TexturetOption& option)
		: m_Format(option.foramt), m_Width(width), m_Height(height)
	{
		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum glwrap = (option.wrap == TextureWrap::Clamp) ? GL_CLAMP_TO_EDGE : GL_REPEAT;

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, glwrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, glwrap);
	}

	GLTexture2D::GLTexture2D(const std::string& path, const TexturetOption& option)
		: m_Path(path)
	{
		if (stbi_is_hdr(path.c_str()))
		{
			LoadHDRImage(path, option);
		}
		else
		{
			LoadLDRImage(path, option);
		}
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

	void GLTexture2D::LoadLDRImage(const std::string& path, const TexturetOption& option)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, option.sRGB ? STBI_rgb : 0);
		FR_CORE_ASSERT(data, "Failed to load texture2D");

		if (!option.sRGB)
		{
			if (channels == 4)
			{
				m_Format = TextureFormat::RGBA;
			}
			else if (channels == 3)
			{
				m_Format = TextureFormat::RGB;
			}
		}
		else
		{
			m_Format = TextureFormat::RGB;
		}

		m_Width = width;
		m_Height = height;

		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);
		auto dataFormat = ConvertOpenGLTextureDataFormat(m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum glwrap = (option.wrap == TextureWrap::Clamp) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, glwrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, glwrap);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	void GLTexture2D::LoadHDRImage(const std::string& path, const TexturetOption& option)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		float32* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		m_IsHDR = true;
		m_Format = TextureFormat::RGBA16f;
		FR_CORE_ASSERT(data, "Failed to load HDR texture2D");

		m_Width = width;
		m_Height = height;

		auto internalFormat = ConvertOpenGLTextureInternalFormat(m_Format);
		auto dataFormat = ConvertOpenGLTextureDataFormat(m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum glwrap = (option.wrap == TextureWrap::Clamp) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, glwrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, glwrap);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_FLOAT, data);

		stbi_image_free(data);
	}

	GLTextureCube::GLTextureCube(uint32 width, uint32 height, const TexturetOption& option)
		: m_Format(option.foramt), m_Width(width), m_Height(height)
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

	GLTextureCube::GLTextureCube(const std::string& path, const TexturetOption& option)
		: m_Path(path)
	{
		if (stbi_is_hdr(path.c_str()))
		{
			LoadHDRImage(path, option);
		}
		else
		{
			LoadLDRImage(path, option);
		}
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

	void GLTextureCube::LoadLDRImage(const std::string& path, const TexturetOption& option)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, option.sRGB ? STBI_rgb : 0);
		if (!option.sRGB)
		{
			if (channels == 4)
			{
				m_Format = TextureFormat::RGBA;
			}
			else if (channels == 3)
			{
				m_Format = TextureFormat::RGB;
			}
		}
		else
		{
			m_Format = TextureFormat::RGB;
		}

		FR_CORE_ASSERT(data, "Failed to load cubeimage");

		m_Width = width;
		m_Height = height;
		uint32_t faceWidth = m_Width / 4;
		uint32_t faceHeight = m_Height / 3;
		FR_CORE_ASSERT(faceWidth == faceHeight, "Non-square faces!");

		std::array<unsigned short*, 6> faces;
		for (size_t i = 0; i < faces.size(); i++)
			faces[i] = new unsigned short[faceWidth * faceHeight * 3]; // 3 BPP

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
		auto type = GL_UNSIGNED_SHORT;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_SHORT, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_SHORT, faces[0]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_SHORT, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_SHORT, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_SHORT, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_SHORT, faces[3]);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_2D, 0);

		for (size_t i = 0; i < faces.size(); i++)
			delete[] faces[i];

		stbi_image_free(data);
	}

	void GLTextureCube::LoadHDRImage(const std::string& path, const TexturetOption& option)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		float32* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		m_IsHDR = true;
		m_Format = TextureFormat::RGBA16f;
		FR_CORE_ASSERT(data, "Failed to load HDR cubeimage");

		m_Width = width;
		m_Height = height;
		uint32_t faceWidth = m_Width / 4;
		uint32_t faceHeight = m_Height / 3;
		FR_CORE_ASSERT(faceWidth == faceHeight, "Non-square faces!");

		std::array<float*, 6> faces;
		for (size_t i = 0; i < faces.size(); i++)
			faces[i] = new float[faceWidth * faceHeight * 3]; // 3 BPP

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

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_FLOAT, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_FLOAT, faces[0]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_FLOAT, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_FLOAT, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_FLOAT, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_FLOAT, faces[3]);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_2D, 0);

		for (size_t i = 0; i < faces.size(); i++)
			delete[] faces[i];

		stbi_image_free(data);
	}

}