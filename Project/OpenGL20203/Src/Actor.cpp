/**
*	@file Actor.cpp
*/
#include "Actor.h"
#include <glm/gtc/matrix_transform.hpp>

/**
*	物体を描画する
*/
void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView)
{
	// uniform変数の位置
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1; // モデル行列用ユニフォーム変数の位置

	const glm::mat4 matModel =
		glm::translate(glm::mat4(1), glm::vec3(actor.position)) *
		glm::rotate(glm::mat4(1), actor.rotation, glm::vec3(0, 1, 0)) *
		glm::scale(glm::mat4(1), actor.scale) *
		glm::translate(glm::mat4(1), actor.adjustment);
	const glm::mat4 matMVP = matProj * matView * matModel;
	pipeline.SetUniform(locMatTRS, matMVP);
	pipeline.SetUniform(locMatModel, matModel);

	actor.tex->Bind(0);
	actor.prim.Draw();
}

/**
*	アクター配列から名前の一致するアクターを検索する
*
*	@param actors	検索対象の配列
*	@param name		検索するアクターの名前
*
*	@retval nullptr以外	最初にnameと名前が一致したアクターのアドレス
*	@retval nullptr		actorsの中に名前が一致するアクターがない
*/
Actor* Find(std::vector<Actor>& actors, const char* name)
{
	Actor* result = nullptr;
	// actors配列からnameが一致する要素を見つけて返す
	for (int i = 0; i < actors.size(); ++i)
	{
		if (actors[i].name == name)
		{
			result = &actors[i];
			break;
		}
	}
	return result;
}