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
	  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

	  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	};

	uint32_t indices[] = {
	   0,  2,  1,  2,  0,  3,
	   4,  5,  6,  6,  7,  4,
	   8,  9, 10, 10, 11,  8,
	  12, 14, 13, 14, 12, 15,
	  16, 17, 18, 18, 19, 16,
	  20, 22, 21, 22, 20, 23,
	};
	

	m_vertexLayout = VertexLayout::Create();
	m_vertexBuffer = Buffer::CreateWithData(		GL_ARRAY_BUFFER, GL_STATIC_DRAW,		vertices, sizeof(float) * 120);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE,		sizeof(float) * 5, 0);	
	//m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE,		sizeof(float) * 5, sizeof(float) * 3);
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE,		sizeof(float) * 5, sizeof(float) * 3);


	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

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
	glBindTexture(GL_TEXTURE_2D, m_texture->Get());//���ε�
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

	
	m_program->Use();
	m_program->SetUniform("tex", 0);
	m_program->SetUniform("tex2", 1);

	/*// ��ġ (1, 0, 0)�� ��. ������ǥ�� ���
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	// ������� ���� (1, 1, 0)��ŭ �����̵��ϴ� ���
	auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	// ������� ���� z������ 90����ŭ ȸ���ϴ� ���
	auto rot = glm::rotate(glm::mat4(1.0f),
		glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// ������� ���� ��� �࿡ ���� 3���� Ȯ���ϴ� ���
	auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
	// Ȯ�� -> ȸ�� -> �����̵� ������ ���� ���� ��ȯ ����
	vec = trans * rot * scale * vec;
	SPDLOG_INFO("transformed vec: [{}, {}, {}]", vec.x, vec.y, vec.z);*/


/*
	auto transform = glm::rotate(
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
		glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)
	);

	auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
*/

	// x������ -55�� ȸ��
	auto model = glm::rotate(glm::mat4(1.0f),
		glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// ī�޶�� �������κ��� z�� �������� -3��ŭ ������
	auto view = glm::translate(glm::mat4(1.0f),
		glm::vec3(0.0f, 0.0f, -3.0f));
	// ��Ⱦ�� 4:3, ����ȭ�� 45���� ���� ����
	auto projection = glm::perspective(glm::radians(45.0f),
		(float)640 / (float)480, 0.01f, 10.0f);
	

	auto transform = projection * view * model;
	m_program->SetUniform("transform", transform);
	/*auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));*/
	return true;
}
/*
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
*/

void Context::Render() 
{

	std::vector<glm::vec3> cubePositions = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f),
	};

	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	auto projection = glm::perspective(glm::radians(45.0f),
		(float)640 / (float)480, 0.01f, 20.0f);

	auto cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
	auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	auto cameraZ = glm::normalize(cameraPos - cameraTarget);
	auto cameraX = glm::normalize(glm::cross(cameraUp, cameraZ));
	auto cameraY = glm::cross(cameraZ, cameraX);

	auto cameraMat = glm::mat4(
		glm::vec4(cameraX, 0.0f),
		glm::vec4(cameraY, 0.0f),
		glm::vec4(cameraZ, 0.0f),
		glm::vec4(cameraPos, 1.0f));

	auto view = glm::inverse(cameraMat);

	for (size_t i = 0; i < cubePositions.size(); i++) {
		auto& pos = cubePositions[i];
		auto model = glm::translate(glm::mat4(1.0f), pos);
		model = glm::rotate(model,
			glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
			glm::vec3(1.0f, 0.5f, 0.0f));
		auto transform = projection * view * model;
		m_program->SetUniform("transform", transform);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

}