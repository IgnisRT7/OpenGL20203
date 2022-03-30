/**
*	@Primitive.cpp
*/
#include "Primitive.h"
#include "GLContext.h"
#include <iostream>

/**
*	データをGPUメモリにコピーする
*
*	@param writeBuffer	コピー先のバッファオブジェクト
*	@param unitSize		要素のバイト数
*	@param offsetCount	コピー先オフセット(要素単位)
*	@param count		コピーする要素数
*	@param data			コピーするデータのアドレス
*
*	@retval true	コピー成功
*	@retval false	コピー失敗
*/
bool CopyData(GLuint writeBuffer, GLsizei unitSize,
	GLsizei offsetCount, size_t count, const void* data)
{
	const GLsizei size = static_cast<GLsizei>(unitSize * count);
	const GLuint readBuffer = GLContext::CreateBuffer(size, data);
	if (!readBuffer)
	{
		std::cerr << "[エラー]" << __func__ << ": コピー元バッファの作成に失敗(size=" <<
			size << ").\n";
		return false;
	}
	const GLsizei offset = static_cast<GLsizei>(unitSize * offsetCount);
	glCopyNamedBufferSubData(readBuffer, writeBuffer, 0, offset, size);
	glDeleteBuffers(1, &readBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[エラー]" << __func__ << ": データのコピーに失敗(size=" <<
			size << ", offset=" << offset << ").\n";
		return false;
	}
}

/**
*	プリミティブを描画する	
*/
void Primitive::Draw() const
{
	glDrawElementsBaseVertex(mode, count, GL_UNSIGNED_SHORT, indices, baseVertex);
}

/**
*	プリミティブ用のメモリを確保する
*
*	@param maxVertexCount	格納可能な最大長点数
*	@param maxIndexCount	格納可能な最大インデックス数
*/
PrimitiveBuffer::PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIdexCount)
{
	//VBO, IBO, VAOを作成
	vboPosition = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboColor = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboTexcoord = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	ibo = GLContext::CreateBuffer(sizeof(GLushort) * maxIndexCount, nullptr);
	vao = GLContext::CreateVertexArray(vboPosition, vboColor, vboTexcoord, ibo);
	if (!vboPosition || !vboColor || !vboTexcoord || !ibo || !vao)
	{
		std::cerr << "[エラー]" << __func__ << ": VAOの作成に失敗\n";
	}

	//描画データ用のメモリを1000個分予約
	primitives.reserve(1000);

	//管理予定の頂点数とインデックス数を設定
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
*	描画データを追加する
*
*	@param vertexCount	追加する頂点データの数
*	@param pPosition	座標データへのポインタ
*	@param pColor		色データへのポインタ
*	@param pTexcoord	テクスチャ座標データへのポインタ
*	@param indexCount	追加するインデックスデータの数
*	@param pIndex		インデックスデータへのポインタ
*
*	@retval true	追加に成功
*	@retval false	追加に失敗
*/
bool PrimitiveBuffer::Add(size_t vertexCount, const glm::vec3* pPosition,
	const glm::vec4* pColor, const glm::vec2* pTexcoord,
	size_t indexCount, const GLushort* pIndex)
{
	//エラーチェック
	if (!vao)
	{
		std::cerr << "[エラー]" << __func__ << ": VAOの作成に失敗しています.\n";
		return false;
	}
	else if (vertexCount > static_cast<size_t>(maxVertexCount) - curVertexCount)
	{
		std::cerr << "[警告]" << __func__ << ": VBOが満杯です(max=" << maxVertexCount <<
			", cur=" << curVertexCount << ", add=" << vertexCount << ")\n";
		return false;
	}
	else if (indexCount > static_cast<size_t>(maxIndexCount) - curIndexCount)
	{
		std::cerr << "[警告]" << __func__ << ": IBOが満杯です(max=" << maxIndexCount <<
			", cur=" << curIndexCount << ", add=" << indexCount << ")\n";
		return false;
	}

	//GPUメモリに頂点座標データをコピー
	if (!CopyData(vboPosition, sizeof(glm::vec3), curVertexCount, vertexCount, pPosition))
	{
		return false;
	}

	//GPUメモリに色データをコピー
	if (!CopyData(vboColor, sizeof(glm::vec4), curVertexCount, vertexCount, pColor))
	{
		return false;
	}

	//GPUメモリにテクスチャ座標データをコピー
	if (!CopyData(vboTexcoord, sizeof(glm::vec2), curVertexCount, vertexCount, pTexcoord))
	{
		return false;
	}

	//GPUメモリにインデックスデータをコピー
	if (!CopyData(ibo, sizeof(GLushort), curIndexCount, indexCount, pIndex))
	{
		return false;
	}

	//描画データを作成
	const Primitive prim(GL_TRIANGLES, static_cast<GLsizei>(indexCount),
		sizeof(GLushort) * curIndexCount, curVertexCount);

	//描画データを配列に追加
	primitives.push_back(prim);
	
	//現在のデータ数を、追加したデータ数だけ増やす
	curVertexCount += static_cast<GLsizei>(vertexCount);
	curIndexCount += static_cast<GLsizei>(indexCount);

	return true;
}

/**
*	プリミティブを取得する
*
*	@param n	プリミティブのインデックス
*	
*	@return nに対応するプリミティブ
*			対応したデータが無い場合は無のデータを返す
*/
const Primitive& PrimitiveBuffer::Get(size_t n) const
{
	if (n > static_cast<int>(primitives.size()))
	{
		std::cerr << "[警告]" << __func__ << ":" << n <<
			"は無効なインデックスです(size=" << primitives.size() << ").\n";
		// 仮の描画データを返す.
		static const Primitive dummy;
		return dummy;
	}
	return primitives[n];
}

/**
*	VAOをグラフィックスパイプラインにバインドする
*/
void PrimitiveBuffer::BindVertexArray() const
{
	glBindVertexArray(vao);
}

/**
*	VAOのバインドを解除する
*/
void PrimitiveBuffer::UnbindVertexArray() const
{
	glBindVertexArray(0);
}