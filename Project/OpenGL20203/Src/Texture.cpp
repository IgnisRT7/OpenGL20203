/**
*	@file Texture.cpp
*/
#include "Texture.h"
#include "GLContext.h"
#include <iostream>

/**
*	コンストラクタ	
* 
*	@param filename 画像ファイル名
*/
Texture::Texture(const char* filename)
{
	id = GLContext::CreateImage2D(filename);
	if (id)
	{
		name = filename;
		std::cout << "[情報]" << __func__ << "テクスチャ" << filename << "を作成.\n";
	}
	else
	{
		std::cout << "[エラー]" << __func__ << "テクスチャ" << filename << "の作成に失敗.\n";
	}
}

/**
*	デストラクタ
*/
Texture::~Texture()
{
	if (id)
	{
		std::cout << "[情報]" << __func__ << "テクスチャ" << name << "を削除\n";
	}
	glDeleteTextures(1, &id);
}

/**
*	オブジェクトが使える状態か調べる
*
*	@retval true	使える
*	@retval false	使えない
*/
bool Texture::IsValid() const
{
	return id;
}

/**
*	テクスチャをグラフィックスパイプラインに割り当てる
*
*	@param unit	割り当てるテクスチャイメージユニットの番号
*/
void Texture::Bind(GLuint unit) const
{
	glBindTextureUnit(unit, id);
}

/**
*	テクスチャの割り当てを解除する
*
*	@param unit 割り当て解除するテクスチャイメージユニットの番号
*/
void Texture::Unbind(GLuint unit) const
{
	glBindTextureUnit(unit, 0);
}