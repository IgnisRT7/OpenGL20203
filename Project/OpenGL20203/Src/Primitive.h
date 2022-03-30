/**
*	@file Primitive.h
*/
#ifndef PRIMITIVE_H_INCLUDED
#define PRIMITIVE_H_INCLUDED
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

/**
*	プリミティブデータ
*/
class Primitive
{
public:
	Primitive() = default;
	Primitive(GLenum m, GLsizei c, size_t o, GLint b) :
		mode(m), count(c), indices(reinterpret_cast<GLvoid*>(o)), baseVertex(b)
	{}
	~Primitive() = default;

	void Draw()const;

private:
	GLenum mode = GL_TRIANGLES; ///< プリミティブの種類
	GLsizei count = 0; ///< 描画するインデックス数
	const GLvoid* indices = 0; ///< 描画開始インデックスのバイトオフセット
	GLint baseVertex = 0; ///< インデックス0バンとみなされる頂点配列内の位置

};

/**
*	複数のプリミティブを管理するクラス
*/
class PrimitiveBuffer
{
public:
	PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIndexCount);
	~PrimitiveBuffer();

	//プリミティブの追加
	bool Add(size_t vertexCount, const glm::vec3* pPosition, const glm::vec4* pColor, 
		const glm::vec2* pTexcoord, size_t indexCount, const GLushort* pIndex);

	//プリミティブの取得
	const Primitive& Get(size_t n) const;

	//VAOバインドの管理
	void BindVertexArray() const;
	void UnbindVertexArray() const;

private:
	std::vector<Primitive> primitives; //描画データの配列

	//バッファID
	GLuint vboPosition = 0;
	GLuint vboColor = 0;
	GLuint vboTexcoord = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	GLsizei maxVertexCount = 0; // 格納できる最大頂点数
	GLsizei curVertexCount = 0; // 格納済み頂点数
	GLsizei maxIndexCount = 0; // 格納できる最大インデックス数
	GLsizei curIndexCount = 0; // 格納済みインデックス数
};

#endif //PRIMITIVE_H_INCLUDED