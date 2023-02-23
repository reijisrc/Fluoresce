//==============================================================================//
// Name : GLUtil.h																// 
// Describe :	GL汎用機能														// 
// Author : Ding Qi																// 
// Create Date : 2023/02/23														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "Engine/Core/Assert.h"
#include "Engine/Graphics/TextureDefine.h"
#include "Engine/Graphics/GraphicsCore.h"

#include <glad/glad.h>

namespace Fluoresce {

	// GL汎用機能
	class GLUtils
	{
	public:
		static GLbitfield ConvertOpenGLClearCommand(ClearCommand command)
		{
			GLbitfield glcommand = 0;
			if (command || ClearCommand::CommandClearColor)
			{
				glcommand |= GL_COLOR_BUFFER_BIT;
			}

			if (command || ClearCommand::CommandClearDepth)
			{
				glcommand |= GL_DEPTH_BUFFER_BIT;
			}

			if (command || ClearCommand::CommandClearStencil)
			{
				glcommand |= GL_STENCIL_BUFFER_BIT;
			}

			return glcommand;
		}

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

		static GLenum ConvertOpenGLTextureDataType(TextureFormat format)
		{
			switch (format)
			{
			case Fluoresce::TextureFormat::RGB:     return GL_UNSIGNED_BYTE;
			case Fluoresce::TextureFormat::RGBA:    return GL_UNSIGNED_BYTE;
			case Fluoresce::TextureFormat::RGBA16f:    return GL_FLOAT;
			}
			FR_CORE_ASSERT(false, "Unknown texture format!");
			return 0;
		}

		static GLenum ConvertOpenGLTextureFilter(TextureFilter filter)
		{
			switch (filter)
			{
			case Fluoresce::TextureFilter::Nearest:
				return GL_NEAREST;
			case Fluoresce::TextureFilter::Linear:
				return GL_LINEAR;
			default:
				break;
			}
			FR_CORE_ASSERT(false, "Unknown texture filter!");
			return 0;
		}

		static GLenum ConvertOpenGLTextureWrap(TextureWrap warp)
		{
			switch (warp)
			{
			case Fluoresce::TextureWrap::Clamp:
				return GL_CLAMP_TO_EDGE;
			case Fluoresce::TextureWrap::Repeat:
				return GL_REPEAT;
			case Fluoresce::TextureWrap::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
			case Fluoresce::TextureWrap::Board:
				return GL_CLAMP_TO_BORDER;
			default:
				break;
			}

			FR_CORE_ASSERT(false, "Unknown texture warp!");
			return 0;
		}
	};
}