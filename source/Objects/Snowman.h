#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Snowman.h"
#include "../Common/Rot.h"
#include "../Object/Mesh.h"
#include "../Object/Object.h"
#include <vector>

#define SNOWMAN_SPEED 2

struct Snowman {
	glm::vec2 pos;
	float hp;
};

Object genSnowman(Snowman s, Object terrain, glm::vec2 playerPos);

Snowman stepSnowman(std::vector<Snowman> snowmen, int id, glm::vec2 playerPos, float dt);