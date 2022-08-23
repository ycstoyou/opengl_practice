#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) 
{
	auto program = ProgramUPtr(new Program());
	if (!program->Link(shaders))
		return nullptr;
	return std::move(program);
}

Program::~Program()
{
	if (m_program) 
	{
		glDeleteProgram(m_program);
	}
}

bool Program::Link(const std::vector<ShaderPtr>& shaders) 
{
	m_program = glCreateProgram();
	for (auto& shader : shaders)//c++ 11부터 가능 set,map 같은 collection 
	{
		glAttachShader(m_program, shader->Get());
	}
	glLinkProgram(m_program);
	int success = 0;

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);//프로그램의 성공상태를 가지고옴

	if (!success) 
	{
		char infoLog[1024];
		glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
		SPDLOG_ERROR("failed to link program: {}", infoLog);
		return false;
	}
	return true;
}