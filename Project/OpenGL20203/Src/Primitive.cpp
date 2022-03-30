/**
*	@Primitive.cpp
*/
#include "Primitive.h"
#include "GLContext.h"
#include <iostream>

/**
*	�f�[�^��GPU�������ɃR�s�[����
*
*	@param writeBuffer	�R�s�[��̃o�b�t�@�I�u�W�F�N�g
*	@param unitSize		�v�f�̃o�C�g��
*	@param offsetCount	�R�s�[��I�t�Z�b�g(�v�f�P��)
*	@param count		�R�s�[����v�f��
*	@param data			�R�s�[����f�[�^�̃A�h���X
*
*	@retval true	�R�s�[����
*	@retval false	�R�s�[���s
*/
bool CopyData(GLuint writeBuffer, GLsizei unitSize,
	GLsizei offsetCount, size_t count, const void* data)
{
	const GLsizei size = static_cast<GLsizei>(unitSize * count);
	const GLuint readBuffer = GLContext::CreateBuffer(size, data);
	if (!readBuffer)
	{
		std::cerr << "[�G���[]" << __func__ << ": �R�s�[���o�b�t�@�̍쐬�Ɏ��s(size=" <<
			size << ").\n";
		return false;
	}
	const GLsizei offset = static_cast<GLsizei>(unitSize * offsetCount);
	glCopyNamedBufferSubData(readBuffer, writeBuffer, 0, offset, size);
	glDeleteBuffers(1, &readBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]" << __func__ << ": �f�[�^�̃R�s�[�Ɏ��s(size=" <<
			size << ", offset=" << offset << ").\n";
		return false;
	}
}

/**
*	�v���~�e�B�u��`�悷��	
*/
void Primitive::Draw() const
{
	glDrawElementsBaseVertex(mode, count, GL_UNSIGNED_SHORT, indices, baseVertex);
}

/**
*	�v���~�e�B�u�p�̃��������m�ۂ���
*
*	@param maxVertexCount	�i�[�\�ȍő咷�_��
*	@param maxIndexCount	�i�[�\�ȍő�C���f�b�N�X��
*/
PrimitiveBuffer::PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIdexCount)
{
	//VBO, IBO, VAO���쐬
	vboPosition = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboColor = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboTexcoord = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	ibo = GLContext::CreateBuffer(sizeof(GLushort) * maxIndexCount, nullptr);
	vao = GLContext::CreateVertexArray(vboPosition, vboColor, vboTexcoord, ibo);
	if (!vboPosition || !vboColor || !vboTexcoord || !ibo || !vao)
	{
		std::cerr << "[�G���[]" << __func__ << ": VAO�̍쐬�Ɏ��s\n";
	}

	//�`��f�[�^�p�̃�������1000���\��
	primitives.reserve(1000);

	//�Ǘ��\��̒��_���ƃC���f�b�N�X����ݒ�
	this->maxVertexCount = maxVertexCount;
	this->maxIndexCount = maxIndexCount;
}

PrimitiveBuffer::~PrimitiveBuffer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vboTexcoord);
	glDeleteBuffers(1, &vboColor);
	glDeleteBuffers(1, &vboPosition);
}

/**
*	�`��f�[�^��ǉ�����
*
*	@param vertexCount	�ǉ����钸�_�f�[�^�̐�
*	@param pPosition	���W�f�[�^�ւ̃|�C���^
*	@param pColor		�F�f�[�^�ւ̃|�C���^
*	@param pTexcoord	�e�N�X�`�����W�f�[�^�ւ̃|�C���^
*	@param indexCount	�ǉ�����C���f�b�N�X�f�[�^�̐�
*	@param pIndex		�C���f�b�N�X�f�[�^�ւ̃|�C���^
*
*	@retval true	�ǉ��ɐ���
*	@retval false	�ǉ��Ɏ��s
*/
bool PrimitiveBuffer::Add(size_t vertexCount, const glm::vec3* pPosition,
	const glm::vec4* pColor, const glm::vec2* pTexcoord,
	size_t indexCount, const GLushort* pIndex)
{
	//�G���[�`�F�b�N
	if (!vao)
	{
		std::cerr << "[�G���[]" << __func__ << ": VAO�̍쐬�Ɏ��s���Ă��܂�.\n";
		return false;
	}
	else if (vertexCount > static_cast<size_t>(maxVertexCount) - curVertexCount)
	{
		std::cerr << "[�x��]" << __func__ << ": VBO�����t�ł�(max=" << maxVertexCount <<
			", cur=" << curVertexCount << ", add=" << vertexCount << ")\n";
		return false;
	}
	else if (indexCount > static_cast<size_t>(maxIndexCount) - curIndexCount)
	{
		std::cerr << "[�x��]" << __func__ << ": IBO�����t�ł�(max=" << maxIndexCount <<
			", cur=" << curIndexCount << ", add=" << indexCount << ")\n";
		return false;
	}

	//GPU�������ɒ��_���W�f�[�^���R�s�[
	if (!CopyData(vboPosition, sizeof(glm::vec3), curVertexCount, vertexCount, pPosition))
	{
		return false;
	}

	//GPU�������ɐF�f�[�^���R�s�[
	if (!CopyData(vboColor, sizeof(glm::vec4), curVertexCount, vertexCount, pColor))
	{
		return false;
	}

	//GPU�������Ƀe�N�X�`�����W�f�[�^���R�s�[
	if (!CopyData(vboTexcoord, sizeof(glm::vec2), curVertexCount, vertexCount, pTexcoord))
	{
		return false;
	}

	//GPU�������ɃC���f�b�N�X�f�[�^���R�s�[
	if (!CopyData(ibo, sizeof(GLushort), curIndexCount, indexCount, pIndex))
	{
		return false;
	}

	//�`��f�[�^���쐬
	const Primitive prim(GL_TRIANGLES, static_cast<GLsizei>(indexCount),
		sizeof(GLushort) * curIndexCount, curVertexCount);

	//�`��f�[�^��z��ɒǉ�
	primitives.push_back(prim);
	
	//���݂̃f�[�^�����A�ǉ������f�[�^���������₷
	curVertexCount += static_cast<GLsizei>(vertexCount);
	curIndexCount += static_cast<GLsizei>(indexCount);

	return true;
}

/**
*	�v���~�e�B�u���擾����
*
*	@param n	�v���~�e�B�u�̃C���f�b�N�X
*	
*	@return n�ɑΉ�����v���~�e�B�u
*			�Ή������f�[�^�������ꍇ�͖��̃f�[�^��Ԃ�
*/
const Primitive& PrimitiveBuffer::Get(size_t n) const
{
	if (n > static_cast<int>(primitives.size()))
	{
		std::cerr << "[�x��]" << __func__ << ":" << n <<
			"�͖����ȃC���f�b�N�X�ł�(size=" << primitives.size() << ").\n";
		// ���̕`��f�[�^��Ԃ�.
		static const Primitive dummy;
		return dummy;
	}
	return primitives[n];
}

/**
*	VAO���O���t�B�b�N�X�p�C�v���C���Ƀo�C���h����
*/
void PrimitiveBuffer::BindVertexArray() const
{
	glBindVertexArray(vao);
}

/**
*	VAO�̃o�C���h����������
*/
void PrimitiveBuffer::UnbindVertexArray() const
{
	glBindVertexArray(0);
}