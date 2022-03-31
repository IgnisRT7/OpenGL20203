/**
*	@file Sampler.cpp
*/
#include "Sampler.h"
#include "GLContext.h"
#include <glad/glad.h>
#include <iostream>

/**
*	コンストラクタ
*
*	@param wrapMode ラップモード
*/
Sampler::Sampler(GLenum wrapMode)
{
	id = GLContext::CreateSampler(wrapMode);
	if (id)
	{
		std::cout << "[情報]" << __func__ << "サンプラを作成.\n";
	}
	else
	{
		std::cout << "[エラー]" << __func__ << "サンプラの作成に失敗.\n";
	}
}

/**
*	デストラクタ
*/
Sampler::~Sampler()
{
	if (id)
	{
		std::cout << "[情報]" << __func__ << "サンプラ" << "を削除\n";
	}
	glDeleteSamplers(1, &id);
}

/**
*	オブジェクトが使える状態か調べる
*
*	@retval true	使える
*	@retval false	使えない
*/
bool Sampler::IsValid() const
{
	return id;
}

/**
*	サンプラを割り当てる
* 
*	@param unit 割り当てるサンプラのユニット番号
*/
void Sampler::Bind(GLuint unit) const
{
	glBindSampler(unit, id);
}

/**
*	サンプラの割り当てを解除する
* 
*	@param unit 割り当て解除するテクスチャイメージユニットの番号
*/
void Sampler::Unbind(GLuint unit) const
{
	glBindSampler(unit, 0);
}