#include "VelociRun.h"

Engine::VelociRun::VelociRun() {
	tempPlx = NULL;
}

Engine::VelociRun::~VelociRun() {
	delete bgTex;
	delete veloTex;

	delete bgSprt;
	delete veloSprt;
	delete shadow;

	delete tempPlx;

	delete score;
	delete hiScore;
	delete gameOver;
	delete pause;
	delete hint;

	delete bgm;
}

void Engine::VelociRun::Init() {
	//Seed for rand
	srand(static_cast<unsigned>(time(0)));

	//Global parameter initial value
	globalXVel = 0.5;
	maxElapsedTime = 1000;
	numObjectsInPool = 4;
	numObjectPerSpawn = 1;
	currentHighScore = ReadHighScore();

	//audio
	bgm = (new Music("2021-08-16_-_8_Bit_Adventure_-_www.FesliyanStudios.com.ogg"))->SetVolume(30)->Play(true);

	jumpSfx = (new Sound("jump.wav"))->SetVolume(50);
	gameOverSfx = (new Sound("death-lose-sfx.wav"))->SetVolume(20);

	//bg
	bgTex = new Texture("Desert_Sky.png");

	bgSprt = new Sprite(bgTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	bgSprt->SetSize((float)game->GetSettings()->screenWidth, (float)game->GetSettings()->screenHeight);

	//Dark overlay
	darkOverlay = new Texture("Dark_Overlay.png");

	overlay = new Sprite(darkOverlay, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	overlay->SetSize((float)game->GetSettings()->screenWidth, (float)game->GetSettings()->screenHeight);

	//Parallax background
	
	//Create Backgrounds
	//Layer 1
	tempPlx = new Parallax(CreateBgSprite("Desert_Dunes.png"));
	tempPlx->SetAnchor();
	dunes.push_back(tempPlx);

	tempPlx = new Parallax(CreateBgSprite("Desert_Dunes.png"));
	tempPlx->SetFollow();
	tempPlx->SetPosition(game->GetSettings()->screenWidth, 0);
	dunes.push_back(tempPlx);

	dunes[0]->SetLeftParallax(dunes[1]);
	dunes[1]->SetLeftParallax(dunes[0]);

	//Layer 2
	tempPlx = new Parallax(CreateBgSprite("Desert_Cacti_Dunes.png"));
	tempPlx->SetAnchor();
	cactiDunes.push_back(tempPlx);

	tempPlx = new Parallax(CreateBgSprite("Desert_Cacti_Dunes.png"));
	tempPlx->SetFollow();
	tempPlx->SetPosition(game->GetSettings()->screenWidth, 0);
	cactiDunes.push_back(tempPlx);

	cactiDunes[0]->SetLeftParallax(cactiDunes[1]);
	cactiDunes[1]->SetLeftParallax(cactiDunes[0]);

	//Layer 3
	tempPlx = new Parallax(CreateBgSprite("Desert_Ground.png"));
	tempPlx->SetAnchor();
	ground.push_back(tempPlx);

	tempPlx = new Parallax(CreateBgSprite("Desert_Ground.png"));
	tempPlx->SetFollow();
	tempPlx->SetPosition(game->GetSettings()->screenWidth, 0);
	ground.push_back(tempPlx);

	ground[0]->SetLeftParallax(ground[1]);
	ground[1]->SetLeftParallax(ground[0]);

	//Obstacles
	Texture* obsTex = new Texture("Desert_Obstacles.png");
	Texture* shadowTex = new Texture("Shadow.png");

	for (int i = 0; i < numObjectsInPool; i++) {
		Obstacles* o = new Obstacles(CreateObsSprite(obsTex), CreateShadowSprite(shadowTex));
		o->GetSprite()->SetBoundingBoxSize(o->GetWidth() - (12 * o->GetSprite()->GetScale()), o->GetHeight() - (3 * o->GetSprite()->GetScale()));
		obstacles.push_back(o);
	}


	//Velociraptor placeholder + shadow
	veloTex = new Texture("DinoSprites - vita.png");
	veloSprt = new Sprite(veloTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	
	shadow = new Sprite(shadowTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	//Velociraptor Anim & Bounding Box & Shadow
	veloSprt->SetNumXFrames(24)->SetNumYFrames(1)->AddAnimation("idle-run", 4, 9)->AddAnimation("duck", 17, 23)->SetScale(5)->SetAnimationDuration(60)->SetPosition(300, game->GetSettings()->screenHeight / 11);

	shadow->SetScale(4)->SetPosition(300 + 2 * veloSprt->GetScale(), game->GetSettings()->screenHeight / 11 - veloSprt->GetScale());

	veloSprt->SetBoundingBoxSize(veloSprt->GetScaleWidth() - (10 * veloSprt->GetScale()), veloSprt->GetScaleHeight() - (8 * veloSprt->GetScale()));

	//Texts placeholder
	score = new Text("lucon.ttf", 24, game->GetDefaultTextShader());
	score->SetScale(1.0f)->SetColor(0, 0, 0)->SetPosition(5, game->GetSettings()->screenHeight - (5 + score->GetFontSize() * score->GetScale()));

	hiScore = new Text("lucon.ttf", 24, game->GetDefaultTextShader());
	hiScore->SetScale(1.0f)->SetColor(0, 0, 0)->SetPosition(5, game->GetSettings()->screenHeight - (2 * (5 + hiScore->GetFontSize() * hiScore->GetScale())));

	hint = new Text("lucon.ttf", 24, game->GetDefaultTextShader());
	hint->SetScale(1.0f)->SetColor(0, 0, 0)->SetPosition(5, game->GetSettings()->screenHeight - (3 * (5 + hint->GetFontSize() * hint->GetScale())));

	gameOver = new Text("8-bit Arcade In.ttf", 100, game->GetDefaultTextShader());
	gameOver->SetScale(1.0f)->SetColor(220, 0, 20)->SetPosition(game->GetSettings()->screenWidth / 2, game->GetSettings()->screenHeight * 2 / 3)->SetText("Game Over")->CenterTextX();

	pause = new Text("8-bit Arcade In.ttf", 100, game->GetDefaultTextShader());
	pause->SetScale(1.0f)->SetColor(0, 0, 0)->SetPosition(game->GetSettings()->screenWidth / 2, game->GetSettings()->screenHeight * 2 / 3)->SetText("Paused")->CenterTextX();

	//Input Mapping
	game->GetInputManager()->AddInputMapping("Jump", SDLK_UP)->AddInputMapping("Jump", SDLK_SPACE)->AddInputMapping("Main-menu", SDLK_ESCAPE)->AddInputMapping("Pause", SDLK_TAB)->AddInputMapping("Duck", SDLK_DOWN)->AddInputMapping("Reset", SDLK_r);

	//Button input mapping
	game->GetInputManager()->AddInputMapping("prev", SDLK_LEFT)->AddInputMapping("next", SDLK_RIGHT)->AddInputMapping("press", SDLK_RETURN);

	//Buttons
	//Button texture
	Texture* buttonTex = new Texture("Circular_Buttons.png");

	//Resume button
	Sprite* resumeSprt = new Sprite(buttonTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	resumeSprt->SetNumXFrames(4)->SetNumYFrames(4)->AddAnimation("normal", 0, 0)->AddAnimation("hover", 0, 3, true)->AddAnimation("press", 3, 3)->SetScale(5)->SetAnimationDuration(100);

	Button* resumeButton = new Button(resumeSprt, "resume");
	resumeButton->SetPosition((game->GetSettings()->screenWidth / 2) - (1.5 * resumeSprt->GetScaleWidth()),
		(game->GetSettings()->screenHeight / 2) - (resumeSprt->GetScaleHeight()));
	buttons.push_back(resumeButton);

	//Restart button
	Sprite* restartSprt = new Sprite(buttonTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	restartSprt->SetNumXFrames(4)->SetNumYFrames(4)->AddAnimation("normal", 4, 4)->AddAnimation("hover", 4, 7, true)->AddAnimation("press", 7, 7)->SetScale(5)->SetAnimationDuration(100);

	Button* restartButton = new Button(restartSprt, "restart");
	restartButton->SetPosition((game->GetSettings()->screenWidth / 2) - (1.5 * restartSprt->GetScaleWidth()),
		(game->GetSettings()->screenHeight / 2) - (restartSprt->GetScaleHeight()));
	buttons.push_back(restartButton);

	//Exit button
	Sprite* exitSprt = new Sprite(buttonTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	exitSprt->SetNumXFrames(4)->SetNumYFrames(4)->AddAnimation("normal", 12, 12)->AddAnimation("hover", 12, 15, true)->AddAnimation("press", 15, 15)->SetScale(5)->SetAnimationDuration(100);

	Button* exitButton = new Button(exitSprt, "exit");
	exitButton->SetPosition((game->GetSettings()->screenWidth / 2) + (0.5 * exitSprt->GetScaleWidth()),
		(game->GetSettings()->screenHeight / 2) - (exitSprt->GetScaleHeight()));
	buttons.push_back(exitButton);
}

void Engine::VelociRun::Update() {
	//Game States
	if (!isGameOver && !isPaused) {
		//Spawn obstacles
		if (elapsedTime >= maxElapsedTime) {
			SpawnObstacles();
			elapsedTime = 0;
		}

		elapsedTime += game->GetGameTime();

		//Scoring & Global Velocity
		if (globalCount % 20 == 0 && globalCount != 0) {
			gameScore += 1;
		}

		if (globalCount % 40 == 0 && globalCount != 0 && globalXVel <= 1) {
			globalXVel += 0.005;
			globalCount = 0;
		}

		globalCount += 1;

		//Update all obstacles velocity
		for (Obstacles* o : obstacles) {
			o->SetVelocity(globalXVel);
		}

		//Update all bg velocity
		for (Parallax* o : dunes) {
			o->SetVelocity(globalXVel * 1 / 10);
		}
		for (Parallax* o : cactiDunes) {
			o->SetVelocity(globalXVel * 2 / 5);
		}
		for (Parallax* o : ground) {
			o->SetVelocity(globalXVel);
		}

		//Detect collision
		for (Obstacles* o : obstacles) {
			if (!o->IsNoUse()) {
				if (o->GetSprite()->GetBoundingBox()->CollideWith(veloSprt->GetBoundingBox())) {
					gameOverSfx->Play(false);
					isGameOver = true;
					buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
					currButtonIndex = 1;
					buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
				}
			}
		}

		//Display texts
		score->SetText("Score : " + to_string(gameScore));
		hiScore->SetText("High Score : " + to_string(currentHighScore));
		hint->SetText("Up & Down to Jump and Duck");

		//Velociraptor temp pos saver and def anim
		vec2 oldVeloPos = veloSprt->GetPosition();
		vx = oldVeloPos.x, vy = oldVeloPos.y;

		veloSprt->PlayAnim("idle-run");

		//Input detections
		//Main menu
		if (game->GetInputManager()->IsKeyPressed("Main-menu")) {
			//Reset global parameters
			elapsedTime = 0;
			yVelocity = 0;
			globalXVel = 0.5;
			globalCount = 0;
			gameScore = 0;
			jump = false;
			duck = false;

			//Reset velociraptor's pos
			vx = 300, vy = game->GetSettings()->screenHeight / 11;
			veloSprt->SetPosition(vx, vy);

			//Reset all obstacles
			for (Obstacles* o : obstacles) {
				o->SetNoUse();
				o->SetPosition(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight / 11);
				o->SetVelocity(globalXVel);
			}
			
			//Change screen
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");

			return;
		}

		//Pause
		if (game->GetInputManager()->IsKeyPressed("Pause")) {
			//Still haven't added a pause screen yet
			//ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");

			//Change game state and button state
			isPaused = true;
			buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
			currButtonIndex = 0;
			buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
			return;
		}

		//Jump
		if (game->GetInputManager()->IsKeyPressed("Jump") && !jump && !duck) {
			//Clamping
			float delta = game->GetGameTime();
			float maxDelta = 1000.0f / 30.0f;

			if (delta >= maxDelta) {
				delta = maxDelta;
			}
			//Set gravity and yVelocity
			float ratio = (delta / 16.7f);
			gravity = 0.05f * ratio;
			yVelocity = 1.0f;
			jump = true;
			jumpSfx->Play(false);
		}
		//Jump physics
		if (vy > game->GetSettings()->screenHeight / 11) {
			yVelocity -= gravity;
		}
		else if (vy < game->GetSettings()->screenHeight / 11) {
			jump = false;
			yVelocity = 0;
			vy = game->GetSettings()->screenHeight / 11;
		}

		if (game->GetInputManager()->IsKeyReleased("Jump") && jump) {
			if (yVelocity > 0.8) {
				yVelocity = 0.8;
			}
		}

		vy += yVelocity * game->GetGameTime();
		veloSprt->SetPosition(vx, vy);

		//Duck
		if (game->GetInputManager()->IsKeyPressed("Duck") && !jump) {
			//Play duck anim
			veloSprt->PlayAnim("duck");
			//Set bounding box size for ducking
			veloSprt->SetBoundingBoxSize(veloSprt->GetScaleWidth() - (10 * veloSprt->GetScale()), veloSprt->GetScaleHeight() - (12 * veloSprt->GetScale()));
			duck = true;
		}

		if (game->GetInputManager()->IsKeyReleased("Duck") && duck) {
			//Reset BBox to default
			veloSprt->SetBoundingBoxSize(veloSprt->GetScaleWidth() - (10 * veloSprt->GetScale()), veloSprt->GetScaleHeight() - (8 * veloSprt->GetScale()));
			duck = false;
		}

		//Update
		veloSprt->Update(game->GetGameTime());

		for (Obstacles* o : obstacles) {
			o->Update(game->GetGameTime());
		}

		//Update all bg
		for (Parallax* o : dunes) {
			o->Update(game->GetGameTime());
		}
		for (Parallax* o : cactiDunes) {
			o->Update(game->GetGameTime());
		}
		for (Parallax* o : ground) {
			o->Update(game->GetGameTime());
		}

	}
	else if (isPaused) {
		//Input detections
		//Previous
		if (game->GetInputManager()->IsKeyPressed("prev")) {
			// Set previous button to normal state
			buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
			// Prev Button
			currButtonIndex = currButtonIndex > 0 ? currButtonIndex - 2 : currButtonIndex;
			// Set current button to hover state
			buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
		}

		//Next
		if (game->GetInputManager()->IsKeyPressed("next")) {
			// Set previous button to normal state
			buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
			// Next Button
			currButtonIndex = (currButtonIndex < (int)buttons.size() - 1) ? currButtonIndex + 2 : currButtonIndex;
			// Set current button to hover state
			buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
		}

		//Press
		if (game->GetInputManager()->IsKeyReleased("press")) {
			// Set current button to press state
			Button* b = buttons[currButtonIndex];
			b->SetButtonState(Engine::ButtonState::PRESS);
			// If start button then go to InGame, exit button then exit
			if ("resume" == b->GetButtonName()) {
				//Reset Text pos
				score->SetPosition(5, game->GetSettings()->screenHeight - (5 + score->GetFontSize() * score->GetScale()))->SetColor(0, 0, 0);

				hiScore->SetPosition(5, game->GetSettings()->screenHeight - (2 * (5 + hiScore->GetFontSize() * hiScore->GetScale())))->SetColor(0, 0, 0);

				hint->SetPosition(5, game->GetSettings()->screenHeight - (3 * (5 + hint->GetFontSize() * hint->GetScale())))->SetColor(0, 0, 0);

				//Change state
				isPaused = false;

				return;
			}
			else if ("exit" == b->GetButtonName()) {
				//Reset global parameters
				elapsedTime = 0;
				globalCount = 0;
				gameScore = 0;
				jump = false;
				duck = false;
				isPaused = false;

				//Reset velociraptor's pos
				vx = 300, vy = game->GetSettings()->screenHeight / 11;
				veloSprt->SetPosition(vx, vy);

				//Reset all obstacles
				for (Obstacles* o : obstacles) {
					o->SetNoUse();
					o->SetPosition(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight / 11);
					o->SetVelocity(globalXVel);
				}

				//Change screen
				ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");

				//Reset Text pos
				score->SetPosition(5, game->GetSettings()->screenHeight - (5 + score->GetFontSize() * score->GetScale()))->SetColor(0, 0, 0);

				hiScore->SetPosition(5, game->GetSettings()->screenHeight - (2 * (5 + hiScore->GetFontSize() * hiScore->GetScale())))->SetColor(0, 0, 0);

				hint->SetPosition(5, game->GetSettings()->screenHeight - (3 * (5 + hint->GetFontSize() * hint->GetScale())))->SetColor(0, 0, 0);

				return;
			}
		}

		//Update all buttons
		for (Button* b : buttons) {
			b->Update(game->GetGameTime());
		}

		//Display texts
		score->SetPosition(game->GetSettings()->screenWidth / 2, game->GetSettings()->screenHeight * 2 / 3 - score->GetFontSize() * score->GetScale() * 1.5)->SetText("Score : " + to_string(gameScore))->SetColor(0, 0, 0)->CenterTextX();

		hiScore->SetPosition(game->GetSettings()->screenWidth / 2, game->GetSettings()->screenHeight * 2 / 3 - hiScore->GetFontSize() * hiScore->GetScale() * 2.5)->SetText("High Score : " + to_string(currentHighScore))->SetColor(0, 0, 0)->CenterTextX();

		hint->SetText("");
	}
	else if (isGameOver) {
		//Reset parameter
		yVelocity = 0;
		globalXVel = 0.5;

		//High score check
		if (gameScore > currentHighScore) {
			WriteHighScore(gameScore);
			currentHighScore = ReadHighScore();
		}

		//Input detections
		//Previous
		if (game->GetInputManager()->IsKeyPressed("prev")) {
			// Set previous button to normal state
			buttons[currButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
			// Prev Button
			currButtonIndex = currButtonIndex > 1 ? currButtonIndex - 1 : currButtonIndex;
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
		if (game->GetInputManager()->IsKeyReleased("press")) {
			// Set current button to press state
			Button* b = buttons[currButtonIndex];
			b->SetButtonState(Engine::ButtonState::PRESS);
			// If start button then go to InGame, exit button then exit
			if ("restart" == b->GetButtonName()) {
				//Reset global parameters
				elapsedTime = 0;
				globalCount = 0;
				gameScore = 0;
				jump = false;
				duck = false;
				isGameOver = false;

				//Reset velociraptor's pos
				vx = 300, vy = game->GetSettings()->screenHeight / 11;
				veloSprt->SetPosition(vx, vy);

				//Reset all obstacles
				for (Obstacles* o : obstacles) {
					o->SetNoUse();
					o->SetPosition(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight / 11);
					o->SetVelocity(globalXVel);
				}

				//Reset Text pos
				score->SetPosition(5, game->GetSettings()->screenHeight - (5 + score->GetFontSize() * score->GetScale()))->SetColor(0, 0, 0);

				hiScore->SetPosition(5, game->GetSettings()->screenHeight - (2 * (5 + hiScore->GetFontSize() * hiScore->GetScale())))->SetColor(0, 0, 0);

				hint->SetPosition(5, game->GetSettings()->screenHeight - (3 * (5 + hint->GetFontSize() * hint->GetScale())))->SetColor(0, 0, 0);

				return;
			}
			else if ("exit" == b->GetButtonName()) {
				//Reset global parameters
				elapsedTime = 0;
				globalCount = 0;
				gameScore = 0;
				jump = false;
				duck = false;
				isGameOver = false;

				//Reset velociraptor's pos
				vx = 300, vy = game->GetSettings()->screenHeight / 11;
				veloSprt->SetPosition(vx, vy);

				//Reset all obstacles
				for (Obstacles* o : obstacles) {
					o->SetNoUse();
					o->SetPosition(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight / 11);
					o->SetVelocity(globalXVel);
				}

				//Change screen
				ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");

				//Reset Text pos
				score->SetPosition(5, game->GetSettings()->screenHeight - (5 + score->GetFontSize() * score->GetScale()))->SetColor(0, 0, 0);

				hiScore->SetPosition(5, game->GetSettings()->screenHeight - (2 * (5 + hiScore->GetFontSize() * hiScore->GetScale())))->SetColor(0, 0, 0);

				hint->SetPosition(5, game->GetSettings()->screenHeight - (3 * (5 + hint->GetFontSize() * hint->GetScale())))->SetColor(0, 0, 0);

				return;
			}
		}
		
		//Update all buttons
		for (Button* b : buttons) {
			b->Update(game->GetGameTime());
		}

		//Display texts
		score->SetPosition(game->GetSettings()->screenWidth / 2, game->GetSettings()->screenHeight * 2 / 3 - score->GetFontSize() * score->GetScale() * 1.5)->SetText("Score : " + to_string(gameScore))->SetColor(0, 0, 0)->CenterTextX();

		hiScore->SetPosition(game->GetSettings()->screenWidth / 2, game->GetSettings()->screenHeight * 2 / 3 - hiScore->GetFontSize() * hiScore->GetScale() * 2.5)->SetText("High Score : " + to_string(currentHighScore))->SetColor(0, 0, 0)->CenterTextX();

		hint->SetText("");
	}
}

void Engine::VelociRun::Draw() {
	//Sky
	bgSprt->Draw();

	//All bg
	for (Parallax* o : dunes) {
		o->Draw();
	}
	for (Parallax* o : cactiDunes) {
		o->Draw();
	}
	for (Parallax* o : ground) {
		o->Draw();
	}

	//Velociraptor
	shadow->Draw();
	veloSprt->Draw();

	//All obstacles
	for (Obstacles* o : obstacles) {
		o->Draw();
	}
	if (isPaused) {
		overlay->Draw();

		buttons[0]->Draw();
		buttons[2]->Draw();

		pause->Draw();
	}

	if (isGameOver) {
		overlay->Draw();
		
		buttons[1]->Draw();
		buttons[2]->Draw();

		gameOver->Draw();
	}

	//Texts
	score->Draw();
	hiScore->Draw();
	hint->Draw();
}

Engine::Sprite* Engine::VelociRun::CreateObsSprite(Texture* texture) {
	return (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetNumXFrames(3)->SetNumYFrames(2)->AddAnimation("obstacle1", 0, 0)->AddAnimation("obstacle2", 1, 1)->AddAnimation("obstacle3", 2, 2)->AddAnimation("obstacle4", 3, 4)->SetScale(4)->SetAnimationDuration(500);
}

Engine::Sprite* Engine::VelociRun::CreateShadowSprite(Texture* texture) {
	return (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetScale(4)->SetPosition(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight / 11);
}

void Engine::VelociRun::SpawnObstacles() {
	int spawnCount = 0;
	for (Obstacles* o : obstacles) {
		if (spawnCount == numObjectPerSpawn) {
			break;
		}
		if (o->IsNoUse()) {
			unsigned int random = 1 + (rand() % 4);
			//x and y position
			float x = (float)(game->GetSettings()->screenWidth);
			float y = game->GetSettings()->screenHeight / 11;
			//y position of airborne obstacles
			if (random > 3) {
				y = game->GetSettings()->screenHeight / 11 + 70;
			}
			//Set position and play random animation
			o->SetUse()->SetPosition(x, y);
			o->GetSprite()->PlayAnim("obstacle" + to_string(random))->ResetAnimFrame();
			spawnCount++;
		}
	}
}

Engine::Sprite* Engine::VelociRun::CreateBgSprite(string name) {
	Texture* bgTex = new Texture(name);

	return(new Sprite(bgTex, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight)->SetPosition(0, 0);
}

//Read
int Engine::VelociRun::ReadHighScore() {
	std::ifstream file("VeloData.txt");
	std::string line;
	int highScore = 0;

	if (file.is_open()) {
		getline(file, line);  // Read the first line "highscore: 0"
		size_t pos = line.find_last_of(" ");  // Find the space before the number
		if (pos != std::string::npos) {
			highScore = std::stoi(line.substr(pos + 1));  // Extract the number after the space
		}
		file.close();
	}

	return highScore;  // Return the extracted high score
}

//Write
void Engine::VelociRun::WriteHighScore(int newHighScore) {
	std::ofstream file("VeloData.txt");

	if (file.is_open()) {
		file << "highscore: " << newHighScore;  // Write "highscore: " followed by the new score
		file.close();
	}
}