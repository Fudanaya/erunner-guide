#include "ParallaxBg.h"

Engine::Parallax::Parallax(Sprite* sprite) {
	state = Engine::BgState::FOLLOW;
	this->sprite = sprite;
}

void Engine::Parallax::Update(float deltaTime) {
	std::cout << "Sprite position: " << sprite->GetPosition().x << ", " << sprite->GetPosition().y << std::endl;
	
	//If state = Anchor then move independently
	if (state == Engine::BgState::ANCHOR) {
		float x = GetX();
		float y = GetY();
		float xVel = GetVelocity();

		//Clamping
		float delta = deltaTime;
		float maxDelta = 1000.0f / 30.0f;

		if (delta >= maxDelta) {
			delta = maxDelta;
		}

		//Movement physics
		x -= xVel * delta;

		if (x <= -GetWidth()) {
			state = Engine::BgState::FOLLOW;
			x = GetWidth();
		}

		sprite->SetPosition(x, y);
		sprite->Update(deltaTime);
	}
	//If state = Follow and left's state = Anchor then move next to Anchor
	if (state == Engine::BgState::FOLLOW && GetLeftParallax()->IsAnchor()) {
		float x = GetLeftParallax()->GetX() + GetLeftParallax()->GetWidth();
		float y = GetY();

		sprite->SetPosition(x, y);
		sprite->Update(deltaTime);
	}
	//Else if left is no longer Anchor then set x to 0
	else if (state == Engine::BgState::FOLLOW && !GetLeftParallax()->IsAnchor() && 0 < GetX() <= xVelocity * deltaTime) {
		float x = 0;
		float y = GetY();

		sprite->SetPosition(x, y);
		sprite->Update(deltaTime);
	}
	//Else if x = 0 then become Anchor
	if (state == Engine::BgState::FOLLOW && !GetLeftParallax()->IsAnchor() && GetX() == 0) {
		state = Engine::BgState::ANCHOR;
	}


	sprite->Update(deltaTime);
}

void Engine::Parallax::Draw() {
	sprite->Draw();
}

//State
Engine::Parallax* Engine::Parallax::SetAnchor() {
	state = Engine::BgState::ANCHOR;
	return this;
}

Engine::Parallax* Engine::Parallax::SetFollow() {
	state = Engine::BgState::FOLLOW;
	return this;
}

bool Engine::Parallax::IsAnchor() {
	return Engine::BgState::ANCHOR == state;
}

//Pointer
Engine::Parallax* Engine::Parallax::SetLeftParallax(Parallax* leftBg) {
	this->leftBg = leftBg;
	return this;
}

Engine::Parallax* Engine::Parallax::GetLeftParallax() {
	return leftBg;
}

//Velocity
Engine::Parallax* Engine::Parallax::SetVelocity(float speed) {
	xVelocity = speed;
	return this;
}

float Engine::Parallax::GetVelocity() {
	return xVelocity;
}

//Position
Engine::Parallax* Engine::Parallax::SetPosition(float x, float y) {
	sprite->SetPosition(x, y);
	return this;
}

float Engine::Parallax::GetX() {
	return sprite->GetPosition().x;
}

float Engine::Parallax::GetY() {
	return sprite->GetPosition().y;
}

//Size
float Engine::Parallax::GetWidth() {
	return sprite->GetScaleWidth();
}