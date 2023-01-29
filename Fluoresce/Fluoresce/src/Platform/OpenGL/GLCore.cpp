//==============================================================================//
// Name : GLCore.cpp															// 
// Describe : 	GLÉRÉA															// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLCore.h"

#include <glad/glad.h>

namespace Fluoresce
{
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

	void GLCore::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
};