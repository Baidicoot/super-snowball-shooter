#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "UI.h"
#include "Letters.h"

UI makeLetterSeg(bool seg[9], glm::vec4 color, float thickness);

UI makeLetter(char letter, glm::vec4 color, float thickness);