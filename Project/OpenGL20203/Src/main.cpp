/**
*	@file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include "Primitive.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#pragma comment(lib,"opengl32.lib")

/// 座標データ
const Position positions[] = 
{
	// 地面
	{-20.0f, 0.0f, 20.0f},
	{ 20.0f, 0.0f, 20.0f},
	{ 20.0f, 0.0f,-20.0f},
	{-20.0f, 0.0f,-20.0f},

	{-0.2f, -0.5f, 0.1f},
	{ 0.3f, -0.5f, 0.1f},
	{ 0.3f,  0.3f, 0.1f},
	{ 0.3f,  0.3f, 0.1f},
	{-0.2f,  0.3f, 0.1f},
	{-0.2f, -0.5f, 0.1f},
	
	//三角形x3
	{-0.33f * 5, 2.0f * 5, 0.5f },
	{ 0.33f * 5, 2.0f * 5, 0.5f },
	{ 0.0f * 5,  1.0f * 5, 0.5f },
	{ 0.33f * 5, 2.0f * 5, 0.5f },
	{ 0.99f * 5, 2.0f * 5, 0.5f },
	{ 0.66f * 5, 1.0f * 5, 0.5f },
	{ 0.0f * 5,  1.0f * 5, 0.5f },
	{ 0.66f * 5, 1.0f * 5, 0.5f },
	{ 0.33f * 5, 0.0f * 5, 0.5f },

	//立方体
	{ 0.0f, 0.0f, 2.0f },
	{ 2.0f, 0.0f, 2.0f },
	{ 2.0f, 2.0f, 2.0f },
	{ 0.0f, 2.0f, 2.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 2.0f, 0.0f, 0.0f },
	{ 2.0f, 2.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f },
};

/// 色データ
const Color colors[] =
{
	//地面 0 - 3
	{0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},

	//非共有面 4 - 9
	{1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},

	//三角形x3 10 - 18
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // 水色
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // 紫色
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // 水色
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // 紫色
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // 水色
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // 紫色

	//立方体 19 - 26
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f}, 
	{1.0f, 1.0f, 1.0f, 1.0f}, 
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
};

// インデックスデータ
const GLushort indices[] =
{
	//地面
	0, 1, 2, 2, 3, 0,
	//四角ポリゴン
	4, 5, 6, 7, 8, 9,
	//三角形x3
	12, 11, 10, 15, 14, 13, 18, 17, 16,
	//立方体
	19, 20, 21, 19, 21, 22, // 手前
	20, 24, 25, 20, 25, 21, // 右
	22, 21, 25, 22, 25, 26, // 上
	19, 23, 26, 19, 26, 22, // 左
	19, 20, 24, 19, 24, 23, // 下
	23, 24, 25, 23, 25, 26, // 奥
};

//描画データ
const Primitive primGround(GL_TRIANGLES, 6, 0 * sizeof(GLushort), 0); //四角形
const Primitive primTriangles(GL_TRIANGLES, 9, 12 * sizeof(GLushort), 0); //三角形
const Primitive primCube(GL_TRIANGLES, 36, 21 * sizeof(GLushort), 0); //立方体

//画像データ
const int imageGroundWidth = 8; // 画面の幅
const int imageGroundHeight = 8; // 画面の高さ
const int imageTriangleWidth = 6;
const int imageTriangleHeight = 6;
const GLuint X = 0xff'18'18'18; // 黒
const GLuint W = 0xff'ff'ff'ff; // 白
const GLuint R = 0xff'10'10'e0; // 赤
const GLuint B = 0xff'e0'10'10; // 青
const GLuint imageGround[imageGroundWidth * imageGroundHeight] =
{
	X, B, B, B, X, W, W, W,
	X, B, B, B, X, W, W, W,
	X, B, B, B, X, W, W, W,
	X, X, X, X, X, X, X, X,
	W, W, X, R, R, R, X, W,
	W, W, X, R, R, R, X, W,
	W, W, X, R, R, R, X, W,
	X, X, X, X, X, X, X, X,
};
const GLuint imageTriangle[imageTriangleWidth * imageTriangleHeight] = 
{
	W, B, W, B, W, B,
	B, W, B, W, B, W,
	W, B, W, B, W, B,
	B, W, B, W, B, W,
	W, B, W, B, W, B,
	B, W, B, W, B, W,
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
	"layout(location=0) uniform mat4 matTRS; \n"
	"void main() { \n"
	"  outColor = vColor; \n"
	"  gl_Position = matTRS * vec4(vPosition, 1.0); \n"
	"} \n";

/// フラグメントシェーダー.
static const GLchar * fsCode =
	"#version 450 \n"
	"layout(location=0) in vec4 inColor; \n"
	"out vec4 fragColor; \n"
	"layout(binding=0) uniform sampler2D texColor; \n"
	"void main() { \n"
	"  fragColor = texture(texColor, gl_FragCoord.xy * 0.01); \n"
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

	// uniform変数の位置
	const GLint locMatTRS = 0;

	//座標返還行列の回転角度
	float degree = 0;

	//テクスチャ作成
	const GLuint texGround = GLContext::CreateImage2D(imageGroundWidth, imageGroundHeight, imageGround);
	const GLuint texTriangle = GLContext::CreateImage2D(imageTriangleWidth, imageTriangleHeight, imageTriangle);
	if (!texGround || !texTriangle)
	{
		return 1;
	}

	//メインループ
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glBindProgramPipeline(pipeline);
		float s = sin(glm::radians(degree));
		float c = cos(glm::radians(degree));
		degree += 0.01f;
		glm::mat4 matT = glm::mat4(1);
		matT[3] = glm::vec4(-0.3,-0.5, 0.0, 1.0);
		glm::mat4 matS = glm::mat4(1);
		matS[0][0] = 0.5;
		matS[1][1] = 1.5;
		glm::mat4 matR = glm::mat4(1);
		matR[0][0] = c;
		matR[0][1] = -s;
		matR[1][0] = s;
		matR[1][1] = c;

		//プロジェクション行列を作成
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		const float aspectRatio =static_cast<float>(w) / static_cast<float>(h);
		const glm::mat4 matProj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

		//ビュー行列を作成
		const glm::mat4 matView = glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		//行列をシェーダに転送する
		const glm::mat4 matModel = glm::mat4(1);
		const glm::mat4 matMVP = matProj * matView * matModel;
		glProgramUniformMatrix4fv(vp, locMatTRS, 1, GL_FALSE, &matMVP[0][0]);

		glBindTextureUnit(0, texGround); // テクスチャを割り当てる

		primGround.Draw();
		primTriangles.Draw();
		//primCube.Draw();

		// テクスチャの割り当てを解除
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindProgramPipeline(0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//後始末
	glDeleteTextures(1, &texGround);
	glDeleteTextures(1, &texTriangle);
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