#include "context.h"
#include "image.h"


ContextUPtr Context::Create()
{
	auto context = ContextUPtr(new Context());
	if (!context->Init())
		return nullptr;
	return std::move(context);
}

bool Context::Init()
{


	float vertices[] = {
	  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	  -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};
	
	uint32_t indices[] = { // note that we start from 0!
	  0, 1, 3, // first triangle
	  1, 2, 3, // second triangle
	};

	m_vertexLayout = VertexLayout::Create();
	m_vertexBuffer = Buffer::CreateWithData(		GL_ARRAY_BUFFER, GL_STATIC_DRAW,		vertices, sizeof(float) * 32);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE,		sizeof(float) * 8, 0);	
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE,		sizeof(float) * 8, sizeof(float) * 3);
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE,		sizeof(float) * 8, sizeof(float) * 6);


	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

	ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
	ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);



	if (!vertShader || !fragShader)
		return false;
	SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
	SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

	m_program = Program::Create({ fragShader, vertShader });
	if (!m_program)
		return false;
	SPDLOG_INFO("program id: {}", m_program->Get());

	auto loc = glGetUniformLocation(m_program->Get(), "color");
	m_program->Use();
	glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

	//cpu
	auto image = Image::Load("./image/container.jpg");
	m_texture = Texture::CreateFromImage(image.get());
	if (!image)
		return false;
// 	SPDLOG_INFO("image: {}x{}, {} channels",
// 		image->GetWidth(), image->GetHeight(), image->GetChannelCount());
	//gpu
	/*auto image = Image::Create(512, 512);
	image->SetCheckImage(16, 16);
	m_texture = Texture::CreateFromImage(image.get());*/

	auto image2 = Image::Load("./image/awesomeface.png");
	m_texture2 = Texture::CreateFromImage(image2.get());

	glActiveTexture(GL_TEXTURE0);//slot number
	glBindTexture(GL_TEXTURE_2D, m_texture->Get());//바인딩
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

	m_program->Use();
	glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);//0번 슬롯의 texture를 사용
	glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);

	/*// 위치 (1, 0, 0)의 점. 동차좌표계 사용
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	// 단위행렬 기준 (1, 1, 0)만큼 평행이동하는 행렬
	auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	// 단위행렬 기준 z축으로 90도만큼 회전하는 행렬
	auto rot = glm::rotate(glm::mat4(1.0f),
		glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// 단위행렬 기준 모든 축에 대해 3배율 확대하는 행렬
	auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
	// 확대 -> 회전 -> 평행이동 순으로 점에 선형 변환 적용
	vec = trans * rot * scale * vec;
	SPDLOG_INFO("transformed vec: [{}, {}, {}]", vec.x, vec.y, vec.z);*/
	auto transform = glm::rotate(
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
		glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)
	);

	auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	return true;
}
void Context::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	static float time = 0.0f;
	float t = sinf(time) * 0.5f + 0.5f;
	auto loc = glGetUniformLocation(m_program->Get(), "color");
	m_program->Use();
	glUniform4f(loc, t * t, 2.0f * t * (1.0f - t), (1.0f - t) * (1.0f - t), 1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	time += 0.016f;


}
/*
void Context::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(m_program->Get());
	m_program->Use();
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}*/