//==============================================================================//
// Name : GLTexture.h															// 
// Describe : 	GLテクスチャ													// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Texture.h"

namespace Fluoresce {

	// GLテクスチャ
	class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(uint32 width, uint32 height, const TexturetSpecification& spec);
		GLTexture2D(const std::string& path, const TexturetSpecification& spec);
		virtual ~GLTexture2D();

		virtual TexturetSpecification GetSpecification() const override { return m_Specification; }
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
		void LoadLDRImage(const std::string& path);
		void LoadHDRImage(const std::string& path);
	private:
		std::string m_Path;
		uint32 m_Width, m_Height;
		uint32 m_RendererID = 0;
		TexturetSpecification m_Specification;
		bool m_IsHDR = false;
	};

	// キューブテクスチャ
	class GLTextureCube : public TextureCube
	{
	public:
		GLTextureCube(uint32 width, uint32 height, const TexturetSpecification& spec);
		GLTextureCube(const std::string& path, const TexturetSpecification& spec);
		virtual ~GLTextureCube();

		virtual TexturetSpecification GetSpecification() const override { return m_Specification; }
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
		void LoadLDRImage(const std::string& path);
		void LoadHDRImage(const std::string& path);

	private:
		std::string m_Path;
		uint32 m_Width, m_Height;
		uint32 m_RendererID = 0;
		TexturetSpecification m_Specification;
		bool m_IsHDR = false;
	};
}