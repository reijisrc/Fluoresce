//==============================================================================//
// Name : GraphicsContext.cpp													// 
// Describe : 	GLコンテキスト													// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Fluoresce
{
	GLContext::GLContext(GLFWwindow* windowHandle) :
		m_WindowHandle(windowHandle)
	{
		FR_CORE_ASSERT(windowHandle)
	}

	void GLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FR_CORE_ASSERT(status, "Failed to initailize Glad!");

		FR_CORE_INFO("OpenGL Version : {0}", (const char*)glGetString(GL_VERSION));
		FR_CORE_INFO("Graphic Device : {0}", (const char*)glGetString(GL_RENDERER));
	}

	void GLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
};
