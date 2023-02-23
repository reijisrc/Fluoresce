//==============================================================================//
// Name : GLShader.cpp															// 
// Describe :	GLシェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Fluoresce {

	static GLenum GetShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		if (type == "compute")
			return GL_COMPUTE_SHADER;

		FR_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
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

	static void UploadUniformInt(uint32 renderId, const std::string& name, sint32 value)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniform1iv(location, 1, &value);
	}

	static void UploadUniformIntArray(uint32 renderId, const std::string& name, sint32* values, uint32_t count)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniform1iv(location, count, values);
	}

	static void UploadUniformFloat(uint32 renderId, const std::string& name, float32 value)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniform1fv(location, 1, &value);
	}

	static void UploadUniformFloatArray(uint32 renderId, const std::string& name, float32* values, uint32_t count)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniform1fv(location, count, values);
	}

	static void UploadUniformFloat2(uint32 renderId, const std::string& name, const Vec2& vec2)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(vec2));
	}

	static void UploadUniformFloat3(uint32 renderId, const std::string& name, const Vec3& vec3)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(vec3));
	}

	static void UploadUniformFloat4(uint32 renderId, const std::string& name, const Vec4& vec4)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(vec4));
	}

	static void UploadUniformMat3(uint32 renderId, const std::string& name, const Mat3& mat)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	static void UploadUniformMat4(uint32 renderId, const std::string& name, const Mat4& mat)
	{
		auto location = glGetUniformLocation(renderId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	static std::string ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				FR_CORE_ERROR("Could not read from file '{0}'", filePath);
			}
		}
		else
		{
			FR_CORE_ERROR("Could not open file : '{0}' ", filePath);
		}
		return result;
	}

	GLEffectShader::GLEffectShader(const std::string& filePath)
	{
		std::string shaderSouce = ReadFile(filePath);
		auto shaderSources = PreProcess(shaderSouce);
		Compile(shaderSources);
	}

	GLEffectShader::GLEffectShader(const std::string& vertexSrc, const std::string& fragSrc, bool compile)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;
		if (compile)
		{
			Compile(sources);
		}
	}

	GLEffectShader::~GLEffectShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void GLEffectShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void GLEffectShader::Unbind() const
	{
		glUseProgram(0);
	}

	void GLEffectShader::SetInt(const std::string& name, sint32 value)
	{
		UploadUniformInt(m_RendererID, name, value);
	}

	void GLEffectShader::SetIntArray(const std::string& name, sint32* values, uint32_t count)
	{
		UploadUniformIntArray(m_RendererID, name, values, count);
	}

	void GLEffectShader::SetFloat(const std::string& name, float32 value)
	{
		UploadUniformFloat(m_RendererID, name, value);
	}

	void GLEffectShader::SetFloatArray(const std::string& name, float32* values, uint32_t count)
	{
		UploadUniformFloatArray(m_RendererID, name, values, count);
	}

	void GLEffectShader::SetFloat3(const std::string& name, const Vec3& vec3)
	{
		UploadUniformFloat3(m_RendererID, name, vec3);
	}

	void GLEffectShader::SetFloat4(const std::string& name, const Vec4& vec4)
	{
		UploadUniformFloat4(m_RendererID, name, vec4);
	}

	void GLEffectShader::SetMat3(const std::string& name, const Mat3& mat)
	{
		UploadUniformMat3(m_RendererID, name, mat);
	}

	void GLEffectShader::SetMat4(const std::string& name, const Mat4& mat)
	{
		UploadUniformMat4(m_RendererID, name, mat);
	}

	std::unordered_map<GLenum, std::string> GLEffectShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeTokenStart = "<type>[";
		const char* typeTokenEnd = "]";
		size_t typeTokenLength = strlen(typeTokenStart);
		size_t pos = source.find(typeTokenStart, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of(typeTokenEnd, pos);
			FR_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength;
			std::string type = source.substr(begin, eol - begin);
			FR_CORE_ASSERT(GetShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of(typeTokenEnd, eol);
			FR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeTokenStart, nextLinePos);
			shaderSources[GetShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void GLEffectShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		FR_CORE_ASSERT(shaderSources.size() <= 2, "Only support  2 shader!");
		std::array<GLenum, 2> GLEffectShaderIDs;
		sint32 GLEffectShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				FR_CORE_ERROR("{0}", infoLog.data());
				FR_CORE_ASSERT(false, "Shader compliation failure!");
				break;
			}
			glAttachShader(program, shader);
			glDeleteShader(shader);
			GLEffectShaderIDs[GLEffectShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			FR_CORE_ERROR("{0}", infoLog.data());
			FR_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : GLEffectShaderIDs)
		{
			glDetachShader(program, id);
		}

		m_RendererID = program;
	}

	GLComputeShader::GLComputeShader(const std::string& filePath)
	{
		std::string shaderSouce = ReadFile(filePath);
		auto shaderSources = PreProcess(shaderSouce);
		Compile(shaderSources);
	}

	GLComputeShader::GLComputeShader(const std::string& computeSrc, bool compile)
	{
		std::string sources = computeSrc;
		if (compile)
		{
			Compile(sources);
		}
	}

	GLComputeShader::~GLComputeShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void GLComputeShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void GLComputeShader::Unbind() const
	{
		glUseProgram(0);
	}

	void GLComputeShader::SetInt(const std::string& name, sint32 value)
	{
		UploadUniformInt(m_RendererID, name, value);
	}

	void GLComputeShader::SetIntArray(const std::string& name, sint32* values, uint32_t count)
	{
		UploadUniformIntArray(m_RendererID, name, values, count);
	}

	void GLComputeShader::SetFloat(const std::string& name, float32 value)
	{
		UploadUniformFloat(m_RendererID, name, value);
	}

	void GLComputeShader::SetFloatArray(const std::string& name, float32* values, uint32_t count)
	{
		UploadUniformFloatArray(m_RendererID, name, values, count);
	}

	void GLComputeShader::SetFloat3(const std::string& name, const Vec3& vec3)
	{
		UploadUniformFloat3(m_RendererID, name, vec3);
	}

	void GLComputeShader::SetFloat4(const std::string& name, const Vec4& vec4)
	{
		UploadUniformFloat4(m_RendererID, name, vec4);
	}

	void GLComputeShader::SetMat3(const std::string& name, const Mat3& mat)
	{
		UploadUniformMat3(m_RendererID, name, mat);
	}

	void GLComputeShader::SetMat4(const std::string& name, const Mat4& mat)
	{
		UploadUniformMat4(m_RendererID, name, mat);
	}

	void GLComputeShader::DispatchCompute(uint32 groupsX, uint32 groupsY, uint32 groupsZ) const
	{
		glDispatchCompute(groupsX, groupsY, groupsZ);
	}

	void GLComputeShader::BindImageTexture(uint32 bingding, uint32 rendererID, uint32 minMapLevel, ImageTextureAccessFlag flag, TextureFormat format) const
	{
		GLenum glformat = ConvertOpenGLTextureInternalFormat(format);

		switch (flag)
		{
		case Fluoresce::ImageTextureAccessFlag::Read:
			glBindImageTexture(bingding, rendererID, minMapLevel, GL_FALSE, 0, GL_READ_ONLY, glformat);
			break;
		case Fluoresce::ImageTextureAccessFlag::Write:
			glBindImageTexture(bingding, rendererID, minMapLevel, GL_FALSE, 0, GL_WRITE_ONLY, glformat);
			break;
		case Fluoresce::ImageTextureAccessFlag::ReadWrite:
			glBindImageTexture(bingding, rendererID, minMapLevel, GL_FALSE, 0, GL_READ_WRITE, glformat);
			break;
		default:
			break;
		}
	}

	std::string GLComputeShader::PreProcess(const std::string& source)
	{
		std::string shaderSources;

		const char* typeTokenStart = "<type>[";
		const char* typeTokenEnd = "]";
		size_t typeTokenLength = strlen(typeTokenStart);
		size_t pos = source.find(typeTokenStart, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of(typeTokenEnd, pos);
			FR_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength;
			std::string type = source.substr(begin, eol - begin);
			FR_CORE_ASSERT(GetShaderTypeFromString(type) == GL_COMPUTE_SHADER, "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of(typeTokenEnd, eol);
			FR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeTokenStart, nextLinePos);
			shaderSources = source.substr(nextLinePos);
		}

		return shaderSources;
	}

	void GLComputeShader::Compile(std::string& shaderSources)
	{
		GLuint program = glCreateProgram();

		GLenum type = GL_COMPUTE_SHADER;
		const std::string& source = shaderSources;

		GLuint shader = glCreateShader(type);
		const GLchar* sourceStr = (const GLchar*)source.c_str();
		glShaderSource(shader, 1, &sourceStr, 0);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			FR_CORE_ERROR("{0}", infoLog.data());
			FR_CORE_ASSERT(false, "Shader compliation failure!");
		}
		glAttachShader(program, shader);
		glDeleteShader(shader);

		glLinkProgram(program);
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			FR_CORE_ERROR("{0}", infoLog.data());
			FR_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		glDetachShader(program, shader);

		m_RendererID = program;
	}

}