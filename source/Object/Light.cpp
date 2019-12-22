#include <glm/glm.hpp>
#include <vector>

#include "Light.h"

using namespace glm;
using namespace std;

Light join(Light a, Light b) {
	vector<vec3> lightBuffer(a.lights);
	vector<vec4> colorBuffer(a.colors);

	lightBuffer.insert(lightBuffer.end(), b.lights.begin(), b.lights.end());
	colorBuffer.insert(colorBuffer.end(), b.colors.begin(), b.colors.end());

	return Light{ lightBuffer, colorBuffer };
}

Light singleLight(vec3 light, vec4 color) {
	vec3 lightArray[] = { light };
	vector<vec3> lightVec(lightArray, lightArray + 1);

	vec4 colorArray[] = { color };
	vector<vec4> colorVec(colorArray, colorArray + 1);

	return Light{ lightVec, colorVec };
}

Light lightEmpty() {
	vector<vec3> el;
	vector<vec4> ec;

	return Light{ el, ec };
}

Light scale(Light a, vec3 s) {
	vector<vec4> colorCopy(a.colors);
	vector<vec3> positions(a.lights);

	for (int i = 0; i < positions.size(); i++) {
		positions[i] = positions[i] * s;
	}

	return Light{ positions, colorCopy };
}

Light translate(Light a, vec3 t) {
	vector<vec4> colorCopy(a.colors);
	vector<vec3> positions(a.lights);

	for (int i = 0; i < positions.size(); i++) {
		positions[i] = positions[i] + t;
	}

	return Light{ positions, colorCopy };
}

void Light::append(Light o) {
	lights.insert(lights.end(), o.lights.begin(), o.lights.end());
	colors.insert(colors.end(), o.colors.begin(), o.colors.end());
}