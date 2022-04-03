/**
*	@file Actor.h
*/
#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include <glad/glad.h>
#include "Primitive.h"
#include "Texture.h"
#include "ProgramPipeline.h"
#include <glm/glm.hpp>
#include <memory>

/**
*	•¨‘Ì‚ğ§Œä‚·‚éƒpƒ‰ƒ[ƒ^
*/
struct Actor
{
	Primitive prim;
	std::shared_ptr<Texture> tex;
	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::vec3 adjustment;
};

/**
*	•¨‘Ì‚ğ•`‰æ‚·‚é
*/
void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView);

#endif // ACTOR_H_INCLUDED