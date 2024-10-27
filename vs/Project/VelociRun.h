#ifndef VELOCIRUN_H
#define VELOCIRUN_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
#include "Button.h"
#include "Music.h"
#include "Sound.h"
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "Obstacles.h"
#include "ParallaxBg.h"
#include "Screen.h"
#include "ScreenManager.h"

namespace Engine {
	class VelociRun :public Engine::Screen {
	public:
		VelociRun();
		~VelociRun();
		virtual void Init();
		virtual void Update();
		virtual void Draw();
	private:
		//Textures
		Texture* bgTex = NULL;
		Texture* darkOverlay = NULL;
		Texture* veloTex = NULL;
		//Sprites
		Sprite* bgSprt = NULL;
		Sprite* overlay = NULL;
		Sprite* veloSprt = NULL;
		Sprite* shadow = NULL;
		//Texts
		Text* score = NULL;
		Text* hiScore = NULL;
		Text* gameOver = NULL;
		Text* pause = NULL;
		Text* hint = NULL;
		//Audio
		Music* bgm = NULL;
		Sound* jumpSfx = NULL;
		Sound* gameOverSfx = NULL;

		//Object Pooling
		Engine::Sprite* CreateObsSprite(Texture* texture);
		Engine::Sprite* CreateShadowSprite(Texture* texture);
		void SpawnObstacles();
		vector<Obstacles*> obstacles;
		float elapsedTime = 0, maxElapsedTime = 0;
		float numObjectsInPool = 0, numObjectPerSpawn = 0;


		//Velociraptor
		float yVelocity = 0, gravity = 0, vx = 0, vy = 0;
		bool jump = false, duck = false;

		//Gameplay
		float globalXVel = 0;
		int globalCount = 0, gameScore = 0;
		bool isGameOver = false, isPaused = false;

		//Background
		Engine::Sprite* CreateBgSprite(string name);

		vector<Parallax*> dunes, cactiDunes, ground;
		Parallax* tempPlx;

		//Buttons
		vector<Button*> buttons;
		int currButtonIndex = 0;

		//Read & write
		int ReadHighScore();
		void WriteHighScore(int newHighScore);
		int currentHighScore;
	};
}

#endif