//==============================================================================//
// Name : GLShader.h															// 
// Describe :	GLシェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Shader.h"

typedef unsigned int GLenum;

namespace Fluoresce {

	// GLシェーダ
	class GLShader : public Shader
	{
	public:
		GLShader(const std::string& filePath);
		GLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc);
		~GLShader();

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

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, sint32 value);
		void UploadUniformIntArray(const std::string& name, sint32* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float32 value);
		void UploadUniformFloatArray(const std::string& name, float32* values, uint32_t count);

		void UploadUniformFloat2(const std::string& name, const Vec2& vec2);
		void UploadUniformFloat3(const std::string& name, const Vec3& vec3);
		void UploadUniformFloat4(const std::string& name, const Vec4& ve4);

		void UploadUniformMat3(const std::string& name, const Mat3& mat);
		void UploadUniformMat4(const std::string& name, const Mat4& mat);
	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32 m_RendererID = 0;
		std::string m_Name;
	};
}