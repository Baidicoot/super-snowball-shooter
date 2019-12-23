#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../Common/loadShaders.h"
#include "Renderer.h"
#include "../Object/Mesh.h"
#include "../UI/UI.h"
#include "../Object/Light.h"
#include "../Object/Object.h"

#include <vector>

#define OBJS 256
#define UIS 64
#define RWIDTH 512
#define RHEIGHT 256
#define DWIDTH 1366
#define DHEIGHT 705
#define SWIDTH 512
#define SHEIGHT 256

struct Renderer {
	GLuint program;
	GLuint depthTracer;
	GLuint quad;
	GLuint tex;
	GLuint shadowCaster;
	GLuint stex;
	glm::vec3 pos;
	glm::vec3 dir;
	GLFWwindow* window;

	bool init(const char* frag, const char* vert, const char* comp, const char* shadow);

	bool close();

	void renderObjects(Object scene, UI ui);
};