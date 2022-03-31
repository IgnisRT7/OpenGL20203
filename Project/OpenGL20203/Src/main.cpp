/**
*	@file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include "Primitive.h"
#include "ProgramPipeline.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#pragma comment(lib,"opengl32.lib")

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
	primitiveBuffer.AddFromObjeFile("Res/Ground.obj");
	primitiveBuffer.AddFromObjeFile("Res/Rectangle.obj");
	primitiveBuffer.AddFromObjeFile("Res/Triangle.obj");
	primitiveBuffer.AddFromObjeFile("Res/Cube.obj");
	primitiveBuffer.AddFromObjeFile("Res/Tree.obj");
	primitiveBuffer.AddFromObjeFile("Res/Warehouse.obj");

	//パイプラインオブジェクトを作成する
	ProgramPipeline pipeline("Res/Simple.vert", "Res/Simple.frag");
	if(!pipeline.IsValid())
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

		primitiveBuffer.BindVertexArray();
		pipeline.Bind();
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
		pipeline.SetUniform(locMatTRS, matMVP);

		// 立方体の描画
		glBindTextureUnit(0, texTriangle);
		primitiveBuffer.Get(2).Draw();
		primitiveBuffer.Get(3).Draw();

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
			{ primitiveBuffer.Get(4), texTree}, // 木
			{ primitiveBuffer.Get(5), texWareHouse}, //家
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
				pipeline.SetUniform(locMatTRS, matMVP);
				
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
				pipeline.SetUniform(locMatTRS, matMVP);

				const int textureNo = mapData[y][x];
				glBindTextureUnit(0, mapTexList[textureNo]);
				primitiveBuffer.Get(0).Draw();
			}
		}

		// テクスチャの割り当てを解除
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindSampler(0, 0);
		pipeline.Unbind();
		primitiveBuffer.UnbindVertexArray();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//後始末
	glDeleteSamplers(1, &sampler);
	glDeleteTextures(1, &texGround);
	glDeleteTextures(1, &texTriangle);

	//GLFWの終了
	glfwTerminate();
}