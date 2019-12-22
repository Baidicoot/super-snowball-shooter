#include <glm/glm.hpp>
#include "../Object/Mesh.h"
#include "../Object/Object.h"
#include <vector>

using namespace std;
using namespace glm;

#include "../Common/Ray.h"
#include "Terrain.h"

#define CAST_DIST 100

Object genMap() {
	vec4 hills[] = {
		vec4(0, -1000, 0, 1000),
		vec4(10, -7.5, 10, 10),
		vec4(20, -7.5, -25, 10),
		vec4(30, -7.5, -30, 10)
	};
	int sn = sizeof(hills) / sizeof(hills[0]);

	vec3 colmap[] = {
		vec3(1), vec3(1), vec3(1), vec3(1)
	};
	int cn = sizeof(colmap) / sizeof(colmap[0]);

	vector<vec4> objs(hills, hills + sn);
	vector<vec3> colors(colmap, colmap + cn);

	return fromMesh(Mesh{ objs, colors });
}

float getHeight(Object a, vec2 xz) {
	vector<Sphere> terrain;

	for (int i = 0; i < a.mesh.spheres.size(); i++) {
		terrain.push_back(fromVector(a.mesh.spheres[i]));
	}

	Intersection i = castRay(Ray{ vec3(xz.x, CAST_DIST, xz.y), vec3(0, -1, 0) }, terrain);
	return CAST_DIST - i.t;
}