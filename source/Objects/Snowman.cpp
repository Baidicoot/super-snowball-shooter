#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include "Snowman.h"
#include "../Common/Rot.h"
#include "Terrain.h"
#include "../Object/Object.h"

#include <vector>

using namespace std;
using namespace glm;

Object genSnowman(Snowman s, Object terrain, vec2 playerPos) {
	vec2 toPlayer = playerPos - s.pos;
	float angleToPlayer = atan2(toPlayer.x, toPlayer.y);

	vec3 pos = vec3(s.pos.x, getHeight(terrain, s.pos), s.pos.y);

	vec4 pos2(pos, 0);
	vec4 snowman[] = {
		vec4(0, 0, 0, 1),
		vec4(0, 1, 0, 0.75),
		vec4(0, 2, 0, 0.5),

		vec4(0.25, 2, 0.385, 0.1),
		vec4(-0.25, 2, 0.385, 0.1),

		vec4(0, 1.9, 0.5, 0.1)
	};
	int sn = sizeof(snowman) / sizeof(snowman[0]);

	vec3 colmap[] = {
		vec3(1), vec3(1), vec3(1),
		vec3(0), vec3(0),
		vec3(1, 0.5, 0)
	};
	int cn = sizeof(colmap) / sizeof(colmap[0]);

	std::vector<vec4> objs(snowman, snowman + sn);

	for (int i = 0; i < objs.size(); i++) {
		vec3 rot = vec3(objs[i].x, objs[i].y, objs[i].z) * pitch(angleToPlayer);
		objs[i] = vec4(rot, objs[i].w) + pos2;
	}

	std::vector<vec3> colors(colmap, colmap + cn);

	return fromMesh(Mesh{objs, colors});
}

Snowman stepSnowman(vector<Snowman> snowmen, int id, vec2 playerPos, float dt) {
	vec2 step(0);

	auto s = snowmen[id];

	vec2 toPlayer = playerPos - s.pos;

	if (length(toPlayer) > 5) {
		step += normalize(toPlayer);
	}

	for (int i = 0; i < snowmen.size(); i++) {
		if (i != id && distance(snowmen[i].pos, s.pos) < 3) {
			step += normalize(s.pos - snowmen[i].pos);
		}
	}

	if (length(step) != 0) {
		step = normalize(step);
	}

	if (length(s.pos + step * (dt * SNOWMAN_SPEED)) > 50) {
		step = vec2(0);
	}

	return Snowman{ s.pos + step*(dt*SNOWMAN_SPEED), s.hp };
}