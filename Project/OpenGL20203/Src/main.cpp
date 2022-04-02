/**
*	@file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include "Primitive.h"
#include "ProgramPipeline.h"
#include "Texture.h"
#include "Sampler.h"
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
	{ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
};

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
	std::shared_ptr<Texture> texTank2(new Texture("Res/T-34.tga"));

	std::shared_ptr<Sampler> sampler(new Sampler(GL_REPEAT));

	glm::vec3 posTank(0, 0, 0); //��Ԃ̈ʒu
	float rotTank = 0; // ��Ԃ̌���

	//���C�����[�v
	double loopTime = glfwGetTime(); // 1/60�b�Ԋu�Ń��[�v�������邽�߂̎����ϐ�
	double diffLoopTime = 0; // �����̍���
	const float deltaTime = 1.0f / 60.0f; // �X�V�Ԋu

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
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				rotTank += glm::radians(90.0f) * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				rotTank -= glm::radians(90.0f) * deltaTime;
			}

			// rotTank��0�̂Ƃ��̐�Ԃ̌����x�N�g��
			glm::vec3 tankFront(0, 0, 1);
			// rotTank���W�A��������]�������]�s������
			const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotTank, glm::vec3(0, 1, 0));
			// �����x�N�g����rotTank������]������
			tankFront = matRot * glm::vec4(tankFront, 1);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				posTank += tankFront * 4.0f * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				posTank -= tankFront * 4.0f * deltaTime;
			}
		}

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
		const glm::mat4 matView = glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		// �s����V�F�[�_�ɓ]������
		const glm::mat4 matModel = glm::mat4(1);
		const glm::mat4 matMVP = matProj * matView * matModel;
		pipeline.SetUniform(locMatTRS, matMVP);
		pipeline.SetUniform(locMatModel, matModel);

		// �O�p�`�Ɨ����̂̕`��
		texTriangle->Bind(0);
		primitiveBuffer.Get(2).Draw();
		primitiveBuffer.Get(3).Draw();

		//��Ԃ�\��
		{
			const glm::mat4 matModel =
				glm::translate(glm::mat4(1), posTank) *
				glm::rotate(glm::mat4(1), rotTank, glm::vec3(0, 1, 0));
			const glm::mat4 matMVP = matProj * matView * matModel;
			pipeline.SetUniform(locMatTRS, matMVP);
			pipeline.SetUniform(locMatModel, matModel);

			texTank->Bind(0);
			primitiveBuffer.Get(6).Draw();
		}
		
		//���2��\��
		{
			const glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(-5, 0, 0));
			const glm::mat4 matMVP = matProj * matView * matModel;
			pipeline.SetUniform(locMatTRS, matMVP);
			pipeline.SetUniform(locMatModel, matModel);

			texTank2->Bind(0);
			primitiveBuffer.Get(7).Draw();
		}

		// �}�b�v�ɔz�u���镨�̂̕\���f�[�^
		struct ObjectData
		{
			Primitive prim; //�\������v���~�e�B�u
			const std::shared_ptr<Texture> tex; //�v���~�e�B�u�ɓ\��e�N�X�`��
		};

		//�`�悷�镨�̂̃��X�g
		const ObjectData objectList[] =
		{
			{ Primitive(), 0}, // �Ȃ�
			{ primitiveBuffer.Get(4), texTree}, // ��
			{ primitiveBuffer.Get(5), texWarehouse}, //��
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
				
				// �s����V�F�[�_�ɓ]������ 
				const glm::mat4 matModel = glm::translate(glm::mat4(1), position);
				const glm::mat4 matMVP = matProj * matView * matModel;
				pipeline.SetUniform(locMatTRS, matMVP);
				pipeline.SetUniform(locMatModel, matModel);
				
				p.tex->Bind(0);
				p.prim.Draw();
			}
		}

		//�}�b�v��(-20-20)-(20,20)�͈̔͂ɕ`��
		const std::shared_ptr<Texture> mapTexList[] = { texGreen, texGround, texRoad };
		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				//�l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�����ʒu�ɕ\������
				const glm::vec3 position(x *4 - 20, 0, y * 4 - 20);

				//�s����V�F�[�_�ɓ]������
				const glm::mat4 matModel = glm::translate(glm::mat4(1), position);
				const glm::mat4 matMVP = matProj * matView * matModel;
				pipeline.SetUniform(locMatTRS, matMVP);
				pipeline.SetUniform(locMatModel, matModel);

				const int textureNo = mapData[y][x];
				mapTexList[textureNo]->Bind(0);
				primitiveBuffer.Get(0).Draw();
			}
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