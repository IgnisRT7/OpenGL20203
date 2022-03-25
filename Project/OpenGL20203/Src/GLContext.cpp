/**
*	@file GLContext.cpp
*/
#include "GLContext.h"
#include <glm/glm.hpp>
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
	*	@param vboTexcoord	VAOに関連付けられるテクスチャ座標データ
	*	@param ibo			VAOに関連付けられるインデックスデータ
	*
	*	@return 作成したVAO
	*/
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor, GLuint vboTexcoord, GLuint ibo)
	{
		if (!vboPosition || !vboColor || !vboTexcoord || !ibo)
		{
			std::cerr << "[エラー]" << __func__ << ":バッファオブジェクトが0です。\n";
			return 0;
		}

		GLuint id = 0;
		glCreateVertexArrays(1, &id);
		
		// 座標データをバインディングポイントに割り当てる
		const GLuint positionIndex = 0;
		const GLuint positionBindingIndex = 0;
		glEnableVertexArrayAttrib(id, positionIndex);
		glVertexArrayAttribFormat(id, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, positionIndex, positionBindingIndex);
		glVertexArrayVertexBuffer(id, positionBindingIndex, vboPosition, 0, sizeof(Position));

		// 色データをバインディングポイントに割り当てる
		const GLuint colorIndex = 1;
		const GLuint colorBindingIndex = 1;
		glEnableVertexArrayAttrib(id, colorIndex);
		glVertexArrayAttribFormat(id, colorIndex, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, colorIndex, colorBindingIndex);
		glVertexArrayVertexBuffer(id, colorBindingIndex, vboColor, 0, sizeof(Color));

		// テクスチャ座標データをバインディングポイントに割り当てる
		const GLuint texcoordIndex = 2;
		const GLuint texcoordBindingIndex = 2;
		glEnableVertexArrayAttrib(id, texcoordIndex);
		glVertexArrayAttribFormat(id, texcoordIndex, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, texcoordIndex, texcoordBindingIndex);
		glVertexArrayVertexBuffer(id, texcoordBindingIndex, vboTexcoord, 0, sizeof(glm::vec2));

		glVertexArrayElementBuffer(id, ibo);
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

		GLint status = 0;
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
					std::cerr << "[エラー]" << __func__ <<
						":シェーダーのビルドに失敗.\n" << buf.data() << "\n";
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
			std::cerr << "[エラー]" << __func__ << ":頂点シェーダの設定に失敗.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}
		 GLint testFp = 0;
		glGetProgramPipelineiv(id, GL_FRAGMENT_SHADER, &testFp);
		if (testFp != fp)
		{
			std::cerr << "[エラー]" << __func__ << ":フラグメントシェーダの設定に失敗.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}

		return id;
	}

	/**
	*	2Dテクスチャを作成する
	*
	*	@param width	画像の幅(ピクセル数)
	*	@param height	画像の高さ(ピクセル数)
	*	@param data		画像データのアドレス
	*
	*	@retval 0以外	作成したテクスチャオブジェクトのID
	*	@retval 0		テクスチャの作成失敗
	*/
	GLuint CreateImage2D(GLsizei width, GLsizei height, const void* data)
	{
		glGetError(); // エラー状態をリセット

		// テクスチャオブジェクトを作成し、GPUメモリを確保する
		GLuint id;
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, GL_RGBA8, width, height);

		// GPUメモリにデータを転送する
		glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		const GLenum result = glGetError();
		if (result != GL_NO_ERROR)
		{
			std::cerr<< "[エラー]" << __func__ << "テクスチャの作成に失敗\n";
			glDeleteTextures(1, &id);
			return 0;
		}

		return id;
	}

	/**
	*	サンプラオブジェクトを作成する
	*
	*	@param wrapMode	ラップモード
	*
	*	@retval 0以外	作成したサンプラオブジェクト
	*	@retval 0		サンプラオブジェクトの作成失敗
	*/
	GLuint CreateSampler(GLenum wrapMode)
	{
		glGetError(); // エラー状態をリセット

		//サンプラオブジェクトを作成する
		GLuint id;
		glCreateSamplers(1, &id);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ << "サンプラの作成に失敗\n";
			glDeleteSamplers(1, &id);
			return 0;
		}

		//ラップモードを設定する
		glSamplerParameteri(id, GL_TEXTURE_WRAP_S, wrapMode);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_T, wrapMode);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ << ":ラップモードではない値が指定された.\n";
			glDeleteSamplers(1, &id);
			return 0;
		}

		//フィルタを設定する
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[エラー]" << __func__ << ":フィルタではない値が指定された.\n";
			glDeleteSamplers(1, &id);
			return 0;
		}

		return id;
	}

} // namespace GLContext