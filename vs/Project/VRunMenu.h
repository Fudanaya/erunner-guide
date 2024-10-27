#ifndef VR_MENU_H
#define VR_MENU_H

#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "Screen.h"
#include "ScreenManager.h"

namespace Engine {
	class VRunMenu :public Engine::Screen {
	public:
		VRunMenu();
		void Init();
		void Update();
		void Draw();
	private:
		//Background
		Sprite* bgSprite;

		//Buttons
		vector<Button*> buttons;
		int currButtonIndex = 0;

		//Title
		Texture* titleTex;
		Sprite* titleSprt;
	};
}

#endif