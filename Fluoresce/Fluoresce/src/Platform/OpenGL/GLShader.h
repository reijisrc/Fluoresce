//==============================================================================//
// Name : GLShader.h															// 
// Describe :	GLシェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Shader.h"

typedef unsigned int GLenum;

namespace Fluoresce {

	// GLエフェクトシェーダ(VS + PS)
	class GLEffectShader : public EffectShader
	{
	public:
		GLEffectShader(const std::string& filePath);
		GLEffectShader(const std::string& vertexSrc, const std::string& fragSrc, bool compile);
		virtual ~GLEffectShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, sint32 value) override;
		virtual void SetIntArray(const std::string& name, sint32* values, uint32_t count) override;

		virtual void SetFloat(const std::string& name, float32 value) override;
		virtual void SetFloatArray(const std::string& name, float32* values, uint32_t count) override;

		virtual void SetFloat3(const std::string& name, const Vec3& vec3) override;
		virtual void SetFloat4(const std::string& name, const Vec4& ve4) override;

		virtual void SetMat3(const std::string& name, const Mat3& mat) override;
		virtual void SetMat4(const std::string& name, const Mat4& mat) override;
	private:
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32 m_RendererID = 0;
	};

	// GLコンピュートシェーダ
	class GLComputeShader : public ComputeShader
	{
	public:
		GLComputeShader(const std::string& filePath);
		GLComputeShader(const std::string& computeSrc, bool compile);
		virtual ~GLComputeShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, sint32 value) override;
		virtual void SetIntArray(const std::string& name, sint32* values, uint32_t count) override;

		virtual void SetFloat(const std::string& name, float32 value) override;
		virtual void SetFloatArray(const std::string& name, float32* values, uint32_t count) override;

		virtual void SetFloat3(const std::string& name, const Vec3& vec3) override;
		virtual void SetFloat4(const std::string& name, const Vec4& ve4) override;

		virtual void SetMat3(const std::string& name, const Mat3& mat) override;
		virtual void SetMat4(const std::string& name, const Mat4& mat) override;

		virtual void DispatchCompute(uint32 groupsX, uint32 groupsY, uint32 groupsZ) const override;
		virtual void BindImageTexture(uint32 slot, uint32 rendererID, uint32 minMapLevel, ImageTextureAccessFlag flag, TextureFormat format) const override;
	private:
		std::string PreProcess(const std::string& source);
		void Compile(std::string& shaderSources);
	private:
		uint32 m_RendererID = 0;
	};
}