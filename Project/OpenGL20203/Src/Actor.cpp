/**
*	@file Actor.cpp
*/
#include "Actor.h"
#include <glm/gtc/matrix_transform.hpp>

/**
*	���̂�`�悷��
*/
void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView)
{
	// uniform�ϐ��̈ʒu
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1; // ���f���s��p���j�t�H�[���ϐ��̈ʒu

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
*	�A�N�^�[�z�񂩂疼�O�̈�v����A�N�^�[����������
*
*	@param actors	�����Ώۂ̔z��
*	@param name		��������A�N�^�[�̖��O
*
*	@retval nullptr�ȊO	�ŏ���name�Ɩ��O����v�����A�N�^�[�̃A�h���X
*	@retval nullptr		actors�̒��ɖ��O����v����A�N�^�[���Ȃ�
*/
Actor* Find(std::vector<Actor>& actors, const char* name)
{
	Actor* result = nullptr;
	// actors�z�񂩂�name����v����v�f�������ĕԂ�
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
*	�Փ˂����o����	
*
*	@param actorA	�Փ˂��Ă��邩���ׂ�A�N�^�[
*	@param actorB	�Փ˂��Ă��邩���ׂ�A�N�^�[	
*
*	@retval true	�Փ˂��Ă���
*	@retval false	�Փ˂��Ă��Ȃ�
*/
bool DetectCollision(Actor& actorA, Actor& actorB)
{
	// ���[���h���W�n�̃R���C�_�[���v�Z����
	Box a = actorA.collider;
	a.min += actorA.position;
	a.max += actorA.position;

	Box b = actorB.collider;
	b.min += actorB.position;
	b.max += actorB.position;

	// a�̍����ʂ�b�̉E���ʂ��E�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	if (a.min.x >= b.max.x)
	{
		return false;
	}
	// a�̉E���ʂ�b�̍����ʂ�荶�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	if (a.max.x <= b.min.x)
	{
		return false;
	}
	
	// a�̉��ʂ�b�̏�ʂ���ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	if (a.min.y >= b.max.y)
	{
		return false;
	}
	// a�̏�ʂ�b�̉��ʂ�艺�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	if (a.max.y <= b.min.y)
	{
		return false;
	}
	
	// a�̉����ʂ�b�̎�O���ʂ���O�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	if (a.min.z >= b.max.z)
	{
		return false;
	}
	// a�̎�O���ʂ�b�̉����ʂ�艜�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	if (a.max.z <= b.min.z)
	{
		return false;
	}
	
	return true;
}