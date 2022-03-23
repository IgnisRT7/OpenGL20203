/**
*	@file Main.cpp
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma comment(lib,"opengl32.lib")

/**
*	エントリーポイント
*/
int main()
{
	//GLFWの初期化
	if (glfwInit() != GLFW_TRUE)
	{
		return 1;
	}

	//描画ウィンドウの作成
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGLGame", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	//OpenGL関数のアドレスを取得する
	using pfnglClear = void(GLAPIENTRY*)(GLbitfield);
	pfnglClear glClear = reinterpret_cast<pfnglClear>(glfwGetProcAddress("glClear"));
	using pfnglClearColor = void(GLAPIENTRY*)(GLfloat, GLfloat, GLfloat, GLfloat);
	pfnglClearColor glClearColor = reinterpret_cast<pfnglClearColor>(glfwGetProcAddress("glClearColor"));

	//メインループ
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//GLFWの終了
	glfwTerminate();
}