/**
*	@file GLContext.h
*/
#ifndef GLCONTEXT_H_INCLUDED
#define GLCONTEXT_H_INCLUDED
#include <glad/glad.h>

/// �O�������W�^
struct Position
{
	float x, y, z;
};

/// RGBA�J���[�^
struct Color
{
	float r, g, b, a;
};

/**
*	OpenGL�R���e�L�X�g�Ɋւ���@�\���i�[���閼�O���
*/
namespace GLContext
{

	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data);
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor);

} // GLContext

#endif// GLCONTEXT_H_INCLUDED