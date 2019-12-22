#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../Objects/Droppable.h"
#include "../Objects/Snowman.h"
#include "../Objects/Snowball.h"
#include "../Objects/Terrain.h"

#include "../Object/Object.h"

#include "Player.h"

#define MAX_SNOWMEN 24
#define MAX_FLARES 18

struct GameState {
	std::vector<Snowman> snowmen;
	std::vector<Droppable> snowballs;
	std::vector<Droppable> flares;
	std::vector<Droppable> flarePickups;
	Object terrain;
	Player player;
	float untilSnowman;
	float untilFlare;
};

Object makeScene(GameState g);

GameState doStep(GameState g, float dt);

GameState emptyGame();

bool dead(GameState g);

GameState throwSnowball(GameState g);
GameState throwFlare(GameState g);