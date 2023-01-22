//==============================================================================//
// Name : GLShader.cpp															// 
// Describe :	GLシェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
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

		FR_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	GLShader::GLShader(const std::string& filePath)
	{
		std::string shaderSouce = ReadFile(filePath);
		auto shaderSources = PreProcess(shaderSouce);
		Compile(shaderSources);

		//
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind(".");
		auto count = (lastDot == std::string::npos) ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
	}

	GLShader::GLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;
		Compile(sources);
	}

	GLShader::~GLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void GLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void GLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void GLShader::SetInt(const std::string& name, sint32 value)
	{
		UploadUniformInt(name, value);
	}
	void GLShader::SetIntArray(const std::string& name, sint32* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}
	void GLShader::SetFloat(const std::string& name, float32 value)
	{
		UploadUniformFloat(name, value);
	}

	void GLShader::SetFloatArray(const std::string& name, float32* values, uint32_t count)
	{
		UploadUniformFloatArray(name, values, count);
	}

	void GLShader::SetFloat3(const std::string& name, const Vec3& vec3)
	{
		UploadUniformFloat3(name, vec3);
	}

	void GLShader::SetFloat4(const std::string& name, const Vec4& vec4)
	{
		UploadUniformFloat4(name, vec4);
	}

	void GLShader::SetMat3(const std::string& name, const Mat3& mat)
	{
		UploadUniformMat3(name, mat);
	}

	void GLShader::SetMat4(const std::string& name, const Mat4& mat)
	{
		UploadUniformMat4(name, mat);
	}

	void GLShader::UploadUniformInt(const std::string& name, sint32 value)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, 1, &value);
	}

	void GLShader::UploadUniformIntArray(const std::string& name, sint32* values, uint32_t count)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void GLShader::UploadUniformFloat(const std::string& name, float32 value)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1fv(location, 1, &value);
	}

	void GLShader::UploadUniformFloatArray(const std::string& name, float32* values, uint32_t count)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1fv(location, count, values);
	}

	void GLShader::UploadUniformFloat2(const std::string& name, const Vec2& vec2)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(vec2));
	}

	void GLShader::UploadUniformFloat3(const std::string& name, const Vec3& vec3)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(vec3));
	}

	void GLShader::UploadUniformFloat4(const std::string& name, const Vec4& vec4)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(vec4));
	}

	void GLShader::UploadUniformMat3(const std::string& name, const Mat3& mat)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void GLShader::UploadUniformMat4(const std::string& name, const Mat4& mat)
	{
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	std::string GLShader::ReadFile(const std::string& filePath)
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

	std::unordered_map<GLenum, std::string> GLShader::PreProcess(const std::string& source)
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

	void GLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		FR_CORE_ASSERT(shaderSources.size() <= 2, "Only support  2 shader!");
		std::array<GLenum, 2> glShaderIDs;
		sint32 glShaderIDIndex = 0;
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
			glShaderIDs[glShaderIDIndex++] = shader;
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

			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			FR_CORE_ERROR("{0}", infoLog.data());
			FR_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		m_RendererID = program;
	}
}