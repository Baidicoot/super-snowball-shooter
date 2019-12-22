#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../Object/Object.h"

#define GRAVITY 1.0

struct Droppable {
	Object initialObject;
	glm::vec3 pos;
	glm::vec3 vel;
	float bounceRadius;
	float lifetime;
	float bounciness;
};

bool objectIntersectsScene(Droppable a, Object scene);

Droppable stepDroppable(Droppable a, Object scene, float dt);

Object genDroppable(Droppable a);

bool droppableStill(Droppable a);
bool despawn(Droppable a);
bool killDroppable(Droppable a, Object scene);