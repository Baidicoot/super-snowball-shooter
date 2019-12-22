#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "UI.h"
#include "Numbers.h"

UI makeSegments(bool seg[7], glm::vec4 color, float thickness);

UI makeDigit(int digit, glm::vec4 color, float thickness);