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
#include "UI/Letters.h"

#include "Control/Game.h"

UI makeGameOverLetter(char letter) {
	UI l = makeLetter(letter, vec4(1, 0, 0, 1), 0.2);
	return scale(l, vec2(0.05, 0.1));
}

UI makePressStartLetter(char letter) {
	UI l = makeLetter(letter, vec4(1), 0.2);
	return scale(l, vec2(0.05, 0.1));
}

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

	float last = glfwGetTime();

	bool died = false;
	bool waiting = true;

	int kills = 0;

	do {
		float time = glfwGetTime();
		if (waiting) {
			vector<UI> press;
			press.push_back(makePressStartLetter('P'));
			press.push_back(makePressStartLetter('R'));
			press.push_back(makePressStartLetter('E'));
			press.push_back(makePressStartLetter('S'));
			press.push_back(makePressStartLetter('S'));
			press.push_back(makePressStartLetter(' '));
			press.push_back(makePressStartLetter('S'));
			press.push_back(makePressStartLetter('P'));
			press.push_back(makePressStartLetter('A'));
			press.push_back(makePressStartLetter('C'));
			press.push_back(makePressStartLetter('E'));

			UI ui = uiEmpty();

			for (int i = 0; i < press.size(); i++) {
				float xdisp = (i + 0.5) / press.size() * 1.5 - 0.75;
				ui = join(ui, translate(press[i], vec2(xdisp, 0)));
			}

			camera.renderObjects(makeScene(game), ui);

			if (glfwGetKey(camera.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				waiting = false;
			}
		}
		else if (!died) {
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

			UI snowballBar = genHPBar(1 - max(game.player.snowballCooldown / 0.5f, 0.0f), vec4(0, 0.5, 1, 1));
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

			died = dead(game);

			if (newState.snowmen.size() < game.snowmen.size()) {
				kills += 1;
			}

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
		}
		else {
			vector<UI> gameOver;
			gameOver.push_back(makeGameOverLetter('G'));
			gameOver.push_back(makeGameOverLetter('A'));
			gameOver.push_back(makeGameOverLetter('M'));
			gameOver.push_back(makeGameOverLetter('E'));
			gameOver.push_back(makeGameOverLetter(' '));
			gameOver.push_back(makeGameOverLetter('O'));
			gameOver.push_back(makeGameOverLetter('V'));
			gameOver.push_back(makeGameOverLetter('E'));
			gameOver.push_back(makeGameOverLetter('R'));

			vector<UI> score;
			score.push_back(makeDigit((kills / 100) % 10, vec4(1, 0, 0, 1), 0.2));
			score.push_back(makeDigit((kills / 10) % 10, vec4(1, 0, 0, 1), 0.2));
			score.push_back(makeDigit(kills % 10, vec4(1, 0, 0, 1), 0.2));

			UI ui = uiEmpty();

			for(int i = 0; i < gameOver.size(); i++) {
				float xdisp = (i+0.5) / gameOver.size() - 0.5;
				ui = join(ui, translate(gameOver[i], vec2(xdisp, 0)));
			}

			for (int i = 0; i < score.size(); i++) {
				float xdisp = (i+0.5) / score.size() / 3 - 0.167;
				ui = join(ui, translate(scale(score[i], vec2(0.05, 0.1)), vec2(xdisp, -0.34)));
			}

			camera.renderObjects(makeScene(game), ui);

			if (glfwGetKey(camera.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				died = false;
				game = emptyGame();
				kills = 0;
			}
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