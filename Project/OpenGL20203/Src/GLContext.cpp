/**
*	@file GLContext.cpp
*/
#include "GLContext.h"
#include <iostream>

/**
*	OpenGLコンテキストに関する機能を格納する名前空間
*/
namespace GLContext
{
	/**
	*	バッファオブジェクトを作成する
	*
	*	@param size データへのサイズ
	*	@param data データへのポインタ
	*	
	*	@retrun 作成したバッファオブジェクト
	*/
	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data)
	{
		GLuint id = 0;
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, size, data, 0);
		return id;
	}

	/**
	*	VertexArrayobjectを作成する
	*
	*	@param vboPosition	VAOに関連付けられる座標データ
	*	@param vboColor		VAOに関連付けられるカラーデータ
	*
	*	@return 作成したVAO
	*/
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor)
	{
		if (!vboPosition || vboColor)
		{
			std::cerr << "[エラー]" << __func__ << ":バッファオブジェクトが0です。\n";
			return 0;
		}

		GLuint id = 0;
		glCreateVertexArrays(1, &id);

		// 座標情報のバインド
		const GLuint positionIndex = 0;
		const GLuint positionBindingIndex = 0;
		glEnableVertexArrayAttrib(id, positionIndex);
		glVertexArrayAttribFormat(id, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, positionIndex, positionBindingIndex);
		glVertexArrayVertexBuffer(id, positionBindingIndex, vboPosition, 0, sizeof(Position));

		// 色情報のバインド
		const GLuint colorIndex = 1;
		const GLuint colorBindingIndex = 1;
		glEnableVertexArrayAttrib(id, colorIndex);
		glVertexArrayAttribFormat(id, colorIndex, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, colorIndex, colorBindingIndex);
		glVertexArrayVertexBuffer(id, colorBindingIndex, vboColor, 0, sizeof(Color));

		return id;
	}

} // namespace GLContext