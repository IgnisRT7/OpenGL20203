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

/// ���W�f�[�^
const Position positions[] = 
{
	// �n��
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
	
	//�O�p�`x3
	{-0.33f * 5, 2.0f * 5, 0.5f },
	{ 0.33f * 5, 2.0f * 5, 0.5f },
	{ 0.0f * 5,  1.0f * 5, 0.5f },
	{ 0.33f * 5, 2.0f * 5, 0.5f },
	{ 0.99f * 5, 2.0f * 5, 0.5f },
	{ 0.66f * 5, 1.0f * 5, 0.5f },
	{ 0.0f * 5,  1.0f * 5, 0.5f },
	{ 0.66f * 5, 1.0f * 5, 0.5f },
	{ 0.33f * 5, 0.0f * 5, 0.5f },

	//������
	{ 0.0f, 0.0f, 2.0f },
	{ 2.0f, 0.0f, 2.0f },
	{ 2.0f, 2.0f, 2.0f },
	{ 0.0f, 2.0f, 2.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 2.0f, 0.0f, 0.0f },
	{ 2.0f, 2.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f },
};

/// �F�f�[�^
const Color colors[] =
{
	//�n�� 0 - 3
	{0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},

	//�񋤗L�� 4 - 9
	{1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},

	//�O�p�`x3 10 - 18
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // ���F
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // ���F
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // ���F
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // ���F
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // ���F
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // ���F
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // ���F
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // ���F
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // ���F

	//������ 19 - 26
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f}, 
	{1.0f, 1.0f, 1.0f, 1.0f}, 
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
};

// �C���f�b�N�X�f�[�^
const GLushort indices[] =
{
	//�n��
	0, 1, 2, 2, 3, 0,
	//�l�p�|���S��
	4, 5, 6, 7, 8, 9,
	//�O�p�`x3
	12, 11, 10, 15, 14, 13, 18, 17, 16,
	//������
	19, 20, 21, 19, 21, 22, // ��O
	20, 24, 25, 20, 25, 21, // �E
	22, 21, 25, 22, 25, 26, // ��
	19, 23, 26, 19, 26, 22, // ��
	19, 20, 24, 19, 24, 23, // ��
	23, 24, 25, 23, 25, 26, // ��
};

//�`��f�[�^
const Primitive primGround(GL_TRIANGLES, 6, 0 * sizeof(GLushort), 0); //�l�p�`
const Primitive primTriangles(GL_TRIANGLES, 9, 12 * sizeof(GLushort), 0); //�O�p�`
const Primitive primCube(GL_TRIANGLES, 36, 21 * sizeof(GLushort), 0); //������

//�摜�f�[�^
const int imageGroundWidth = 8; // ��ʂ̕�
const int imageGroundHeight = 8; // ��ʂ̍���
const int imageTriangleWidth = 6;
const int imageTriangleHeight = 6;
const GLuint X = 0xff'18'18'18; // ��
const GLuint W = 0xff'ff'ff'ff; // ��
const GLuint R = 0xff'10'10'e0; // ��
const GLuint B = 0xff'e0'10'10; // ��
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

/// ���_�V�F�[�_�[.
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

/// �t���O�����g�V�F�[�_�[.
static const GLchar * fsCode =
	"#version 450 \n"
	"layout(location=0) in vec4 inColor; \n"
	"out vec4 fragColor; \n"
	"layout(binding=0) uniform sampler2D texColor; \n"
	"void main() { \n"
	"  fragColor = texture(texColor, gl_FragCoord.xy * 0.01); \n"
	"} \n";

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
	const GLuint vboPosition = GLContext::CreateBuffer(sizeof(positions), positions);
	const GLuint vboColor = GLContext::CreateBuffer(sizeof(colors), colors);
	const GLuint ibo = GLContext::CreateBuffer(sizeof(indices), indices);
	const GLuint vao = GLContext::CreateVertexArray(vboPosition, vboColor, ibo);
	if (!vao)
	{
		return 1;
	}

	//�p�C�v���C���I�u�W�F�N�g���쐬����
	const GLuint vp = GLContext::CreateProgram(GL_VERTEX_SHADER, vsCode);
	const GLuint fp = GLContext::CreateProgram(GL_FRAGMENT_SHADER, fsCode);
	const GLuint pipeline = GLContext::Createpipeline(vp, fp);
	if (!pipeline)
	{
		return 1;
	}

	// uniform�ϐ��̈ʒu
	const GLint locMatTRS = 0;

	//���W�Ԋҍs��̉�]�p�x
	float degree = 0;

	//�e�N�X�`���쐬
	const GLuint texGround = GLContext::CreateImage2D(imageGroundWidth, imageGroundHeight, imageGround);
	const GLuint texTriangle = GLContext::CreateImage2D(imageTriangleWidth, imageTriangleHeight, imageTriangle);
	if (!texGround || !texTriangle)
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

		//�v���W�F�N�V�����s����쐬
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		const float aspectRatio =static_cast<float>(w) / static_cast<float>(h);
		const glm::mat4 matProj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

		//�r���[�s����쐬
		const glm::mat4 matView = glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		//�s����V�F�[�_�ɓ]������
		const glm::mat4 matModel = glm::mat4(1);
		const glm::mat4 matMVP = matProj * matView * matModel;
		glProgramUniformMatrix4fv(vp, locMatTRS, 1, GL_FALSE, &matMVP[0][0]);

		glBindTextureUnit(0, texGround); // �e�N�X�`�������蓖�Ă�

		primGround.Draw();
		primTriangles.Draw();
		//primCube.Draw();

		// �e�N�X�`���̊��蓖�Ă�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindProgramPipeline(0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//��n��
	glDeleteTextures(1, &texGround);
	glDeleteTextures(1, &texTriangle);
	glDeleteProgramPipelines(1, &pipeline);
	glDeleteProgram(fp);
	glDeleteProgram(vp);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vboColor);
	glDeleteBuffers(1, &vboPosition);

	//GLFW�̏I��
	glfwTerminate();
}