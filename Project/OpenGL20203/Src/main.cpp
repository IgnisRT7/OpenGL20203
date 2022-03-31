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

/// 座標データ:地面
const glm::vec3 posGround[] = 
{
	// 地面 0 - 3
	{-2.0f, 0.0f, 2.0f},
	{ 2.0f, 0.0f, 2.0f},
	{ 2.0f, 0.0f,-2.0f},
	{-2.0f, 0.0f,-2.0f},
};

/// 座標データ: 四角形
const glm::vec3 posRectAngle[] =
{	
	{-0.2f, -0.5f, 0.1f},
	{ 0.3f, -0.5f, 0.1f},
	{ 0.3f,  0.3f, 0.1f},
	{ 0.3f,  0.3f, 0.1f},
	{-0.2f,  0.3f, 0.1f},
	{-0.2f, -0.5f, 0.1f},
};

/// 座標データ:三角形
const glm::vec3 posTriangles[] =
{
	{(-0.33f / 2.0f) * 10.0f, (0.5f / 2.0f) * 10.0f, 0.6f },
	{(0.33f / 2.0f) * 10.0f, (0.5f / 2.0f) * 10.0f, 0.6f },
	{(0.00f / 2.0f) * 10.0f, (-0.5f / 2.0f) * 10.0f, 0.6f },
	{(-0.33f / 2.0f - 0.165f) * 10.0f, (0.5f / 2.0f + 0.5f) * 10.0f, 0.6f },
	{(0.33f / 2.0f - 0.165f) * 10.0f, (0.5f / 2.0f + 0.5f) * 10.0f, 0.6f },
	{(0.00f / 2.0f - 0.165f) * 10.0f, (-0.5f / 2.0f + 0.5f) * 10.0f, 0.6f },
	{(-0.33f / 2.0f + 0.165f) * 10.0f, (0.5f / 2.0f + 0.5f) * 10.0f, 0.6f },
	{(0.33f / 2.0f + 0.165f) * 10.0f, (0.5f / 2.0f + 0.5f) * 10.0f, 0.6f },
	{(0.00f / 2.0f + 0.165f) * 10.0f, (-0.5f / 2.0f + 0.5f) * 10.0f, 0.6f },
};

/// 座標データ:立方体 
const glm::vec3 posCube[] =
{
	{ 0.0f, 0.0f, 2.0f },
	{ 2.0f, 0.0f, 2.0f },
	{ 2.0f, 2.0f, 2.0f },
	{ 0.0f, 2.0f, 2.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 2.0f, 0.0f, 0.0f },
	{ 2.0f, 2.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f },
};

/// 座標データ:木
const glm::vec3 posTree[] =
{
	{ 0.0f, 3.0f, 0.0f},
	{ 0.0f, 1.0f,-1.0f},
	{-1.0f, 1.0f, 0.0f},
	{ 0.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 0.0f},
	{ 0.0f, 1.0f,-1.0f},
	
	{ 0.0f, 2.0f, 0.0f},
	{ 0.0f, 0.0f,-0.5f},
	{-0.5f, 0.0f, 0.0f},
	{ 0.0f, 0.0f, 0.5f},
	{ 0.5f, 0.0f, 0.0f},
	{ 0.0f, 0.0f,-0.5f},
};

/// 座標データ:建物
const glm::vec3 posWarehouse[] =
{
	{ -2.0f, 0.0f, -2.0f },
	{-2.0f, 0.0f, 2.0f},
	{ 2.0f, 0.0f, 2.0f},
	{ 2.0f, 0.0f,-2.0f},
	{-2.0f, 0.0f,-2.0f},
	{-2.0f, 2.0f,-2.0f},
	{-2.0f, 2.0f, 2.0f},
	{ 2.0f, 2.0f, 2.0f},
	{ 2.0f, 2.0f,-2.0f},
	{-2.0f, 2.0f,-2.0f},
	{ 2.0f, 2.0f, 2.0f},
	{ 2.0f, 2.0f,-2.0f},
};

/// 色データ: 地面
const glm::vec4 colGround[] =
{
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
};

/// 色データ:四角形
const glm::vec4 colRectangle[] =
{
	{1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
};

/// 色データ:三角形x3
const glm::vec4 colTriangles[] =
{
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // 水色
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // 紫色
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // 水色
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // 紫色
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // 水色
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // 紫色
};

/// 色データ;立方体
const glm::vec4 colCube[] =
{
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f}, 
	{ 1.0f, 1.0f, 1.0f, 1.0f}, 
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
};

/// 色データ:木
const glm::vec4 colTree[] =
{
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
};

/// 色データ:建物
const glm::vec4 colWarehouse[] =
{
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
	{ 1.0f, 1.0f, 1.0f, 1.0f},
};

/// テクスチャ座標データ:地面
const glm::vec2 tcGround[] =
{
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },
};

/// テクスチャ座標データ:四角形
const glm::vec2 tcRectangle[] =
{
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
};

/// テクスチャ座標データ:三角形x3
const glm::vec2 tcTriangles[] = 
{
	{ 0.0, 0.1 },
	{ 1.0, 0.1 },
	{ 0.5, 0.9 },
	{ 0.0, 0.1 },
	{ 1.0, 0.1 },
	{ 0.5, 0.9 },
	{ 0.0, 0.1 },
	{ 1.0, 0.1 },
	{ 0.5, 0.9 },
};

/// テクスチャ座標データ:立方体
const glm::vec2 tcCube[] =
{
	{ 0.0f, 0.0f}, { 0.0f, 0.0f}, { 0.0f, 0.0f}, { 0.0f, 0.0f},
	{ 0.0f, 0.0f}, { 0.0f, 0.0f}, { 0.0f, 0.0f}, { 0.0f, 0.0f},
};

/// テクスチャ座標データ:木
const glm::vec2 tcTree[] =
{
	{ 0.5f, 1.0f},
	{ 0.0f, 0.5f},
	{ 0.25f, 0.5f},
	{ 0.5f, 0.5f},
	{ 0.75f, 0.5f},
	{ 1.0f, 0.5f},

	{ 0.5f, 0.5f},
	{ 0.0f, 0.0f},
	{ 0.25f, 0.0f},
	{ 0.5f, 0.0f},
	{ 0.75f, 0.0f},
	{ 1.0f, 0.0f},
};

/// テクスチャ座標データ:建物
const glm::vec2 tcWarehouse[] =
{
	{ 0.0f, 0.0f}, { 0.25f, 0.0f}, { 0.5f, 0.0f}, { 0.75f, 0.0f}, { 1.0f, 0.0f},
	{ 0.0f, 0.5f}, { 0.25f, 0.5f}, { 0.5f, 0.5f}, { 0.75f, 0.5f}, { 1.0f, 0.5f},
	{ 0.25f, 1.0f}, { 0.0f, 1.0f},
};

/// インデックスデータ:地面
const GLushort indexGround[] =
{
	0, 1, 2, 2, 3, 0,
};

/// インデックスデータ:四角
const GLushort indexRectangle[] =
{
	0, 1, 2, 3, 4, 5,
};

/// インデックスデータ:三角形
const GLushort indexTriangles[] =
{
	2, 1, 0, 5, 4, 3, 8, 7, 6,
};

/// インデックスデータ:立方体
const GLushort indexCube[] =
{
	0, 1, 2, 2, 3, 0, 
	4, 5, 1, 1, 0, 4, 
	5, 6, 2, 2, 1, 5,
	6, 7, 3, 3, 2, 6, 
	7, 4, 0, 0, 3, 7, 
	7, 6, 5, 5, 4, 7,  
};

/// インデックスデータ:木
const GLushort indexTree[] =
{
	0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 1, 4, 3, 3, 2, 1,
	6, 7, 8, 6, 8, 9, 6, 9, 10, 6, 10, 11, 7, 10, 9, 9, 8, 7,
};

/// インデックスデータ:建物
const GLushort indexWarehouse[] =
{
	0, 1, 6, 6, 5, 0,
	1, 2, 7, 7, 6, 1,
	2, 3, 8, 8, 7, 2,
	3, 4, 9, 9, 8, 3,
	5, 6,10,10,11, 5,
};

//画像データ
const GLuint X = 0xff'18'18'18; // 黒
const GLuint W = 0xff'ff'ff'ff; // 白
const GLuint R = 0xff'10'10'e0; // 赤
const GLuint B = 0xff'e0'10'10; // 青
const GLuint G = 0xff'22'B1'4C; // 緑

/// 頂点シェーダー.
static const GLchar* vsCode =
	"#version 450 \n"
	"layout(location=0) in vec3 vPosition; \n"
	"layout(location=1) in vec4 vColor; \n"
	"layout(location=2) in vec2 vTexcoord; \n"
	"layout(location=0) out vec4 outColor; \n"
	"layout(location=1) out vec2 outTexcoord; \n"
	"out gl_PerVertex { \n"
	"  vec4 gl_Position; \n"
	"}; \n"
	"layout(location=0) uniform mat4 matTRS; \n"
	"void main() { \n"
	"  outColor = vColor; \n"
	"  outTexcoord = vTexcoord; \n"
	"  gl_Position = matTRS * vec4(vPosition, 1.0); \n"
	"} \n";

/// フラグメントシェーダー.
static const GLchar * fsCode =
	"#version 450 \n"
	"layout(location=0) in vec4 inColor; \n"
	"layout(location=1) in vec2 inTexcoord; \n"
	"out vec4 fragColor; \n"
	"layout(binding=0) uniform sampler2D texColor; \n"
	"void main() { \n"
	"  vec4 tc = texture(texColor, inTexcoord); \n"
	"  fragColor = inColor * tc; \n"
	"} \n";

/// マップデータ
int mapData[10][10] =
{
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0},
	{ 2, 2, 2, 2, 2, 2, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 2, 2, 2, 2, 2, 2},
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 2, 2, 1, 0, 0, 0}, 
};

int objectMapData[10][10] =
{
	{ 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
	{ 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
	{ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
};

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
	PrimitiveBuffer primitiveBuffer(100'000, 300'000);

	//描画データの追加
	primitiveBuffer.Add(std::size(posGround), posGround, colGround, tcGround, std::size(indexGround), indexGround);
	primitiveBuffer.Add(std::size(posRectAngle), posRectAngle, colRectangle, tcRectangle, std::size(indexRectangle), indexRectangle);
	primitiveBuffer.Add(std::size(posTriangles), posTriangles, colTriangles, tcTriangles, std::size(indexTriangles), indexTriangles);
	primitiveBuffer.Add(std::size(posCube), posCube, colCube, tcCube, std::size(indexCube), indexCube);
	primitiveBuffer.Add(std::size(posTree), posTree, colTree, tcTree, std::size(indexTree), indexTree);
	primitiveBuffer.Add(std::size(posWarehouse), posWarehouse, colWarehouse, tcWarehouse, std::size(indexWarehouse), indexWarehouse);

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
	const GLuint texGround = GLContext::CreateImage2D("Res/Tile.tga");
	const GLuint texTriangle = GLContext::CreateImage2D("Res/Triangle.tga");
	const GLuint texGreen = GLContext::CreateImage2D("Res/Green.tga");
	const GLuint texRoad = GLContext::CreateImage2D("Res/Road.tga");
	const GLuint texTree = GLContext::CreateImage2D("Res/Tree.tga");
	const GLuint texWareHouse = GLContext::CreateImage2D("Res/WareHouse.tga");
	if (!texGround || !texTriangle || !texGreen || !texRoad || !texTree || !texWareHouse)
	{
		return 1;
	}

	//サンプラを作成
	const GLuint sampler = GLContext::CreateSampler(GL_REPEAT);
	if (!sampler)
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
		glBindSampler(0, sampler);
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

		// プロジェクション行列を作成
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		const float aspectRatio =static_cast<float>(w) / static_cast<float>(h);
		const glm::mat4 matProj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

		// ビュー行列を作成
		const glm::mat4 matView = glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		// 行列をシェーダに転送する
		const glm::mat4 matModel = glm::mat4(1);
		const glm::mat4 matMVP = matProj * matView * matModel;
		glProgramUniformMatrix4fv(vp, locMatTRS, 1, GL_FALSE, &matMVP[0][0]);

		// 立方体の描画
		glBindTextureUnit(0, texTriangle);
		primTriangles.Draw();
		primCube.Draw();

		// マップに配置する物体の表示データ
		struct ObjectData
		{
			Primitive prim; //表示するプリミティブ
			GLuint tex; //プリミティブに貼るテクスチャ
		};

		//描画する物体のリスト
		const ObjectData objectList[] =
		{
			{ Primitive(), 0}, // なし
			{ primTree, texTree}, // 木
			{ primWareHouse, texWareHouse}, //家
		};

		//木を植える
		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				// 描画する物体の番号を取得.
				const int objectNo = objectMapData[y][x];
				if (objectNo <= 0 || objectNo >= std::size(objectList))
				{
					continue;
				}
				const ObjectData p = objectList[objectNo];
				
				// 四角形が4x4mなので、xとyを4倍した位置に表示する.
				const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);
				
				// 行列をシェーダに転送する 
				const glm::mat4 matModel = glm::translate(glm::mat4(1), position);
				const glm::mat4 matMVP = matProj * matView * matModel;
				glProgramUniformMatrix4fv(vp, locMatTRS, 1, GL_FALSE, &matMVP[0][0]);
				
				glBindTextureUnit(0, p.tex); // テクスチャを割り当てる.
				p.prim.Draw();
			}
		}

		//マップを(-20-20)-(20,20)の範囲に描画
		const GLuint mapTexList[] = {texGreen, texGround, texRoad };
		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				//四角形が4x4mなので、xとyを4倍した位置に表示する
				const glm::vec3 position(x *4 - 20, 0, y * 4 - 20);

				//行列をシェーダに転送する
				const glm::mat4 matModel = glm::translate(glm::mat4(1), position);
				const glm::mat4 matMVP = matProj * matView * matModel;
				glProgramUniformMatrix4fv(vp, locMatTRS, 1, GL_FALSE, &matMVP[0][0]);

				const int textureNo = mapData[y][x];
				glBindTextureUnit(0, mapTexList[textureNo]);
				primGround.Draw();
			}
		}

		// テクスチャの割り当てを解除
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindSampler(0, 0);
		glBindProgramPipeline(0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//後始末
	glDeleteSamplers(1, &sampler);
	glDeleteTextures(1, &texGround);
	glDeleteTextures(1, &texTriangle);
	glDeleteProgramPipelines(1, &pipeline);
	glDeleteProgram(fp);
	glDeleteProgram(vp);

	//GLFWの終了
	glfwTerminate();
}