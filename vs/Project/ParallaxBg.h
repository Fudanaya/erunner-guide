#ifndef PARALLAX_H
#define PARALLAX_H

#include "Sprite.h"
#include <vector>

namespace Engine {
	enum class BgState {
		ANCHOR,
		FOLLOW
	};
	class Parallax {
	public:
		Parallax(Sprite* sprite);
		void Update(float deltaTime);
		void Draw();

		//State
		Parallax* SetAnchor();
		Parallax* SetFollow();
		bool IsAnchor();

		//Pointer
		Parallax* SetLeftParallax(Parallax* leftBg);
		Parallax* GetLeftParallax();

		//Velocity
		Parallax* SetVelocity(float speed);
		float GetVelocity();

		//Position
		Parallax* SetPosition(float x, float y);
		float GetX();
		float GetY();

		//Size
		float GetWidth();
	private:
		Parallax* leftBg;
		Sprite* sprite;
		BgState state;

		float x = 0, y = 0;
		float xVelocity = 0;
	};
}

#endif