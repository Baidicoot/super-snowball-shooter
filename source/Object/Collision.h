#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"

bool checkCollision(glm::vec4 s, Mesh m);

bool meshesCollide(Mesh a, Mesh b);