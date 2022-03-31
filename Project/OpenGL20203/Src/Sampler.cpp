/**
*	@file Sampler.cpp
*/
#include "Sampler.h"
#include "GLContext.h"
#include <glad/glad.h>
#include <iostream>

/**
*	�R���X�g���N�^
*
*	@param wrapMode ���b�v���[�h
*/
Sampler::Sampler(GLenum wrapMode)
{
	id = GLContext::CreateSampler(wrapMode);
	if (id)
	{
		std::cout << "[���]" << __func__ << "�T���v�����쐬.\n";
	}
	else
	{
		std::cout << "[�G���[]" << __func__ << "�T���v���̍쐬�Ɏ��s.\n";
	}
}

/**
*	�f�X�g���N�^
*/
Sampler::~Sampler()
{
	if (id)
	{
		std::cout << "[���]" << __func__ << "�T���v��" << "���폜\n";
	}
	glDeleteSamplers(1, &id);
}

/**
*	�I�u�W�F�N�g���g�����Ԃ����ׂ�
*
*	@retval true	�g����
*	@retval false	�g���Ȃ�
*/
bool Sampler::IsValid() const
{
	return id;
}

/**
*	�T���v�������蓖�Ă�
* 
*	@param unit ���蓖�Ă�T���v���̃��j�b�g�ԍ�
*/
void Sampler::Bind(GLuint unit) const
{
	glBindSampler(unit, id);
}

/**
*	�T���v���̊��蓖�Ă���������
* 
*	@param unit ���蓖�ĉ�������e�N�X�`���C���[�W���j�b�g�̔ԍ�
*/
void Sampler::Unbind(GLuint unit) const
{
	glBindSampler(unit, 0);
}