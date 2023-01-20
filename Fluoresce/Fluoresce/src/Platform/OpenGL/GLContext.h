//==============================================================================//
// Name : GLContext.h															// 
// Describe : 	GLコンテキスト													// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/GraphicsContext.h"

struct GLFWwindow;

namespace Fluoresce 
{
	class GLContext : public GraphicsContext
	{
	public:
		GLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffer() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}