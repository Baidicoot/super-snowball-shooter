#include "Game.h"
#include <random>
#include <iostream>

using namespace std;
using namespace glm;

Object makeScene(GameState g) {
	vec2 pos = g.player.pos + getFacing(g.player, vec2(0, 3));
	float height = getHeight(g.terrain, pos) + 5;

	Object joined = fromLight(singleLight(vec3(pos.x, height, pos.y), vec4(1, 1, 1, 50)));
	joined.append(g.terrain);

	for (int i = 0; i < g.snowballs.size(); i++) {
		joined.append(genDroppable(g.snowballs[i]));
	}

	for (int i = 0; i < g.flares.size(); i++) {
		joined.append(genDroppable(g.flares[i]));
	}

	for (int i = 0; i < g.flarePickups.size(); i++) {
		joined.append(genDroppable(g.flarePickups[i]));
	}

	for (int i = 0; i < g.snowmen.size(); i++) {
		joined.append(genSnowman(g.snowmen[i], g.terrain, g.player.pos));
	}

	return joined;
}

GameState doStep(GameState g, float dt) {
	vector<Droppable> newSnowballs;

	for (int i = 0; i < g.snowballs.size(); i++) {
		Droppable newBall = stepDroppable(g.snowballs[i], g.terrain, dt);
		if (!killDroppable(newBall, g.terrain)) {
		//if (!despawn(newBall)) {
			newSnowballs.push_back(newBall);
		}
	}

	vector<Droppable> newFlarePickups;

	for (int i = 0; i < g.flarePickups.size(); i++) {
		Droppable newFlare = stepDroppable(g.flarePickups[i], g.terrain, dt);
		if (distance(vec2(newFlare.pos.x, newFlare.pos.z), g.player.pos) < 1 && g.player.nflares < 9) {
			g.player.nflares += 1;
		}
		else if (!despawn(newFlare)) {
			newFlarePickups.push_back(newFlare);
		}
	}

	vector<Snowman> newSnowmen;

	for (int i = 0; i < g.snowmen.size(); i++) {
		Snowman newMan = stepSnowman(g.snowmen, i, g.player.pos, dt);
		if (newMan.hp > 0) {
			newSnowmen.push_back(newMan);
		}
		else if (float(rand()) / RAND_MAX > 0.9 && newFlarePickups.size() < MAX_FLARES) {
			float height = getHeight(g.terrain, newMan.pos) + 0.5;
			newFlarePickups.push_back(spawnFlarePickup(vec3(newMan.pos.x, height, newMan.pos.y), vec3(0)));
		}
	}

	for (int b = 0; b < newSnowballs.size(); b++) {
		for (int m = 0; m < newSnowmen.size(); m++) {
			if (objectIntersectsScene(newSnowballs[b], genSnowman(newSnowmen[m], g.terrain, g.player.pos))) {
				newSnowballs.erase(newSnowballs.begin() + b);
				b--;
				newSnowmen[m].hp -= 0.34;
				break;
			}
		}
	}

	vector<Droppable> newFlares;

	for (int i = 0; i < g.flares.size(); i++) {
		Droppable newBall = stepDroppable(g.flares[i], g.terrain, dt);
		if (!despawn(newBall)) {
			newFlares.push_back(newBall);
		}
	}

	float tilSnowman = g.untilSnowman - dt;
	float tilFlare = g.untilFlare - dt;

	if (tilSnowman < 0 && newSnowmen.size() < MAX_SNOWMEN) {
		vec2 randPos = normalize(vec2(rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2))) * 25.0f - vec2(12.5);

		newSnowmen.push_back(Snowman{ randPos, 1 });
		tilSnowman = 5;
	}

	if (tilFlare < 0 && newFlarePickups.size() < MAX_FLARES * 0.75) {
		vec2 randPos = normalize(vec2(rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2))) * 25.0f - vec2(12.5);
		float height = getHeight(g.terrain, randPos) + 0.5;

		newFlarePickups.push_back(spawnFlarePickup(vec3(randPos.x, height, randPos.y), vec3(0)));
		tilFlare = 5;
	}

	Player newPlayer = stepPlayer(g.player, g.snowmen, dt);

	return GameState{ newSnowmen, newSnowballs, newFlares, newFlarePickups, g.terrain, newPlayer, tilSnowman, tilFlare };
}

GameState throwSnowball(GameState g) {
	vector<Droppable> newSnowballs(g.snowballs);
	if (canSnowball(g.player)) {
		newSnowballs.push_back(snowball(g.player, g.terrain));
		g.player.snowballCooldown = 0.5;
	}

	return GameState{ g.snowmen, newSnowballs, g.flares, g.flarePickups, g.terrain, g.player, g.untilSnowman, g.untilFlare };
}

GameState throwFlare(GameState g) {
	vector<Droppable> newFlares(g.flares);
	if (canFlare(g.player)) {
		newFlares.push_back(flare(g.player, g.terrain));
		g.player.flareCooldown = 5;
		g.player.nflares -= 1;
	}

	return GameState{ g.snowmen, g.snowballs, newFlares, g.flarePickups, g.terrain, g.player, g.untilSnowman, g.untilFlare };
}

bool dead(GameState g) {
	return g.player.hp <= 0;
}

GameState emptyGame() {
	vector<Snowman> snowmen;
	vector<Droppable> snowballs;
	vector<Droppable> flares;
	vector<Droppable> flarePickups;
	return GameState{ snowmen, snowballs, flares, flarePickups, genMap(), spawnPlayer(vec2(0)), 5, 0 };
}