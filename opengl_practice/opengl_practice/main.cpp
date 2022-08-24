#include "common.h"
#include "shader.h"
#include "program.h"
//#include "context.h"

#define WINDOW_NAME "Triangle Example"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
	SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
	glViewport(0, 0, width, height);
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
#include "spdlog/cfg/env.h"
void load_levels_example()
{
	// Set the log level to "info" and mylogger to "trace":
	// SPDLOG_LEVEL=info,mylogger=trace && ./example
	spdlog::cfg::load_env_levels();
	// or from command line:
	// ./example SPDLOG_LEVEL=info,mylogger=trace
	// #include "spdlog/cfg/argv.h" // for loading levels from argv
	// spdlog::cfg::load_argv_levels(args, argv);
}
int main(int argc, const char** argv) {
	// 시작을 알리는 로그
	SPDLOG_INFO("Start program");
	load_levels_example();
	spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
	// glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
	SPDLOG_INFO("Initialize glfw");
	if (!glfwInit()) {
		const char* description = nullptr;
		glfwGetError(&description);
		SPDLOG_ERROR("failed to initialize glfw: {}", description);
		return -1;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw 윈도우 생성, 실패하면 에러 출력후 종료
	SPDLOG_INFO("Create glfw window");
	auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
		nullptr, nullptr);
	if (!window) {
		SPDLOG_ERROR("failed to create glfw window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad를 활용한 OpenGL 함수 로딩
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		SPDLOG_ERROR("failed to initialize glad");
		glfwTerminate();
		return -1;
	}
	auto glVersion = glGetString(GL_VERSION);
	SPDLOG_INFO("OpenGL context version: {}", glVersion);
	std::string s = fmt::format("The answer is {}.", 42);
	// s == "The answer is 42."
	fmt::print("{}\n", s);
	spdlog::trace("Trace");
	spdlog::debug("Debug");
	spdlog::info("Support for floats {:03.2f}", 1.23456);
	spdlog::warn("Easy padding in numbers like {:08d}", 12);
	spdlog::error("Some error message with arg: {}", 1);
	spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);

	ShaderPtr vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);//return type shader unique
	ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
	SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
	SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());


	auto program = Program::Create({ fragmentShader, vertexShader });
	printf("program id: {}", program->Get());

	OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window, OnKeyEvent);

	/*auto context = Context::Create();
	if (!context) {
		SPDLOG_ERROR("failed to create context");
		glfwTerminate();
		return -1;
	}*/

	// glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
	SPDLOG_INFO("Start main loop");
	while (!glfwWindowShouldClose(window)) {
		//context->Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//context.reset();

	glfwTerminate();
	return 0;
}