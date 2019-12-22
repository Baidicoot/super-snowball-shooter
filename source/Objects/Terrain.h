#pragma once

#include <glm/glm.hpp>
#include "../Object/Mesh.h"
#include "../Object/Object.h"

Object genMap();

float getHeight(Object a, glm::vec2 xy);