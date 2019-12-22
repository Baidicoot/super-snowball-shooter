#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

#include "../Object/Object.h"
#include "Terrain.h"
#include "Droppable.h"

bool objectIntersectsScene(Droppable a, Object scene) {
	return objectsCollide(translate(a.initialObject, a.pos), scene);
}

Droppable stepDroppable(Droppable a, Object scene, float dt) {
	vec3 newPos = a.pos + a.vel*dt;
	Droppable newDrop = Droppable{ a.initialObject, newPos, a.vel + vec3(0, -GRAVITY*dt, 0), a.bounceRadius, a.lifetime - dt, a.bounciness };
	if (objectIntersectsScene(newDrop, scene)) {
		float height = getHeight(scene, vec2(newPos.x, newPos.z)) + a.bounceRadius;
		newDrop = Droppable{ a.initialObject, vec3(newPos.x, height, newPos.z), a.vel * a.bounciness * vec3(1, -1, 1), a.bounceRadius, newDrop.lifetime, a.bounciness };
	}

	return newDrop;
}

bool droppableStill(Droppable a) {
	return (length(a.vel) < 0.05);
}

bool killDroppable(Droppable a, Object scene) {
	return (droppableStill(a) && objectIntersectsScene(a, scene)) || (a.lifetime <= 0);
}

bool despawn(Droppable a) {
	return a.lifetime <= 0;
}

Object genDroppable(Droppable a) {
	return translate(a.initialObject, a.pos);
}