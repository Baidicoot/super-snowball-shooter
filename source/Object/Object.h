#pragma once

#include "Mesh.h"
#include "Light.h"
#include "Collision.h"

#include <glm/glm.hpp>
#include <vector>

struct Object {
	Mesh mesh;
	Light light;

	void append(Object o);
};

Object join(Object a, Object b);

Object emptyScene();

Object fromMesh(Mesh a);
Object fromLight(Light a);
Object fromBoth(Mesh a, Light b);

Object translate(Object a, glm::vec3 t);
Object scale(Object a, glm::vec4 s);

bool objectsCollide(Object a, Object b);