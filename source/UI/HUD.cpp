#include "UI.h"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

UI genCrosshairs(vec2 thickness, vec4 color) {
	vec4 elems[] = {
		vec4(-1, -thickness.x / 2, 1, thickness.x / 2),
		vec4(-thickness.y / 2, -1, thickness.y / 2, 1)
	};
	vector<vec4> elemVec(elems, elems + 2);

	vec4 colors[] = {
		color, color
	};
	vector<vec4> colorVec(colors, colors + 2);

	return UI{ elemVec, colorVec };
}

UI genHPBar(float hp, vec4 color) {
	vector<vec4> elems;
	vector<vec4> colors;

	elems.push_back(vec4(-1, -1, hp * 2 - 1, 1));
	colors.push_back(color);

	return UI{ elems, colors };
}