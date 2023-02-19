//==============================================================================//
// Name : Shader.cpp															// 
// Describe :	シェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/Shader.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLShader.h"

namespace Fluoresce {

	Ref<EffectShader> EffectShader::Create(const std::string& filePath)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLEffectShader>(filePath);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

	Ref<EffectShader> EffectShader::CreateBySrc(const std::string& vertexSrc, const std::string& fragSrc)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLEffectShader>(vertexSrc, fragSrc, true);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	Ref<ComputeShader> ComputeShader::Create(const std::string& filePath)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLComputeShader>(filePath);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

	Ref<ComputeShader> ComputeShader::CreateBySrc(const std::string& computeSrc)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLComputeShader>(computeSrc, true);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		auto& shadername = name;
		FR_CORE_ASSERT(!Exists(shadername), "Shader aleady exists!");
		m_Shaders[shadername] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath, ShaderType type)
	{
		Ref<Shader> shader = nullptr;
		switch (type)
		{
		case Fluoresce::ShaderType::EffectShader:
			shader = EffectShader::Create(filePath);
			Add(name, shader);
			break;
		case Fluoresce::ShaderType::ComputeShader:
			shader = ComputeShader::Create(filePath);
			Add(name, shader);
			break;
		default:
			break;
		}

		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		FR_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}