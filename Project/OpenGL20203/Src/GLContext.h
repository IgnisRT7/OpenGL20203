/**
*	@file GLContext.h
*/
#ifndef GLCONTEXT_H_INCLUDED
#define GLCONTEXT_H_INCLUDED
#include <glad/glad.h>

/// 三次元座標型
struct Position
{
	float x, y, z;
};

/// RGBAカラー型
struct Color
{
	float r, g, b, a;
};

/**
*	OpenGLコンテキストに関する機能を格納する名前空間
*/
namespace GLContext
{

	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data);
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor);

} // GLContext

#endif// GLCONTEXT_H_INCLUDED