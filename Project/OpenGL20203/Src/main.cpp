/**
*	@file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include "Primitive.h"
#include "ProgramPipeline.h"
#include "Texture.h"
#include "Sampler.h"
#include "Actor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <memory>
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
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
	{ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
};

// アクターの配列
std::vector<Actor> actors;

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
	primitiveBuffer.AddFromObjeFile("Res/Tiger_I.obj");
	primitiveBuffer.AddFromObjeFile("Res/T34.obj");
	primitiveBuffer.AddFromObjeFile("Res/HouseRender.obj");
	primitiveBuffer.AddFromObjeFile("Res/deneme.obj");
	primitiveBuffer.AddFromObjeFile("Res/Bullet.obj");

	//パイプラインオブジェクトを作成する
	ProgramPipeline pipeline("Res/FragmentLighting.vert", "Res/FragmentLighting.frag");
	if(!pipeline.IsValid())
	{
		return 1;
	}

	// uniform変数の位置
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1; // モデル行列用ユニフォーム変数の位置

	//座標返還行列の回転角度
	float degree = 0;

	//テクスチャ作成
	std::shared_ptr<Texture> texGround(new Texture("Res/Tile.tga"));
	std::shared_ptr<Texture> texTriangle(new Texture("Res/Triangle.tga"));
	std::shared_ptr<Texture> texGreen(new Texture("Res/Green.tga"));
	std::shared_ptr<Texture> texRoad(new Texture("Res/Road.tga"));
	std::shared_ptr<Texture> texTree(new Texture("Res/Tree.tga"));
	std::shared_ptr<Texture> texWarehouse(new Texture("Res/WareHouse.tga"));
	std::shared_ptr<Texture> texTank(new Texture("Res/PzVl_Tiger_I.tga"));
	std::shared_ptr<Texture> texTankT34(new Texture("Res/T-34.tga"));
	std::shared_ptr<Texture> texBrickHouse(new Texture("Res/House38UVTexture.tga"));
	std::shared_ptr<Texture> texTeraHouse(new Texture("Res/deneme_wire_143224087_BaseColor.tga"));
	std::shared_ptr<Texture> texBullet(new Texture("Res/Bullet.tga"));

	std::shared_ptr<Sampler> sampler(new Sampler(GL_REPEAT));

	// マップに配置する物体の表示データ
	struct ObjectData
	{
		const char* name;
		Primitive prim; //表示するプリミティブ
		const std::shared_ptr<Texture> tex; //プリミティブに貼るテクスチャ
	};

	//描画する物体のリスト
	const ObjectData objectList[] =
	{
		{ "", Primitive(), 0}, // なし
		{ "Tree", primitiveBuffer.Get(4), texTree}, // 木
		{ "Warehouse", primitiveBuffer.Get(5), texWarehouse}, //家
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
				
			actors.push_back(Actor{ p.name, p.prim, p.tex, position, glm::vec3(1), 0.0f, glm::vec3(0)});
		}
	}

	//マップを(-20-20)-(20,20)の範囲に描画
	struct MapData
	{
		const char* name;
		Primitive prim;
		const std::shared_ptr<Texture> tex;
	};
	MapData mapDataList[] =
	{
		{ "Green", primitiveBuffer.Get(0), texGreen },
		{ "Ground", primitiveBuffer.Get(0), texGround },
		{ "Road", primitiveBuffer.Get(0), texRoad },
	};

	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			//四角形が4x4mなので、xとyを4倍した位置に表示する
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);
			const int textureNo = mapData[y][x];
			const MapData p = mapDataList[textureNo];

			actors.push_back(Actor{ p.name, p.prim, p.tex, position, glm::vec3(1), 0.0f, glm::vec3(0)});
		}
	}

	// 三角形のパラメータ
	actors.push_back(Actor{ "Triangle", primitiveBuffer.Get(2), texTriangle, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0), });
	// 立方体のパラメータ
	actors.push_back(Actor{ "Cube", primitiveBuffer.Get(3), texTriangle, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0), });
	// 戦車のパラメータ
	actors.push_back(Actor{ "Tiger-I", primitiveBuffer.Get(6), texTank, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0) });
	// T-34戦車のパラメータ
	actors.push_back(Actor{ "T-34", primitiveBuffer.Get(7), texTankT34, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0) });
	// 建物のパラメータ
	actors.push_back(Actor{ "BrickHouse", primitiveBuffer.Get(8), texBrickHouse, glm::vec3(10.5f, 0, 0), glm::vec4(4), 0.0f, glm::vec3(-2.6f, 2.0f, 1.5f) });
	// 課題用建物のパラメータ
	actors.push_back(Actor{ "TeraHouse", primitiveBuffer.Get(9), texTeraHouse, glm::vec3(-13.0f, 0, -5.0f) , glm::vec4(0.03f), 0.0f, glm::vec3(0) });

	//メインループ
	double loopTime = glfwGetTime(); // 1/60秒間隔でループ処理するための時刻変数
	double diffLoopTime = 0; // 自国の差分
	const float deltaTime = 1.0f / 60.0f; // 更新間隔
	int oldShotButton = 0; // 前回のショットボタンの状態

	while (!glfwWindowShouldClose(window))
	{
		// 現在時刻を取得
		const double curLoopTime = glfwGetTime();
		// 現在時刻と前回自国の差を、自国の差分に加算
		diffLoopTime += curLoopTime - loopTime;
		// 前回自国を現在時刻に更新
		loopTime = curLoopTime;
		// 時刻の差分が1/60秒未満なら、ループの先頭に戻る
		if (diffLoopTime < deltaTime)
		{
			continue;
		}

		//
		// ゲーム状態を更新する
		//
		for (; diffLoopTime >= deltaTime; diffLoopTime -= deltaTime)
		{
			// 戦車を移動させる
			Actor* tank = Find(actors, "Tiger-I");
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				tank->rotation += glm::radians(90.0f) * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				tank->rotation -= glm::radians(90.0f) * deltaTime;
			}

			// tank.rotationが0のときの戦車の向きベクトル
			glm::vec3 tankFront(0, 0, 1);
			// rotTankラジアンだけ回転させる回転行列を作る
			const glm::mat4 matRot = glm::rotate(glm::mat4(1), tank->rotation, glm::vec3(0, 1, 0));
			// 向きベクトルをtank.rotationだけ回転させる
			tankFront = matRot * glm::vec4(tankFront, 1);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				tank->position += tankFront * 4.0f * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				tank->position -= tankFront * 4.0f * deltaTime;
			}

			// マウス左ボタンの状態を取得する
			int shotButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

			// マウス左ボタンが押された瞬間に弾アクターを追加する
			if (shotButton == GLFW_PRESS && oldShotButton != GLFW_PRESS)
			{
				//発射位置を方の先端に設定
				glm::vec3 position = tank->position + tankFront * 6.0f;
				position.y += 2.0f;

				Actor bullet = { "Bullet", primitiveBuffer.Get(10), texBullet, position, glm::vec3(1), tank->rotation, glm::vec3(0)};

				// 1.5秒後に弾を消す
				bullet.lifespan = 1.5f;

				// 戦車の向いている方向に、30m/sの速度で移動させる
				bullet.velocity = tankFront * 30.0f;

				actors.push_back(bullet);
			}

			// 『前回のショットボタンの状態』を更新する
			oldShotButton = shotButton;
		}

		// アクターの状態を更新する
		for (int i = 0; i < actors.size(); ++i)
		{
			// アクターの寿命を減らす
			if (actors[i].lifespan > 0)
			{
				actors[i].lifespan -= deltaTime;

				// 寿命の尽きたアクターを『削除待ち』状態にする
				if (actors[i].lifespan <= 0)
				{
					actors[i].isDead = true;
					continue; // 削除待ちアクターは更新をスキップ
				}
			}

			// アクターの位置を更新する
			actors[i].position += actors[i].velocity * deltaTime;
		}

		// 削除待ちのアクターを削除する
		actors.erase(std::remove_if(actors.begin(), actors.end(), [](Actor& a){ return a.isDead; }), actors.end());

		//
		// ゲーム状態を描画する
		//

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		primitiveBuffer.BindVertexArray();
		pipeline.Bind();
		sampler->Bind(0);
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
		const glm::mat4 matView = glm::lookAt(glm::vec3(0, 30, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		// アクターを描画する
		for (int i = 0; i < actors.size(); ++i)
		{
			Draw(actors[i], pipeline, matProj, matView);
		}

		// テクスチャの割り当てを解除
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		sampler->Unbind(0);
		pipeline.Unbind();
		primitiveBuffer.UnbindVertexArray();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//GLFWの終了
	glfwTerminate();
}