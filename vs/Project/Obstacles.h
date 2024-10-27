#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "Sprite.h"
#include "Game.h"

namespace Engine {
	enum class ObstacleState {
		INUSE,
		NOUSE
	};
	class Obstacles {
	public:
		Obstacles(Sprite* sprite, Sprite* shadow);
		void Update(float deltaTime);
		void Draw();

		//Position related
		Obstacles* SetPosition(float x, float y);
		float GetX();
		float GetY();
		float GetShadowY();

		//Size
		float GetHeight();
		float GetWidth();

		//State
		Obstacles* SetUse();
		Obstacles* SetNoUse();
		bool IsNoUse();

		//Velocity
		Obstacles* SetVelocity(float x);
		float GetVelocity();

		//Misc
		Sprite* GetSprite();
	private:
		Sprite* shadow = NULL;
		Sprite* sprite = NULL;
		ObstacleState state;

		float xVelocity = 0;
	};
}

#endif

//#ifndef OBSTACLES_H
//#define OBSTACLES_H
//
//#include "Sprite.h"
//
//namespace Engine {
//	enum class ObstacleState {
//		INUSE,
//		NOUSE
//	};
//	class Obstacles {
//	public:
//		Obstacles(Sprite* sprite);
//		~Obstacles();
//		void Update(float deltaTime);
//		void Draw();
//
//		//Position related
//		Obstacles* SetPosition(float x, float y);
//		float GetX();
//		float GetY();
//
//		//Size
//		float GetHeight();
//		float GetWidth();
//
//		//State
//		Obstacles* SetUse();
//		Obstacles* SetNoUse();
//		bool IsNoUse();
//
//		//Velocity
//		Obstacles* SetVelocity(float x);
//		float GetVelocity();
//
//		//Misc
//		Sprite* GetSprite();
//	private:
//		Sprite* sprite = NULL;
//		ObstacleState state;
//
//		float x = 0, y = 0;
//		float xVelocity = 0;
//	};
//}
//
//#endif