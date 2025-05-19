#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "Framework/Utilities.h" 
class Goomba : public GameObject
{
	int speed;
	sf::Texture GoombaSpriteSheet;
	Animation walk;
	Animation* currentAnimation;
	bool hasCollided = false;

public:
	Goomba();
	~Goomba();
	void printVelocity() { Output("Goomba Velocity: %f %f\n", velocity.x, velocity.y);  }
	void update(float dt)  override;
};