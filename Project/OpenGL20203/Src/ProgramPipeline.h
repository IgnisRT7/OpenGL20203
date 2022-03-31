/**
*	@file ProgramPipeline.h
*/
#ifndef PROGRAMPIPELINE_H_INCLUDED
#define PROGRAMPIPELINE_H_INCLUDED
#include <glad/glad.h>
#include <glm/glm.hpp>

/**
*	�V�F�[�_�v���O�������Ǘ�����N���X
*/
class ProgramPipeline
{
public:
	ProgramPipeline(const char* vsCode, const char* fsCode);
	~ProgramPipeline();

	//�I�u�W�F�N�g�̗L�����𔻒肷��
	bool IsValid() const;

	//���j�t�H�[���ϐ��̐ݒ�
	bool SetUniform(GLint, const glm::mat4&) const;

	//�o�C���h�Ǘ�
	void Bind() const;
	void Unbind() const;

private:
	GLuint vp = 0; // ���_�V�F�[�_�I�u�W�F�N�g
	GLuint fp = 0; // �t���O�����g�V�F�[�_�I�u�W�F�N�g
	GLuint pipeline = 0; // �p�C�v���C���I�u�W�F�N�g
};


#endif // PROGRAMPIPELINE_H_INCLUDED