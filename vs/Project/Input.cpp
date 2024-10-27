#include "Input.h"

using namespace Engine;

bool Input::IsKeyPressed(string name)
{
	auto it = pressedKeyMap.find(name);
	if (it != pressedKeyMap.end()) {
		// Found the key
		return it->second;
	}
	// Didn't find the key
	return false;
}

bool Input::IsKeyReleased(string name)
{
	for (auto& val : releasedKeys) // access by reference to avoid copying
	{
		if (val == name)
		{
			return true;
		}
	}
	// Didn't find
	return false;
}

vec2 Input::GetMouseCoords()
{
	return this->mouseCoords;
}

Engine::Input* Input::AddInputMapping(string mapName, unsigned int keyId)
{
	keyCodeNameMap[keyId].insert(mapName);
	return this;
}

//Engine::Input* Input::RemoveInputMapping(string mapName)
//{
//	// Find all occurrences of the key in keyCodeNameMap and remove them
//	for (auto it = keyCodeNameMap.begin(); it != keyCodeNameMap.end(); ) {
//		if (it->second == mapName) {
//			it = keyCodeNameMap.erase(it);  // Erase the current element and move to the next
//		}
//		else {
//			++it;  // Move to the next element if no erase occurred
//		}
//	}
//
//	// Remove from pressedKeyMap if it exists
//	auto pressedIt = pressedKeyMap.find(mapName);
//	if (pressedIt != pressedKeyMap.end()) {
//		pressedKeyMap.erase(pressedIt);
//	}
//
//	return this;
//}

void Engine::Input::Reset()
{
	releasedKeys.clear();
}

void Input::PressKey(unsigned int keyID)
{
	// Check if the key is mapped to any actions
	auto it = keyCodeNameMap.find(keyID);
	if (it != keyCodeNameMap.end()) {
		// Set all actions associated with the key as pressed
		for (const string& mapName : it->second) {
			pressedKeyMap[mapName] = true;
		}
	}

	//// Here we are treating _keyMap as an associative array.
	//// if keyID doesn't already exist in _keyMap, it will get added
	//auto it = keyCodeNameMap.find(keyID);
	//if (it != keyCodeNameMap.end()) {
	//	string mapName = it->second;
	//	pressedKeyMap[mapName] = true;
	//	return;
	//}
}

void Input::ReleaseKey(unsigned int keyID)
{
	// Check if the key is mapped to any actions
	auto it = keyCodeNameMap.find(keyID);
	if (it != keyCodeNameMap.end()) {
		// Set all actions associated with the key as released
		for (const string& mapName : it->second) {
			pressedKeyMap[mapName] = false;
			releasedKeys.push_back(mapName);
		}
	}

	/*auto it = keyCodeNameMap.find(keyID);
	if (it != keyCodeNameMap.end()) {
		string mapName = it->second;
		pressedKeyMap[mapName] = false;
		releasedKeys.push_back(mapName);
	}*/
}

void Input::SetMouseCoords(float x, float y)
{
	mouseCoords.x = x;
	mouseCoords.y = y;
}

void Input::OpenGameController()
{
	if (SDL_NumJoysticks() > 0) {
		if (SDL_IsGameController(0))
		{
			controller = SDL_GameControllerOpen(0);
			if (controller != nullptr)
				cout << "Game Controller is Detected" << endl;
		}
	}
}

void Input::CloseGameController()
{
	if (controller != nullptr) {
		SDL_GameControllerClose(0);
		cout << "Game Controller is Removed" << endl;
	}
}
