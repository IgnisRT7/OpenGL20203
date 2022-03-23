/**
*	@file GLContext.cpp
*/
#include "GLContext.h"
#include <iostream>

/**
*	OpenGL�R���e�L�X�g�Ɋւ���@�\���i�[���閼�O���
*/
namespace GLContext
{
	/**
	*	�o�b�t�@�I�u�W�F�N�g���쐬����
	*
	*	@param size �f�[�^�ւ̃T�C�Y
	*	@param data �f�[�^�ւ̃|�C���^
	*	
	*	@retrun �쐬�����o�b�t�@�I�u�W�F�N�g
	*/
	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data)
	{
		GLuint id = 0;
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, size, data, 0);
		return id;
	}

	/**
	*	VertexArrayobject���쐬����
	*
	*	@param vboPosition	VAO�Ɋ֘A�t��������W�f�[�^
	*	@param vboColor		VAO�Ɋ֘A�t������J���[�f�[�^
	*
	*	@return �쐬����VAO
	*/
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor)
	{
		if (!vboPosition || vboColor)
		{
			std::cerr << "[�G���[]" << __func__ << ":�o�b�t�@�I�u�W�F�N�g��0�ł��B\n";
			return 0;
		}

		GLuint id = 0;
		glCreateVertexArrays(1, &id);

		// ���W���̃o�C���h
		const GLuint positionIndex = 0;
		const GLuint positionBindingIndex = 0;
		glEnableVertexArrayAttrib(id, positionIndex);
		glVertexArrayAttribFormat(id, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, positionIndex, positionBindingIndex);
		glVertexArrayVertexBuffer(id, positionBindingIndex, vboPosition, 0, sizeof(Position));

		// �F���̃o�C���h
		const GLuint colorIndex = 1;
		const GLuint colorBindingIndex = 1;
		glEnableVertexArrayAttrib(id, colorIndex);
		glVertexArrayAttribFormat(id, colorIndex, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, colorIndex, colorBindingIndex);
		glVertexArrayVertexBuffer(id, colorBindingIndex, vboColor, 0, sizeof(Color));

		return id;
	}

} // namespace GLContext