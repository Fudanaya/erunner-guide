#include "Setting.h"
#include "Game.h"

#include "VRunHub.h"

int main(int argc, char** argv) {
	Engine::Setting* setting = new Engine::Setting();
	setting->screenWidth = 1152;
	setting->screenHeight = 648;
	setting->windowFlag = Engine::WindowFlag::FULLSCREEN; //Game is developed for Fullscreen with 1366 x 768 size
	setting->vsync = true;
	setting->targetFrameRate = 75;
	Engine::Game* game = new Engine::VRunHub(setting); //VRunHub.h
	game->Run();
	delete setting;
	delete game;

	return 0;
}
