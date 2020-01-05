#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "Mesh.h"

Mesh join(Mesh a, Mesh b) {
	vector<vec4> sphereBuffer(a.spheres);
	vector<vec3> colorBuffer(a.colors);

	sphereBuffer.insert(sphereBuffer.end(), b.spheres.begin(), b.spheres.end());
	colorBuffer.insert(colorBuffer.end(), b.colors.begin(), b.colors.end());

	return Mesh{ sphereBuffer, colorBuffer };
}

Mesh single(vec4 sphere, vec3 color) {
	vec4 sphereArray[] = { sphere };
	vector<vec4> sphereVec(sphereArray, sphereArray + 1);

	vec3 colorArray[] = { color };
	vector<vec3> colorVec(colorArray, colorArray + 1);

	return Mesh{ sphereVec, colorVec };
}

Mesh meshEmpty() {
	vector<vec4> es;
	vector<vec3> ec;
	return Mesh{es, ec};
}

Mesh scale(Mesh a, vec4 s) {
	vector<vec4> positions(a.spheres);
	vector<vec3> colorCopy(a.colors);

	for (int i = 0; i < positions.size(); i++) {
		positions[i] = positions[i] * s;
	}

	return Mesh{ positions, colorCopy };
}

Mesh translate(Mesh a, vec3 t) {
	vector<vec3> colorCopy(a.colors);
	vector<vec4> positions(a.spheres);

	for (int i = 0; i < positions.size(); i++) {
		positions[i] = positions[i] + vec4(t, 0);
	}

	return Mesh{ positions, colorCopy };
}

void Mesh::append(Mesh o) {
	spheres.insert(spheres.end(), o.spheres.begin(), o.spheres.end());
	colors.insert(colors.end(), o.colors.begin(), o.colors.end());
}

Mesh decimate(Mesh a, vec3 pos, float maxDist) {
	Mesh b;
	for (int i = 0; i < a.spheres.size(); i++) {
		float sDist = distance(vec3(a.spheres[i].x, a.spheres[i].y, a.spheres[i].z), pos) - a.spheres[i].w;
		if (sDist < maxDist && a.spheres[i].w > 0) {
			b.spheres.push_back(a.spheres[i]);
			b.colors.push_back(a.colors[i]);
		}
	}
	return b;
}