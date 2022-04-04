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

/// �}�b�v�f�[�^
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

// �A�N�^�[�̔z��
std::vector<Actor> actors;

/**
*	OpenGL����̃��b�Z�[�W����������
*
*	@param source	���b�Z�[�W�̔��M��(OpenGL,Windows,�V�F�[�_�[�Ȃ�)
*	@param type		���b�Z�[�W�̎��(�G���[�A�x���Ȃ�)
*	@param id		���b�Z�[�W����ʂɎ��ʂ���l
*	@param severity	���b�Z�[�W�̏d�v�x(���A���A��A�Œ�)
*	@param length	���b�Z�[�W�̕������A�����Ȃ烁�b�Z�[�W��0�I�[
*	@param message	���b�Z�[�W�̓��e
*   @param userparam �R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^
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
*	�G���g���[�|�C���g
*/
int main()
{
	//GLFW�̏�����
	if (glfwInit() != GLFW_TRUE)
	{
		return 1;
	}

	//�`��E�B���h�E�̍쐬
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

	//OpenGL�֐��̃A�h���X���擾����
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;
	}

	glDebugMessageCallback(DebugCallback, nullptr);
	
	// VAO���쐬����.
	PrimitiveBuffer primitiveBuffer(100'000, 300'000);

	//�`��f�[�^�̒ǉ�
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

	//�p�C�v���C���I�u�W�F�N�g���쐬����
	ProgramPipeline pipeline("Res/FragmentLighting.vert", "Res/FragmentLighting.frag");
	if(!pipeline.IsValid())
	{
		return 1;
	}

	// uniform�ϐ��̈ʒu
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1; // ���f���s��p���j�t�H�[���ϐ��̈ʒu

	//���W�Ԋҍs��̉�]�p�x
	float degree = 0;

	//�e�N�X�`���쐬
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

	// �}�b�v�ɔz�u���镨�̂̕\���f�[�^
	struct ObjectData
	{
		const char* name;
		Primitive prim; //�\������v���~�e�B�u
		const std::shared_ptr<Texture> tex; //�v���~�e�B�u�ɓ\��e�N�X�`��
	};

	//�`�悷�镨�̂̃��X�g
	const ObjectData objectList[] =
	{
		{ "", Primitive(), 0}, // �Ȃ�
		{ "Tree", primitiveBuffer.Get(4), texTree}, // ��
		{ "Warehouse", primitiveBuffer.Get(5), texWarehouse}, //��
	};

	//�؂�A����
	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			// �`�悷�镨�̂̔ԍ����擾.
			const int objectNo = objectMapData[y][x];
			if (objectNo <= 0 || objectNo >= std::size(objectList))
			{
				continue;
			}
			const ObjectData p = objectList[objectNo];
				
			// �l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�����ʒu�ɕ\������.
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);
				
			actors.push_back(Actor{ p.name, p.prim, p.tex, position, glm::vec3(1), 0.0f, glm::vec3(0)});
		}
	}

	//�}�b�v��(-20-20)-(20,20)�͈̔͂ɕ`��
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
			//�l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�����ʒu�ɕ\������
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);
			const int textureNo = mapData[y][x];
			const MapData p = mapDataList[textureNo];

			actors.push_back(Actor{ p.name, p.prim, p.tex, position, glm::vec3(1), 0.0f, glm::vec3(0)});
		}
	}

	// �O�p�`�̃p�����[�^
	actors.push_back(Actor{ "Triangle", primitiveBuffer.Get(2), texTriangle, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0), });
	// �����̂̃p�����[�^
	actors.push_back(Actor{ "Cube", primitiveBuffer.Get(3), texTriangle, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0), });
	// ��Ԃ̃p�����[�^
	actors.push_back(Actor{ "Tiger-I", primitiveBuffer.Get(6), texTank, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0) });
	// T-34��Ԃ̃p�����[�^
	actors.push_back(Actor{ "T-34", primitiveBuffer.Get(7), texTankT34, glm::vec3(0), glm::vec3(1), 0.0f, glm::vec3(0) });
	// �����̃p�����[�^
	actors.push_back(Actor{ "BrickHouse", primitiveBuffer.Get(8), texBrickHouse, glm::vec3(10.5f, 0, 0), glm::vec4(4), 0.0f, glm::vec3(-2.6f, 2.0f, 1.5f) });
	// �ۑ�p�����̃p�����[�^
	actors.push_back(Actor{ "TeraHouse", primitiveBuffer.Get(9), texTeraHouse, glm::vec3(-13.0f, 0, -5.0f) , glm::vec4(0.03f), 0.0f, glm::vec3(0) });

	//���C�����[�v
	double loopTime = glfwGetTime(); // 1/60�b�Ԋu�Ń��[�v�������邽�߂̎����ϐ�
	double diffLoopTime = 0; // �����̍���
	const float deltaTime = 1.0f / 60.0f; // �X�V�Ԋu
	int oldShotButton = 0; // �O��̃V���b�g�{�^���̏��

	while (!glfwWindowShouldClose(window))
	{
		// ���ݎ������擾
		const double curLoopTime = glfwGetTime();
		// ���ݎ����ƑO�񎩍��̍����A�����̍����ɉ��Z
		diffLoopTime += curLoopTime - loopTime;
		// �O�񎩍������ݎ����ɍX�V
		loopTime = curLoopTime;
		// �����̍�����1/60�b�����Ȃ�A���[�v�̐擪�ɖ߂�
		if (diffLoopTime < deltaTime)
		{
			continue;
		}

		//
		// �Q�[����Ԃ��X�V����
		//
		for (; diffLoopTime >= deltaTime; diffLoopTime -= deltaTime)
		{
			// ��Ԃ��ړ�������
			Actor* tank = Find(actors, "Tiger-I");
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				tank->rotation += glm::radians(90.0f) * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				tank->rotation -= glm::radians(90.0f) * deltaTime;
			}

			// tank.rotation��0�̂Ƃ��̐�Ԃ̌����x�N�g��
			glm::vec3 tankFront(0, 0, 1);
			// rotTank���W�A��������]�������]�s������
			const glm::mat4 matRot = glm::rotate(glm::mat4(1), tank->rotation, glm::vec3(0, 1, 0));
			// �����x�N�g����tank.rotation������]������
			tankFront = matRot * glm::vec4(tankFront, 1);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				tank->position += tankFront * 4.0f * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				tank->position -= tankFront * 4.0f * deltaTime;
			}

			// �}�E�X���{�^���̏�Ԃ��擾����
			int shotButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

			// �}�E�X���{�^���������ꂽ�u�Ԃɒe�A�N�^�[��ǉ�����
			if (shotButton == GLFW_PRESS && oldShotButton != GLFW_PRESS)
			{
				//���ˈʒu����̐�[�ɐݒ�
				glm::vec3 position = tank->position + tankFront * 6.0f;
				position.y += 2.0f;

				Actor bullet = { "Bullet", primitiveBuffer.Get(10), texBullet, position, glm::vec3(1), tank->rotation, glm::vec3(0)};

				// 1.5�b��ɒe������
				bullet.lifespan = 1.5f;

				// ��Ԃ̌����Ă�������ɁA30m/s�̑��x�ňړ�������
				bullet.velocity = tankFront * 30.0f;

				actors.push_back(bullet);
			}

			// �w�O��̃V���b�g�{�^���̏�ԁx���X�V����
			oldShotButton = shotButton;
		}

		// �A�N�^�[�̏�Ԃ��X�V����
		for (int i = 0; i < actors.size(); ++i)
		{
			// �A�N�^�[�̎��������炷
			if (actors[i].lifespan > 0)
			{
				actors[i].lifespan -= deltaTime;

				// �����̐s�����A�N�^�[���w�폜�҂��x��Ԃɂ���
				if (actors[i].lifespan <= 0)
				{
					actors[i].isDead = true;
					continue; // �폜�҂��A�N�^�[�͍X�V���X�L�b�v
				}
			}

			// �A�N�^�[�̈ʒu���X�V����
			actors[i].position += actors[i].velocity * deltaTime;
		}

		// �폜�҂��̃A�N�^�[���폜����
		actors.erase(std::remove_if(actors.begin(), actors.end(), [](Actor& a){ return a.isDead; }), actors.end());

		//
		// �Q�[����Ԃ�`�悷��
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

		// �v���W�F�N�V�����s����쐬
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		const float aspectRatio =static_cast<float>(w) / static_cast<float>(h);
		const glm::mat4 matProj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

		// �r���[�s����쐬
		const glm::mat4 matView = glm::lookAt(glm::vec3(0, 30, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		// �A�N�^�[��`�悷��
		for (int i = 0; i < actors.size(); ++i)
		{
			Draw(actors[i], pipeline, matProj, matView);
		}

		// �e�N�X�`���̊��蓖�Ă�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		sampler->Unbind(0);
		pipeline.Unbind();
		primitiveBuffer.UnbindVertexArray();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//GLFW�̏I��
	glfwTerminate();
}