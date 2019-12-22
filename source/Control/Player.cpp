#include "Player.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

#include "../Objects/Snowman.h"
#include "../Object/Object.h"
#include "../Objects/Terrain.h"
#include "../Common/Rot.h"

vec3 getCameraPos(Player p, Object scene) {
	float height = getHeight(scene, p.pos) + 1;
	return vec3(p.pos.x, height, p.pos.y);
}

vec3 getCameraDir(Player p) {
	return vec3(p.facing, 0, 0);
}

Player advance(Player p, vec2 dir, float dt) {
	vec3 step = vec3(dir.x, 0, dir.y) * pitch(p.facing);

	if (length(p.pos + vec2(step.x, step.z)*dt) > 50) {
		step = vec3(0);
	}

	return Player{ p.pos + vec2(step.x, step.z)*dt, p.facing, p.snowballCooldown, p.flareCooldown, p.nflares, p.hp };
}

vec2 getFacing(Player p, vec2 vect) {
	vec3 step = vec3(vect.x, 0, vect.y) * pitch(p.facing);
	return vec2(step.x, step.z);
}

Player stepPlayer(Player p, std::vector<Snowman> enemies, float dt) {
	float dmg = 0;
	float cooling = 1 * dt;

	for (int i = 0; i < enemies.size(); i++) {
		float d = distance(enemies[i].pos, p.pos);
		if (d <= DMG_DIST) {
			float normD = (d - MAX_DMG_DIST);
			float amount = 1 - (normD / (DMG_DIST - MAX_DMG_DIST));

			dmg += DMG_PER_SNOWMAN * dt * amount;
		}
	}

	float regen = 0;

	if (dmg <= 0) {
		regen = REGEN * dt;
	}

	return Player{ p.pos, p.facing, p.snowballCooldown - cooling, p.flareCooldown - cooling, p.nflares, min(p.hp - min(dmg, 0.5f * dt) + regen, 1.0f) };
}

bool canSnowball(Player p) {
	return p.snowballCooldown <= 0;
}

bool canFlare(Player p) {
	return p.flareCooldown <= 0 && p.nflares > 0;
}

Droppable snowball(Player p, Object terrain) {
	return spawnSnowball(getCameraPos(p, terrain) + vec3(0, -0.5, 0), vec3(0, 1.5, 5) * pitch(p.facing));
}

Droppable flare(Player p, Object terrain) {
	return spawnFlare(getCameraPos(p, terrain) + vec3(0, -0.5, 0), vec3(0, 1, 2.5) * pitch(p.facing));
}

Player spawnPlayer(vec2 pos) {
	return Player{ pos, 0, 0, 0, 9, 1 };
}