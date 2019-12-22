#include <glm/glm.hpp>
#include <vector>

#include "UI.h"

using namespace glm;
using namespace std;

UI join(UI a, UI b) {
	vector<vec4> rectBuffer(a.rects);
	vector<vec4> colorBuffer(a.colors);

	rectBuffer.insert(rectBuffer.end(), b.rects.begin(), b.rects.end());
	colorBuffer.insert(colorBuffer.end(), b.colors.begin(), b.colors.end());

	return UI{ rectBuffer, colorBuffer };
}

UI translate(UI a, vec2 uv) {
	vector<vec4> copyColors(a.colors);
	vector<vec4> rects(a.rects);

	for (int i = 0; i < rects.size(); i++) {
		rects[i] = rects[i] + vec4(uv, uv);
	}

	return UI{ rects, copyColors };
}

UI scale(UI a, glm::vec2 s) {
	vector<vec4> copyColors(a.colors);
	vector<vec4> rects(a.rects);

	for (int i = 0; i < rects.size(); i++) {
		rects[i] = rects[i] * vec4(s, s);
	}

	return UI{ rects, copyColors };
}

UI single(vec4 rect, vec4 color) {
	vec4 rectArray[] = { rect };
	vector<vec4> rectVec(rectArray, rectArray + 1);

	vec4 colorArray[] = { color };
	vector<vec4> colorVec(colorArray, colorArray + 1);

	return UI{ rectVec, colorVec };
}

UI uiEmpty() {
	vector<vec4> er;
	vector<vec4> ec;

	return UI{ er, ec };
}