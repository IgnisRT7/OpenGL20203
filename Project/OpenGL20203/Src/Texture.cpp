/**
*	@file Texture.cpp
*/
#include "Texture.h"
#include "GLContext.h"
#include <iostream>

/**
*	�R���X�g���N�^	
* 
*	@param filename �摜�t�@�C����
*/
Texture::Texture(const char* filename)
{
	id = GLContext::CreateImage2D(filename);
	if (id)
	{
		name = filename;
		std::cout << "[���]" << __func__ << "�e�N�X�`��" << filename << "���쐬.\n";
	}
	else
	{
		std::cout << "[�G���[]" << __func__ << "�e�N�X�`��" << filename << "�̍쐬�Ɏ��s.\n";
	}
}

/**
*	�f�X�g���N�^
*/
Texture::~Texture()
{
	if (id)
	{
		std::cout << "[���]" << __func__ << "�e�N�X�`��" << name << "���폜\n";
	}
	glDeleteTextures(1, &id);
}

/**
*	�I�u�W�F�N�g���g�����Ԃ����ׂ�
*
*	@retval true	�g����
*	@retval false	�g���Ȃ�
*/
bool Texture::IsValid() const
{
	return id;
}

/**
*	�e�N�X�`�����O���t�B�b�N�X�p�C�v���C���Ɋ��蓖�Ă�
*
*	@param unit	���蓖�Ă�e�N�X�`���C���[�W���j�b�g�̔ԍ�
*/
void Texture::Bind(GLuint unit) const
{
	glBindTextureUnit(unit, id);
}

/**
*	�e�N�X�`���̊��蓖�Ă���������
*
*	@param unit ���蓖�ĉ�������e�N�X�`���C���[�W���j�b�g�̔ԍ�
*/
void Texture::Unbind(GLuint unit) const
{
	glBindTextureUnit(unit, 0);
}