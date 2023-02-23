//==============================================================================//
// Name : GLCore.cpp															// 
// Describe : 	GLÉRÉA															// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLCore.h"

#include "Platform/OpenGL/GLUtils.h"

namespace Fluoresce
{
	static GraphicsCore::APICapabilities s_caps;

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         FR_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       FR_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          FR_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: FR_CORE_TRACE(message); return;
		}

		FR_CORE_ASSERT(false, "Unknown severity level!");
	}


	void GLCore::Init()
	{
#ifdef FR_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
		glGetIntegerv(GL_MAX_SAMPLES, &s_caps.MaxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &s_caps.MaxAnisotropy);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &s_caps.MaxTextureUnits);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LINE_SMOOTH);
	}

	void GLCore::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		glViewport(x, y, width, height);
	}

	void GLCore::SetClearColor(const FrVec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLCore::SetClearColor(const Vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLCore::Clear(ClearCommand command)
	{
		glClear(GLUtils::ConvertOpenGLClearCommand(command));
	}

	void GLCore::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = (indexCount == 0) ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void GLCore::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void GLCore::SetLineWidth(float32 width)
	{
		glLineWidth(width);
	}

	void GLCore::SetDepthTest(bool enable)
	{
		enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void GLCore::SetDepthTestFunc(DepthTestFunc func)
	{
		switch (func)
		{
		case Fluoresce::DepthTestFunc::DepthTest_Default:
			glDepthFunc(GL_LESS);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_Always:
			glDepthFunc(GL_ALWAYS);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_Never:
			glDepthFunc(GL_NEVER);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_Less:
			glDepthFunc(GL_LESS);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_Equal:
			glDepthFunc(GL_EQUAL);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_Lequal:
			glDepthFunc(GL_LEQUAL);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_Greater:
			glDepthFunc(GL_GREATER);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_NotEqual:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case Fluoresce::DepthTestFunc::DepthTest_Gequal:
			glDepthFunc(GL_GEQUAL);
			break;
		default:
			break;
		}
	}

	void GLCore::SetMemoryBarrier(GPUMemoryBarrier barriers)
	{
		switch (barriers)
		{
		case Fluoresce::GPUMemoryBarrier::Barrier_ShaderStorage:
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			break;
		case Fluoresce::GPUMemoryBarrier::Barrier_Texture:
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
			break;
		default:
			break;
		}
	}

	const GraphicsCore::APICapabilities& GLCore::GetCapabilities() const
	{
		return s_caps;
	}
};