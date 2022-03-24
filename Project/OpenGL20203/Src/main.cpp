/**
*	@file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#pragma comment(lib,"opengl32.lib")

/// 座標データ
const Position positions[] = 
{
	{-0.3f, -0.3f, 0.4f},
	{ 0.2f, -0.3f, 0.4f},
	{ 0.2f,  0.5f, 0.4f},
	{-0.3f,  0.5f, 0.4f},

	{-0.2f, -0.5f, 0.1f},
	{ 0.3f, -0.5f, 0.1f},
	{ 0.3f,  0.3f, 0.1f},
	{ 0.3f,  0.3f, 0.1f},
	{-0.2f,  0.3f, 0.1f},
	{-0.2f, -0.5f, 0.1f},

	{-0.33f, -0.5f, 0.5f },
	{ 0.33f, -0.5f, 0.5f },
	{ 0.0f,  0.5f, 0.5f },
};

/// 色データ
const Color colors[] =
{
	{0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},

	{1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},

	{ 0.0f, 1.0f, 1.0f, 1.0f }, // 水色
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // 紫色
};

// インデックスデータ
const GLushort indices[] =
{
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 7, 8, 9,
};

/// 頂点シェーダー.
static const GLchar* vsCode =
	"#version 450 \n"
	"layout(location=0) in vec3 vPosition; \n"
	"layout(location=1) in vec4 vColor; \n"
	"layout(location=0) out vec4 outColor; \n"
	"out gl_PerVertex { \n"
	"  vec4 gl_Position; \n"
	"}; \n"
	"void main() { \n"
	"  outColor = vColor; \n"
	"  float s = sin(radians(57.3)); \n"
	"  float c = cos(radians(57.3)); \n"
	"  mat4 matT = mat4(1); \n"
	"  matT[3] = vec4(-0.3,-0.5, 0.0, 1.0); \n"
	"  mat4 matS = mat4(1); \n"
	"  matS[0][0] = 0.5; \n"
	"  matS[1][1] = 1.5; \n"
	"  mat4 matR = mat4(1); \n"
	"  matR[0][0] = c; \n"
	"  matR[0][1] = -s; \n"
	"  matR[1][0] = s; \n"
	"  matR[1][1] = c; \n"
	"  gl_Position = matT * matS * matR * vec4(vPosition, 1.0); \n"
	"} \n";

/// フラグメントシェーダー.
static const GLchar * fsCode =
	"#version 450 \n"
	"layout(location=0) in vec4 inColor; \n"
	"out vec4 fragColor; \n"
	"void main() { \n"
	"  fragColor = inColor; \n"
	"} \n";

/**
*	OpenGLからのメッセージを処理する
*
*	@param source	メッセージの発信者(OpenGL,Windows,シェーダーなど)
*	@param type		メッセージの種類(エラー、警告など)
*	@param id		メッセージを一位に識別する値
*	@param severity	メッセージの重要度(高、中、低、最低)
*	@param length	メッセージの文字数、負数ならメッセージは0終端
*	@param message	メッセージの内容
*   @param userparam コールバック設定時に指定したポインタ
*/
void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* _source;
	const char* _type; 
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		_source = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		_source = "WINDOW SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		_source = "SHADER COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		_source = "THIRD PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		_source = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
	default:
		_source = "UNKNOWN";
		break;
	}

	int typeColor = 0;
	switch (type)
	{
	case GL_DEBUG_TYPE_PORTABILITY:
	case GL_DEBUG_TYPE_PERFORMANCE:
	case GL_DEBUG_TYPE_OTHER:
	case GL_DEBUG_TYPE_MARKER:
		typeColor = 30;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		typeColor = 33;
		break;
	case GL_DEBUG_TYPE_ERROR:
		typeColor = 31;
		break;
	default:
		typeColor = 30;
		break;
	}
	std::string test("\033[31m aho \033[m");

	std::string cmdStart;
	cmdStart += "\033[" + std::to_string(typeColor) + "m ";

	std::string cmdEnd;
	cmdEnd += " \033[m";

	if (length < 0)
	{
		std::cerr << "source=" + source << ": "
			<< cmdStart << message << cmdEnd << "\n";
	}
	else
	{
		const std::string s(message, message +length);
//		std::cerr << "source=" << _source << ": "
//			<< cmdStart << s << cmdEnd << "\n";
		std::cerr << s << "\n";
	}
}

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
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;
	}

	glDebugMessageCallback(DebugCallback, nullptr);
	
	// VAOを作成する.
	const GLuint vboPosition = GLContext::CreateBuffer(sizeof(positions), positions);
	const GLuint vboColor = GLContext::CreateBuffer(sizeof(colors), colors);
	const GLuint ibo = GLContext::CreateBuffer(sizeof(indices), indices);
	const GLuint vao = GLContext::CreateVertexArray(vboPosition, vboColor, ibo);
	if (!vao)
	{
		return 1;
	}

	//パイプラインオブジェクトを作成する
	const GLuint vp = GLContext::CreateProgram(GL_VERTEX_SHADER, vsCode);
	const GLuint fp = GLContext::CreateProgram(GL_FRAGMENT_SHADER, fsCode);
	const GLuint pipeline = GLContext::Createpipeline(vp, fp);
	if (!pipeline)
	{
		return 1;
	}

	//メインループ
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glBindProgramPipeline(pipeline);

		//glDrawArrays(GL_TRIANGLES, 0, sizeof(positions) / sizeof(positions[0]));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_SHORT, 0);

		glBindProgramPipeline(0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//後始末
	glDeleteProgramPipelines(1, &pipeline);
	glDeleteProgram(fp);
	glDeleteProgram(vp);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vboColor);
	glDeleteBuffers(1, &vboPosition);

	//GLFWの終了
	glfwTerminate();
}