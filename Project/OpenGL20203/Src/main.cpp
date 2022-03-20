/**
*	@file Main.cpp
*/
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
		return 0;
	}
}