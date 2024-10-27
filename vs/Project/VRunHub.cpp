#include "VRunHub.h"

Engine::VRunHub::VRunHub(Setting* setting) :Engine::Game(setting) {
	setting->windowTitle = "VelociRun";
}

Engine::VRunHub::~VRunHub(){}

void Engine::VRunHub::Init() {
	Engine::ScreenManager::GetInstance(this)->AddScreen("mainmenu", new VRunMenu())->AddScreen("ingame", new VelociRun())->/*AddScreen("settings", new VRunSettings())->*/SetCurrentScreen("mainmenu");
}

void Engine::VRunHub::Update() {
	Engine::ScreenManager::GetInstance(this)->Update();
}

void Engine::VRunHub::Render() {
	Engine::ScreenManager::GetInstance(this)->Draw();
}