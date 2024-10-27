#include "VRunMenu.h"

Engine::VRunMenu::VRunMenu() {
	bgSprite = NULL;
	titleTex = NULL;
	titleSprt = NULL;
}

void Engine::VRunMenu::Init() {
	//Create background
	Texture* bgTexture = new Texture("Desert_Background.png");
	bgSprite = (new Sprite(bgTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize((float)game->GetSettings()->screenWidth, (float)game->GetSettings()->screenHeight);

	//Button texture
	Texture* buttonTex = new Texture("Main_Buttons.png");

	//Start button
	Sprite* startSprt = new Sprite(buttonTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	startSprt->SetNumXFrames(4)->SetNumYFrames(3)->AddAnimation("normal", 0, 0)->AddAnimation("hover", 0, 3, true)->AddAnimation("press", 0, 3)->SetScale(5)->SetAnimationDuration(100);

	Button* startButton = new Button(startSprt, "start");
	startButton->SetPosition((game->GetSettings()->screenWidth / 2) - (startSprt->GetScaleWidth() / 2),
		game->GetSettings()->screenHeight / 2);
	buttons.push_back(startButton);

	//Settings button

	//Exit button
	Sprite* exitSprt = new Sprite(buttonTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	exitSprt->SetNumXFrames(4)->SetNumYFrames(3)->AddAnimation("normal", 8, 8)->AddAnimation("hover", 8, 11, true)->AddAnimation("press", 8, 11)->SetScale(5)->SetAnimationDuration(100);

	Button* exitButton = new Button(exitSprt, "exit");
	exitButton->SetPosition((game->GetSettings()->screenWidth / 2) - (exitSprt->GetScaleWidth() / 2),
		game->GetSettings()->screenHeight / 3);
	buttons.push_back(exitButton);

	//Set start button as default
	currButtonIndex = 0;
	buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

	//Create title
	titleTex = new Texture("VelociRun_Title.png");

	titleSprt = new Sprite(titleTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	titleSprt->SetPosition(((game->GetSettings()->screenWidth / 2) - (titleSprt->GetScaleWidth() / 2)), game->GetSettings()->screenHeight * 7.5 / 10);

	//Input Mapping
	game->GetInputManager()->AddInputMapping("prev", SDLK_UP)->AddInputMapping("next", SDLK_DOWN)->AddInputMapping("press", SDLK_RETURN);

}

void Engine::VRunMenu::Update() {
	//Input detections
	//Previous
	if (game->GetInputManager()->IsKeyPressed("prev")) {
		// Set previous button to normal state
		buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Prev Button
		currButtonIndex = currButtonIndex > 0 ? currButtonIndex - 1 : currButtonIndex;
		// Set current button to hover state
		buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	//Next
	if (game->GetInputManager()->IsKeyPressed("next")) {
		// Set previous button to normal state
		buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Next Button
		currButtonIndex = (currButtonIndex < (int)buttons.size() - 1) ? currButtonIndex + 1 : currButtonIndex;
		// Set current button to hover state
		buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	//Press
	if (game->GetInputManager()->IsKeyPressed("press")) {
		// Set current button to press state
		Button* b = buttons[currButtonIndex];
		b->SetButtonState(Engine::ButtonState::PRESS);
		// If start button then go to InGame, exit button then exit
		if ("start" == b->GetButtonName()) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("ingame");
		}
		else if ("exit" == b->GetButtonName()) {
			game->SetState(Engine::State::EXIT);
		}
	}

	//Update all buttons
	for (Button* b : buttons) {
		b->Update(game->GetGameTime());
	}
}

void Engine::VRunMenu::Draw() {
	//Render background
	bgSprite->Draw();
	
	//Render all buttons
	for (Button* b : buttons) {
		b->Draw();
	}
	//Render title 
	titleSprt->Draw();
}