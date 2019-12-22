#include "Collision.h"

#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"

using namespace glm;
using namespace std;

bool checkCollision(vec4 s, Mesh m) {
	for (int i = 0; i < m.spheres.size(); i++) {
		if (distance(vec3(s.x, s.y, s.z), vec3(m.spheres[i].x, m.spheres[i].y, m.spheres[i].z)) <= s.w + m.spheres[i].w) {
			return true;
		}
	}

	return false;
}

bool meshesCollide(Mesh a, Mesh b) {
	for (int i = 0; i < a.spheres.size(); i++) {
		if (checkCollision(a.spheres[i], b)) return true;
	}

	return false;
}