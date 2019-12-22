#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Droppable.h"

#define SNOWBALL_R 0.05
#define FLARE_R 0.2

Object snowballMesh();
Droppable spawnSnowball(glm::vec3 pos, glm::vec3 vel);

Object flareMesh();
Droppable spawnFlare(glm::vec3 pos, glm::vec3 vec);

Object flarePickupMesh();
Droppable spawnFlarePickup(glm::vec3 pos, glm::vec3 vec);