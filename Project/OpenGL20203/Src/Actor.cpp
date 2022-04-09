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

/**
*	衝突を検出する	
*
*	@param actorA	衝突しているか調べるアクター
*	@param actorB	衝突しているか調べるアクター	
*
*	@retval true	衝突している
*	@retval false	衝突していない
*/
bool DetectCollision(Actor& actorA, Actor& actorB)
{
	// ワールド座標系のコライダーを計算する
	Box a = actorA.collider;
	a.min += actorA.position;
	a.max += actorA.position;

	Box b = actorB.collider;
	b.min += actorB.position;
	b.max += actorB.position;

	// aの左側面がbの右側面より右にあるなら、衝突していない
	if (a.min.x >= b.max.x)
	{
		return false;
	}
	// aの右側面がbの左側面より左にあるなら、衝突していない
	if (a.max.x <= b.min.x)
	{
		return false;
	}
	
	// aの下面がbの上面より上にあるなら、衝突していない
	if (a.min.y >= b.max.y)
	{
		return false;
	}
	// aの上面がbの下面より下にあるなら、衝突していない
	if (a.max.y <= b.min.y)
	{
		return false;
	}
	
	// aの奥側面がbの手前側面より手前にあるなら、衝突していない
	if (a.min.z >= b.max.z)
	{
		return false;
	}
	// aの手前側面がbの奥側面より奥にあるなら、衝突していない
	if (a.max.z <= b.min.z)
	{
		return false;
	}
	
	return true;
}