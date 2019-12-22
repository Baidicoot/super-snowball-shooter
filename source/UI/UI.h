#pragma once

#include <vector>
#include <glm/glm.hpp>

struct UI {
	std::vector<glm::vec4> rects;
	std::vector<glm::vec4> colors;
};

UI join(UI a, UI b);

UI single(glm::vec4 rect, glm::vec4 color);

UI translate(UI a, glm::vec2 uv);

UI scale(UI a, glm::vec2 s);

UI uiEmpty();