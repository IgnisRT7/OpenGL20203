/**
*	@Primitive.cpp
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Primitive.h"
#include "GLContext.h"
#include <fstream>
#include <string>
#include <stdio.h>
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
	return true;
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
PrimitiveBuffer::PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIndexCount)
{
	//VBO, IBO, VAOを作成
	vboPosition = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboColor = GLContext::CreateBuffer(sizeof(glm::vec4) * maxVertexCount, nullptr);
	vboTexcoord = GLContext::CreateBuffer(sizeof(glm::vec2) * maxVertexCount, nullptr);
	vboNormal = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	ibo = GLContext::CreateBuffer(sizeof(GLushort) * maxIndexCount, nullptr);
	vao = GLContext::CreateVertexArray(vboPosition, vboColor, vboTexcoord, vboNormal, ibo);
	if (!vboPosition || !vboColor || !vboTexcoord || !vboNormal || !ibo || !vao)
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
	glDeleteBuffers(1, &vboNormal);
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
*	@param pNormal		法線データへのポインタ
*	@param indexCount	追加するインデックスデータの数
*	@param pIndex		インデックスデータへのポインタ
*
*	@retval true	追加に成功
*	@retval false	追加に失敗
*/
bool PrimitiveBuffer::Add(size_t vertexCount, const glm::vec3* pPosition,
	const glm::vec4* pColor, const glm::vec2* pTexcoord, const glm::vec3* pNormal,
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

	//GPUメモリに法線データをコピー
	if (!CopyData(vboNormal, sizeof(glm::vec3), curVertexCount, vertexCount, pNormal))
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
*	OBJファイルからプリミティブを追加する
*
*	@param filename	ロードするOBJファイル名
*
*	@retval true	追加成功
*	@retval false	追加失敗
*/
bool PrimitiveBuffer::AddFromObjeFile(const char* filename)
{
	//ファイルを開く
	std::ifstream ifs(filename);
	if (!ifs)
	{
		std::cerr << "[エラー]" << __func__ << ":`" << filename << "`を開けません.\n";
		return false;
	}

	//データ読み取り用の配列を準備
	std::vector<glm::vec3> objPositions; // OBJファイルの頂点座標
	std::vector<glm::vec2> objTexcoords; // OBJファイルのテクスチャ座標
	std::vector<glm::vec3> objNormals; // OBJファイルの法線
	struct Index
	{
		int v = 0;
		int vt = 0;
		int vn = 0;
	};
	std::vector<Index> objIndices; // OBJファイルのインデックス

	//配列用のメモリを予約
	objPositions.reserve(100'000);
	objTexcoords.reserve(100'000);
	objNormals.reserve(100'100);
	objIndices.reserve(100'000);

	// ファイルからモデルのデータを読み込む
	size_t lineNo = 0; // 読み込んだ行数
	while (!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line); // ファイルから1行読み込む
		++lineNo;

		// 行の先頭にある空白を読み飛ばす
		const size_t posData = line.find_first_not_of(" \t");
		if (posData != std::string::npos)
		{
			line = line.substr(posData);
		}

		// 空行またはコメント行なら無視して次の行へ進む
		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		// データの種類を取得
		const size_t posEndOfType = line.find_first_of(" \t");
		const std::string type = line.substr(0, posEndOfType);
		const char* p = line.c_str() + posEndOfType; // データ部分を刺すポインタ

		// タイプ別のデータ読み込み処理
		if (type == "v") // 頂点座標
		{
			glm::vec3 v(0);
			if (sscanf(p, "%f %f %f", &v.x, &v.y, &v.z) != 3)
			{
				std::cerr << "[警告]" << __func__ << ":頂点座標の読み取りに失敗.\n" <<
					"  " << filename << "(" << lineNo << "行目): " << line << "\n";
			}
			objPositions.push_back(v);
		}
		else if (type == "vt") //テクスチャ座標
		{
			glm::vec2 vt(0);
			if (sscanf(p, "%f %f", &vt.x, &vt.y) != 2)
			{
				std::cerr << "[警告]" << __func__ << ":テクスチャ座標の読み取りに失敗.\n" <<
					"  " << filename << "(" << lineNo << "行目): " << line << "\n";
			}
			objTexcoords.push_back(vt);
		}
		else if (type == "vn") // 法線
		{
			glm::vec3 vn(0);
			if (sscanf(p, "%f %f %f", &vn.x, &vn.y, &vn.z) != 3)
			{
				std::cerr << "[警告]" << __func__ << ":法線の読み取りに失敗.\n" <<
					"  " << filename << "(" << lineNo << "行目): " << line << "\n";
			}
			 objNormals.push_back(vn);

		}
		else if (type == "f") //面
		{
			Index f[3];
			bool isSuccess = false; // 解析判定用

			//頂点・テクスチャ座標のみ
			if(sscanf(p, "%d/%d %d/%d %d/%d",
				&f[0].v, &f[0].vt,
				&f[1].v, &f[1].vt,
				&f[2].v, &f[2].vt) == 6)
			{
				isSuccess = true;
			}

			//頂点・テクスチャ座標・法線用
			if (sscanf(p, "%d/%d/%d %d/%d/%d %d/%d/%d",
				&f[0].v, &f[0].vt, &f[0].vn,
				&f[1].v, &f[1].vt, &f[1].vn,
				&f[2].v, &f[2].vt, &f[2].vn) == 9)
			{
				isSuccess = true;
			}

			if(!isSuccess)
			{
				std::cerr << "[警告]" << __func__ << ":面データの読み取りに失敗.\n"
					 "  " << filename << "(" << lineNo << "行目): " << line << "\n";
			}
			else
			{
				for (int i = 0; i < 3; ++i)
				{
					objIndices.push_back(f[i]);
				}
			}
		}
		else // 未対応
		{
			std::cerr << "[警告]" << __func__ << ":未対応の形式です.\n" <<
				"  " << filename << "(" << lineNo << "行目): " << line << "\n";
		}
	}

	//データ変換用の配列を準備
	std::vector<glm::vec3> positions; // OpenGL用の頂点座標
	std::vector<glm::vec4> colors; // OpenGLの色
	std::vector<glm::vec2> texcoords; // OpenGL用のテクスチャ座標
	std::vector<glm::vec3> normals; // OpenGL用の法線
	std::vector<GLushort> indices; // OpenGL用のインデックス

	//データ変換用のメモリを確保
	const size_t indexCount = objIndices.size();
	positions.reserve(indexCount);
	texcoords.reserve(indexCount);
	normals.reserve(indexCount);
	indices.reserve(indexCount);

	// OBJファイルのデータをOpenGLのデータに変換
	for (size_t i = 0; i < indexCount; ++i)
	{
		// インデックスデータを追加
		indices.push_back(static_cast<GLushort>(i));

		// 頂点座標を変換
		const int v = objIndices[i].v - 1;
		if (v < static_cast<int>(objPositions.size()))
		{
			positions.push_back(objPositions[v]);
		}
		else
		{
			std::cerr << "[警告]" << __func__ << ":頂点座標インデックス" << v <<
				"は範囲[0, " << objPositions.size() << ")の外を指しています.\n" <<
				"  " << filename << "\n";
			positions.push_back(glm::vec3(0));
		}

		// テクスチャ座標を変換
		const int vt = objIndices[i].vt - 1;
		if (vt < static_cast<int>(objTexcoords.size()))
		{
			texcoords.push_back(objTexcoords[vt]);
		}
		else
		{
			std::cerr << "[警告]" << __func__ << ":頂点座標インデックス" << vt <<
				"は範囲[0, " << objTexcoords.size() << ")の外を指しています.\n" <<
				"  " << filename << "\n";
			texcoords.push_back(glm::vec2(0));
		}

		// 法線インデックスが無いデータの場合、適当な法線を用意する
		if (objIndices[i].vn == 0)
		{
			//適当な法線データを追加
			const glm::vec3 normal = glm::vec3(0, 1, 0);
			normals.push_back(normal);
		}
		else
		{
			// 法線を変換
			const int vn = objIndices[i].vn - 1;
			if (vn < static_cast<int>(objNormals.size()))
			{
				normals.push_back(objNormals[vn]);
			}
			else
			{
				std::cerr << "[警告]" << __func__ << ":法線インデックス" << vn <<
					"は範囲[0, " << objNormals.size() << ")の外を指しています.\n" <<
					"  " << filename << "\n";
				normals.push_back(glm::vec3(0, 1, 0));
			}
		}

		//色データを設定
		colors.resize(positions.size(), glm::vec4(1));
	}

	//プリミティブを追加する
	const bool result = Add(positions.size(), positions.data(), colors.data(),
		texcoords.data(), normals.data(), indices.size(), indices.data());
	if (result)
	{
		std::cout << "[情報]" << __func__ << ":" << filename << "(頂点数=" <<
			positions.size() << " インデックス数=" << indices.size() << ")\n";
	}
	else
	{
		std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込みに失敗.\n";
	}
	return result;
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