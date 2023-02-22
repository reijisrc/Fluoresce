//==============================================================================//
// Name : Shader.h																// 
// Describe :	シェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	enum class ShaderType
	{
		EffectShader = 0,	// (VS + PS)
		ComputeShader
	};

	// シェーダ
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, sint32 value) = 0;
		virtual void SetIntArray(const std::string& name, sint32* values, uint32 count) = 0;

		virtual void SetFloat(const std::string& name, float32 value) = 0;
		virtual void SetFloatArray(const std::string& name, float32* values, uint32 count) = 0;

		virtual void SetFloat3(const std::string& name, const Vec3& vec3) = 0;
		virtual void SetFloat4(const std::string& name, const Vec4& ve4) = 0;

		virtual void SetMat3(const std::string& name, const Mat3& mat) = 0;
		virtual void SetMat4(const std::string& name, const Mat4& mat) = 0;
	};

	// エフェクトシェーダ(VS + PS)
	class EffectShader : public Shader
	{
	public:
		static Ref<EffectShader> Create(const std::string& filePath);
		static Ref<EffectShader> CreateBySrc(const std::string& vertexSrc, const std::string& fragSrc);
	};

	// コンピュートシェーダ
	class ComputeShader : public Shader
	{
	public:
		virtual void DispatchCompute(uint32 groupsX, uint32 groupsY, uint32 groupsZ) const = 0;
	public:
		static Ref<ComputeShader> Create(const std::string& filePath);
		static Ref<ComputeShader> CreateBySrc(const std::string& computeSrc);
	};

	// シェーダライブラリ
	class ShaderLibrary
	{
	public:
		virtual ~ShaderLibrary() = default;

		void Add(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& name, const std::string& filePath, ShaderType type);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}