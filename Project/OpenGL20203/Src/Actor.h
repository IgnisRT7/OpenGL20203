/**
*	@file Actor.h
*/
#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include <glad/glad.h>
#include "Primitive.h"
#include "Texture.h"
#include "ProgramPipeline.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>

/**
*	���̂𐧌䂷��p�����[�^
*/
struct Actor
{
	std::string name;// �A�N�^�[�̖��O
	Primitive prim;
	std::shared_ptr<Texture> tex;
	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::vec3 adjustment;
};

/**
*	���̂�`�悷��
*/
void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView);

Actor* Find(std::vector<Actor>& actors, const char* name);
#endif // ACTOR_H_INCLUDED