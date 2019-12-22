#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../Objects/Snowman.h"
#include "../Object/Object.h"
#include "../Objects/Terrain.h"
#include "../Common/Rot.h"
#include "../Objects/Droppable.h"
#include "../Objects/Snowball.h"

#define DMG_PER_SNOWMAN 0.05f
#define DMG_DIST 6.0f
#define MAX_DMG_DIST 5.0f
#define REGEN 0.025f

struct Player {
	glm::vec2 pos;
	float facing;
	float snowballCooldown;
	float flareCooldown;
	int nflares;
	float hp;
};

glm::vec3 getCameraPos(Player p, Object scene);

glm::vec3 getCameraDir(Player p);

glm::vec2 getFacing(Player p, glm::vec2 vect);

Player advance(Player p, glm::vec2 dir, float dt);

bool canSnowball(Player p);
bool canFlare(Player p);

Player stepPlayer(Player p, std::vector<Snowman> enemies, float dt);

Droppable snowball(Player p, Object terrain);
Droppable flare(Player p, Object terrain);

Player spawnPlayer(glm::vec2 pos);