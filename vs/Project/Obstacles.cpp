#include "Obstacles.h"

Engine::Obstacles::Obstacles(Sprite* sprite, Sprite* shadow) {
	this->sprite = sprite;
	this->shadow = shadow;
	state = Engine::ObstacleState::NOUSE;
}

void Engine::Obstacles::Update(float deltaTime) {
	//No update if obstacle is NOUSE
	if (state == Engine::ObstacleState::NOUSE) {
		return;
	}

	//Basic parameter
	float x = GetX();
	float y = GetY();
	float shadowY = GetShadowY();
	float xVel = GetVelocity();

	//Change in state & velocity if offscreen
	if (state == Engine::ObstacleState::INUSE && x < -sprite->GetScaleWidth()) {
		state = Engine::ObstacleState::NOUSE;
		xVel = 0;
	}

	//Clamping
	float delta = deltaTime;
	float maxDelta = 1000.0f / 30.0f;

	if (delta >= maxDelta) {
		delta = maxDelta;
	}

	//Movement physic
	x -= xVel * delta;
	sprite->SetPosition(x, y);
	shadow->SetPosition(x, shadowY);
	sprite->Update(deltaTime);
	shadow->Update(deltaTime);
}

void Engine::Obstacles::Draw() {
	if (state == Engine::ObstacleState::NOUSE) {
		return;
	}

	shadow->Draw();
	sprite->Draw();
}

//Position related
Engine::Obstacles* Engine::Obstacles::SetPosition(float x, float y) {
	sprite->SetPosition(x, y);
	return this;
}

float Engine::Obstacles::GetX() {
	return sprite->GetPosition().x;
}

float Engine::Obstacles::GetY() {
	return sprite->GetPosition().y;
}

float Engine::Obstacles::GetShadowY() {
	return shadow->GetPosition().y;
}

//Size
float Engine::Obstacles::GetHeight() {
	return sprite->GetScaleHeight();
}

float Engine::Obstacles::GetWidth() {
	return sprite->GetScaleWidth();
}

//State
Engine::Obstacles* Engine::Obstacles::SetUse() {
	this->state = Engine::ObstacleState::INUSE;
	return this;
}

Engine::Obstacles* Engine::Obstacles::SetNoUse() {
	this->state = Engine::ObstacleState::NOUSE;
	return this;
}

bool Engine::Obstacles::IsNoUse() {
	return Engine::ObstacleState::NOUSE == state;
}

//Velocity
Engine::Obstacles* Engine::Obstacles::SetVelocity(float x) {
	xVelocity = x;
	return this;
}

float Engine::Obstacles::GetVelocity() {
	return xVelocity;
}

//Misc
Engine::Sprite* Engine::Obstacles::GetSprite() {
	return sprite;
}