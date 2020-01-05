#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
using namespace std;

#include "../Common/loadShaders.h"
#include "Renderer.h"
#include "../Object/Mesh.h"
#include "../UI/UI.h"
#include "../Object/Light.h"
#include "../Object/Object.h"

const GLfloat vertex_buffer_data[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
};

GLuint genQuad() {
	GLuint quadID;
	glGenVertexArrays(1, &quadID);
	glBindVertexArray(quadID);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	return buffer;
}

GLuint genTex(GLuint w, GLuint h) {
	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

void Renderer::renderObjects(Object scene, UI ui) {

	//printf("%f %f %f %f", objs[0], objs[1], objs[2], objs[3]);

	auto tmp = decimate(scene.mesh, pos, 10);
	vector<vec4> objs = tmp.spheres;
	vector<vec3> colors = tmp.colors;

	auto light = scene.light;

	float t = glfwGetTime();

	glUseProgram(depthTracer);
	glUniform1f(glGetUniformLocation(depthTracer, "time"), t);
	glUniform2iv(glGetUniformLocation(depthTracer, "rdims"), 1, &ivec2(RWIDTH, RHEIGHT)[0]);
	glUniform3fv(glGetUniformLocation(depthTracer, "pos"), 1, &pos[0]);
	glUniform3fv(glGetUniformLocation(depthTracer, "dir"), 1, &dir[0]);
	glUniform4fv(glGetUniformLocation(depthTracer, "objs"), objs.size(), &objs[0][0]);
	glUniform1i(glGetUniformLocation(depthTracer, "nobj"), objs.size());

	glDispatchCompute(RWIDTH, RHEIGHT, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glUseProgram(shadowCaster);
	glUniform2iv(glGetUniformLocation(depthTracer, "rdims"), 1, &ivec2(RWIDTH, RHEIGHT)[0]);
	glUniform2iv(glGetUniformLocation(shadowCaster, "sdims"), 1, &ivec2(SWIDTH, SHEIGHT)[0]);
	glUniform3fv(glGetUniformLocation(shadowCaster, "pos"), 1, &pos[0]);
	glUniform4fv(glGetUniformLocation(shadowCaster, "objs"), objs.size(), &objs[0][0]);
	glUniform1i(glGetUniformLocation(shadowCaster, "nobj"), objs.size());
	glUniform3fv(glGetUniformLocation(shadowCaster, "lights"), light.lights.size(), &light.lights[0][0]);
	glUniform4fv(glGetUniformLocation(shadowCaster, "lcols"), light.lights.size(), &light.colors[0][0]);
	glUniform1i(glGetUniformLocation(shadowCaster, "nlight"), light.lights.size());

	glDispatchCompute(SWIDTH, SHEIGHT, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	glUniform2iv(glGetUniformLocation(program, "rdims"), 1, &ivec2(RWIDTH, RHEIGHT)[0]);
	glUniform2iv(glGetUniformLocation(program, "sdims"), 1, &ivec2(SWIDTH, SHEIGHT)[0]);
	glUniform2iv(glGetUniformLocation(program, "ddims"), 1, &ivec2(DWIDTH, DHEIGHT)[0]);
	glUniform3fv(glGetUniformLocation(program, "colors"), colors.size(), &colors[0][0]);
	glUniform4fv(glGetUniformLocation(program, "rects"), ui.rects.size(), &ui.rects[0][0]);
	glUniform4fv(glGetUniformLocation(program, "uicols"), ui.rects.size(), &ui.colors[0][0]);
	glUniform1i(glGetUniformLocation(program, "nui"), ui.rects.size());
	glUniform3fv(glGetUniformLocation(program, "pos"), 1, &pos[0]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
};

bool Renderer::close() {
	glDisableVertexAttribArray(0);

	return true;
}

bool Renderer::init(const char* vert, const char* frag, const char* comp, const char* shadow) {
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int count;
	window = glfwCreateWindow(DWIDTH, DHEIGHT, "ART", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	program = LoadShaders(vert, frag);
	depthTracer = LoadCompute(comp);
	shadowCaster = LoadCompute(shadow);
	quad = genQuad();
	tex = genTex(RWIDTH, RHEIGHT);
	stex = genTex(SWIDTH, SHEIGHT);
	pos = vec3(0, 1, -5);
	dir = vec3(0);

	glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glBindImageTexture(1, stex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, stex);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	return true;
};