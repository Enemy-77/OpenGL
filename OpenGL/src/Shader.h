#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgrameSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// set uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	ShaderProgrameSource ParseShader(const std::string& filepath);
	unsigned int create_shader(const std::string& vertex_shader, const std::string& framegment_shader);
	unsigned int CompilerShader(const std::string& source, unsigned int type);
	int GetUniformLocation(const std::string& name);
};