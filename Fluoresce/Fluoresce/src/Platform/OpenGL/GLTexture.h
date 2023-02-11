//==============================================================================//
// Name : GLTexture.h															// 
// Describe : 	GLテクスチャ													// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/10														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Texture.h"

#include <glad/glad.h>

namespace Fluoresce {

	// GLテクスチャ
	class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(uint32 width, uint32 height, const TexturetOption& option);
		GLTexture2D(const std::string& path, const TexturetOption& option);
		virtual ~GLTexture2D();

		virtual TextureFormat GetFormat() const override { return m_Format; }
		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual uint32 GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32 size) override;

		virtual void Bind(uint32 slot) const override;

		virtual bool IsHDR() const override
		{
			return m_IsHDR;
		}

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		void LoadLDRImage(const std::string& path, const TexturetOption& option);
		void LoadHDRImage(const std::string& path, const TexturetOption& option);
	private:
		std::string m_Path;
		uint32 m_Width, m_Height;
		uint32 m_RendererID = 0;
		TextureFormat m_Format;
		bool m_IsHDR = false;
	};

	// キューブテクスチャ
	class GLTextureCube : public TextureCube
	{
	public:
		GLTextureCube(uint32 width, uint32 height, const TexturetOption& option);
		GLTextureCube(const std::string& path, const TexturetOption& option);
		virtual ~GLTextureCube();

		virtual TextureFormat GetFormat() const override { return m_Format; }
		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual uint32 GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32 size) override;

		virtual void Bind(uint32 slot) const override;

		virtual bool IsHDR() const override 
		{
			return m_IsHDR;
		}

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		void LoadLDRImage(const std::string& path, const TexturetOption& option);
		void LoadHDRImage(const std::string& path, const TexturetOption& option);

	private:
		std::string m_Path;
		uint32 m_Width, m_Height;
		uint32 m_RendererID = 0;
		TextureFormat m_Format;
		bool m_IsHDR = false;
	};
}