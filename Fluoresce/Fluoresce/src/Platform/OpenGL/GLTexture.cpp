//==============================================================================//
// Name : GLTexture.cpp															// 
// Describe : 	GLテクスチャ													// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/20														// 
//==============================================================================//

#include "frpch.h"
#include "Platform/OpenGL/GLTexture.h"

#include "Platform/OpenGL/GLUtils.h"
#include "stb_image.h"

namespace Fluoresce {

	static uint32 GetChannels(TextureFormat format)
	{
		if (format == Fluoresce::TextureFormat::RGBA || format == Fluoresce::TextureFormat::RGBA16f)
		{
			return 4;
		}
		else if (format == Fluoresce::TextureFormat::RGB)
		{
			return 3;
		}
		return 0;
	}

	GLTexture2D::GLTexture2D(uint32 width, uint32 height, const TexturetSpecification& spec)
		: m_Width(width), m_Height(height), m_Specification(spec)
	{
		auto internalFormat = GLUtil::ConvertOpenGLTextureInternalFormat(m_Specification.Format);
		auto filter = GLUtil::ConvertOpenGLTextureFilter(m_Specification.Filter);
		auto wrap = GLUtil::ConvertOpenGLTextureWrap(m_Specification.Wrap);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, m_Specification.MipmapLevel, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, wrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, wrap);
	}

	GLTexture2D::GLTexture2D(const std::string& path, const TexturetSpecification& spec)
		: m_Path(path)
	{
		if (stbi_is_hdr(path.c_str()))
		{
			LoadHDRImage(path);
		}
		else
		{
			LoadLDRImage(path);
		}
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void GLTexture2D::SetData(void* data, uint32 size)
	{
		uint32 channels = GetChannels(m_Specification.Format);
		auto dataFormat = GLUtil::ConvertOpenGLTextureDataFormat(m_Specification.Format);
		auto type = GLUtil::ConvertOpenGLTextureDataType(m_Specification.Format);
		FR_CORE_ASSERT(size == m_Width * m_Height * channels, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, type, data);
	}

	void GLTexture2D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void GLTexture2D::LoadLDRImage(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, m_Specification.sRGB ? STBI_rgb : 0);
		FR_CORE_ASSERT(data, "Failed to load texture2D");

		if (!m_Specification.sRGB)
		{
			if (channels == 4)
			{
				m_Specification.Format = TextureFormat::RGBA;
			}
			else if (channels == 3)
			{
				m_Specification.Format = TextureFormat::RGB;
			}
		}
		else
		{
			m_Specification.Format = TextureFormat::RGB;
		}

		m_Width = width;
		m_Height = height;

		auto internalFormat = GLUtil::ConvertOpenGLTextureInternalFormat(m_Specification.Format);
		auto dataFormat = GLUtil::ConvertOpenGLTextureDataFormat(m_Specification.Format);
		auto filter = GLUtil::ConvertOpenGLTextureFilter(m_Specification.Filter);
		auto wrap = GLUtil::ConvertOpenGLTextureWrap(m_Specification.Wrap);
		auto type = GLUtil::ConvertOpenGLTextureDataType(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, m_Specification.MipmapLevel, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, wrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, wrap);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, type, data);

		stbi_image_free(data);
	}

	void GLTexture2D::LoadHDRImage(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		float32* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		m_IsHDR = true;
		m_Specification.Format = TextureFormat::RGBA16f;
		FR_CORE_ASSERT(data, "Failed to load HDR texture2D");

		m_Width = width;
		m_Height = height;

		auto internalFormat = GLUtil::ConvertOpenGLTextureInternalFormat(m_Specification.Format);
		auto dataFormat = GLUtil::ConvertOpenGLTextureDataFormat(m_Specification.Format);
		auto filter = GLUtil::ConvertOpenGLTextureFilter(m_Specification.Filter);
		auto wrap = GLUtil::ConvertOpenGLTextureWrap(m_Specification.Wrap);
		auto type = GLUtil::ConvertOpenGLTextureDataType(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, wrap);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, wrap);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, type, data);

		stbi_image_free(data);
	}

	GLTextureCube::GLTextureCube(uint32 width, uint32 height, const TexturetSpecification& spec)
		: m_Width(width), m_Height(height), m_Specification(spec)
	{
		auto internalFormat = GLUtil::ConvertOpenGLTextureInternalFormat(m_Specification.Format);
		auto filter = GLUtil::ConvertOpenGLTextureFilter(m_Specification.Filter);
		auto wrap = GLUtil::ConvertOpenGLTextureWrap(m_Specification.Wrap);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);
	}

	GLTextureCube::GLTextureCube(const std::string& path, const TexturetSpecification& spec)
		: m_Path(path)
	{
		if (stbi_is_hdr(path.c_str()))
		{
			LoadHDRImage(path);
		}
		else
		{
			LoadLDRImage(path);
		}
	}


	GLTextureCube::~GLTextureCube()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void GLTextureCube::SetData(void* data, uint32 size)
	{
		auto internalFormat = GLUtil::ConvertOpenGLTextureInternalFormat(m_Specification.Format);
		auto dataFormat = GLUtil::ConvertOpenGLTextureDataFormat(m_Specification.Format);
		auto type = GLUtil::ConvertOpenGLTextureDataType(m_Specification.Format);

		uint32_t faceWidth = m_Width / 4;
		uint32_t faceHeight = m_Height / 3;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, data);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, data);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, data);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	void GLTextureCube::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void GLTextureCube::LoadLDRImage(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, m_Specification.sRGB ? STBI_rgb : 0);
		if (!m_Specification.sRGB)
		{
			if (channels == 4)
			{
				m_Specification.Format = TextureFormat::RGBA;
			}
			else if (channels == 3)
			{
				m_Specification.Format = TextureFormat::RGB;
			}
		}
		else
		{
			m_Specification.Format = TextureFormat::RGB;
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

		auto internalFormat = GLUtil::ConvertOpenGLTextureInternalFormat(m_Specification.Format);
		auto dataFormat = GLUtil::ConvertOpenGLTextureDataFormat(m_Specification.Format);
		auto wrap = GLUtil::ConvertOpenGLTextureWrap(m_Specification.Wrap);
		auto type = GLUtil::ConvertOpenGLTextureDataType(m_Specification.Format);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[0]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[3]);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_2D, 0);

		for (size_t i = 0; i < faces.size(); i++)
			delete[] faces[i];

		stbi_image_free(data);
	}

	void GLTextureCube::LoadHDRImage(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		float32* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		m_IsHDR = true;
		m_Specification.Format = TextureFormat::RGBA16f;
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

		auto internalFormat = GLUtil::ConvertOpenGLTextureInternalFormat(m_Specification.Format);
		auto dataFormat = GLUtil::ConvertOpenGLTextureDataFormat(m_Specification.Format);
		auto wrap = GLUtil::ConvertOpenGLTextureWrap(m_Specification.Wrap);
		auto type = GLUtil::ConvertOpenGLTextureDataType(m_Specification.Format);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[0]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, type, faces[3]);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_2D, 0);

		for (size_t i = 0; i < faces.size(); i++)
			delete[] faces[i];

		stbi_image_free(data);
	}

}