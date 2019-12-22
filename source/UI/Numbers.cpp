#include <vector>
#include <glm/glm.hpp>

#include "Numbers.h"
#include "UI.h"

using namespace std;
using namespace glm;

UI makeSegments(bool seg[7], vec4 color, float thickness) {
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
		segments.push_back(vec4(1-thickness, -1, 1, 0));
		colors.push_back(color);
	}
	if (seg[3]) {
		segments.push_back(vec4(1-thickness, 0, 1, 1));
		colors.push_back(color);
	}
	if (seg[4]) {
		segments.push_back(vec4(-1, -1, 1, -1 + thickness));
		colors.push_back(color);
	}
	if (seg[5]) {
		segments.push_back(vec4(-1, -thickness/2, 1, thickness/2));
		colors.push_back(color);
	}
	if (seg[6]) {
		segments.push_back(vec4(-1, 1-thickness, 1, 1));
		colors.push_back(color);
	}

	return UI{ segments, colors };
}

UI makeDigit(int digit, vec4 color, float thickness) {
	bool segments[7] = { false, false, false, false, false, false, false };

	if (digit == 0) {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[4] = true;
		segments[6] = true;
	}
	else if (digit == 1) {
		segments[2] = true;
		segments[3] = true;
	}
	else if (digit == 2) {
		segments[0] = true;
		segments[3] = true;
		segments[4] = true;
		segments[5] = true;
		segments[6] = true;
	}
	else if (digit == 3) {
		segments[2] = true;
		segments[3] = true;
		segments[4] = true;
		segments[5] = true;
		segments[6] = true;
	}
	else if (digit == 4) {
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[5] = true;
	}
	else if (digit == 5) {
		segments[1] = true;
		segments[2] = true;
		segments[4] = true;
		segments[5] = true;
		segments[6] = true;
	}
	else if (digit == 6) {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[4] = true;
		segments[5] = true;
		segments[6] = true;
	}
	else if (digit == 7) {
		segments[2] = true;
		segments[3] = true;
		segments[6] = true;
	}
	else if (digit == 8) {
		segments[0] = true;
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[4] = true;
		segments[5] = true;
		segments[6] = true;
	}
	else if (digit == 9) {
		segments[1] = true;
		segments[2] = true;
		segments[3] = true;
		segments[4] = true;
		segments[5] = true;
		segments[6] = true;
	}

	return makeSegments(segments, color, thickness);
}