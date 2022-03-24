/**
*	@file Primitive.h
*/
#ifndef PRIMITIVE_H_INCLUDED
#define PRIMITIVE_H_INCLUDED
#include <glad/glad.h>

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

#endif //PRIMITIVE_H_INCLUDED