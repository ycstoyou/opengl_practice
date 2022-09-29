
#include "context.h"
/*
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmfg/fginterface.h"
#include "dcmtk/dcmfg/fgbase.h"
#include "dcmtk/dcmfg/fgplanpo.h"*/
#include "LoadDicom.h"
#define WINDOW_NAME "Triangle Example"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {


	SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
	auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
	context->Reshape(width, height);

}

void OnKeyEvent(GLFWwindow* window,
	int key, int scancode, int action, int mods) {
	SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
		key, scancode,
		action == GLFW_PRESS ? "Pressed" :
		action == GLFW_RELEASE ? "Released" :
		action == GLFW_REPEAT ? "Repeat" : "Unknown",
		mods & GLFW_MOD_CONTROL ? "C" : "-",
		mods & GLFW_MOD_SHIFT ? "S" : "-",
		mods & GLFW_MOD_ALT ? "A" : "-");
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Render() {
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void OnCursorPos(GLFWwindow* window, double x, double y) {
	auto context = (Context*)glfwGetWindowUserPointer(window);
	context->MouseMove(x, y);
}
void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
	auto context = (Context*)glfwGetWindowUserPointer(window);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	context->MouseButton(button, action, x, y);
}
int main(int argc, const char** argv)
{
	LoadDicom dicom;
	dicom.openDicom();

}

/*
int main(int argc, const char** argv) {
	// ������ �˸��� �α�
	SPDLOG_INFO("Start program");
	
	SPDLOG_INFO("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
	// glfw ���̺귯�� �ʱ�ȭ, �����ϸ� ���� ����� ����
	SPDLOG_INFO("Initialize glfw");
	if (!glfwInit()) {
		const char* description = nullptr;
		glfwGetError(&description);
		SPDLOG_ERROR("failed to initialize glfw: {}", description);
		return -1;
	}
	//update
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//update
	// glfw ������ ����, �����ϸ� ���� ����� ����
	SPDLOG_INFO("Create glfw window");
	auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
		nullptr, nullptr);
	if (!window) {
		SPDLOG_ERROR("failed to create glfw window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//kllll
	// glad�� Ȱ���� OpenGL �Լ� �ε�
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		SPDLOG_ERROR("failed to initialize glad");
		glfwTerminate();
		return -1;
	}
	auto glVersion = glGetString(GL_VERSION);
	SPDLOG_INFO("OpenGL context version: {}", glVersion);
	
	// s == "The answer is 42."
	auto context = Context::Create();
	if (!context) {
		SPDLOG_ERROR("failed to create context");
		glfwTerminate();
		return -1;
	}
	//999
//fix
	ShaderPtr vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);//return type shader unique
	ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
	SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
	SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());


	auto program = Program::Create({ fragmentShader, vertexShader });
	printf("program id: {}", program->Get());
	
	glfwSetWindowUserPointer(window, context.get());
	
	OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window, OnKeyEvent);
	glfwSetCursorPosCallback(window, OnCursorPos);
	glfwSetMouseButtonCallback(window, OnMouseButton);
	// glfw ���� ����, ������ close ��ư�� ������ ���� ����
	SPDLOG_INFO("Start main loop");
	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		context->ProcessInput(window);
		context->Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	context.reset();
	//context = nullptr;

	glfwTerminate();
	return 0;
}
*/
