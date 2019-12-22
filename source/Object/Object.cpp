#include "Mesh.h"
#include "Light.h"
#include "Collision.h"
#include "Object.h"

#include <glm/glm.hpp>
#include <vector>
using namespace std;
using namespace glm;

Object join(Object a, Object b) {
	return Object{ join(a.mesh, b.mesh), join(a.light, b.light) };
}

Object emptyScene() {
	return Object{ meshEmpty(), lightEmpty() };
}

Object fromMesh(Mesh a) {
	return Object{ a, lightEmpty() };
}

Object fromLight(Light a) {
	return Object{ meshEmpty(), a };
}

Object fromBoth(Mesh a, Light b) {
	return Object{ a, b };
}

Object translate(Object a, glm::vec3 t) {
	return Object{ translate(a.mesh, t), translate(a.light, t) };
}

Object scale(Object a, glm::vec4 s) {
	return Object{ scale(a.mesh, s), scale(a.light, vec3(s.x, s.y, s.z)) };
}

bool objectsCollide(Object a, Object b) {
	return meshesCollide(a.mesh, b.mesh);
}

void Object::append(Object o) {
	light.append(o.light);
	mesh.append(o.mesh);
}