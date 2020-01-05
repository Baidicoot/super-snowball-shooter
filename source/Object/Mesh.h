#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Mesh {
	std::vector<glm::vec4> spheres;
	std::vector<glm::vec3> colors;

	void append(Mesh o);
};

Mesh decimate(Mesh a, glm::vec3 pos, float maxDist);

Mesh join(Mesh a, Mesh b);

Mesh single(glm::vec4 sphere, glm::vec3 color);

Mesh meshEmpty();

Mesh scale(Mesh a, glm::vec4 s);

Mesh translate(Mesh a, glm::vec3 t);