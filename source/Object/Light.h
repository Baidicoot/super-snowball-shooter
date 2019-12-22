#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Light {
	std::vector<glm::vec3> lights;
	std::vector<glm::vec4> colors;

	void append(Light o);
};

Light lightEmpty();

Light join(Light a, Light b);

Light singleLight(glm::vec3 light, glm::vec4 color);

Light scale(Light a, glm::vec3 s);

Light translate(Light a, glm::vec3 t);