#pragma once

#include "UI.h"
#include <glm/glm.hpp>
#include <vector>

UI genCrosshairs(glm::vec2 thickness, glm::vec4 color);

UI genHPBar(float hp, glm::vec4 color);