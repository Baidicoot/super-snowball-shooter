#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
using namespace std;

#include "Common/loadShaders.h"
#include "Rendering/Renderer.h"
#include "Common/Rot.h"
#include "Object/Mesh.h"
#include "Objects/Snowman.h"
#include "Objects/Terrain.h"
#include "Objects/Snowball.h"
#include "UI/UI.h"
#include "UI/HUD.h"
#include "UI/Numbers.h"
#include "Object/Light.h"
#include "Object/Object.h"
#include "Control/Player.h"

#include "Control/Game.h"

int main(void)
{
	/*
	MAX WORK GROUP TOTAL: 65536x65536x65536
	MAX LOCAL GROUP SIZE: 1024x1024x64
	MAX INVOCATIONS SIZE: 1024
	*/

	Renderer camera;
	if (!camera.init("source/Shaders/vert.glsl", "source/Shaders/frag.glsl", "source/Shaders/depth.glsl", "source/Shaders/shadow.glsl")) {
		return false;
	}

	float dt = 1.0 / 30;

	const float turnRate = 2.0;

	GameState game = emptyGame();
	game.player.pos = vec2(20, -25);

	float last = glfwGetTime();

	do {
		float time = glfwGetTime();

		UI ui = makeDigit(game.player.nflares, vec4(1), 0.25);
		ui = scale(ui, vec2(0.0528, 0.1024));
		ui = translate(ui, vec2(-0.85, 0.75));

		camera.pos = getCameraPos(game.player, game.terrain);
		camera.dir = getCameraDir(game.player);

		UI crosshair = genCrosshairs(vec2(0.1), vec4(0, 1, 0, 0.5));
		crosshair = scale(crosshair, vec2(0.0528, 0.1024));

		UI hpBar = genHPBar(game.player.hp, vec4(1, 0, 0, 1));
		hpBar = scale(hpBar, vec2(0.2, 0.05));
		hpBar = translate(hpBar, vec2(0.775, 0.9));

		UI snowballBar = genHPBar(1-max(game.player.snowballCooldown / 0.5f, 0.0f), vec4(0, 0.5, 1, 1));
		snowballBar = scale(snowballBar, vec2(0.2, 0.05));
		snowballBar = translate(snowballBar, vec2(0.775, 0.75));

		hpBar = join(snowballBar, hpBar);
		hpBar = join(single(vec4(0.5625, 0.675, 0.9875, 0.975), vec4(0.5)), hpBar);

		ui = join(ui, crosshair);
		ui = join(ui, hpBar);

		GameState newState = doStep(game, dt);
		float dhp = game.player.hp - newState.player.hp;

		UI blueScreen = single(vec4(-1, -1, 1, 1), vec4(0, 0, 1, dhp / dt));
		ui = join(blueScreen, ui);

		game = newState;

		camera.renderObjects(makeScene(game), ui);

		if (glfwGetKey(camera.window, GLFW_KEY_UP) == GLFW_PRESS) {
			game.player = advance(game.player, vec2(0, 5), dt);
		}
		if (glfwGetKey(camera.window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			game.player = advance(game.player, vec2(0, -5), dt);
		}
		if (glfwGetKey(camera.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			game.player = advance(game.player, vec2(5, 0), dt);
		}
		if (glfwGetKey(camera.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			game.player = advance(game.player, vec2(-5, 0), dt);
		}
		if (glfwGetKey(camera.window, GLFW_KEY_A) == GLFW_PRESS) {
			game.player.facing -= turnRate * dt;
		}
		if (glfwGetKey(camera.window, GLFW_KEY_D) == GLFW_PRESS) {
			game.player.facing += turnRate * dt;
		}
		if (glfwGetKey(camera.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			game = throwSnowball(game);
		}
		if (glfwGetKey(camera.window, GLFW_KEY_F) == GLFW_PRESS) {
			game = throwFlare(game);
		}

		glfwSwapBuffers(camera.window);
		glfwPollEvents();

		float curr = glfwGetTime();

		if (curr - last >= 1) {
			cout << dt << "s per frame (" << 1.0 / dt << " fps)" << endl;
			last = curr;
		}

		dt = curr - time;
	} while (glfwGetKey(camera.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(camera.window) == 0);
	
	glfwTerminate();
	camera.close();

	return 0;
}