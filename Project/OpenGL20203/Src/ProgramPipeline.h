/**
*	@file ProgramPipeline.h
*/
#ifndef PROGRAMPIPELINE_H_INCLUDED
#define PROGRAMPIPELINE_H_INCLUDED
#include <glad/glad.h>
#include <glm/glm.hpp>

/**
*	シェーダプログラムを管理するクラス
*/
class ProgramPipeline
{
public:
	ProgramPipeline(const char* vsCode, const char* fsCode);
	~ProgramPipeline();

	//オブジェクトの有効性を判定する
	bool IsValid() const;

	//ユニフォーム変数の設定
	bool SetUniform(GLint, const glm::mat4&) const;

	//バインド管理
	void Bind() const;
	void Unbind() const;

private:
	GLuint vp = 0; // 頂点シェーダオブジェクト
	GLuint fp = 0; // フラグメントシェーダオブジェクト
	GLuint pipeline = 0; // パイプラインオブジェクト
};


#endif // PROGRAMPIPELINE_H_INCLUDED