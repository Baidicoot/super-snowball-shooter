#include <glm/glm.hpp>
#include <vector>
#include "../Object/Object.h"
#include "Snowball.h"
#include "Droppable.h"
#include "../Object/Collision.h"

using namespace std;
using namespace glm;

Object snowballMesh() {
	return fromMesh(single(vec4(0, 0, 0, SNOWBALL_R), vec3(1)));
}

Droppable spawnSnowball(vec3 pos, vec3 vel) {
	return Droppable{ snowballMesh(), pos, vel, SNOWBALL_R, 10, 0.25 };
}

Object flareMesh() {
	vec4 spheres[] = {
		vec4(0, -0.1, 0, 0.1),
		vec4(0, 0, 0, 0.1),
		vec4(0, 0.1, 0, 0.1)
	};

	vec3 colors[] = {
		vec3(1, 0, 0),
		vec3(1, 0, 0),
		vec3(1, 0.5, 0)
	};

	vector<vec4> sphereVec(spheres, spheres + 3);
	vector<vec3> colorVec(colors, colors + 3);

	return fromBoth(Mesh{ sphereVec, colorVec }, singleLight(vec3(0, 0.5, 0), vec4(1, 0.5, 0, 50)));
}

Droppable spawnFlare(vec3 pos, vec3 vel) {
	return Droppable{ flareMesh(), pos, vel, FLARE_R, 20, 0.5 };
}

Object flarePickupMesh() {
	vec4 spheres[] = {
		vec4(0, -0.1, 0, 0.1),
		vec4(0, 0, 0, 0.1),
		vec4(0, 0.1, 0, 0.1)
	};

	vec3 colors[] = {
		vec3(1, 0, 0),
		vec3(1, 0, 0),
		vec3(1, 0.5, 0)
	};

	vector<vec4> sphereVec(spheres, spheres + 3);
	vector<vec3> colorVec(colors, colors + 3);

	return fromBoth(Mesh{ sphereVec, colorVec }, singleLight(vec3(0, 0.25, 0), vec4(0, 1, 0, 2)));
}

Droppable spawnFlarePickup(vec3 pos, vec3 vel) {
	return Droppable{ flarePickupMesh(), pos, vel, FLARE_R, 15, 1 };
}