/**
*	@file GLContext.cpp
*/
#include "GLContext.h"
#include <iostream>
#include <vector>

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

	/**
	*	シェーダプログラムをビルドする
	*
	*	@param type	シェーダの種類
	*	@param code	シェーダプログラムへのポインタ
	*
	*	@retval 0 >=	作成したプログラムオブジェクト
	*	@retval 0		プログラムオブジェクトの作成失敗
	*/
	GLuint CreateProgram(GLenum type, const GLchar* code)
	{
		GLuint program = glCreateShaderProgramv(type, 1, &code);

		GLuint status = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				std::vector<char> buf;
				buf.resize(infoLen);
				if ((int)buf.size() >= infoLen)
				{
					glGetProgramInfoLog(program, infoLen, nullptr, buf.data());
					std::cerr << "[エラー]" << __func__ << ":シェーダーのビルドに失敗.\n" << buf.data() << "\n";
				}
			}
			glDeleteProgram(program);
			return 0;
		}
		return program;
	}

	/**
	*	パイプラインオブジェクトを作成する
	*
	*	@param vp	頂点シェーダプログラム
	*	@param fp	フラグメントシェーダプログラム
	*
	*	@retval 0より大きい	作成したパイプラインオブジェクト
	*	@retval 0			パイプラインオブジェクトの作成に失敗
	*/
	GLuint Createpipeline(GLuint vp, GLuint fp)
	{
		GLuint id;
		glCreateProgramPipelines(1, &id);
		glUseProgramStages(id, GL_VERTEX_SHADER_BIT, vp);
		glUseProgramStages(id, GL_FRAGMENT_SHADER_BIT, fp);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ <<":プログラムパイプラインの作成に失敗.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}

		GLint testVp = 0;
		glGetProgramPipelineiv(id, GL_VERTEX_SHADER, &testVp);
		if (testVp != vp)
		{
			std::cerr << "[エラー]" << __func__ << ":フラグメントシェーダの設定に失敗.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}
		return id;
	}

} // namespace GLContext