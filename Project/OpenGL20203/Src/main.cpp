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

	//�p�C�v���C���I�u�W�F�N�g���쐬����
	ProgramPipeline pipeline("Res/Simple.vert", "Res/Simple.frag");
	if(!pipeline.IsValid())
	{
		return 1;
	}

	// uniform�ϐ��̈ʒu
	const GLint locMatTRS = 0;

	//���W�Ԋҍs��̉�]�p�x
	float degree = 0;

	//�e�N�X�`���쐬
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

	//�T���v�����쐬
	const GLuint sampler = GLContext::CreateSampler(GL_REPEAT);
	if (!sampler)
	{
		return 1;
	}

	//���C�����[�v
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

		// �����̂̕`��
		glBindTextureUnit(0, texTriangle);
		primitiveBuffer.Get(2).Draw();
		primitiveBuffer.Get(3).Draw();

		// �}�b�v�ɔz�u���镨�̂̕\���f�[�^
		struct ObjectData
		{
			Primitive prim; //�\������v���~�e�B�u
			GLuint tex; //�v���~�e�B�u�ɓ\��e�N�X�`��
		};

		//�`�悷�镨�̂̃��X�g
		const ObjectData objectList[] =
		{
			{ Primitive(), 0}, // �Ȃ�
			{ primitiveBuffer.Get(4), texTree}, // ��
			{ primitiveBuffer.Get(5), texWareHouse}, //��
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
				
				glBindTextureUnit(0, p.tex); // �e�N�X�`�������蓖�Ă�.
				p.prim.Draw();
			}
		}

		//�}�b�v��(-20-20)-(20,20)�͈̔͂ɕ`��
		const GLuint mapTexList[] = {texGreen, texGround, texRoad };
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

				const int textureNo = mapData[y][x];
				glBindTextureUnit(0, mapTexList[textureNo]);
				primitiveBuffer.Get(0).Draw();
			}
		}

		// �e�N�X�`���̊��蓖�Ă�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindSampler(0, 0);
		pipeline.Unbind();
		primitiveBuffer.UnbindVertexArray();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//��n��
	glDeleteSamplers(1, &sampler);
	glDeleteTextures(1, &texGround);
	glDeleteTextures(1, &texTriangle);

	//GLFW�̏I��
	glfwTerminate();
}