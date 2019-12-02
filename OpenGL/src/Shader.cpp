#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"

#include <gl/glew.h>

Shader::Shader(const std::string & filepath) : m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgrameSource source = ParseShader(filepath);
	m_RendererID = create_shader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID))
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value))
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}


ShaderProgrameSource Shader::ParseShader(const std::string& filepath)
{
	/* open the file */
	std::ifstream stream(filepath);

	enum ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	/* read file one line by one line */
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		/* if find */
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
			ss[(int)type] << line << "\n";
	}
	return{ ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompilerShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Fail to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::create_shader(const std::string& vertex_shader, const std::string& framegment_shader)
{
	unsigned int programe = glCreateProgram();
	unsigned int vs = CompilerShader(vertex_shader, GL_VERTEX_SHADER);
	unsigned int fs = CompilerShader(framegment_shader, GL_FRAGMENT_SHADER);

	glAttachShader(programe, vs);
	glAttachShader(programe, fs);
	glLinkProgram(programe);
	glValidateProgram(programe);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return programe;
}


int Shader::GetUniformLocation(const std::string & name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform" << name << "does not exist" << std::endl;
	else
		m_UniformLocationCache[name] = location;
	return location;
}
