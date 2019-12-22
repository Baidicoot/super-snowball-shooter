#include <glm/glm.hpp>
#include <vector>

#include "UI.h"
#include "Letters.h"

using namespace std;
using namespace glm;

UI makeLetterSeg(bool seg[9], vec4 color, float thickness) {
	vector<vec4> segments;
	vector<vec4> colors;

	if (seg[0]) {
		segments.push_back(vec4(-1, -1, -1 + thickness, 0));
		colors.push_back(color);
	}
	if (seg[1]) {
		segments.push_back(vec4(-1, 0, -1 + thickness, 1));
		colors.push_back(color);
	}
	if (seg[2]) {
		segments.push_back(vec4(1 - thickness, -1, 1, 0));
		colors.push_back(color);
	}
	if (seg[3]) {
		segments.push_back(vec4(1 - thickness, 0, 1, 1));
		colors.push_back(color);
	}
	if (seg[4]) {
		segments.push_back(vec4(-1, -1, 1, -1 + thickness));
		colors.push_back(color);
	}
	if (seg[5]) {
		segments.push_back(vec4(-1, -thickness / 2, 1, thickness / 2));
		colors.push_back(color);
	}
	if (seg[6]) {
		segments.push_back(vec4(-1, 1 - thickness, 1, 1));
		colors.push_back(color);
	}
	if (seg[7]) {
		segments.push_back(vec4(-thickness / 2, -1, thickness / 2, 0));
		colors.push_back(color);
	}
	if (seg[8]) {
		segments.push_back(vec4(-thickness / 2, 0, thickness / 2, 1));
		colors.push_back(color);
	}

	return UI{ segments, colors };
}

UI makeLetter(char letter, vec4 color, float thickness) {
	bool segments[9] = {
		false, false, false,
		false, false, false,
		false, false, false
	};

	if (letter == 'A') {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[5] = true;
		segments[6] = true;
	}
	if (letter == 'E') {
		segments[0] = true;
		segments[1] = true;
		segments[4] = true;
		segments[5] = true;
		segments[6] = true;
	}
	if (letter == 'G') {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[4] = true;
		segments[6] = true;
	}
	if (letter == 'M') {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[6] = true;
		segments[7] = true;
		segments[8] = true;
	}
	if (letter == 'O') {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[4] = true;
		segments[6] = true;
	}
	if (letter == 'R') {
		segments[0] = true;
		segments[1] = true;
		segments[3] = true;
		segments[5] = true;
		segments[6] = true;
		segments[7] = true;
	}
	if (letter == 'V') {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[4] = true;
	}

	return makeLetterSeg(segments, color, thickness);
}