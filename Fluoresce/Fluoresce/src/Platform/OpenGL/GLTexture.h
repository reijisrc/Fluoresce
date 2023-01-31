//==============================================================================//
// Name : GLTexture.h															// 
// Describe : 	GLテクスチャ													// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2022/10/15														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Texture.h"

#include <glad/glad.h>

namespace Fluoresce {

	// GLテクスチャ
	class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(uint32 width, uint32 height);
		GLTexture2D(const std::string& path);
		virtual ~GLTexture2D();

		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32 size) override;

		virtual void Bind(uint32 slot) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		std::string m_Path;
		uint32 m_Width, m_Height;
		uint32 m_RendererID = 0;
		GLenum m_InternalFormat, m_DataFormat;
	};
}